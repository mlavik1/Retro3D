#include "scene_renderer.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "Engine/game_engine.h"
#include "Window/window.h"
#include "World/level.h"
#include "Misc/st_assert.h"
#include <algorithm>
#include "Component/camera_component.h" // TEMP
#include "Component/sprite_component.h" // TEMP
#include "World/world.h"
#include <map>
#include "Actor/actor.h"
#include "Actor/player_controller.h" // TEMP
#include <API/SDL/sdl_render_target.h>
#include "Misc/path_utils.h"
#include <cmath>

/* BEGIN: TEMP FOR TESTING */
float camWidth = 0.2f;

/* END: TEMP FOR TESTING */

namespace Retro3D
{
	// modified version of http://sdl.beuc.net/sdl.wiki/Pixel_Access
	static inline Uint32 getpixel(const SDL_Surface* arg_surface, const int arg_x, const int arg_y)
	{
		const int bpp = arg_surface->format->BytesPerPixel;
		const Uint8* pixel = (Uint8*)arg_surface->pixels + arg_y * arg_surface->pitch + arg_x * bpp;

		switch (bpp)
		{
		case 1:
			return *pixel;
			break;
		case 2:
			return *(Uint16 *)pixel;
			break;
		case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
#else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
#endif
			break;
		case 4:
			return *(Uint32*)pixel;
			break;
		default:
			return 0;
		}
	}

	SceneRenderer::SceneRenderer()
	{
		mRenderTexture = nullptr;

        SetResolution(320, 200);

        RecalculateLightIntensities();
	}


	SceneRenderer::~SceneRenderer()
	{
	}

    void SceneRenderer::RecalculateLightIntensities()
    {
        for (int i = 0; i < 2048; i++)
        {
            mLightIntensities[i] = mAmbientLight / std::pow(std::sqrt(1.0f + i * 0.05f), mLightFade);
        }
    }

    void SceneRenderer::SetResolution(int xRes, int yRes)
    {
        mTexWidth = xRes;
        mTexHeight = yRes;

        /*** Set up pixel arrays ***/
        mClearPixels = std::vector<unsigned char>(mTexWidth * mTexHeight * 4, 0); // clear pixel array. "pixels" array is set to this before rendering
        for (size_t i = 0; i < mClearPixels.size(); i++)
        {
            mClearPixels[i] = SDL_ALPHA_OPAQUE; // fill with 255, so we won't have to set the alpha when rendering
        }
        mPixels = mClearPixels; // pixel array containing the rendered scene

        // Set up and clear the depth buffer
        mClearDepthBuffer = std::vector<float>(mTexWidth, 0);
        for (size_t i = 0; i < mClearDepthBuffer.size(); i++)
        {
            mClearDepthBuffer[i] = 10000.0f;
        }
        mDepthBuffer = mClearDepthBuffer;
    }

	/**
	* Set level to render
	**/
	void SceneRenderer::SetLevel(Level* arg_level)
	{
		mLevel = arg_level;
		
		mLevelDataLoaded = mLevel->IsLoaded();

		if (mLevelDataLoaded)
		{
			LoadLevelData();
		}
	}

	void SceneRenderer::LoadLevelData()
	{
        // null-initialise texture array
        for (size_t i = 0; i < 256; i++)
        {
            mTextureSurfaceMap[i] = nullptr;
        }

        // register all textures used in the level
		for (auto textureKeyPair : mLevel->GetTextureMap())
		{
			std::string fullPath = PathUtils::CombinePaths(GGameEngine->GetProjectDirectory(), textureKeyPair.second);
			SDL_Surface* surface = IMG_Load(fullPath.c_str());
			__AssertComment(surface != nullptr, std::string("Unable to load texture: " + fullPath)); // todo
			mTextureSurfaceMap[textureKeyPair.first] = surface;
		}
		if (mLevel->GetSkyboxTexture() != "")
		{
			mSkyboxTexture = IMG_Load(PathUtils::CombinePaths(GGameEngine->GetProjectDirectory(), mLevel->GetSkyboxTexture()).c_str());
			__Assert(mSkyboxTexture != nullptr);
		}

		float newFov;
		if (GGameEngine->GetGameConfig().GetFloat("camera", "fov", newFov))
		{
			mFOV = newFov;
		}

		mLevelDataLoaded = true;
	}

