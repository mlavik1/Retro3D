#ifndef RETRO3D_SCRIPTCOMPONENT_H
#define RETRO3D_SCRIPTCOMPONENT_H

#include "component.h"

#include <string>

namespace Retro3D
{
	class ScriptComponent : public Component
	{
		DEFINE_CLASS(ScriptComponent, Component)

	private:
		std::string mScriptClass;
		bool mCanExecute = false;

		bool createScriptInstance();

	public:
		ScriptComponent();
		void SetScriptClass(const char* arg_class);

		std::string GetScriptObjectName();
		std::string GetScriptClassName();

		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

	};
}

#endif
