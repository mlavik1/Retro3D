#include "WidgetManager.h"

#include "Engine/game_engine.h"
#include "UI/Interfaces/widget_renderer.h"
#include "Engine/input_manager.h"
#include "Text/text_input_method_context.h"
#include "Text/input_method_manager.h"

namespace Retro3D
{
	WidgetManager::WidgetManager()
	{
		mCurrentRenderingRenderTarget = nullptr;
		mRootWidget = new Widget();
		mRootWidget->SetSize(1.0f, 1.0f);
		mSelectedWidget = nullptr;
	}

	void WidgetManager::RenderWidgetRecursive(Widget* arg_widget, IRenderTarget* arg_window)
	{
		__Assert(arg_widget->mParentWidget != nullptr);
		__Assert(arg_window != nullptr);

		IWidgetRenderer* widgetRenderer = GGameEngine->GetWidgetRenderer();
		const WidgetTransform& widgetTrans = arg_widget->GetAbsoluteTransform();
		const WidgetTransform& parentTrans = arg_widget->mParentWidget->GetAbsoluteTransform();

		// TODO: USE INT CALCULATION
		// Calculate content rect (rect to render widget in) and visible rect (usually the same, unless parent widget is smaller)
		const glm::vec2 contentXYBounds = widgetTrans.mPosition + widgetTrans.mSize;
		const glm::vec2 parentXYBounds = parentTrans.mPosition + parentTrans.mSize;
		const float visiblePosX = std::fminf(std::fmaxf(widgetTrans.mPosition.x, parentTrans.mPosition.x), parentXYBounds.x);
		const float visiblePosY = std::fminf(std::fmaxf(widgetTrans.mPosition.y, parentTrans.mPosition.y), parentXYBounds.y);
		const float visibleSizeW = std::fminf(contentXYBounds.x, parentXYBounds.x) - visiblePosX;
		const float visibleSizeH = std::fminf(contentXYBounds.y, parentXYBounds.y) - visiblePosY;
		WidgetRenderParams renderParams;
		renderParams.mContentRect = Rectangle(widgetTrans.mPosition, widgetTrans.mSize);
		renderParams.mVisibleRect = Rectangle(glm::vec2(visiblePosX, visiblePosY), glm::vec2(visibleSizeW, visibleSizeH));

		for (ObjectPtrBase<Visual> visual : arg_widget->mVisuals)
		{
			if (visual->IsActivated())
				visual->RenderVisual(widgetRenderer, renderParams);
		}

		for (size_t i = 0; i < arg_widget->GetNumChildWidgets(); i++)
		{
			ObjectPtr<Widget> currentWidget = arg_widget->GetChildWidgetAt(i);
			RenderWidgetRecursive(currentWidget.Get(), arg_window);
		}
	}

	void WidgetManager::IterateWidgetsRecursive(Widget* arg_widget, std::function<bool(Widget*)> arg_function)
	{
		bool continueWithChildren = arg_function(arg_widget);
		if (continueWithChildren)
		{
			for (ObjectPtrBase<Widget>& child : arg_widget->mChildWidgets)
				IterateWidgetsRecursive(child.Get(), arg_function);
		}
	}

	void WidgetManager::SetSelectedWidget(Widget* arg_widget)
	{
		InputMethodManager* imManager = GGameEngine->GetInputMethodManager();
		mSelectedWidget = arg_widget;

		ITextInputMethodContext* imContext = dynamic_cast<ITextInputMethodContext*>(arg_widget);
		if (imContext)
		{
			imManager->SetContext(imContext);
			imManager->EnableInput();
		}
		else
		{
			imManager->DisableInput();
		}
	}


	void WidgetManager::AddWidget(Widget* arg_widget)
	{
		mRootWidget->AddChildWidget(arg_widget);
		if (!arg_widget->mHasCreatedContent)
		{
			arg_widget->CreateContent();
		}
	}

	void WidgetManager::RemoveWidget(Widget* arg_widget)
	{
		mRootWidget->RemoveChildWidget(arg_widget);
	}

	void WidgetManager::TickWidgets(float arg_deltatime)
	{
		std::function<bool(Widget*)> func = [&](Widget* arg_widget) -> bool
		{
			arg_widget->OnTick(arg_deltatime);
			return true;
		};

		IterateWidgetsRecursive(mRootWidget.Get(), func);

		InputMethodManager* imManager = GGameEngine->GetInputMethodManager();
		ITextInputMethodContext* imContext = dynamic_cast<ITextInputMethodContext*>(mSelectedWidget.GetObjectSafe());
		if (imContext)
		{
			std::string imResultString;
			if (imManager->GetString(imResultString))
			{
				imContext->HandleTextInputMethodResult(imResultString);
			}
		}
	}

	void WidgetManager::RenderWidgets(IRenderTarget* arg_target)
	{
		mCurrentRenderingRenderTarget = arg_target;

		// TODO: Make sure this happens before ticking widgets for the first time.
		// We don't want the absolute transforms of widgets to be incorrect the first frame.
		if (mRootWidget.IsValid())
		{
			int winWidth = 0;
			int winHeight = 0;
			arg_target->GetRenderContextSize(winWidth, winHeight);
			mRootWidget->SetSize(static_cast<float>(winWidth), static_cast<float>(winHeight));
			mRootWidget->SetHorizontalPositioning(WidgetPositioningMode::Absolute);
			mRootWidget->SetVerticalPositioning(WidgetPositioningMode::Absolute);
			mRootWidget->SetHorizontalScaling(WidgetScalingMode::Absolute);
			mRootWidget->SetVerticalScaling(WidgetScalingMode::Absolute);
		}

		IWidgetRenderer* widgetRenderer = GGameEngine->GetWidgetRenderer();
		widgetRenderer->SetRenderTarget(arg_target);
		for (size_t i = 0; i < mRootWidget->GetNumChildWidgets(); i++)
		{
			ObjectPtr<Widget> currentWidget = mRootWidget->GetChildWidgetAt(i);
			RenderWidgetRecursive(currentWidget.Get(), arg_target);
		}
		
	}

