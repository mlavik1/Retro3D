#ifndef RETRO3D_AUDIOMANAGER_H
#define RETRO3D_AUDIOMANAGER_H

#include <string>

namespace Retro3D
{
	class AudioTrack;

	class AudioManager
	{
	public:
		void PlayAudioTrack(AudioTrack* arg_track);
		void PlayAudioFile(std::string arg_file);
	};
}

#endif
