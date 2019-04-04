#ifndef RETRO3D_AUDIORES_H
#define RETRO3D_AUDIORES_H

#include "resource.h"
#include <SDL2/SDL_mixer.h>

namespace Retro3D
{
	class AudioRes : public Resource
	{
	private:
		Mix_Chunk* mAudioChunk = nullptr;

	public:
		~AudioRes();

		virtual bool LoadResource(const std::string& arg_path) override;

		inline Mix_Chunk* GetAudioChunk() { return mAudioChunk; }

	};
}

#endif