	void WidgetManager::RegisterScriptObject(Widget* arg_widget)
	{
		if (!arg_widget->mScriptObject.is_null())
		{
			mScriptWidgetMap[arg_widget->mScriptObject.get_ptr()] = arg_widget;
		}
	}

	void WidgetManager::UnRegisterScriptObject(Widget* arg_widget)
	{
		if (!arg_widget->mScriptObject.is_null())
		{
			mScriptWidgetMap.erase(arg_widget->mScriptObject.get_ptr());
		}
	}

	Widget* WidgetManager::GetWidgetFromScriptObject(chaiscript::Boxed_Value arg_obj)
	{
		if (!arg_obj.is_null())
		{
			auto entry = mScriptWidgetMap.find(arg_obj.get_ptr());
			if (entry != mScriptWidgetMap.end())
			{
				return entry->second.Get();
			}
		}
		return nullptr;
	}

	void WidgetManager::OnKeyDown(const char* arg_key)
	{
		// TODO: forward to selected/highlighted widget
		if (mSelectedWidget != nullptr)
		{
			mSelectedWidget->OnKeyDown(arg_key);
		}
	}

	void WidgetManager::OnKeyUp(const char* arg_key)
	{
		// TODO: forward to selected/highlighted widget
		if (mSelectedWidget != nullptr)
		{
			mSelectedWidget->OnKeyUp(arg_key);
		}
	}

	void WidgetManager::OnMouseButtonDown(MouseButtonID arg_button)
	{
		glm::vec2 mousePos = GGameEngine->GetInputManager()->GetMousePosition();

		Widget* lastWidget = nullptr;

		std::function<bool(Widget*)> func = [&](Widget* arg_widget) -> bool
		{
			const glm::vec2 widgetPos = arg_widget->mAbsoluteTransform.mPosition;
			const glm::vec2 widgetRBPos = arg_widget->mAbsoluteTransform.mSize + widgetPos;

			if (mousePos.x > widgetPos.x && mousePos.x < widgetRBPos.x && mousePos.y > widgetPos.y && mousePos.y < widgetRBPos.y)
			{
				arg_widget->OnMouseButtonDown(arg_button);

				auto widgetArrayIter = mMouseButtonDownWidgets.find((int)arg_button);
				if (widgetArrayIter == mMouseButtonDownWidgets.end())
				{
					mMouseButtonDownWidgets.emplace((int)arg_button, std::vector<WeakObjectPtr<Widget>>());
					widgetArrayIter = mMouseButtonDownWidgets.find((int)arg_button);
				}
				widgetArrayIter->second.push_back(arg_widget);
				lastWidget = arg_widget;

				return true;
			}
			return false;
		};

		IterateWidgetsRecursive(mRootWidget.Get(), func);

		if (lastWidget != nullptr)
		{
			SetSelectedWidget(lastWidget);
		}
	}

	void WidgetManager::OnMouseButtonUp(MouseButtonID arg_button)
	{
		auto widgetArrayIter = mMouseButtonDownWidgets.find((int)arg_button);
		if (widgetArrayIter != mMouseButtonDownWidgets.end())
		{
			for (ObjectPtrBase<Widget>& widget : widgetArrayIter->second)
			{
				widget->OnMouseButtonUp(arg_button);
			}
			widgetArrayIter->second.clear();
		}
	}

	void WidgetManager::OnMouseMotion(const glm::vec2& arg_motion)
	{
		glm::vec2 mousePos = GGameEngine->GetInputManager()->GetMousePosition();

		std::vector<WeakObjectPtr<Widget>> widgetsNoLongerHovered;

		std::function<bool(Widget*)> funcMouseMotion = [&](Widget* arg_widget) -> bool
		{
			const glm::vec2 widgetPos = arg_widget->mAbsoluteTransform.mPosition;
			const glm::vec2 widgetRBPos = arg_widget->mAbsoluteTransform.mSize + widgetPos;

			if (mousePos.x > widgetPos.x && mousePos.x < widgetRBPos.x && mousePos.y > widgetPos.y && mousePos.y < widgetRBPos.y)
			{
				if (!arg_widget->mIsHovered)
				{
					arg_widget->OnMouseEnter();
					arg_widget->mIsHovered = true;
				}

				arg_widget->OnMouseMotion(arg_motion);

				return true;
			}
			else
			{
				// Tell widget and children that we are no longer hovering
				if (arg_widget->mIsHovered)
				{
					widgetsNoLongerHovered.push_back(arg_widget);
				}
			}
			return false;
		};

		IterateWidgetsRecursive(mRootWidget.Get(), funcMouseMotion);

		// Iterate through all widgets that no longer are hovered, and call OnMouseLeave()
		std::function<bool(Widget*)> funcOnMouseLeave = [&](Widget* arg_widget) -> bool
		{
			if (arg_widget->mIsHovered)
			{
				arg_widget->OnMouseLeave();
				arg_widget->mIsHovered = false;

				return true; // continue with children
			}
			
			return false;
		};

		for (auto& widget : widgetsNoLongerHovered)
		{
			IterateWidgetsRecursive(widget.Get(), funcOnMouseLeave);
		}

	}

}