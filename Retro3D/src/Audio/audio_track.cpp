#include "audio_track.h"

namespace Retro3D
{
	AudioTrack::AudioTrack(AudioRes* arg_audio)
	{
		mAudioRes = arg_audio;
	}

	void AudioTrack::SetLooping(bool arg_loop)
	{
		mLoop = arg_loop;
	}
}
