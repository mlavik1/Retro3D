#include "audio_component.h"
#include "Resource/resource_manager.h"
#include "Engine/game_engine.h"
#include "Audio/audio_manager.h"

namespace Retro3D
{
	AudioTrack* AudioComponent::LoadAudio(std::string arg_file, std::string arg_name)
	{
		// TODO: can we do this async?  - block until loaded if we try to use the audio before it's done loading.
		ResPtr<AudioRes> audioRes = GGameEngine->GetResourceManager()->LoadResource<AudioRes>(arg_file);
		if (audioRes.IsValid())
		{
			auto existingTrack = mAudioTracks.find(arg_name);
			if (existingTrack != mAudioTracks.end())
			{
				delete existingTrack->second;
			}
			AudioTrack* audioTrack = new AudioTrack(audioRes.Get());
			mAudioTracks[arg_name] = audioTrack;
			return audioTrack;
		}
		return nullptr;
	}

	void AudioComponent::PlayAudioTrack(std::string arg_name)
	{
		auto trackIter = mAudioTracks.find(arg_name);
		if (trackIter != mAudioTracks.end())
		{
			AudioTrack* track = trackIter->second;
			GGameEngine->GetAudioManager()->PlayAudioTrack(track);
		}
	}

	void AudioComponent::PlayAllAudioTracks()
	{
		for (auto audioTrackIter : mAudioTracks)
		{
			GGameEngine->GetAudioManager()->PlayAudioTrack(audioTrackIter.second);
		}
	}

}
