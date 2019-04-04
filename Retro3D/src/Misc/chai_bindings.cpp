#include "chai_bindings.h"

#include "Engine/game_engine.h"
#include "Actor/actor.h"
#include "Actor/player.h"
#include "Component/component.h"
#include "Component/script_component.h"
#include "Component/sprite_component.h"
#include "Component/camera_component.h"
#include "Component/audio_component.h"
#include "World/level.h"
#include "glm/glm.hpp"
#include "Engine/input_manager.h"
#include "Engine/script_manager.h"
#include "Audio/audio_manager.h"
#include "Text/config_reader.h"
#include "World/world.h"
#include "UI/Widgets/image_widget.h"
#include "UI/Widgets/text_widget.h"
#include "UI/Widgets/text_input_widget.h"
#include "UI/Widgets/uniform_grid_panel_widget.h"
#include "UI/Widgets/button_widget.h"
#include "UI/Widgets/image_widget_style.h"
#include "UI/Managers/WidgetManager.h"
#include "Actor/player_controller.h"
#include "Object/weak_objectptr.h"
#include "Actor/player.h"

namespace Retro3D
{
	GameEngine* scripthelper_getengine() { return GGameEngine; }

	Actor* scripthelper_CreateActor() { return new Actor(); }
	Player* scripthelper_CreatePlayer() { return new Player(); }

	ScriptComponent* scripthelper_CreateScriptComponent() { return new ScriptComponent(); }
	CameraComponent* scripthelper_CreateCameraComponent() { return new CameraComponent(); }
	SpriteComponent* scripthelper_CreateSpriteComponent() { return new SpriteComponent(); }
	AudioComponent* scripthelper_CreateAudioComponent() { return new AudioComponent(); }


