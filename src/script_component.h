#ifndef RETRO3D_SCRIPTCOMPONENT_H
#define RETRO3D_SCRIPTCOMPONENT_H

#include "component.h"
#include "chaiscript/chaiscript.hpp"
#include <string>


namespace Retro3D
{
	class ScriptComponent : public Component
	{
		DEFINE_CLASS(ScriptComponent, Component)

	private:
		std::string mScriptClass;
		bool mCanExecute = false;

		chaiscript::Boxed_Value mScriptObject;

		bool createScriptInstance();

	public:
		ScriptComponent();
		void SetScriptClass(std::string arg_class);

		std::string GetScriptObjectName();
		std::string GetScriptClassName();

		chaiscript::Boxed_Value& GetScriptObject();

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

	};
}

#endif
