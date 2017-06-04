#ifndef RETRO3D_SPRITECOMPONENT_H
#define RETRO3D_SPRITECOMPONENT_H

#include "component.h"

namespace Retro3D
{
	class Texture;

	class SpriteComponent : public Component
	{
		DEFINE_CLASS(SpriteComponent, Component)

	public:
		SpriteComponent();

		void SetTexture(const char* arg_texture);

		inline Texture* GetVisibleTexture() { return mVisibleTexture; }

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

	private:
		Texture* mVisibleTexture = nullptr;
	};
}

#endif
