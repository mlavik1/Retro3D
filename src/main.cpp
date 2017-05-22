#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "sdl2/SDL.h"
#include "sdl2/sdl_image.h"
#include <iostream>
#include <vector>
#include <math.h>

#undef main // TEMP - TODO

#define MAP_DIM_X 15
#define MAP_DIM_Y 15

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
};

float fov = 60;
float camWidth = 0.2f;
glm::vec3 camPos;
glm::mat4 camRot;



// taken from http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}



int main(int argc, char** argv)
{
	/*** SDL initialisation ***/
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	SDL_Window* window = SDL_CreateWindow("2.5D test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,	-1,	SDL_RENDERER_ACCELERATED);
	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	std::cout << "Renderer: " << info.name << std::endl;
	std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}

	/*** Set up the render target texture ***/
	const int texWidth = 800;
	const int texHeight = 600;
	SDL_Texture* renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

	/*** Set up pixel arrays ***/
	std::vector<unsigned char> clearPixels(texWidth * texHeight * 4, 0); // clear pixel array. "pixels" array is set to this before rendering
	for (size_t i = 0; i < clearPixels.size(); i++)
	{
		clearPixels[i] = SDL_ALPHA_OPAQUE; // fill with 255, so we won't have to set the alpha when rendering
	}
	std::vector<unsigned char> pixels = clearPixels; // pixel array containing the rendered scene

	/*** Textures for wall/ceiling/floor ***/
	SDL_Surface* wallTextureSurface = IMG_Load("resources\\textures\\wall1.png");
	SDL_Surface* ceilingTextureSurface = IMG_Load("resources\\textures\\test2.png");
	SDL_Surface* floorTextureSurface = IMG_Load("resources\\textures\\floor1.png");
	
	/*** Position the camera ***/
	camPos = glm::vec3(2.0f, 1.0f, 0.5f);
	camRot = glm::mat4(1.0f);

	bool running = true;
	while (running)
	{
		const Uint64 start = SDL_GetPerformanceCounter();

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
		for(int x = 0; x < texWidth; x++)
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
				if (gridY >= MAP_DIM_Y || gridX >= MAP_DIM_X || gridY < 0 || gridX < 0)
					break;

				gridCellValue = map[gridY][gridX];
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
			for(int z = fmaxf(pixelsToSkip / 2, 0); z < texHeight - pixelsToSkip / 2; z++)
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

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_UpdateTexture(renderTexture, NULL, &pixels[0], texWidth * 4);

		SDL_RenderCopy(renderer, renderTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

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

			SDL_PumpEvents();
			const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

			if (keyboardState[SDL_SCANCODE_W])
			{
				camPos += camForward * movementSpeed;
			}
			if (keyboardState[SDL_SCANCODE_S])
			{
				camPos -= camForward * movementSpeed;
			}
			if (keyboardState[SDL_SCANCODE_D])
			{
				camPos += camRight * movementSpeed;
			}
			if (keyboardState[SDL_SCANCODE_A])
			{
				camPos -= camRight * movementSpeed;
			}
			if (keyboardState[SDL_SCANCODE_E])
			{
				camRot *= glm::rotate(-movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (keyboardState[SDL_SCANCODE_Q])
			{
				camRot *= glm::rotate(movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
