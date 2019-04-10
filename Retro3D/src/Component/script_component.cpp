#include "script_component.h"

#include "Engine/script_manager.h"
#include "Engine/game_engine.h"

namespace Retro3D
{
	IMPLEMENT_CLASS(ScriptComponent);


	std::unordered_map<void*, ScriptComponent*> ScriptComponent::ScriptObjectMap = std::unordered_map<void*, ScriptComponent*>();

	ScriptComponent::ScriptComponent()
	{

	}

	ScriptComponent::~ScriptComponent()
	{
		if (mCanExecute)
			ScriptObjectMap.erase(mScriptObject.get_ptr());
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
		if (mCanExecute && funcOnStart != nullptr)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				funcOnStart(mScriptObject); // TODO: only call if function exists
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in ScriptComponent::OnStart: " << ex.what();
			}
		}
	}

	void ScriptComponent::OnTick(float arg_deltatime)
	{
		if (mCanExecute && funcOnTick != nullptr)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				funcOnTick(mScriptObject, GGameEngine->GetDeltaTime()); // TODO: only call if function exists
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in " << this->mScriptClass << "::OnTick: " << ex.what();
			}
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
		std::string createInstanceCall = mScriptClass + std::string("();");
		try
		{
			mScriptObject = chaiScriptCore->eval(createInstanceCall); // will exist as long as mScriptObject

		}
		catch (std::exception ex)
		{
			LOG_ERROR() << "Failed to create script object for " << mScriptClass << ". Exception: " <<ex.what();
			return false;
		}

        // Find OnStart
        try { funcOnStart = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&)>>("OnStart");
        } catch(std::exception ex) {}
		
        // Find OnTick
        try { funcOnTick = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&, float)>>("OnTick");
        } catch (std::exception ex) {}

		ScriptObjectMap[mScriptObject.get_ptr()] = this;

		mCanExecute = true;
		return true;
	}

}
