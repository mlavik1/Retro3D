#include "script_manager.h"

#include <fstream>
#include <streambuf>
#include "debug.h"

#include "chai_bindings.h"

namespace Retro3D
{
	ScriptManager::ScriptManager()
	{
		mChaiScript = new chaiscript::ChaiScript();

		ChaiBindings::AddBindings(mChaiScript);
	}

	ScriptManager::~ScriptManager()
	{
		delete mChaiScript;
	}

	void ScriptManager::RegisterScript(const char* arg_file)
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

	bool ScriptManager::IsRegistered(const char* arg_class)
	{
		return mRegisteredScripts.find(arg_class) != mRegisteredScripts.end();
	}
}
