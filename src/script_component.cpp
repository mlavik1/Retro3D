#include "script_component.h"

#include "script_manager.h"
#include "chaiscript/chaiscript.hpp"
#include "game_engine.h"

namespace Retro3D
{
	IMPLEMENT_CLASS(ScriptComponent);

	ScriptComponent::ScriptComponent()
	{

	}

	void ScriptComponent::SetScriptClass(const char* arg_class)
	{
		mScriptClass = arg_class;
	}

	std::string ScriptComponent::GetScriptObjectName()
	{
		return mObjectName;
	}

	std::string ScriptComponent::GetScriptClassName()
	{
		return mScriptClass;
	}


	void ScriptComponent::OnStart()
	{
		createScriptInstance(); // Create an instance of the class defined in script.
		if (mCanExecute)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			std::string createInstanceCall = GetScriptObjectName() + std::string(".OnStart()");
			chaiScriptCore->eval(createInstanceCall);
		}
	}

	void ScriptComponent::OnTick(float arg_deltatime)
	{
		if (mCanExecute)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			// TODO: call functions the "right" way
			std::string createInstanceCall = GetScriptObjectName() + std::string(".OnTick(") + std::to_string(GGameEngine->GetDeltaTime()) + std::string(");");
			chaiScriptCore->eval(createInstanceCall);

			//auto func = chaiScriptCore->eval<std::function<void(float)> >("fun(dt) {" + GetScriptObjectName() + ".OnTick(dt); }");
			//func(GGameEngine->GetDeltaTime());

		}
	}


	bool ScriptComponent::createScriptInstance()
	{
		bool canCreate = GetScriptClassName() != "";
		if (!canCreate)
			return false;

		chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
		std::string createInstanceCall = std::string("var ") + GetScriptObjectName() + std::string(" = ") + mScriptClass + std::string("();");
		chaiScriptCore->eval(createInstanceCall);
		mCanExecute = true;
		return true;
	}

}
