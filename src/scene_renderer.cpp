#include "scene_renderer.h"
#include <sdl2/SDL_image.h>
#include <sdl2/SDL.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "game_engine.h"
#include "window.h"
#include "level.h"
#include "st_assert.h"
#include <algorithm>
#include "camera_component.h" // TEMP
#include "sprite_component.h" // TEMP
#include "world.h"
#include <map>
#include "actor.h"

/* BEGIN: TEMP FOR TESTING */
float camWidth = 0.2f;

const int texWidth = 800;
const int texHeight = 600;

/* END: TEMP FOR TESTING */

namespace Retro3D
{
	SceneRenderer::SceneRenderer()
	{
		Window* window = GGameEngine->GetWindow();

		/*** Set up the render target texture ***/
		mRenderTexture = SDL_CreateTexture(window->GetSDLRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

		/*** Set up pixel arrays ***/
		mClearPixels = std::vector<unsigned char>(texWidth * texHeight * 4, 0); // clear pixel array. "pixels" array is set to this before rendering
		for (size_t i = 0; i < mClearPixels.size(); i++)
		{
			mClearPixels[i] = SDL_ALPHA_OPAQUE; // fill with 255, so we won't have to set the alpha when rendering
		}
		mPixels = mClearPixels; // pixel array containing the rendered scene

		// Set up and clear the depth buffer
		mClearDepthBuffer = std::vector<float>(texWidth, 0);
		for (size_t i = 0; i < mClearDepthBuffer.size(); i++)
		{
			mClearDepthBuffer[i] = 10000.0f;
		}
		mDepthBuffer = mClearDepthBuffer;
	}


	SceneRenderer::~SceneRenderer()
	{
	}

	/**
	* Set level to render
	**/
	void SceneRenderer::SetLevel(Level* arg_level)
	{
		mLevel = arg_level;
		for (auto textureKeyPair : mLevel->GetTextureMap())
		{
			std::string fullPath = std::string("resources//textures//") + textureKeyPair.second;
			SDL_Surface* surface = IMG_Load(fullPath.c_str());
			__AssertComment(surface != nullptr, std::string("Unable to load texture: " + fullPath)); // todo
			mTextureSurfaceMap[textureKeyPair.first] = surface;
		}
		if (mLevel->GetSkyboxTexture() != "")
		{
			mSkyboxTexture = IMG_Load((std::string("resources//textures//") + mLevel->GetSkyboxTexture()).c_str());
			__Assert(mSkyboxTexture != nullptr);
		}

		float newFov;
		if (GGameEngine->GetGameConfig().GetFloat("camera", "fov", newFov))
		{
			mFOV = newFov;
		}
		
	}

	/**
	* Set active camera component to render from.
	**/
	void SceneRenderer::SetCameraComponent(CameraComponent* arg_comp)
	{
		mCameraComponent = arg_comp;
	}

	/**
	* Render the whole scene.
	* Will render walls, ceiling, floor, skybox and sprites
	**/
	void SceneRenderer::RenderScene()
	{
		__Assert(mLevel != nullptr);

		glm::vec3 camPos;
		glm::mat4 camRot;
		if (mCameraComponent.IsValid()) // TEMP!
		{
			camPos = mCameraComponent->GetCameraTransform().GetPosition();
			camRot = mCameraComponent->GetCameraTransform().GetRotation();
		}

		const std::string& skyboxTexture = mLevel->GetSkyboxTexture();
		bool renderSkybox = skyboxTexture != "";

		// Setup sprite render objects - TODO: don't do this here!
		const std::vector<SpriteComponent*> spriteComps = GGameEngine->GetWorld()->GetComponentsOfType<SpriteComponent>();
		std::multimap<float, SpriteRenderObject> renderObjects;
		for (SpriteComponent* comp : spriteComps)
		{
			const glm::vec3 dirToCompCentre = comp->GetActor()->GetTransform().GetPosition() - mCameraComponent->GetCameraTransform().GetPosition();
			if (comp->GetVisibleTexture() != nullptr
				&& glm::dot(mCameraComponent->GetCameraTransform().GetForwardVector(), dirToCompCentre) > 0.0f)
			{
				const glm::vec3 actorPos = comp->GetActor()->GetTransform().GetPosition();
				const float dist = glm::length(actorPos - camPos);
				renderObjects.emplace(dist, SpriteRenderObject(comp->GetActor()->GetTransform().GetPosition(), comp->GetVisibleTexture()));
			}
		}
		
		/*** Camera space ***/
		const glm::mat4 invCamRot = glm::inverse(camRot);
		const float camAspect = (float)texWidth / texHeight;
		const float camHeight = camWidth / camAspect;
		const float d = (camWidth / 2.0f) / tanf(mFOV * 0.5f * 3.141592654 / 180.0f); // distance from eye
		const glm::vec3 camForward = camRot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		const glm::vec3 camRight = camRot * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		const glm::vec3 camUp = camRot * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		const glm::vec3 camRightScaled = camRight * camWidth * 0.5f;
		const glm::vec3 camUpScaled = camUp * camHeight * 0.5f;

		const glm::vec3 screenCentreWorld = camPos + camForward * d; // centre of the screen of the camera

		/*** Trace in 2D, for each x ***/
		#pragma omp parallel for // enable for better performance
		for (int x = 0; x < texWidth; x++)
		{
			const float relX = (float)x / texWidth;
			const float viewX = (relX - 0.5f) * 2.0f; // range: (-1.0, 1.0)

			const glm::vec3 pixelWorld = screenCentreWorld + viewX * camRightScaled;
			const glm::vec2 rayDir = glm::normalize(pixelWorld - camPos);

			// trace in X-Y plane
			const glm::vec2 rayStart = pixelWorld;
			float t = 0.0f;
			glm::vec2 currPos = rayStart;
			const float xIntersOffset = 1.0f / fmaxf(fabsf(rayDir.x), 0.00001f);	// distance between each intersection at the X-axis
			const float yIntersOffset = 1.0f / fmaxf(fabsf(rayDir.y), 0.00001f);	// distance between each intersection at the Y-axis
			float tNextXIntersect = fabsf(rayDir.x) < 0.0001f ? 99999.0f : ((rayDir.x > 0 ? floorf(currPos.x + 1.0f) : floorf(currPos.x)) - currPos.x) / rayDir.x;	 // t at next intersection point with X-axis
			float tNextYIntersect = fabsf(rayDir.y) < 0.0001f ? 99999.0f : ((rayDir.y > 0 ? floorf(currPos.y + 1.0f) : floorf(currPos.y)) - currPos.y) / rayDir.y;; // t at next intersection point with Y-axis
			bool xAxisInters;

			char gridCellValue = 0; // hit result (0 == nothing)

			/*** Jump to nearest grid cell intersection (inters. with x-axis or y-axis) something is hit ***/
			while (t < 100.0f)
			{
				if (tNextXIntersect < tNextYIntersect)
				{
					t = tNextXIntersect;
					tNextXIntersect += xIntersOffset;
					xAxisInters = true;
				}
				else
				{
					t = tNextYIntersect;
					tNextYIntersect += yIntersOffset;
					xAxisInters = false;
				}

				// update current position
				currPos = rayStart + rayDir*(t*1.01f); // TODO: if X-axis intersect, add extra tolerance in X component
				const int gridX = (int)currPos.x;
				const int gridY = (int)currPos.y;

				// check new grid cell:
				if (gridY >= mLevel->GetDimensionY() || gridX >= mLevel->GetDimensionX() || gridY < 0 || gridX < 0)
					break;

				gridCellValue = mLevel->GetWallMapCell(gridX, gridY);
				if (gridCellValue != 0) // hit something!
				{
					break;
				}
			}

			const glm::vec3 currPosTop = glm::vec3(currPos.x, currPos.y, 1.0f);
			const glm::vec3 dirToCurrPosTop = glm::normalize(currPosTop - camPos);
			const float cosA = glm::dot(dirToCurrPosTop, camForward);
			const float dDivCosA = d / cosA;
			const glm::vec3 currPosTopProjCam = camPos + dirToCurrPosTop * dDivCosA;
			const float wallHalfHeight = currPosTopProjCam.z - camPos.z;
			const float wallHeight = wallHalfHeight * 2.0f;
			const int wallHeightScreenSpace = (wallHeight / camHeight) * texHeight;
			const int pixelsToSkip = texHeight - wallHeightScreenSpace;

			if (gridCellValue != 0)
			{
				const SDL_Surface* wallTextureSurface = mTextureSurfaceMap[gridCellValue];
				const int gridX = floorf(currPos.x);
				const int gridY = floorf(currPos.y);
				
				if (mLevel->IsInGrid(gridX, gridY))
				{
					/*** Draw wall ***/
					const int zMax = std::min(texHeight - pixelsToSkip / 2, texHeight);
					for (int z = fmaxf(pixelsToSkip / 2, 0); z < zMax; z++)
					{
						const int offset = (texWidth * 4 * z) + x * 4;

						const float realZ = 1.0f - ((float)(z - pixelsToSkip / 2) / wallHeightScreenSpace);
						const glm::vec2 uv = xAxisInters ? glm::vec2(currPos.y - gridY, 1.0f - realZ) : glm::vec2(currPos.x - gridX, 1.0f - realZ);
						const Uint32 pixelColour = getpixel(wallTextureSurface, uv.x * wallTextureSurface->w, uv.y * wallTextureSurface->h);
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

						mPixels[offset + 0] = b;
						mPixels[offset + 1] = g;
						mPixels[offset + 2] = r;

						mDepthBuffer[x] = t; // store depth
					}
				}
			}
			
			/*** Draw ceiling ***/
			const int zMax = std::min(pixelsToSkip / 2, texHeight);
			for (int z = 0; z < zMax; z++)
			{
				const float relZ = (float)z / texHeight;
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 pixelWorld2 = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 ceilRayDir = glm::normalize(pixelWorld2 - camPos);

				const float tRoof = (1.0f - camPos.z) / ceilRayDir.z;
				const glm::vec3 roofHit = camPos + ceilRayDir*tRoof;

				const int offset = (texWidth * 4 * z) + x * 4;

				const int gridX = floorf(roofHit.x);
				const int gridY = floorf(roofHit.y);

				if (mLevel->IsInGrid(gridX, gridY))
				{
					const char& ceilingTextureKey = mLevel->GetCeilingMapCell(gridX, gridY);

					if (ceilingTextureKey != 0)
					{
						const SDL_Surface* ceilingTextureSurface = mTextureSurfaceMap[ceilingTextureKey];
						const glm::vec2 uv(roofHit.x - gridX, roofHit.y - gridY);
						const Uint32 pixelColour = getpixel(ceilingTextureSurface, uv.x * ceilingTextureSurface->w, uv.y * ceilingTextureSurface->h);
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

						mPixels[offset + 0] = b;
						mPixels[offset + 1] = g;
						mPixels[offset + 2] = r;
					}
					else if (renderSkybox)
					{
						float tSkybox;
						const bool xAxisSkyboxInters = fabsf(ceilRayDir.x) > fabsf(ceilRayDir.y);
						if (xAxisSkyboxInters)
							tSkybox = (200.0f * (ceilRayDir.x > 0 ? 1.0f : -1.0f)) / ceilRayDir.x;
						else
							tSkybox = (200.0f * (ceilRayDir.y > 0 ? 1.0f : -1.0f)) / ceilRayDir.y;
						const glm::vec3 skyboxHit = camPos + ceilRayDir*tSkybox;

						const float u = !xAxisSkyboxInters ? (200.0f + skyboxHit.x - camPos.x) / 400.0f : (200.0f + skyboxHit.y - camPos.y) / 400.0f;
						const float v = 1.0f - (200.0f + skyboxHit.z - camPos.z) / 400.0f; // TODO: inverted??
						//const glm::vec2 uv(u, v - 1.0f*std::floorf(v)); // TODO
						const glm::vec2 uv(((!xAxisSkyboxInters && ceilRayDir.y < 0.0f) || xAxisSkyboxInters && ceilRayDir.x < 0.0f) ? (1.0f - u) : u, std::fabsf(v));
						const Uint32 pixelColour = getpixel(mSkyboxTexture, uv.x * mSkyboxTexture->w, uv.y * mSkyboxTexture->h);
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

						mPixels[offset + 0] = b;
						mPixels[offset + 1] = g;
						mPixels[offset + 2] = r;
					}
				}
			}

			/*** Draw floor ***/
			for (int z = wallHeightScreenSpace + pixelsToSkip / 2; z < texHeight; z++)
			{
				const float relZ = (float)z / texHeight;
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 pixelWorld2 = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 floorRayDir = glm::normalize(pixelWorld2 - camPos);

				const float tFloor = (0.0f - camPos.z) / floorRayDir.z;
				const glm::vec3 floorHit = camPos + floorRayDir*tFloor;

				const int offset = (texWidth * 4 * z) + x * 4;

				const int gridX = floorf(floorHit.x);
				const int gridY = floorf(floorHit.y);
				if (!mLevel->IsInGrid(gridX, gridY))
					continue;
				const char& floorTextureKey = mLevel->GetFloorMapCell(gridX, gridY);
				if (floorTextureKey == 0)
					continue;

				const SDL_Surface* floorTextureSurface = mTextureSurfaceMap[floorTextureKey];
				const glm::vec2 uv(floorHit.x - gridX, floorHit.y - gridY);
				const Uint32 pixelColour = getpixel(floorTextureSurface, uv.x * floorTextureSurface->w, uv.y * floorTextureSurface->h);
				const Uint8 r = pixelColour;
				const Uint8 g = *(((Uint8*)&pixelColour) + 1);
				const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

				mPixels[offset + 0] = b;
				mPixels[offset + 1] = g;
				mPixels[offset + 2] = r;
			}
		}

		/*** Render sprites ***/
		for (auto iter = renderObjects.rbegin(); iter != renderObjects.rend(); iter++)
		{
			const glm::vec3& spriteCentrePos = (*iter).second.mPosition;
			const Texture& spriteTexture = *(*iter).second.mTexture;
			const float spriteWidth = 1.0f;  // TODO
			const float spriteHeight = 1.0f; // TODO
			const glm::vec3 spriteLeftPos = spriteCentrePos - camRight * 0.5f;

			// calculate sprite pos local to camera space
			const glm::vec3 w1 = spriteCentrePos - screenCentreWorld;
			const glm::vec3 w2 = spriteLeftPos - screenCentreWorld;
			const glm::vec3 spriteCentreLocal = invCamRot * glm::vec4(w1.x, w1.y, w1.z, 0.0f);
			const glm::vec3 spriteLeftLocal = invCamRot * glm::vec4(w2.x, w2.y, w2.z, 0.0f);

			const glm::vec3 camPosLocal = glm::vec3(0.0f, 1.0f, 0.0f) * d * -1.0f;

			const glm::vec3 u1 = camPosLocal - spriteCentreLocal;
			const glm::vec3 u2 = camPosLocal - spriteLeftLocal;

			// b + zt = 0  =>  t = -b / z
			const float t1 = -1.0f * spriteCentreLocal.y / u1.y;
			const float t2 = -1.0f * spriteLeftLocal.y / u2.y;
			const glm::vec3 spriteCentreProj = spriteCentreLocal + u1 * t1; // projection of sprite centre onto camera
			const glm::vec3 spriteLeftProj = spriteLeftLocal + u2 * t2;		// projection of sprite leftmost pos onto camera

			const float spriteWidthProj = fabsf(spriteCentreProj.x - spriteLeftProj.x);
			const int spriteWidthScreenSpace = (spriteWidthProj / camWidth) * texWidth;
			const int spriteXOffset = (spriteCentreProj.x / camWidth) * texWidth;
			const int spriteZOffset = (spriteLeftProj.z / camHeight) * texHeight;
			const int startX = (texWidth / 2) + spriteXOffset - (spriteWidthScreenSpace / 2);
			const int endX = (texWidth / 2) + spriteXOffset + (spriteWidthScreenSpace / 2);

			const float distToCentreSprite = glm::length(w1);
			const float sqrDistToCentreSprite = distToCentreSprite * distToCentreSprite;

			if (endX >= 0 && startX < texWidth)
			{ 
				const float sizeRatio = spriteWidthProj / spriteWidth;
				const float spriteHeightProj = sizeRatio * spriteHeight;
				const int screenHeight = (spriteHeightProj / camHeight) * texHeight;
				const int startZ = std::max(0, (texHeight / 2) + spriteZOffset - (screenHeight / 2));
				const int endZ = std::min(texHeight - 1, (texHeight / 2) + spriteZOffset + (screenHeight / 2));

				const int startX2 = std::max(startX, 0);
				const int endX2 = std::min(endX, texWidth - 1);
				for (int x = startX2; x < endX2; x++)
				{
					const float relXOffset = ((x - startX) / (float)spriteWidthScreenSpace);
					const float uCoord = relXOffset * spriteTexture.GetSDLSurface()->w;
					const float xOFfsetFromCentre = (relXOffset - 0.5f) * spriteWidth; // dist from sprite centre

					// Compare depth to depth buffer
					const float sqrDistFromCam = (xOFfsetFromCentre * xOFfsetFromCentre) + sqrDistToCentreSprite;
					const float currDepth = mDepthBuffer[x];
					if (sqrDistFromCam > (currDepth * currDepth))
						continue; // we have already rendered a wall between the camera and the sprite
					
					// Render each pixel in Z-axis
					for (int z = startZ; z < endZ; z++)
					{
						const int offset = (texWidth * 4 * z) + x * 4;
						const float vCoord = (z - startZ) / (float)spriteWidthScreenSpace;
						const glm::vec2 uv(uCoord, vCoord);
						const Uint32 pixelColour = getpixel(spriteTexture.GetSDLSurface(), uv.x, uv.y * spriteTexture.GetSDLSurface()->h);
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);
						// TODO: do this when loading picture. Make sure RGB pictures hae RGBA with A=255 
						const Uint8 a = spriteTexture.GetSDLSurface()->format->BytesPerPixel == 3 ? 255 : *(((Uint8*)&pixelColour) + 3); // TODO: do this when loading picture. Make sure RGB pictures hae RGBA with A=255

						if (a > 0)
						{
							mPixels[offset + 0] = b;
							mPixels[offset + 1] = g;
							mPixels[offset + 2] = r;
						}
					}
				}
			}

		}

		SDL_SetRenderDrawColor(GGameEngine->GetWindow()->GetSDLRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(GGameEngine->GetWindow()->GetSDLRenderer());

		SDL_UpdateTexture(mRenderTexture, NULL, &mPixels[0], texWidth * 4);

		SDL_RenderCopy(GGameEngine->GetWindow()->GetSDLRenderer(), mRenderTexture, NULL, NULL);
		SDL_RenderPresent(GGameEngine->GetWindow()->GetSDLRenderer());

		mPixels = mClearPixels; // TODO
		mDepthBuffer = mClearDepthBuffer; // TODO

	}


	// from http://sdl.beuc.net/sdl.wiki/Pixel_Access
	Uint32 SceneRenderer::getpixel(const SDL_Surface *arg_surface, int arg_x, int arg_y)
	{
		const int& bpp = arg_surface->format->BytesPerPixel;
		Uint8 *pixel = (Uint8 *)arg_surface->pixels + arg_y * arg_surface->pitch + arg_x * bpp;

		switch (bpp) {
		case 1:
			return *pixel;
			break;

		case 2:
			return *(Uint16 *)pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
			else
				return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
			break;

		case 4:
			return *(Uint32 *)pixel;
			break;

		default:
			return 0;
		}
	}
}
