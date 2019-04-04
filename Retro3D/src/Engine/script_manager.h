#ifndef RETRO3D_SCRIPTMANAGER_H
#define RETRO3D_SCRIPTMANAGER_H

#include <set>
#include <string>
#include "chaiscript/chaiscript.hpp"

namespace Retro3D
{
	class ScriptComponent;

	class ScriptManager
	{
	private:
		chaiscript::ChaiScript* mChaiScript;
		std::set<std::string> mRegisteredScripts;

		chaiscript::Boxed_Value mGameManagerObject;

	public:
		ScriptManager();
		~ScriptManager();

		/** Loads a script file and registers its code for later use */
		void RegisterScript(std::string arg_file);

		/** Checks if the script file already has been loaded */
		bool IsRegistered(const char* arg_script);

		inline chaiscript::ChaiScript* GetChaiScriptCore() { return mChaiScript; }

		chaiscript::Boxed_Value& GetGameManagerScriptObject() { return mGameManagerObject; }

		void SetGameManagerScriptObject(chaiscript::Boxed_Value& arg_object) { mGameManagerObject = arg_object; }


		/** Loads all the scripts listed in ScriptFiles.txt */
		void LoadScriptsFromIni();
	};
}

#endif
