#ifndef RETRO3D_TEXTURE_H
#define RETRO3D_TEXTURE_H

struct SDL_Surface;

namespace Retro3D
{
	class Texture
	{
	public:
		Texture();
		~Texture();
		bool LoadTexture(const char* arg_file);

		inline SDL_Surface* GetSDLSurface() const { return mSDLSurface; } // TEMP - handle here

	private:
		SDL_Surface* mSDLSurface;
	};
}

#endif
