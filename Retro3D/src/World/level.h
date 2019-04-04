#ifndef RETRO3D_LEVEL_H
#define RETRO3D_LEVEL_H

#include "Text/config_reader.h"

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
		bool mIsLoaded;

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

		bool LoadLevel(const std::string arg_name);

		bool IsLoaded() const { return mIsLoaded; }

		inline const int GetMapIndex(const int arg_x, const int arg_y) const
		{
			return arg_x + arg_y*mDimX;
		}

		inline const char GetWallMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mWallMap[index];
		}
		inline const char GetFloorMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mFloorMap[index];
		}
		inline const char GetCeilingMapCell(const int arg_x, const int arg_y) const
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mCeilingMap[index];
		}
		inline const bool IsInGrid(const int arg_x, const int arg_y) const
		{
			return arg_x >= 0 && arg_y >= 0 && arg_x < mDimX && arg_y < mDimY;
		}

		inline const int GetDimensionX() { return mDimX; }
		inline const int GetDimensionY() { return mDimY; }

		inline const std::unordered_map<char, std::string> GetTextureMap() { return mTextureMap; }

		inline const std::string& GetSkyboxTexture() { return mSkyboxTexture; }

		const bool IsWalkableFloor(const int arg_x, const int arg_y) const;
	};
}

#endif
