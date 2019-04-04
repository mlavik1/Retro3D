#ifndef RETRO3D_AUDIOTRACK_H
#define RETRO3D_AUDIOTRACK_H

#include "Object/object.h"
#include "Resource/audio_res.h"
#include "Resource/res_ptr.h"

namespace Retro3D
{
	class AudioTrack : public Object
	{
		friend class AudioManager;

	private:
		ResPtr<AudioRes> mAudioRes;
		int mCurrentChannel = -1;
		bool mLoop = false;

	public:
		AudioTrack(AudioRes* arg_audio);

		void SetLooping(bool arg_loop);

	};
}

#endif
