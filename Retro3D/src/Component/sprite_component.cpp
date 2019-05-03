#include "sprite_component.h"

#include "Graphics/texture.h"
#include "Misc/debug.h"
#include <cmath>

namespace Retro3D
{
	IMPLEMENT_CLASS(SpriteComponent)

	SpriteComponent::SpriteComponent()
	{
        SetupAnimation(1, 1, 0.0f, false);
        mUVOffset = glm::vec2(0.0f, 0.0f);
        mUVScale = glm::vec2(1.0f, 1.0f);
	}

	void SpriteComponent::SetTexture(std::string arg_texture)
	{
        mTexture = new Texture();
		if (!mTexture->LoadTexture(arg_texture.c_str()))
		{
			delete mTexture;
            mTexture = nullptr;
		}
	}

	void SpriteComponent::OnStart()
	{
		Component::OnStart();
	}

	void SpriteComponent::OnTick(float arg_deltatime)
	{
        Component::OnTick(arg_deltatime);

        if (mAnimationPlaybackInfo.mNumCols > 1 && mAnimationPlaybackInfo.mCurrTime <= mAnimationPlaybackInfo.mDuration)
        {
            mAnimationPlaybackInfo.mCurrTime += arg_deltatime;
            if (mAnimationPlaybackInfo.mCurrTime > mAnimationPlaybackInfo.mDuration)
            {
                if(mAnimationPlaybackInfo.mLooping)
                    mAnimationPlaybackInfo.mCurrTime -= mAnimationPlaybackInfo.mDuration * (std::floorf(mAnimationPlaybackInfo.mCurrTime / mAnimationPlaybackInfo.mDuration));
            }

            float tTime = mAnimationPlaybackInfo.mCurrTime / mAnimationPlaybackInfo.mDuration;
            mAnimationPlaybackInfo.mCurrCol = (int)(mAnimationPlaybackInfo.mNumCols * tTime);
            mUVOffset = glm::vec2((float)mAnimationPlaybackInfo.mCurrCol / mAnimationPlaybackInfo.mNumCols, (float)mAnimationPlaybackInfo.mCurrRow / mAnimationPlaybackInfo.mNumRows);
            mUVScale = glm::vec2(1.0f / mAnimationPlaybackInfo.mNumCols, 1.0f / mAnimationPlaybackInfo.mNumRows);
        }
	}

    void SpriteComponent::SetupAnimation(int cols, int rows, float duration, bool loop)
    {
        mAnimationPlaybackInfo.mNumCols = cols;
        mAnimationPlaybackInfo.mNumRows = rows;
        mAnimationPlaybackInfo.mDuration = duration;
        mAnimationPlaybackInfo.mLooping = loop;
    }

    void SpriteComponent::PlayAnimation(int animationRow)
    {
        if (animationRow < mAnimationPlaybackInfo.mNumRows)
        {
            mAnimationPlaybackInfo.mCurrRow = animationRow;
            mAnimationPlaybackInfo.mCurrTime = 0.0f;
        }
        else
            LOG_WARNING() << "SpriteComponent::SetAnimation called with invalid row index: " << animationRow;
    }
}
