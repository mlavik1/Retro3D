#include "script_manager.h"

#include <fstream>
#include <streambuf>
#include "Misc/debug.h"

#include "Misc/chai_bindings.h"

#include <queue>
#include <fstream>
#include <sstream>

namespace Retro3D
{
	ScriptManager::ScriptManager()
	{
		mChaiScript = new chaiscript::ChaiScript();

		ChaiBindings::AddBindings(mChaiScript);

		LoadScriptsFromIni();

	}

	ScriptManager::~ScriptManager()
	{
		delete mChaiScript;
		mChaiScript = nullptr;
	}

	void ScriptManager::RegisterScript(std::string arg_file)
	{
		if (mChaiScript != nullptr && mRegisteredScripts.find(arg_file) == mRegisteredScripts.end())
		{
			std::ifstream fileStream(arg_file);
			if (!fileStream.is_open())
			{
				LOG_ERROR() << "Did not find script file: " << arg_file;
				return;
			}
			std::string strScript((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());

			// UTF? => remove first 3 bytes
			if (strScript.compare(0, 3, "\xEF\xBB\xBF") == 0)
			{
				strScript.erase(0, 3);
			}

			try
			{
				mChaiScript->eval(strScript);
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Failed to evaluate script: " << arg_file << " - " << ex.what();
				return;
			}
			mRegisteredScripts.insert(arg_file);
			LOG_INFO() << "Loaded script file: " << arg_file;
		}
	}

	bool ScriptManager::IsRegistered(const char* arg_script)
	{
		return mRegisteredScripts.find(arg_script) != mRegisteredScripts.end();
	}

	void ScriptManager::LoadScriptsFromIni()
	{
		const char* ScriptsRootDir = "resources//chaiscript//";
		std::string scriptFilePath = ScriptsRootDir + std::string("ScriptFiles.txt");
		std::queue<std::string> fileList;

		std::ifstream inFile;
		inFile.open(scriptFilePath);
		if (inFile.is_open())
		{
			std::string currLine;
			while (std::getline(inFile, currLine))
			{
				if(currLine[currLine.size() - 1] == '\r')
					currLine.erase(currLine.size() - 1);
				if (currLine != "")
					fileList.push(currLine);
			}
		}

		while (!fileList.empty())
		{
			std::string currFile = fileList.front();
			RegisterScript(ScriptsRootDir + currFile);
			fileList.pop();
		}
	}
}
