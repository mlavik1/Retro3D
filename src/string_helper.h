#ifndef RETRO3D_STRINGHELPER_H
#define RETRO3D_STRINGHELPER_H

/*=============================================================================================
Made this for educational purposes.

TODO: CONSIDER REMOVING
==============================================================================================*/

#include <string>
#include <vector>

#define RETRO3D_POW10_MIN_POW -10
#define RETRO3D_POW10_MAX_POW 10

namespace Retro3D
{
	class StringHelper
	{
	public:
		static bool ToInt(const char* arg_text, int& out_value);
		static bool ToFloat(const char* arg_text, float& out_value);

		static bool ToUint8(const char& arg_char, uint8_t& out_value);

		static inline float GetFastPow10(const int& arg_pow) { return pow10Array[arg_pow - RETRO3D_POW10_MIN_POW]; }

	private:
		static float pow10Array[1 + RETRO3D_POW10_MAX_POW + -RETRO3D_POW10_MIN_POW];
	};
}

#endif
