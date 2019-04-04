#ifndef RETRO3D_UI_IMAGE_VISUAL_H
#define RETRO3D_UI_IMAGE_VISUAL_H

/*=============================================================================================
Image Visual

A Visual that renders an image.
==============================================================================================*/

#include "UI/Visual/visual.h"
#include <string>
#include "Resource/texture_res.h"
#include "Resource/res_ptr.h"

namespace Retro3D
{
	class ImageVisual : public Visual
	{
	private:
		/** Path to the image file. */
		std::string mImagePath;

		/** Image Resource, used for rendering. */
		ResPtr<TextureRes> mImageRes;

	public:
		ImageVisual();
		~ImageVisual();

		/** Renders the image. */
		void RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams);

		/** Sets the path to the image file. */
		void SetImagePath(std::string arg_img);

		/** Gets the path to the image file. */
		std::string GetImagePath() { return mImagePath; }

		TextureRes* GetImageRes() { return mImageRes.IsValid() ? mImageRes.Get() : nullptr; }
	};
}

#endif
