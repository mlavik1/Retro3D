#include "sprite_component.h"

#include "Graphics/texture.h"

namespace Retro3D
{
	IMPLEMENT_CLASS(SpriteComponent)

	SpriteComponent::SpriteComponent()
	{
		
	}

	void SpriteComponent::SetTexture(std::string arg_texture)
	{
		mVisibleTexture = new Texture();
		if (!mVisibleTexture->LoadTexture(arg_texture.c_str()))
		{
			delete mVisibleTexture;
			mVisibleTexture = nullptr;
		}
	}

	void SpriteComponent::OnStart()
	{
		Component::OnStart();
	}

	void SpriteComponent::OnTick(float arg_deltatime)
	{
		Component::OnTick(arg_deltatime);
	}

}