	/**
	* Set active camera component to render from.
	**/
	void SceneRenderer::SetCameraComponent(CameraComponent* arg_comp)
	{
		mCameraComponent = arg_comp;
	}

    void SceneRenderer::SetLightFade(float fade)
    {
        mLightFade = std::fmaxf(fade, 0.0f);
        RecalculateLightIntensities();
    }

    void SceneRenderer::SetAmbientLight(float light)
    {
        mAmbientLight = std::fmaxf(std::fminf(light, 1.0f), 0.0f);
        RecalculateLightIntensities();
    }

	/**
	* Render the whole scene.
	* Will render walls, ceiling, floor, skybox and sprites
	**/
	void SceneRenderer::RenderScene(IRenderTarget* arg_rendertarget)
	{
		if (mLevel == nullptr || !mLevel->IsLoaded())
		{
			return;
		}
		else if(!mLevelDataLoaded)
		{
			LoadLevelData();
		}

		if (!mCameraComponent.IsValid())
		{
			if (GGameEngine->GetPlayerController() && GGameEngine->GetPlayerController()->GetPlayer())
			{
				for (CameraComponent* comp : ((Actor*)GGameEngine->GetPlayerController()->GetPlayer())->GetComponents<CameraComponent>())
				{
					mCameraComponent = comp;
					break;
				}
			}
		}

		if (!mCameraComponent.IsValid())
		{
			return;
		}

		if (mRenderTexture != nullptr)
		{
			SDL_DestroyTexture(mRenderTexture);
		}

		ISDLRenderTarget* SDLRenderTarget = dynamic_cast<ISDLRenderTarget*>(arg_rendertarget);

		if (!SDLRenderTarget)
		{
			LOG_ERROR() << "Unhandled render target";
			return;
		}

		/*** Set up the render target texture ***/
		mRenderTexture = SDL_CreateTexture(SDLRenderTarget->GetSDLRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, mTexWidth, mTexHeight);

		glm::vec3 camPos = mCameraComponent->GetCameraTransform().GetPosition();
		glm::mat4 camRot = mCameraComponent->GetCameraTransform().GetRotation();
        
        // TODO (do before loop)
        camPos.z = std::fminf(1.0f, camPos.z);
        camPos.z = std::fmaxf(0.1f, camPos.z);

		const std::string& skyboxTexture = mLevel->GetSkyboxTexture();
		bool renderSkybox = skyboxTexture != "";

        unsigned char* pixelData = mPixels.data();

		// Setup sprite render objects - TODO: don't do this here!
		const std::vector<SpriteComponent*> spriteComps = GGameEngine->GetWorld()->GetComponentsOfType<SpriteComponent>();
		std::multimap<float, SpriteRenderObject> renderObjects;
		for (SpriteComponent* comp : spriteComps)
		{
			const glm::vec3 dirToCompCentre = comp->GetActor()->GetTransform().GetPosition() - mCameraComponent->GetCameraTransform().GetPosition();
			if (comp->GetTexture() != nullptr
				&& glm::dot(mCameraComponent->GetCameraTransform().GetForwardVector(), dirToCompCentre) > 0.0f)
			{
				const glm::vec3 actorPos = comp->GetActor()->GetTransform().GetPosition();
				const float dist = glm::length(actorPos - camPos);
				renderObjects.emplace(dist, SpriteRenderObject(comp->GetActor()->GetTransform().GetPosition(), comp->GetTexture(), comp->mUVOffset, comp->mUVScale));
			}
		}
		
		/*** Camera space ***/
		const glm::mat4 invCamRot = glm::inverse(camRot);
		const float camAspect = (float)mTexWidth / mTexHeight;
		const float camHeight = camWidth / camAspect;
		const float d = (camWidth / 2.0f) / tanf(mFOV * 0.5f * 3.141592654f / 180.0f); // distance from eye
		const glm::vec3 camForward = camRot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        const glm::vec2 camForward2D = glm::vec2(camForward.x, camForward.y);
        const glm::vec3 camRight = camRot * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		const glm::vec3 camUp = camRot * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		const glm::vec3 camRightScaled = camRight * camWidth * 0.5f;
		const glm::vec3 camUpScaled = camUp * camHeight * 0.5f;

		const glm::vec3 screenCentreWorld = camPos + camForward * d; // centre of the screen of the camera

        const float camToScreenRatio = mTexHeight / camHeight;

		/*** Trace in 2D, for each x ***/
		for (int x = 0; x < mTexWidth; x++)
		{
			const float relX = (float)x / mTexWidth;
			const float viewX = (relX - 0.5f) * 2.0f; // range: (-1.0, 1.0)

			const glm::vec3 pixelWorld = screenCentreWorld + viewX * camRightScaled;
            const float pixelDist = glm::length(pixelWorld - camPos);
			const glm::vec2 rayDir = (pixelWorld - camPos) / pixelDist;

			// trace in X-Y plane
			const glm::vec2 rayStart = pixelWorld;
			float t = 0.0f;
			glm::vec2 currPos = rayStart;
			const float xIntersOffset = 1.0f / std::max(std::abs(rayDir.x), 0.00001f);	// distance between each intersection at the X-axis
			const float yIntersOffset = 1.0f / std::max(std::abs(rayDir.y), 0.00001f);	// distance between each intersection at the Y-axis
			float tNextXIntersect = std::abs(rayDir.x) < 0.0001f ? 99999.0f : ((rayDir.x > 0 ? std::floor(currPos.x + 1.0f) : std::floor(currPos.x)) - currPos.x) / rayDir.x;	 // t at next intersection point with X-axis
			float tNextYIntersect = std::abs(rayDir.y) < 0.0001f ? 99999.0f : ((rayDir.y > 0 ? std::floor(currPos.y + 1.0f) : std::floor(currPos.y)) - currPos.y) / rayDir.y;; // t at next intersection point with Y-axis
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

            // TODO: Make this adjustable
            const float wallHeight = 1.0f;

            // Get wall bottom position and project it onto the camera
            const float cosA = glm::dot(rayDir, camForward2D); // angle between ray dir and cam forward
            const float hyp = t + pixelDist; // distance to currPos (pixelDist = distance from camera to pixel on view plane; t = distance from view plane to wall)
            const float distPerp = cosA * hyp; // adj side = perpendicular distance (to view plane)
            const float projDistFactor = 1.0f / (distPerp / d);
            const float wallHeightProj = wallHeight * projDistFactor; // d = perpendicular distance from cam to view plane (depends on FOV)
            const float wallBottomProj = camPos.z * projDistFactor;

			const int wallHeightScreenSpace = static_cast<int>(wallHeightProj * camToScreenRatio);
            const int wallBottomScreenSpace = mTexHeight / 2 + static_cast<int>(wallBottomProj * camToScreenRatio);
            const int wallTopScreenSpace = wallBottomScreenSpace - wallHeightScreenSpace; //wallBottomScreenSpace - wallHeightScreenSpace;

            const float lightIntensity = GetLightIntensity(currPos - glm::vec2(camPos));

			if (gridCellValue != 0)
			{
				const SDL_Surface* wallTextureSurface = mTextureSurfaceMap[gridCellValue];
				const int gridX = static_cast<int>(std::floor(currPos.x));
				const int gridY = static_cast<int>(std::floor(currPos.y));
				
				if (mLevel->IsInGrid(gridX, gridY))
				{
					/*** Draw wall ***/
                    const int zMin = std::max(wallTopScreenSpace, 0);
					const int zMax = std::min(wallBottomScreenSpace, mTexHeight - 1);
                    assert(zMax >= zMin);
					for (int z = zMin; z < zMax; z++)
					{
						const int offset = (mTexWidth * 4 * z) + x * 4;

                        const float tZ = (z - wallTopScreenSpace) / (float)(wallBottomScreenSpace - wallTopScreenSpace);
                        const float v = 1.0f - std::min(wallHeight * tZ, 1.0f);
                        const float u = xAxisInters ? currPos.y - gridY : currPos.x - gridX;
						const Uint32 pixelColour = getpixel(wallTextureSurface, static_cast<int>(u * (wallTextureSurface->w - 1)), static_cast<int>(v * (wallTextureSurface->h - 1)));
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

                        pixelData[offset] = b * lightIntensity;
                        pixelData[offset + 1] = g * lightIntensity;
                        pixelData[offset + 2] = r * lightIntensity;
					}
                    mDepthBuffer[x] = t; // store depth
				}
			}
			
			/*** Draw ceiling ***/
			for (int z = 0; z < wallTopScreenSpace; z++)
			{
				const float relZ = (float)z / (mTexHeight - 1);
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 ceilPixelWorld = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 ceilRayDir = (ceilPixelWorld - camPos);

				const float tRoof = (1.0f - camPos.z) / ceilRayDir.z;
				const glm::vec3 roofHit = camPos + ceilRayDir*tRoof;

				const int offset = (mTexWidth * 4 * z) + x * 4;

				const int gridX = static_cast<int>(std::floor(roofHit.x));
				const int gridY = static_cast<int>(std::floor(roofHit.y));

				if (mLevel->IsInGrid(gridX, gridY))
				{
                    const float lightIntensity = GetLightIntensity(roofHit - camPos);

					const char ceilingTextureKey = mLevel->GetCeilingMapCell(gridX, gridY);

					if (ceilingTextureKey != 0)
					{
						const SDL_Surface* ceilingTextureSurface = mTextureSurfaceMap[ceilingTextureKey];
						const glm::vec2 uv(roofHit.x - gridX, roofHit.y - gridY);
						const Uint32 pixelColour = getpixel(ceilingTextureSurface, static_cast<int>(uv.x * ceilingTextureSurface->w), static_cast<int>(uv.y * ceilingTextureSurface->h));
                        
                        const Uint8 r = *(((Uint8*)&pixelColour));
                        const Uint8 g = *(((Uint8*)&pixelColour) + 1);
                        const Uint8 b = *(((Uint8*)&pixelColour) + 2);

                        pixelData[offset] = b * lightIntensity; // b
                        pixelData[offset + 1] = g * lightIntensity; // g
                        pixelData[offset + 2] = r * lightIntensity; // r
					}
					else if (renderSkybox)
					{
						float tSkybox;
						const bool xAxisSkyboxInters = std::abs(ceilRayDir.x) > std::abs(ceilRayDir.y);
						if (xAxisSkyboxInters)
							tSkybox = (200.0f * (ceilRayDir.x > 0 ? 1.0f : -1.0f)) / ceilRayDir.x;
						else
							tSkybox = (200.0f * (ceilRayDir.y > 0 ? 1.0f : -1.0f)) / ceilRayDir.y;
						const glm::vec3 skyboxHit = camPos + ceilRayDir*tSkybox;

						const float u = !xAxisSkyboxInters ? (200.0f + skyboxHit.x - camPos.x) / 400.0f : (200.0f + skyboxHit.y - camPos.y) / 400.0f;
						const float v = 1.0f - (200.0f + skyboxHit.z - camPos.z) / 400.0f; // TODO: inverted??
						//const glm::vec2 uv(u, v - 1.0f*std::floorf(v)); // TODO
						const glm::vec2 uv(((!xAxisSkyboxInters && ceilRayDir.y < 0.0f) || xAxisSkyboxInters && ceilRayDir.x < 0.0f) ? (1.0f - u) : u, std::abs(v));
						const Uint32 pixelColour = getpixel(mSkyboxTexture, static_cast<int>(uv.x * mSkyboxTexture->w), static_cast<int>(uv.y * mSkyboxTexture->h));

                        pixelData[offset] = *(((Uint8*)&pixelColour) + 2); // b
                        pixelData[offset + 1] = *(((Uint8*)&pixelColour) + 1); // g
                        pixelData[offset + 2] = pixelColour; // r
					}
				}
			}

			/*** Draw floor ***/
			for (int z = wallBottomScreenSpace; z < mTexHeight; z++)
			{
				const float relZ = (float)z / (mTexHeight - 1);
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 floorPixelWorld = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 floorRayDir = (floorPixelWorld - camPos);

				const float tFloor = (0.0f - camPos.z) / floorRayDir.z;
				const glm::vec3 floorHit = camPos + floorRayDir*tFloor;

				const int offset = (mTexWidth * 4 * z) + x * 4;

				const int gridX = static_cast<int>(std::floor(floorHit.x));
				const int gridY = static_cast<int>(std::floor(floorHit.y));
				if (!mLevel->IsInGrid(gridX, gridY))
					continue;
				const char floorTextureKey = mLevel->GetFloorMapCell(gridX, gridY);
				if (floorTextureKey == 0)
					continue;

                const float lightIntensity = GetLightIntensity(floorHit - camPos);

				const SDL_Surface* floorTextureSurface = mTextureSurfaceMap[floorTextureKey];
				const glm::vec2 uv(floorHit.x - gridX, floorHit.y - gridY);
				const Uint32 pixelColour = getpixel(floorTextureSurface, static_cast<int>(uv.x * floorTextureSurface->w), static_cast<int>(uv.y * floorTextureSurface->h));
                
                const Uint8 r = *(((Uint8*)&pixelColour));
                const Uint8 g = *(((Uint8*)&pixelColour) + 1);
                const Uint8 b = *(((Uint8*)&pixelColour) + 2);

                pixelData[offset] = b * lightIntensity; // b
                pixelData[offset + 1] = g * lightIntensity; // g
                pixelData[offset + 2] = r * lightIntensity; // r
			}
		}

		/*** Render sprites ***/
		for (auto iter = renderObjects.rbegin(); iter != renderObjects.rend(); iter++)
		{
            // ----- TODO: Clean up this godforsaken mess -----
            
            const glm::vec3& spriteCentrePos = (*iter).second.mPosition;
			const Texture& spriteTexture = *(*iter).second.mTexture;
			const float spriteWidth = 1.0f;  // TODO
			const float spriteHeight = 1.0f; // TODO
			const glm::vec3 spriteLeftPos = spriteCentrePos - camRight * 0.5f * spriteWidth;

            const glm::vec2 uvOffset = (*iter).second.mUVOffset;
            const glm::vec2 uvScale = (*iter).second.mUVScale;

			// calculate sprite pos local to camera space
			const glm::vec2 w1 = spriteCentrePos - screenCentreWorld;
			const glm::vec2 w2 = spriteLeftPos - screenCentreWorld;
			const glm::vec2 spriteCentreLocal = invCamRot * glm::vec4(w1.x, w1.y, 0.0f, 0.0f);
			const glm::vec2 spriteLeftLocal = invCamRot * glm::vec4(w2.x, w2.y, 0.0f, 0.0f);

			const glm::vec2 camPosLocal = glm::vec3(0.0f, -d, 0.0f);

			const glm::vec2 u1 = camPosLocal - spriteCentreLocal;
			const glm::vec2 u2 = camPosLocal - spriteLeftLocal;

			// b + zt = 0  =>  t = -b / z
            // (t = perpendicular distance)
			const float t1 = -1.0f * spriteCentreLocal.y / u1.y;
			const float t2 = -1.0f * spriteLeftLocal.y / u2.y;
			const glm::vec2 spriteCentreProj = spriteCentreLocal + u1 * t1; // projection of sprite centre onto camera
			const glm::vec2 spriteLeftProj = spriteLeftLocal + u2 * t2;		// projection of sprite leftmost pos onto camera

			const float spriteWidthProj = std::abs(spriteCentreProj.x - spriteLeftProj.x) * 2.0f;
            const float zOffsetProj = (camPos.z - spriteCentrePos.z) * (spriteWidthProj / spriteWidth);

			const int spriteWidthScreenSpace = static_cast<int>((spriteWidthProj / camWidth) * mTexWidth);
			const int spriteXOffset = static_cast<int>((spriteCentreProj.x / camWidth) * mTexWidth);
            const int spriteZOffset = static_cast<int>((zOffsetProj / camHeight) * mTexHeight);
			const int startX = (mTexWidth / 2) + spriteXOffset - (spriteWidthScreenSpace / 2);
			const int endX = (mTexWidth / 2) + spriteXOffset + (spriteWidthScreenSpace / 2);

			const float distToCentreSprite = glm::length(w1);
			const float sqrDistToCentreSprite = distToCentreSprite * distToCentreSprite;

            const float lightIntensity = GetLightIntensity(spriteCentrePos - camPos);

			if (endX >= 0 && startX < mTexWidth)
			{ 
				const float sizeRatio = spriteWidthProj / spriteWidth;
				const float spriteHeightProj = sizeRatio * spriteHeight;
				const int screenHeight = static_cast<int>((spriteHeightProj / camHeight) * mTexHeight);
				const int startZ = std::max(0, (mTexHeight / 2) + spriteZOffset - (screenHeight / 2));
				const int endZ = std::min(mTexHeight - 1, (mTexHeight / 2) + spriteZOffset + (screenHeight / 2));

				const int startX2 = std::max(startX, 0);
				const int endX2 = std::min(endX, mTexWidth - 1);
				for (int x = startX2; x < endX2; x++)
				{
                    const float relXOffset = ((x - startX) / (float)spriteWidthScreenSpace);
                    const float uNorm = relXOffset * uvScale.x + uvOffset.x;
					const float uCoord = uNorm * spriteTexture.GetSDLSurface()->w;
					const float xOFfsetFromCentre = (relXOffset - 0.5f) * spriteWidth; // dist from sprite centre

					// Compare depth to depth buffer
					const float sqrDistFromCam = (xOFfsetFromCentre * xOFfsetFromCentre) + sqrDistToCentreSprite;
					const float currDepth = mDepthBuffer[x];
					if (sqrDistFromCam > (currDepth * currDepth))
						continue; // we have already rendered a wall between the camera and the sprite
					
					// Render each pixel in Z-axis
					for (int z = startZ; z < endZ; z++)
					{
						const int offset = (mTexWidth * 4 * z) + x * 4;
                        const float vCoord = ((z - startZ) / (float)spriteWidthScreenSpace) * uvScale.y + uvOffset.y;
						const glm::vec2 uv(uCoord, vCoord);
						const Uint32 pixelColour = getpixel(spriteTexture.GetSDLSurface(), static_cast<int>(uv.x), static_cast<int>(uv.y * spriteTexture.GetSDLSurface()->h));
						const Uint8 r = pixelColour;
						const Uint8 g = *(((Uint8*)&pixelColour) + 1);
						const Uint8 b = *(((Uint8*)&pixelColour) + 2);
						// TODO: do this when loading picture. Make sure RGB pictures hae RGBA with A=255 
						const Uint8 a = spriteTexture.GetSDLSurface()->format->BytesPerPixel == 3 ? 255 : *(((Uint8*)&pixelColour) + 3); // TODO: do this when loading picture. Make sure RGB pictures hae RGBA with A=255

						if (a > 0)
						{
                            pixelData[offset + 0] = b * lightIntensity;
                            pixelData[offset + 1] = g * lightIntensity;
                            pixelData[offset + 2] = r * lightIntensity;
						}
					}
				}
			}

		}

		SDL_UpdateTexture(mRenderTexture, NULL, &mPixels[0], mTexWidth * 4);

		SDL_RenderCopy(SDLRenderTarget->GetSDLRenderer(), mRenderTexture, NULL, NULL);

		mPixels = mClearPixels; // TODO
		mDepthBuffer = mClearDepthBuffer; // TODO

	}
}
