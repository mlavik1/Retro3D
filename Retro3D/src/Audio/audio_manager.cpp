#include "audio_manager.h"
#include "audio_track.h"
#include "sdl2/SDL_mixer.h"
#include "Resource/audio_res.h"
#include "Resource/res_ptr.h"
#include "Engine/game_engine.h"
#include "Resource/resource_manager.h"

namespace Retro3D
{
	void AudioManager::PlayAudioTrack(AudioTrack* arg_track)
	{
		if (arg_track->mAudioRes.IsValid())
		{
			int channel = Mix_PlayChannel(-1, arg_track->mAudioRes->GetAudioChunk(), arg_track->mLoop ? -1 : 0);
			arg_track->mCurrentChannel = channel;
		}
	}

	void AudioManager::PlayAudioFile(std::string arg_file)
	{
		ResPtr<AudioRes> audioRes = GGameEngine->GetResourceManager()->LoadResource<AudioRes>(arg_file);
		if (audioRes.IsValid())
		{
			AudioTrack audioTrack(audioRes.Get());
			PlayAudioTrack(&audioTrack);
		}
	}
}
