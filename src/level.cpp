
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

		std::ifstream levelFile(fullPath);
		if (!levelFile.is_open())
			return false;


		/**
		* Parse level config file.
		* TODO: MOVE TO A HELPER CLASS - so we can re-use this for other config files
		*/
		std::string currLine;
		std::string currSection;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sectionVarMap;
		while (std::getline(levelFile, currLine))
		{
			const char* cstrPtr = currLine.c_str();
			const size_t& len = currLine.size();
			size_t iLeft = 0;
			size_t iRight = 0;
			bool isSection = false;
			bool isVariableAssignment = false;
			std::string currVarName;
			while (iRight < len)
			{
				const char* currChar = cstrPtr + iRight;
				if (isSection)
				{
					if (*currChar == ']')
					{
						currSection = currLine.substr(iLeft, iRight - iLeft);
						break;
					}
				}
				else
				{
					if (*currChar == '[')
					{
						isSection = true;
						iLeft = iRight + 1;
					}
					if (*currChar == ':')
					{
						isVariableAssignment = true;
						currVarName = currLine.substr(iLeft, iRight - iLeft);
						iLeft = iRight + 1;
					}
				}

				iRight++;
			}

			if (isVariableAssignment && iLeft < iRight)
			{
				std::string varValue = currLine.substr(iLeft, 1 + iRight - iLeft);
				sectionVarMap[currSection][currVarName] = varValue;
			}
		}


		std::string wallMapName;
		std::string floorMapName;
		std::string ceilingMapName;
		int dimX = 0;
		int dimY = 0;

		/*** TODO: Create a config file parser, with functions to get variable values as specified type ***/
		auto levelSectionIter = sectionVarMap.find("level");
		if (levelSectionIter != sectionVarMap.end())
		{
			const std::unordered_map<std::string, std::string> levelSectionVarMap = (*levelSectionIter).second;
			auto wallMapVarIter = levelSectionVarMap.find("map_wall");
			if (wallMapVarIter != levelSectionVarMap.end())
			{
				wallMapName = (*wallMapVarIter).second;
			}
			auto dimXVarIter = levelSectionVarMap.find("dim_x");
			if (dimXVarIter != levelSectionVarMap.end())
			{
				std::string strDimX = (*dimXVarIter).second;
				try
				{
					dimX = std::atoi(strDimX.c_str());
				}
				catch (std::invalid_argument ex){}
			}
			auto dimYVarIter = levelSectionVarMap.find("dim_y");
			if (dimYVarIter != levelSectionVarMap.end())
			{
				std::string strDimY = (*dimYVarIter).second;
				try
				{
					dimY = std::atoi(strDimY.c_str());
				}
				catch (std::invalid_argument ex) {}
			}
		}
		else
		{
			return false;
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
