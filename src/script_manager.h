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

	public:
		ScriptManager();
		~ScriptManager();

		void RegisterScript(const char* arg_file);
		bool IsRegistered(const char* arg_class);

		inline chaiscript::ChaiScript* GetChaiScriptCore() { return mChaiScript; }
	};
}

#endif
