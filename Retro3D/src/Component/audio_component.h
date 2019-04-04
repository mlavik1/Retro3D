#ifndef RETRO3D_AUDIOCOMPONENT_H
#define RETRO3D_AUDIOCOMPONENT_H

#include "component.h"
#include <unordered_map>
#include <string>
#include "Audio/audio_track.h"

namespace Retro3D
{
	class AudioComponent : public Component
	{
	private:
		std::unordered_map<std::string, AudioTrack*> mAudioTracks;

	public:
		/**
		* Tries to load the specified audio file, and adds it to map of audio tracks.
		* @param arg_file The audio file to load.
		* @param arg_name The unieque ID to use in map of audio tracks. Will overwrite existing
		**/
		AudioTrack* LoadAudio(std::string arg_file, std::string arg_name = "default");

		/**
		* Plays the specified audio track.
		**/
		void PlayAudioTrack(std::string arg_name);

		void PlayAllAudioTracks();
	};
}

#endif
