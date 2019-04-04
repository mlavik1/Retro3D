#ifndef RETRO3D_CONFIGREADER_H
#define RETRO3D_CONFIGREADER_H

/*=============================================================================================
Config reader class.

Used for reading config files.
Config files can contain sections, and variables.

Example: (from GameConfig.ini)
[camera]
fov:90

Usage: Call ReadFile(filepath), and then access the variables by calling GetString/GetInt/GetFloat.
You may leave the section empty.

==============================================================================================*/

#include <string>
#include <unordered_map>

namespace Retro3D
{
	class ConfigReader
	{
	private:
		/** All variables, ordered by section. */
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mSectionVarMap;
	
		std::string mFilePath;

	public:
		ConfigReader();

		/** Loads the config file at the specified location. */
		bool ReadFile(std::string arg_filepath, bool arg_create = false);

		/** Gets all variables in the specified section. */
		std::unordered_map<std::string, std::string> GetSectionAsMap(const char* arg_section);

		/**
		* Gets the string value of the specified variable
		* @param  arg_section  The section of the variables. 
		* @param  arg_variables  The name of the variable.
		* @return The string value of the variales.
		*/
		bool GetString(const std::string& arg_section, const std::string& arg_variable, std::string& out_value) const;
		
		/**
		* Gets the int value of the specified variable
		* @param  arg_section  The section of the variables.
		* @param  arg_variables  The name of the variable.
		* @return The int value of the variales.
		*/
		bool GetInt(const std::string& arg_section, const std::string& arg_variable, int& out_value) const;
		
		/**
		* Gets the float value of the specified variable
		* @param  arg_section  The section of the variables.
		* @param  arg_variables  The name of the variable.
		* @return The float value of the variales.
		*/
		bool GetFloat(const std::string& arg_section, const std::string& arg_variable, float& out_value) const;

		void SetString(const std::string& arg_section, const std::string& arg_variable, const std::string& arg_value);

		void SetFloat(const std::string& arg_section, const std::string& arg_variable, const float arg_value);

		void SetInt(const std::string& arg_section, const std::string& arg_variable, const int arg_value);

		/**
		* Saves the config file to the previously set filepath.
		*/
		void Save();

		/**
		* Saves the config file to the specified file path.
		* @param  arg_filepath  Filepath to save to. Will overwrite existing files.
		*/
		void SaveToFile(std::string arg_filepath);

	};
}

#endif

