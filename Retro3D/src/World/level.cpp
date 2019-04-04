
#include "World/level.h"

#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "Misc/st_assert.h"
#include "Engine/game_engine.h"
#include "Engine/script_manager.h"
#include "chaiscript/chaiscript.hpp"

namespace Retro3D
{
	Level::Level()
	{
		mIsLoaded = false;
	}

	void Level::setDimension(int arg_x, int arg_y)
	{
		mDimX = arg_x;
		mDimY = arg_y;
		mWallMap.resize(arg_x * arg_y);
		mFloorMap.resize(arg_x * arg_y);
		mCeilingMap.resize(arg_x * arg_y);
	}
	 
	bool Level::LoadLevel(const std::string arg_name)
	{
		LOG_INFO() << "Loading level: " << arg_name;

		std::string fullPath = std::string("resources//levels//") + std::string(arg_name) + std::string(".level");

		ConfigReader levelReader;
		if (!levelReader.ReadFile(fullPath.c_str()))
		{
			LOG_ERROR() << "Failed to load level: " << arg_name;
			return false;
		}
		std::string wallMapName;
		std::string floorMapName;
		std::string ceilingMapName;
		std::string skyboxTexture;
		std::string chaiscriptName;
		int dimX = 0;
		int dimY = 0;

		levelReader.GetString("level", "map_wall", wallMapName);
		levelReader.GetString("level", "map_floor", floorMapName);
		levelReader.GetString("level", "map_ceiling", ceilingMapName);
		levelReader.GetString("level", "skybox", skyboxTexture);
		levelReader.GetInt("level", "dim_x", dimX);
		levelReader.GetInt("level", "dim_x", dimY);
		levelReader.GetString("level", "ChaiScript", chaiscriptName);

		auto textureMap = levelReader.GetSectionAsMap("textures");
		for (auto texturePtr : textureMap)
		{
			char key = texturePtr.first.c_str()[0];
			mTextureMap[key] = texturePtr.second;
		}

		// set map dimension
		if (dimX > 0 && dimY > 0)
			setDimension(dimX, dimY);
		else
			return false;

		if (wallMapName != "")
			loadMap(wallMapName.c_str(), MapType::WallMap);
		if (floorMapName != "")
			loadMap(floorMapName.c_str(), MapType::FloorMap);
		if (ceilingMapName != "")
			loadMap(ceilingMapName.c_str(), MapType::CeilingMap);

		mSkyboxTexture = skyboxTexture;

		if (chaiscriptName != "")
		{
			chaiscript::ChaiScript* chai = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				chai->eval(chaiscriptName + std::string("();"));
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in LoadLevel: " << ex.what();
			}
		}

		mIsLoaded = true;
		return true;
	}


	bool Level::loadMap(const char* arg_filename, MapType arg_type)
	{
		std::vector<char>* mapArray = nullptr;
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
		std::ifstream mapFile(fullPath.c_str());
		if (!mapFile.is_open())
		{
			LOG_ERROR() << "failed to load map: " << arg_filename;
			return false;
		}

		int i_y = mDimY - 1;
		std::string currLine;
		while (std::getline(mapFile, currLine))
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
					__AssertComment(i_x < mDimX && i_y < mDimY, "Invalid map file");
					char cellValue = currCell.c_str()[0];
					const int mapIndex = GetMapIndex(i_x, i_y);
					if (mTextureMap.find(cellValue) == mTextureMap.end())
						cellValue = 0;
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

	const bool Level::IsWalkableFloor(const int arg_x, const int arg_y) const
	{
		if (IsInGrid(arg_x, arg_y))
		{
			const int index = GetMapIndex(arg_x, arg_y);
			return mWallMap[index] == 0;
		}
		return false;
	}

}
