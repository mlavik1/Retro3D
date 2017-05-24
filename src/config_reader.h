#ifndef RETRO3D_CONFIGREADER_H
#define RETRO3D_CONFIGREADER_H

#include <string>
#include <unordered_map>

namespace Retro3D
{
	class ConfigReader
	{
	private:
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mSectionVarMap;
	
	public:
		bool ReadFile(const char* arg_filepath);
		std::unordered_map<std::string, std::string> GetSectionAsMap(const char* arg_section);
		bool GetString(const char* arg_section, const char* arg_variable, std::string& out_value);
		bool GetInt(const char* arg_section, const char* arg_variable, int& out_value);
	};
}

#endif

