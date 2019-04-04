#ifndef RETRO3D_SCRIPTCOMPONENT_H
#define RETRO3D_SCRIPTCOMPONENT_H

/*=============================================================================================
Script component class.

Has a ChaiScript class that defines the behaviour of the component.
The ChaiScript file should look like this:
class YourClass
{
	def YourClass() {} // Constructor
	def OnStart() {} // Called when the game starts, or when the component starts ticking (if added during runtime)
	def OnTick(deltatime) {} // Called every frame
}

==============================================================================================*/

#include "component.h"
#include "chaiscript/chaiscript.hpp"
#include <string>
#include <unordered_map>

namespace Retro3D
{
	class ScriptComponent : public Component
	{
		DEFINE_CLASS(ScriptComponent, Component)

	private:
		/**
		* The name of your ChaiScript class.
		* Note: the script file must have been registered using ScriptManager::RegisterScript
		*/
		std::string mScriptClass;

		bool mCanExecute = false;

		/** This is the instance of the ChaiScript class. */
		chaiscript::Boxed_Value mScriptObject;

		std::function<void(chaiscript::Boxed_Value&)> funcOnStart;
		std::function<void(chaiscript::Boxed_Value&, float)> funcOnTick;

		bool createScriptInstance();

	public:
		ScriptComponent();
		~ScriptComponent();
		void SetScriptClass(std::string arg_class);

		std::string GetScriptObjectName();

		/**
		* Gets the name of the ChaiScript class.
		*/
		std::string GetScriptClassName();

		/**
		* Gets the instance of the ChaiScript class
		*/
		chaiscript::Boxed_Value& GetScriptObject();

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

		static std::unordered_map<void*, ScriptComponent*> ScriptObjectMap; // TEMP - TODO: move to ScriptManager

	};
}

#endif
