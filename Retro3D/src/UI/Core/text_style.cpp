#include "text_style.h"

namespace Retro3D
{
	TextStyle::TextStyle()
	{
		mFontName = "resources//fonts//msmincho.ttc"; // TODO: store default paths somewhere
		mFontSize = 12;
		mColour = Colour(1.0f, 1.0f, 1.0f);
		mWrapText = true;
	}

	void TextStyle::SetFontName(const std::string& arg_fontname)
	{
		mFontName = arg_fontname;
	}

	void TextStyle::SetFontSize(unsigned int arg_fontsize)
	{
		mFontSize = arg_fontsize;
	}

	void TextStyle::SetColour(const Colour& arg_colour)
	{
		mColour = arg_colour;
	}

	void TextStyle::SetWrapText(bool arg_wwap)
	{
		mWrapText = arg_wwap;
	}

}
