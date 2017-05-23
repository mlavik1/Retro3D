#ifndef RETRO3D_LEVEL_H
#define RETRO3D_LEVEL_H

#include "level.h"

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

		std::vector<int> mWallMap;
		std::vector<int> mCeilingMap;
		std::vector<int> mFloorMap;

		void setDimension(int arg_x, int arg_y);
		bool loadMap(const char* arg_filename, MapType arg_type);

	public:
		Level();

		bool LoadLevel(const char* arg_name);

		inline int GetMapIndex(const int& arg_x, const int& arg_y) { return arg_x + arg_y*mDimX;; }

	};
}

#endif
