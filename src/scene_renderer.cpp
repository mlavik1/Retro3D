#include "scene_renderer.h"

#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>
#include "game_engine.h"
#include "window.h"
#include "input_manager.h"
#include "level.h"


/* BEGIN: TEMP FOR TESTING */
//#define MAP_DIM_X 15
//#define MAP_DIM_Y 15

/*
int map[MAP_DIM_Y][MAP_DIM_X] = {
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,0,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,1,1,1,0,0,0,0,1,1,1,1,1,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,0,1,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,0,1,1,1,1,1,1,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,1,0,0,0,0,1 },
	{ 1,0,1,1,1,1,1,0,0,1,0,0,0,0,0 },
	{ 1,0,1,0,0,0,0,0,0,1,0,0,0,0,0 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 }
};*/

float fov = 60;
float camWidth = 0.2f;
glm::vec3 camPos;
glm::mat4 camRot;

const int texWidth = 800;
const int texHeight = 600;
SDL_Texture* renderTexture;

SDL_Surface* wallTextureSurface;
SDL_Surface* ceilingTextureSurface;
SDL_Surface* floorTextureSurface;

std::vector<unsigned char> pixels;
std::vector<unsigned char> clearPixels;

/* END: TEMP FOR TESTING */


namespace Retro3D
{

