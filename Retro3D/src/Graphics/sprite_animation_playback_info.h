#ifndef RETRO3D_SPRITEANIMATIONPLAYBACKINFO_H
#define RETRO3D_SPRITEANIMATIONPLAYBACKINFO_H

namespace Retro3D
{
    struct SpriteAnimationPlaybackInfo
    {
    public:
        int mNumCols = 1;
        int mNumRows = 1;
        float mDuration = 1.0f;
        bool mLooping = false;

        float mCurrTime = 0.0f;
        int mCurrCol = 0;
        int mCurrRow = 0;
    };
}

#endif
