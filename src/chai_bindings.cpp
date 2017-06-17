#include "chai_bindings.h"

#include "game_engine.h"
#include "actor.h"
#include "player.h"
#include "component.h"
#include "script_component.h"
#include "sprite_component.h"
#include "camera_component.h"
#include "level.h"
#include "glm/glm.hpp"
#include "input_manager.h"
#include "script_manager.h"
#include "config_reader.h"
#include "world.h"

namespace Retro3D
{
	GameEngine* scripthelper_getengine() { return GGameEngine; }

	Actor* scripthelper_CreateActor() { return new Actor(); }

	ScriptComponent* scripthelper_CreateScriptComponent() { return new ScriptComponent(); }
	CameraComponent* scripthelper_CreateCameraComponent() { return new CameraComponent(); }
	SpriteComponent* scripthelper_CreateSpriteComponent() { return new SpriteComponent(); }


	void ChaiBindings::AddBindings(chaiscript::ChaiScript* arg_chaiscript)
	{
		arg_chaiscript->add(chaiscript::user_type<GameEngine>(), "GameEngine");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetDeltaTime), "GetDeltaTime");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetCurrentLevel), "GetCurrentLevel");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetInputManager), "GetInputManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetScriptManager), "GetScriptManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetGameConfig), "GetGameConfig");

		arg_chaiscript->add(chaiscript::user_type<Level>(), "Level");
		//arg_chaiscript->add(chaiscript::fun(&Level::LoadLevel), "LoadLevel");

		
		arg_chaiscript->add(chaiscript::user_type<ConfigReader>(), "ConfigReader");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetString_string), "GetString");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetInt_string), "GetInt");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetFloat_string), "GetFloat");


		// Actors
		arg_chaiscript->add(chaiscript::user_type<Actor>(), "Actor");
		//arg_chaiscript->add(chaiscript::constructor<Actor()>(), "Actor");
		arg_chaiscript->add(chaiscript::fun(&Actor::AddComponent), "AddComponent");
		arg_chaiscript->add(chaiscript::fun(&Actor::GetTransform), "GetTransform");
		arg_chaiscript->add(chaiscript::fun(&Actor::SetActorName), "SetActorName");
		arg_chaiscript->add(chaiscript::fun(&Actor::GetActorName), "GetActorName");
		arg_chaiscript->add(chaiscript::fun([](Actor & o, std::string arg_name)
		{
			for (ScriptComponent* comp : o.GetComponents<ScriptComponent>())
			{
				if (comp->GetScriptClassName() == arg_name)
					return comp;
			}
			return (ScriptComponent*)nullptr;
		}), "GetScriptComponentByClass");


		arg_chaiscript->add(chaiscript::user_type<Player>(), "Player");


		// ChaiScriptObject
		arg_chaiscript->add(chaiscript::fun([](chaiscript::Boxed_Value boxedValue)
		{
			auto iter = ScriptComponent::ScriptObjectMap.find(boxedValue.get_ptr());
			if (iter != ScriptComponent::ScriptObjectMap.end())
			{
				return (*iter).second;
			}
			return (ScriptComponent*)nullptr;
		}), "GetScriptComponent");

		// Components
		arg_chaiscript->add(chaiscript::user_type<Component>(), "Component");

		arg_chaiscript->add(chaiscript::user_type<ScriptComponent>(), "ScriptComponent");
		arg_chaiscript->add(chaiscript::fun(&ScriptComponent::SetScriptClass), "SetScriptClass");
		arg_chaiscript->add(chaiscript::fun(&ScriptComponent::GetScriptClassName), "GetScriptClassName");
		arg_chaiscript->add(chaiscript::base_class<Component, ScriptComponent>());
		arg_chaiscript->add(chaiscript::fun(&ScriptComponent::GetScriptObject), "GetScriptObject");

		arg_chaiscript->add(chaiscript::user_type<SpriteComponent>(), "SpriteComponent");
		arg_chaiscript->add(chaiscript::fun(&SpriteComponent::SetTexture), "SetTexture");
		arg_chaiscript->add(chaiscript::base_class<Component, SpriteComponent>());

		arg_chaiscript->add(chaiscript::user_type<CameraComponent>(), "CameraComponent");
		arg_chaiscript->add(chaiscript::base_class<Component, CameraComponent>());


		// ObjectPtr
		arg_chaiscript->add(chaiscript::user_type<ObjectPtr<Actor>>(), "ActorPtr");
		arg_chaiscript->add(chaiscript::fun(&ObjectPtr<Actor>::Get), "Get");


		// Managers
		arg_chaiscript->add(chaiscript::user_type<InputManager>(), "InputManager");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKey_String), "GetKey");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKeyDown_String), "GetKeyDown");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKeyUp_String), "GetKeyUp");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMousePressed), "GetMousePressed");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMouseReleased), "GetMouseReleased");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMousePosition), "GetMousePosition");

		arg_chaiscript->add(chaiscript::user_type<ScriptManager>(), "ScriptManager");


		// Transform
		arg_chaiscript->add(chaiscript::user_type<Transform>(), "Transform");
		arg_chaiscript->add(chaiscript::fun(&Transform::GetPosition), "GetPosition");
		arg_chaiscript->add(chaiscript::fun(&Transform::SetPosition), "SetPosition");


		// glm
		arg_chaiscript->add(chaiscript::user_type<glm::vec3>(), "vec3");
		arg_chaiscript->add(chaiscript::constructor<glm::vec3(float, float, float)>(), "vec3");
		arg_chaiscript->add(chaiscript::fun([](glm::vec3 a, glm::vec3 b) { return a + b; }), "+");


		// glm
		arg_chaiscript->add(chaiscript::user_type<glm::vec2>(), "vec2");
		arg_chaiscript->add(chaiscript::constructor<glm::vec2(float, float)>(), "vec2");
		arg_chaiscript->add(chaiscript::fun([](glm::vec2 a, glm::vec2 b) { return a + b; }), "+");


		// helper functions
		arg_chaiscript->add(chaiscript::fun(&scripthelper_getengine), "GetGameEngine");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateActor), "CreateActor");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateScriptComponent), "CreateScriptComponent");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateSpriteComponent), "CreateSpriteComponent");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateCameraComponent), "CreateCameraComponent");

		arg_chaiscript->add(chaiscript::fun([](const std::string& name)
		{
			for (ObjectPtr<Actor> actor : GGameEngine->GetWorld()->GetActors())
			{
				if (actor->GetActorName() == name)
					return actor.Get();
			}
			return (Actor*)nullptr;
		}), "GetActorByName");

	}
}
