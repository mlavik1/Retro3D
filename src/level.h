#ifndef RETRO3D_LEVEL_H
#define RETRO3D_LEVEL_H

#include "config_reader.h"

#include <vector>

namespace Retro3D
{
	enum MapType
	{
		WallMap, FloorMap, CeilingMap
	};

	class Level
	{
	private:
		int mDimX;
		int mDimY;

		std::vector<char> mWallMap;
		std::vector<char> mCeilingMap;
		std::vector<char> mFloorMap;

		std::unordered_map<char, std::string> mTextureMap;
		std::string mSkyboxTexture;

		void setDimension(int arg_x, int arg_y);
		bool loadMap(const char* arg_filename, MapType arg_type);

	public:
		Level();

		bool LoadLevel(const char* arg_name);

		inline int GetMapIndex(const int& arg_x, const int& arg_y) const { return arg_x + arg_y*mDimX;; }

		inline const int GetWallMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mWallMap[index];
		}
		inline const int GetFloorMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mFloorMap[index];
		}
		inline const int GetCeilingMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mCeilingMap[index];
		}
		inline bool IsInGrid(const int arg_x, const int arg_y) const
		{
			return arg_x >= 0 && arg_y >= 0 && arg_x < mDimX && arg_y < mDimY;
		}

		inline const int& GetDimensionX() { return mDimX; }
		inline const int& GetDimensionY() { return mDimY; }

		inline const std::unordered_map<char, std::string> GetTextureMap() { return mTextureMap; }

		inline const std::string& GetSkyboxTexture() { return mSkyboxTexture; }
	};
}

#endif
