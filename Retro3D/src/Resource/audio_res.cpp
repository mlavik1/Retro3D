#include "audio_res.h"
#include "Misc/debug.h"

namespace Retro3D
{
	AudioRes::~AudioRes()
	{
		if (mAudioChunk != nullptr)
		{
			Mix_FreeChunk(mAudioChunk);
			mAudioChunk = nullptr;
		}
	}

	bool AudioRes::LoadResource(const std::string& arg_path)
	{
		mAudioChunk = Mix_LoadWAV(arg_path.c_str());
		if (mAudioChunk == nullptr)
		{
			LOG_ERROR() << "Failed to load WAV: " << arg_path;
			return false;
		}
		return true;
	}
}
