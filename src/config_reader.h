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
		bool GetString(const char* arg_section, const char* arg_variable, std::string& out_value) const;
		bool GetInt(const char* arg_section, const char* arg_variable, int& out_value) const;
		bool GetFloat(const char* arg_section, const char* arg_variable, float& out_value) const;

		// std::string versions (used by ChaiScript)
		bool GetString_string(const std::string& arg_section, const std::string& arg_variable, std::string& out_value) const { return GetString(arg_section.c_str(), arg_variable.c_str(), out_value); }
		bool GetInt_string(const std::string& arg_section, const std::string& arg_variable, int& out_value) const { return GetInt(arg_section.c_str(), arg_variable.c_str(), out_value); }
		bool GetFloat_string(const std::string& arg_section, const std::string& arg_variable, float& out_value) const { return GetFloat(arg_section.c_str(), arg_variable.c_str(), out_value); }

	};
}

#endif