	SceneRenderer::SceneRenderer()
	{
		Window* window = GGameEngine->GetWindow();

		/*** Set up the render target texture ***/
		renderTexture = SDL_CreateTexture(window->GetSDLRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

		/*** Set up pixel arrays ***/
		clearPixels = std::vector<unsigned char>(texWidth * texHeight * 4, 0); // clear pixel array. "pixels" array is set to this before rendering
		for (size_t i = 0; i < clearPixels.size(); i++)
		{
			clearPixels[i] = SDL_ALPHA_OPAQUE; // fill with 255, so we won't have to set the alpha when rendering
		}
		pixels = clearPixels; // pixel array containing the rendered scene

														 /*** Textures for wall/ceiling/floor ***/
		wallTextureSurface = IMG_Load("resources\\textures\\wall1.png");
		ceilingTextureSurface = IMG_Load("resources\\textures\\test2.png");
		floorTextureSurface = IMG_Load("resources\\textures\\floor1.png");

		/*** Position the camera ***/
		camPos = glm::vec3(2.0f, 1.0f, 0.5f);
		camRot = glm::mat4(1.0f);
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::RenderScene()
	{
		const Uint64 start = SDL_GetPerformanceCounter();


		Level* currentLevel = GGameEngine->GetCurrentLevel();


		/*** Camera space ***/
		const float camAspect = (float)texWidth / texHeight;
		const float camHeight = camWidth / camAspect;
		const float d = (camWidth / 2.0f) / tanf(camAspect / 2.0f); // distance from eye
		const glm::vec3 camForward = camRot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		const glm::vec3 camRight = camRot * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		const glm::vec3 camUp = camRot * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		const glm::vec3 camRightScaled = camRight * camWidth * 0.5f;
		const glm::vec3 camUpScaled = camUp * camHeight * 0.5f;

		const glm::vec3 screenCentreWorld = camPos + camForward * d; // centre of the screen of the camera

		/*** Trace in 2D, for each x ***/
		//#pragma omp parallel for // ENABLE FOR PERFORMANCE GAIN!
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

			int gridCellValue = 0; // hti result (0 == nothing)

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
				const int gridX = floorf(currPos.x);
				const int gridY = floorf(currPos.y);

				// check new grid cell:
				if (gridY >= currentLevel->GetDimensionY() || gridX >= currentLevel->GetDimensionX() || gridY < 0 || gridX < 0)
					break;

				gridCellValue = currentLevel->GetWallMapCell(gridX, gridY);
				if (gridCellValue != 0) // hit something!
				{
					break;
				}
			}

			const float acosAngle = (glm::length(screenCentreWorld - camPos) / glm::length(pixelWorld - camPos));
			const float dist = glm::length((currPos - rayStart)) * acosAngle; // fish-eye correction
			const float height = 1.0f / dist; // TODO
			const int heightScreenSpace = height * texHeight;
			const int pixelsToSkip = texHeight - heightScreenSpace;

			/*** Draw wall ***/
			for (int z = fmaxf(pixelsToSkip / 2, 0); z < texHeight - pixelsToSkip / 2; z++)
			{
				const int offset = (texWidth * 4 * z) + x * 4;
				if (offset + 3 >= pixels.size())
					break; // TODO

				const float realZ = 1.0f - ((float)(z - pixelsToSkip / 2) / heightScreenSpace);
				const int gridX = floorf(currPos.x);
				const int gridY = floorf(currPos.y);
				const glm::vec2 uv = xAxisInters ? glm::vec2(currPos.y - gridY, 1.0f - realZ) : glm::vec2(currPos.x - gridX, 1.0f - realZ);
				const Uint32 pixelColour = getpixel(wallTextureSurface, uv.x * wallTextureSurface->w, uv.y * wallTextureSurface->h);
				const Uint8 r = pixelColour;
				const Uint8 g = *(((Uint8*)&pixelColour) + 1);
				const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

				pixels[offset + 0] = b;
				pixels[offset + 1] = g;
				pixels[offset + 2] = r;
			}

			/*** Draww ceiling ***/
			for (int z = 0; z < pixelsToSkip / 2; z++)
			{
				const float relZ = (float)z / texHeight;
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 pixelWorld2 = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 rayDir2 = glm::normalize(pixelWorld2 - camPos);

				const float tRoof = (1.0f - camPos.z) / rayDir2.z;
				const glm::vec3 roofHit = camPos + rayDir2*tRoof;

				const int offset = (texWidth * 4 * z) + x * 4;
				if (offset + 3 >= pixels.size())
					break; // TODO


				const int gridX = floorf(roofHit.x);
				const int gridY = floorf(roofHit.y);
				const glm::vec2 uv(roofHit.x - gridX, roofHit.y - gridY);
				const Uint32 pixelColour = getpixel(ceilingTextureSurface, uv.x * ceilingTextureSurface->w, uv.y * ceilingTextureSurface->h);
				const Uint8 r = pixelColour;
				const Uint8 g = *(((Uint8*)&pixelColour) + 1);
				const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

				pixels[offset + 0] = b;
				pixels[offset + 1] = g;
				pixels[offset + 2] = r;
			}

			/*** Draw floor ***/
			for (int z = heightScreenSpace + pixelsToSkip / 2; z < texHeight; z++)
			{
				const float relZ = (float)z / texHeight;
				const float viewZ = (relZ - 0.5f) * -2.0f; // range: (-1.0, 1.0)
				const glm::vec3 pixelWorld2 = pixelWorld + viewZ * camUpScaled;
				const glm::vec3 rayDir2 = glm::normalize(pixelWorld2 - camPos);

				const float tFloor = (0.0f - camPos.z) / rayDir2.z;
				const glm::vec3 floorHit = camPos + rayDir2*tFloor;

				const int offset = (texWidth * 4 * z) + x * 4;
				if (offset + 3 >= pixels.size())
					break; // TODO

				const int gridX = floorf(floorHit.x);
				const int gridY = floorf(floorHit.y);
				const glm::vec2 uv(floorHit.x - gridX, floorHit.y - gridY);
				const Uint32 pixelColour = getpixel(floorTextureSurface, uv.x * floorTextureSurface->w, uv.y * floorTextureSurface->h);
				const Uint8 r = pixelColour;
				const Uint8 g = *(((Uint8*)&pixelColour) + 1);
				const Uint8 b = *(((Uint8*)&pixelColour) + 2);;

				pixels[offset + 0] = b;
				pixels[offset + 1] = g;
				pixels[offset + 2] = r;
			}

		}

		SDL_SetRenderDrawColor(GGameEngine->GetWindow()->GetSDLRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(GGameEngine->GetWindow()->GetSDLRenderer());

		SDL_UpdateTexture(renderTexture, NULL, &pixels[0], texWidth * 4);

		SDL_RenderCopy(GGameEngine->GetWindow()->GetSDLRenderer(), renderTexture, NULL, NULL);
		SDL_RenderPresent(GGameEngine->GetWindow()->GetSDLRenderer());

		pixels = clearPixels; // TODO

		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		const float deltaSeconds = (end - start) / static_cast< float >(freq);
		std::cout << "Frame time: " << deltaSeconds * 1000.0 << "ms" << std::endl;

		/*** Handle keyboard input ***/
		{
			const glm::vec3 camForward = camRot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			const glm::vec3 camRight = camRot * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

			const float movementSpeed = deltaSeconds * 2.0f;

			InputManager* inputManager = GGameEngine->GetInputManager();

			if (inputManager->GetKey("w"))
			{
				camPos += camForward * movementSpeed;
			}
			if (inputManager->GetKey("s"))
			{
				camPos -= camForward * movementSpeed;
			}
			if (inputManager->GetKey("d"))
			{
				camPos += camRight * movementSpeed;
			}
			if (inputManager->GetKey("a"))
			{
				camPos -= camRight * movementSpeed;
			}
			if (inputManager->GetKey("e"))
			{
				camRot *= glm::rotate(-movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (inputManager->GetKey("q"))
			{
				camRot *= glm::rotate(movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
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
