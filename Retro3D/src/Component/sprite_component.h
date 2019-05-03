#ifndef RETRO3D_SPRITECOMPONENT_H
#define RETRO3D_SPRITECOMPONENT_H

#include "component.h"
#include <string>
#include "Graphics/sprite_animation_playback_info.h"
#include "glm/vec2.hpp"

namespace Retro3D
{
	class Texture;

	class SpriteComponent : public Component
	{
		DEFINE_CLASS(SpriteComponent, Component)

	public:
		SpriteComponent();

		void SetTexture(std::string arg_texture);
        void SetupAnimation(int cols, int rows, float duration, bool loop);
        void PlayAnimation(int animationRow);
		
        Texture* GetTexture() { return mTexture; }
        SpriteAnimationPlaybackInfo GetAnimationPlaybackInfo() { return mAnimationPlaybackInfo; }

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

        glm::vec2 mUVOffset;
        glm::vec2 mUVScale;

	private:
		Texture* mTexture = nullptr;
        SpriteAnimationPlaybackInfo mAnimationPlaybackInfo;
	};
}

#endif
