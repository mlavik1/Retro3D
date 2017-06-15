#ifndef RETRO3D_SPRITECOMPONENT_H
#define RETRO3D_SPRITECOMPONENT_H

#include "component.h"
#include <string>

namespace Retro3D
{
	class Texture;

	class SpriteComponent : public Component
	{
		DEFINE_CLASS(SpriteComponent, Component)

	public:
		SpriteComponent();

		void SetTexture(std::string arg_texture);

		inline Texture* GetVisibleTexture() { return mVisibleTexture; }

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

	private:
		Texture* mVisibleTexture = nullptr;
	};
}

#endif