	void ChaiBindings::AddBindings(chaiscript::ChaiScript* arg_chaiscript)
	{
		arg_chaiscript->add(chaiscript::user_type<GameEngine>(), "GameEngine");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::Shutdown), "Shutdown");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetDeltaTime), "GetDeltaTime");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetPlayerController), "GetPlayerController");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetCurrentLevel), "GetCurrentLevel");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetInputManager), "GetInputManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetScriptManager), "GetScriptManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetWidgetManager), "GetWidgetManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetAudioManager), "GetAudioManager");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetGameConfig), "GetGameConfig");

		arg_chaiscript->add(chaiscript::user_type<Level>(), "Level");
		arg_chaiscript->add(chaiscript::fun(&Level::LoadLevel), "LoadLevel");

		
		arg_chaiscript->add(chaiscript::user_type<ConfigReader>(), "ConfigReader");
		arg_chaiscript->add(chaiscript::constructor<ConfigReader()>(), "ConfigReader");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::ReadFile), "ReadFile");
		arg_chaiscript->add(chaiscript::fun([](ConfigReader& arg_reader, std::string arg_filepath) { return arg_reader.ReadFile(arg_filepath); }), "ReadFile");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetString), "GetString");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetInt), "GetInt");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::GetFloat), "GetFloat");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::SetString), "SetString");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::SetInt), "SetInt");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::SetFloat), "SetFloat");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::Save), "Save");
		arg_chaiscript->add(chaiscript::fun(&ConfigReader::SaveToFile), "SaveToFile");


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

		// Player
		arg_chaiscript->add(chaiscript::user_type<Player>(), "Player");
		arg_chaiscript->add(chaiscript::base_class<Actor, Player>());

		// PlayerController
		arg_chaiscript->add(chaiscript::user_type<PlayerController>(), "PlayerController");
		arg_chaiscript->add(chaiscript::fun(&PlayerController::GetPlayer), "GetPlayer");
		arg_chaiscript->add(chaiscript::fun(&PlayerController::SetPlayer), "SetPlayer");


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

		arg_chaiscript->add(chaiscript::user_type<AudioComponent>(), "AudioComponent");
		arg_chaiscript->add(chaiscript::base_class<Component, AudioComponent>());
		arg_chaiscript->add(chaiscript::fun(&AudioComponent::LoadAudio), "LoadAudio");
		arg_chaiscript->add(chaiscript::fun(&AudioComponent::PlayAudioTrack), "PlayAudioTrack");
		arg_chaiscript->add(chaiscript::fun(&AudioComponent::PlayAllAudioTracks), "PlayAllAudioTracks");


		// ObjectPtr
		arg_chaiscript->add(chaiscript::user_type<WeakObjectPtr<Actor>>(), "ActorPtr");
		arg_chaiscript->add(chaiscript::base_class<ObjectPtrBase<Actor>, WeakObjectPtr<Actor>>());
		arg_chaiscript->add(chaiscript::constructor<WeakObjectPtr<Actor>(Actor*)>(), "ActorPtr");
		arg_chaiscript->add(chaiscript::fun(&WeakObjectPtr<Actor>::Get), "Get");


		// Managers
		arg_chaiscript->add(chaiscript::user_type<InputManager>(), "InputManager");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKey_String), "GetKey");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKeyDown_String), "GetKeyDown");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetKeyUp_String), "GetKeyUp");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMousePressed), "GetMousePressed");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMouseReleased), "GetMouseReleased");
		arg_chaiscript->add(chaiscript::fun(&InputManager::GetMousePosition), "GetMousePosition");

		arg_chaiscript->add(chaiscript::user_type<ScriptManager>(), "ScriptManager");
		arg_chaiscript->add(chaiscript::fun(&ScriptManager::RegisterScript), "RegisterScript");

		arg_chaiscript->add(chaiscript::user_type<WidgetManager>(), "WidgetManager");
		arg_chaiscript->add(chaiscript::fun(&WidgetManager::AddWidget), "AddWidget");
		arg_chaiscript->add(chaiscript::fun(&WidgetManager::RemoveWidget), "RemoveWidget");

		arg_chaiscript->add(chaiscript::user_type<AudioManager>(), "AudioManager");
		arg_chaiscript->add(chaiscript::fun(&AudioManager::PlayAudioFile), "PlayAudioFile");

		arg_chaiscript->add(chaiscript::user_type<AudioTrack>(), "AudioTrack");
		arg_chaiscript->add(chaiscript::fun(&AudioTrack::SetLooping), "SetLooping");


		// Widgets
		arg_chaiscript->add(chaiscript::user_type<Widget>(), "Widget");
		arg_chaiscript->add(chaiscript::fun(&Widget::AddVisual), "AddVisual");
		arg_chaiscript->add(chaiscript::fun(&Widget::AddChildWidget), "AddChildWidget");
		arg_chaiscript->add(chaiscript::fun([](Widget& widget, const float x, const float y) { widget.SetSize(x, y); }), "SetSize");
		arg_chaiscript->add(chaiscript::fun([](Widget& widget, const float x, const float y) { widget.SetPosition(x, y); }), "SetPosition");
		arg_chaiscript->add(chaiscript::fun([](Widget& widget, const float x, const float y) { widget.SetPivot(x, y); }), "SetPivot");
		arg_chaiscript->add(chaiscript::fun(&Widget::GetVerticalPositioning), "GetVerticalPositioning");
		arg_chaiscript->add(chaiscript::fun(&Widget::SetVerticalPositioning), "SetVerticalPositioning");
		arg_chaiscript->add(chaiscript::fun(&Widget::GetHorizontalPositioning), "GetHorizontalPositioning");
		arg_chaiscript->add(chaiscript::fun(&Widget::SetHorizontalPositioning), "SetHorizontalPositioning");
		arg_chaiscript->add(chaiscript::fun(&Widget::GetVerticalScaling), "GetVerticalScaling");
		arg_chaiscript->add(chaiscript::fun(&Widget::SetVerticalScaling), "SetVerticalScaling");
		arg_chaiscript->add(chaiscript::fun(&Widget::GetHorizontalScaling), "GetHorizontalScaling");
		arg_chaiscript->add(chaiscript::fun(&Widget::SetHorizontalScaling), "SetHorizontalScaling");
		arg_chaiscript->add(chaiscript::fun(&Widget::SetWidgetScriptClass), "SetWidgetScriptClass");
		arg_chaiscript->add(chaiscript::fun(&Widget::BindOnMousePressed), "BindOnMousePressed");
		arg_chaiscript->add(chaiscript::fun(&Widget::BindOnMouseReleased), "BindOnMouseReleased");

		arg_chaiscript->add(chaiscript::user_type<ImageWidget>(), "ImageWidget");
		arg_chaiscript->add(chaiscript::fun(&ImageWidget::SetColour), "SetColour");
		arg_chaiscript->add(chaiscript::fun(&ImageWidget::SetImagePath), "SetImagePath");
		arg_chaiscript->add(chaiscript::base_class<Widget, ImageWidget>());

		arg_chaiscript->add(chaiscript::user_type<TextWidget>(), "TextWidget");
		arg_chaiscript->add(chaiscript::base_class<Widget, TextWidget>());
		arg_chaiscript->add(chaiscript::fun(&TextWidget::GetText), "GetText");
		arg_chaiscript->add(chaiscript::fun(&TextWidget::SetText), "SetText");
		arg_chaiscript->add(chaiscript::fun(&TextWidget::GetTextStyle), "GetTextStyle");
		arg_chaiscript->add(chaiscript::fun(&TextWidget::SetTextStyle), "SetTextStyle");

		arg_chaiscript->add(chaiscript::user_type<TextInputWidget>(), "TextInputWidget");
		arg_chaiscript->add(chaiscript::base_class<Widget, TextInputWidget>());
		arg_chaiscript->add(chaiscript::base_class<TextWidget, TextInputWidget>());

		chaiscript::ModulePtr moduleWidgetPositioningMode = chaiscript::ModulePtr(new chaiscript::Module());
		chaiscript::utility::add_class<WidgetPositioningMode>(*moduleWidgetPositioningMode,
			"WidgetPositioningMode",
			{ { WidgetPositioningMode::Absolute, "Absolute" },
			{	WidgetPositioningMode::Relative, "Relative" }
			}
		);
		chaiscript::ModulePtr mWidgetScalingMode = chaiscript::ModulePtr(new chaiscript::Module());
		chaiscript::utility::add_class<WidgetScalingMode>(*mWidgetScalingMode,
			"WidgetPositioningMode",
			{ { WidgetScalingMode::Absolute, "Absolute" },
			{	WidgetScalingMode::Relative, "Relative" }
			}
		);

		arg_chaiscript->add(chaiscript::user_type<TextStyle>(), "TextStyle");
		arg_chaiscript->add(chaiscript::fun(&TextStyle::SetColour), "SetColour");
		arg_chaiscript->add(chaiscript::fun(&TextStyle::SetFontName), "SetFontName");
		arg_chaiscript->add(chaiscript::fun(&TextStyle::SetFontSize), "SetFontSize");
		arg_chaiscript->add(chaiscript::fun(&TextStyle::SetWrapText), "SetWrapText");

		arg_chaiscript->add(chaiscript::user_type<Colour>(), "Colour");
		arg_chaiscript->add(chaiscript::constructor<Colour(float, float, float)>(), "Colour");
		arg_chaiscript->add(chaiscript::constructor<Colour(float, float, float, float)>(), "Colour");

		arg_chaiscript->add(chaiscript::user_type<ButtonWidget>(), "ButtonWidget");
		arg_chaiscript->add(chaiscript::base_class<Widget, ButtonWidget>());
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::SetText), "SetText");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::GetTextWidget), "GetTextWidget");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::SetBackgroundStyle), "SetBackgroundStyle");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::SetBackgroundStyleOnHover), "SetBackgroundStyleOnHover");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::SetBackgroundStyleOnClick), "SetBackgroundStyleOnClick");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::GetBackgroundStyle), "GetBackgroundStyle");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::GetBackgroundStyleOnHover), "GetBackgroundStyleOnHover");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::GetBackgroundStyleOnClick), "GetBackgroundStyleOnClick");
		arg_chaiscript->add(chaiscript::fun(&ButtonWidget::BindOnButtonClicked), "BindOnButtonClicked");

		arg_chaiscript->add(chaiscript::user_type<PanelWidget>(), "PanelWidget");
		arg_chaiscript->add(chaiscript::base_class<Widget, PanelWidget>());

		arg_chaiscript->add(chaiscript::user_type<GridPanelBase>(), "GridPanelBase");
		arg_chaiscript->add(chaiscript::base_class<Widget, GridPanelBase>());
		arg_chaiscript->add(chaiscript::base_class<PanelWidget, GridPanelBase>());
		arg_chaiscript->add(chaiscript::fun(&GridPanelBase::SetDimension), "SetDimension");

		arg_chaiscript->add(chaiscript::user_type<UniformGridPanelWidget>(), "UniformGridPanelWidget");
		arg_chaiscript->add(chaiscript::base_class<Widget, UniformGridPanelWidget>());
		arg_chaiscript->add(chaiscript::base_class<PanelWidget, UniformGridPanelWidget>());
		arg_chaiscript->add(chaiscript::base_class<GridPanelBase, UniformGridPanelWidget>());
		arg_chaiscript->add(chaiscript::fun(&UniformGridPanelWidget::SetGridCellSize), "SetGridCellSize");
		arg_chaiscript->add(chaiscript::fun(&UniformGridPanelWidget::AddWidgetToGridCell), "AddWidgetToGridCell");
		arg_chaiscript->add(chaiscript::fun(&UniformGridPanelWidget::SetAutoSize), "SetAutoSize");

		arg_chaiscript->add(chaiscript::user_type<ImageWidgetStyle>(), "ImageWidgetStyle");
		arg_chaiscript->add(chaiscript::constructor<ImageWidgetStyle()>(), "ImageWidgetStyle");
		arg_chaiscript->add(chaiscript::fun(&ImageWidgetStyle::SetImagePath), "SetImagePath");
		arg_chaiscript->add(chaiscript::fun(&ImageWidgetStyle::SetColour), "SetColour");

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
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreatePlayer), "CreatePlayer");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateScriptComponent), "CreateScriptComponent");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateSpriteComponent), "CreateSpriteComponent");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateCameraComponent), "CreateCameraComponent");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_CreateAudioComponent), "CreateAudioComponent");

		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([]()
		{
			ImageWidget* widget = new ImageWidget();
			return widget;
		}), "CreateImageWidget");
		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([]()
		{
			TextWidget* widget = new TextWidget();
			return widget;
		}), "CreateTextWidget");
		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([]()
		{
			TextInputWidget* widget = new TextInputWidget();
			return widget;
		}), "CreateTextInputWidget");
		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([]()
		{
			ButtonWidget* widget = new ButtonWidget();
			return widget;
		}), "CreateButtonWidget");
		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([]()
		{
			UniformGridPanelWidget* widget = new UniformGridPanelWidget();
			return widget;
		}), "CreateUniformGridPanelWidget");
		// TEMP - TODO: Create a CreateWidget(typename) function
		arg_chaiscript->add(chaiscript::fun([](const std::string& scriptclass)
		{
			Widget* widget = new Widget();
			widget->SetWidgetScriptClass(scriptclass.c_str());
			return widget;
		}), "CreateScriptWidget");

		arg_chaiscript->add(chaiscript::fun([](const std::string& name)
		{
			for (ObjectPtr<Actor> actor : GGameEngine->GetWorld()->GetActors())
			{
				if (actor->GetActorName() == name)
					return actor.Get();
			}
			return (Actor*)nullptr;
		}), "GetActorByName");

		// ChaiScriptObject
		arg_chaiscript->add(chaiscript::fun([](chaiscript::Boxed_Value boxedValue)
		{
			return GGameEngine->GetWidgetManager()->GetWidgetFromScriptObject(boxedValue);
		}), "GetWidget");

		// GameManager
		arg_chaiscript->add(chaiscript::fun([]()
		{
			return GGameEngine->GetScriptManager()->GetGameManagerScriptObject();
		}), "GetGameManager");

	}


}
