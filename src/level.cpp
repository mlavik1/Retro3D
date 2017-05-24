
#include "level.h"

#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "st_assert.h"

namespace Retro3D
{
	Level::Level()
	{

	}

	void Level::setDimension(int arg_x, int arg_y)
	{
		mDimX = arg_x;
		mDimY = arg_y;
		mWallMap.resize(arg_x * arg_y);
		mFloorMap.resize(arg_x * arg_y);
		mCeilingMap.resize(arg_x * arg_y);
	}

	bool Level::LoadLevel(const char* arg_name)
	{
		std::string fullPath = std::string("resources//levels//") + std::string(arg_name) + std::string(".level");


		ConfigReader levelReader;
		if (!levelReader.ReadFile(fullPath.c_str()))
			return false;

		std::string wallMapName;
		std::string floorMapName;
		std::string ceilingMapName;
		int dimX = 0;
		int dimY = 0;

		levelReader.GetString("level", "map_wall", wallMapName);
		levelReader.GetString("level", "map_floor", floorMapName);
		levelReader.GetString("level", "map_ceiling", ceilingMapName);
		levelReader.GetInt("level", "dim_x", dimX);
		levelReader.GetInt("level", "dim_x", dimY);

		auto textureMap = levelReader.GetSectionAsMap("textures");
		for (auto texturePtr : textureMap)
		{

		}

		// set map dimension
		if (dimX > 0 && dimY > 0)
			setDimension(dimX, dimY);
		else
			return false;

		if (wallMapName != "")
			loadMap(wallMapName.c_str(), MapType::WallMap);

		return true;
	}


	bool Level::loadMap(const char* arg_filename, MapType arg_type)
	{
		std::vector<int>* mapArray = nullptr;
		switch (arg_type)
		{
		case Retro3D::WallMap:
			mapArray = &mWallMap;
			break;
		case Retro3D::FloorMap:
			mapArray = &mFloorMap;
			break;
		case Retro3D::CeilingMap:
			mapArray = &mCeilingMap;
			break;
		default:
			return false;
			break;
		}

		std::string fullPath = std::string("resources//levels//") + std::string(arg_filename);
		std::ifstream mapFIle(fullPath);
		if (!mapFIle.is_open())
			return false;

		int i_y = mDimY - 1;
		std::string currLine;
		while (std::getline(mapFIle, currLine))
		{
			std::vector<std::string> cells;
			std::stringstream ss;
			ss.str(currLine);
			std::string currCell;
			int i_x = 0;
			while (std::getline(ss, currCell, ' '))
			{
				try
				{
					LOG_INFO() << "";
					__AssertComment(i_x < mDimX && i_y < mDimY, "Invalid map file");
					int cellValue = std::atoi(currCell.c_str());
					const int& mapIndex = GetMapIndex(i_x, i_y);
					(*mapArray)[mapIndex] = cellValue;
					i_x++;
				}
				catch (std::invalid_argument ex) {}
			}
			i_y--;
		}

		__AssertComment(i_y == -1, "Invalid map file");

		return true;
	}

}
