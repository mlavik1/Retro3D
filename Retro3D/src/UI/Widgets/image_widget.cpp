#include "image_widget.h"

namespace Retro3D
{
	ImageWidget::ImageWidget()
	{
		mColourVisual = new ColourVisual();
		mImageVisual = new ImageVisual();

		AddVisual(mColourVisual.Get());
		AddVisual(mImageVisual.Get());

		SetImageWidgetStyle(mImageWidgetStyle);
	}

	void ImageWidget::SetImagePath(const std::string &arg_path)
	{
		mImageWidgetStyle.SetImagePath(arg_path);
		SetImageWidgetStyle(mImageWidgetStyle);
	}

	void ImageWidget::SetColour(Colour arg_colour)
	{
		mImageWidgetStyle.SetColour(arg_colour);
		SetImageWidgetStyle(mImageWidgetStyle);
	}

	void ImageWidget::CreateContent()
	{

	}

	void ImageWidget::SetImageWidgetStyle(ImageWidgetStyle& arg_style)
	{
		mImageWidgetStyle = arg_style;

		mImageVisual->SetImagePath(arg_style.GetImagePath());
		mColourVisual->SetColour(arg_style.GetColour());

		mColourVisual->Activate(arg_style.GetColour().IsVisible());
		mImageVisual->Activate(arg_style.GetImagePath() != "");
	}

	void ImageWidget::setupVisuals()
	{
		mVisuals.clear();
	}
}
