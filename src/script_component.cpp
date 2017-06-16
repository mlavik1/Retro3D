#include "script_component.h"

#include "script_manager.h"
#include "game_engine.h"

namespace Retro3D
{
	IMPLEMENT_CLASS(ScriptComponent);

	ScriptComponent::ScriptComponent()
	{

	}

	void ScriptComponent::SetScriptClass(std::string arg_class)
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
			
			try
			{
				chaiScriptCore->eval(createInstanceCall);
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in ScriptComponent::OnStart: " << ex.what();
			}
		}
	}

	void ScriptComponent::OnTick(float arg_deltatime)
	{
		if (mCanExecute)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			// TODO: call functions the "right" way
			std::string createInstanceCall = GetScriptObjectName() + std::string(".OnTick(") + std::to_string(GGameEngine->GetDeltaTime()) + std::string(");");
			
			try
			{
				chaiScriptCore->eval(createInstanceCall);
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in ScriptComponent::OnTick: " << ex.what();
			}

			//auto func = chaiScriptCore->eval<std::function<void(float)> >("fun(dt) {" + GetScriptObjectName() + ".OnTick(dt); }");
			//func(GGameEngine->GetDeltaTime());

		}
	}

	chaiscript::Boxed_Value& ScriptComponent::GetScriptObject()
	{
		return mScriptObject;
	}

	bool ScriptComponent::createScriptInstance()
	{
		bool canCreate = GetScriptClassName() != "";
		if (!canCreate)
			return false;

		chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
		std::string createInstanceCall = std::string("var ") + GetScriptObjectName() + std::string(" = ") + mScriptClass + std::string("();");
		try
		{
			chaiScriptCore->eval(createInstanceCall);
			mScriptObject = GGameEngine->GetScriptManager()->GetChaiScriptCore()->eval(GetScriptObjectName());
		}
		catch (std::exception ex)
		{
			LOG_ERROR() << "Failed to create script object for " << mScriptClass << ". Exception: " <<ex.what();
			return false;
		}
		
		mCanExecute = true;
		return true;
	}

}
