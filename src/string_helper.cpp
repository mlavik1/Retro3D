#include "string_helper.h"

namespace Retro3D
{
	float StringHelper::pow10Array[1 + RETRO3D_POW10_MAX_POW + -RETRO3D_POW10_MIN_POW] = { 0.0000000001f, 0.000000001f, 0.00000001f, 0.0000001f, 0.000001f, 0.00001f, 0.0001f, 0.001f, 0.01f, 0.1f, 1.0f, 10.0f, 100.0f, 1000.0f, 10000.0f, 100000.0f, 1000000.0f, 10000000.0f, 100000000.0f, 1000000000.0f, 10000000000.0f };

	bool StringHelper::ToInt(const char* arg_text, int& out_value)
	{
		if (*arg_text == '\0')
			return false;
		out_value = 0;

		const bool isNegative = arg_text[0] == '-';
		const size_t iFirst = isNegative ? 1 : 0;

		size_t iPos = iFirst;
		while(true)
		{
			const char& currChar = arg_text[iPos];
			if (currChar == '\0')
				break;
			iPos++;
		}
		iPos--;
		if (iPos > RETRO3D_POW10_MAX_POW)
			return false; // number is too high
		const size_t iLast = iPos;

		uint8_t currValue;
		while (iPos != -1)
		{
			const char& currChar = arg_text[iPos];
			if (!ToUint8(currChar, currValue))
				return false; // not a number
			out_value += currValue * GetFastPow10(iLast - iPos);
			iPos--;
		}
		if (isNegative)
			out_value *= -1.0f;
		return true;
	}

	bool StringHelper::ToFloat(const char* arg_text, float& out_value)
	{
		if (*arg_text == '\0')
			return false;
		out_value = 0.0f;

		const bool isNegative = arg_text[0] == '-';
		const size_t iFirst = isNegative ? 1 : 0;
		size_t iSeparator = -1;

		size_t iPos = iFirst;
		while (true)
		{
			const char& currChar = arg_text[iPos];
			switch (currChar)
			{
			case '.':
			case ',':
				iSeparator = iPos;
				break;
			case '\0':
				goto EndOfString;
				break;
			}
			iPos++;
		}
	EndOfString:
		iPos--;
		const size_t iLastInt = iSeparator == -1 ? iPos : iSeparator;
		int currPow = iLastInt - iPos;
		if (currPow < RETRO3D_POW10_MIN_POW)
		{
			iPos -= (RETRO3D_POW10_MAX_POW - currPow);
			currPow = iLastInt - iPos;
		}

		uint8_t currValue;
		while (iPos != -1)
		{
			if (iPos != iSeparator)
			{
				const char& currChar = arg_text[iPos];
				if (!ToUint8(currChar, currValue))
					return false; // not a number
				out_value += currValue * GetFastPow10(currPow);
				currPow++;
			}
			iPos--;
		}
		if (isNegative)
			out_value *= -1.0f;
		return true;
	}

	bool StringHelper::ToUint8(const char& arg_char, uint8_t& out_value)
	{
		switch (arg_char)
		{
		case '0':
			out_value = 0;
			break;
		case '1':
			out_value = 1;
			break;
		case '2':
			out_value = 2;
			break;
		case '3':
			out_value = 3;
			break;
		case '4':
			out_value = 4;
			break;
		case '5':
			out_value = 5;
			break;
		case '6':
			out_value = 6;
			break;
		case '7':
			out_value = 7;
			break;
		case '8':
			out_value = 8;
			break;
		case '9':
			out_value = 9;
			break;
		default:
			return false;
		}
		return true;
	}
}
