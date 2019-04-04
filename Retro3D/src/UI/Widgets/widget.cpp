#include "widget.h"
#include "UI/Interfaces/widget_renderer.h"
#include "Misc/st_assert.h"
#include "chaiscript/chaiscript.hpp"
#include "Engine/script_manager.h"
#include "Engine/game_engine.h"
#include "UI/Managers/WidgetManager.h"

namespace Retro3D
{
	Widget::Widget()
		: mTransform()
	{
		mTransformIsDirty = true;
		mTransform.mSize = glm::vec2(1.0f, 1.0f);
	}

	Widget::~Widget()
	{
		if (!mScriptObject.is_null())
		{
			GGameEngine->GetWidgetManager()->UnRegisterScriptObject(this);
		}
	}

	void Widget::SetTransformDirty()
	{
		mTransformIsDirty = true;
	}

	void Widget::AddChildWidget(Widget* arg_widget)
	{
		__AssertComment(arg_widget != nullptr, "Called Widget::AddChildWidget with a null-widget in parameters.");

		ObjectPtr<Widget> widget(arg_widget);

		if (arg_widget->mParentWidget.IsValid())
		{
			bool removed = arg_widget->mParentWidget->RemoveChildWidget(arg_widget);
			__AssertComment(removed, "Widget not contained in its parent's child widget container");
		}
		arg_widget->mParentWidget = this;
		mChildWidgets.push_back(widget);

		if (!arg_widget->mHasCreatedContent)
		{
			arg_widget->CreateContent();
		}
	}

	bool Widget::RemoveChildWidget(Widget* arg_widget)
	{
		for (size_t i = 0; i < mChildWidgets.size(); i++)
		{
			ObjectPtr<Widget>& widget = mChildWidgets[i];
			if (widget == arg_widget)
			{
				mChildWidgets.erase(mChildWidgets.begin() + i);
				return true;
			}
		}
		return false;
	}

	void Widget::AddVisual(Visual* arg_visual)
	{
		mVisuals.push_back(arg_visual);
	}

	void Widget::CreateContent()
	{
		if (mWidgetScriptClass != "")
		{
			SetupContentFromScript();
		}
		mHasCreatedContent = true;
	}

	void Widget::OnInitialise()
	{

	}

	void Widget::OnTick(float arg_deltatime)
	{
		if (funcOnTick != nullptr)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				funcOnTick(mScriptObject, arg_deltatime); // TODO: only call if function exists
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in Widget::OnTick: " << ex.what();
			}
		}
	}

	void Widget::SetPosition(glm::vec2 arg_pos)
	{
		mTransform.mPosition = arg_pos;
		SetTransformDirty();
	}

	void Widget::SetSize(glm::vec2 arg_size)
	{
		mTransform.mSize = arg_size;
		SetTransformDirty();
	}

	void Widget::SetPivot(glm::vec2 arg_pivot)
	{
		mTransform.mPivot = arg_pivot;
		SetTransformDirty();
	}

	void Widget::SetVerticalPositioning(WidgetPositioningMode arg_mode)
	{
		mTransform.mVerticalPositioning = arg_mode;
		SetTransformDirty();
	}

	void Widget::SetHorizontalPositioning(WidgetPositioningMode arg_mode)
	{
		mTransform.mHorizontalPositioning = arg_mode;
		SetTransformDirty();
	}

	void Widget::SetVerticalScaling(WidgetScalingMode arg_mode)
	{
		mTransform.mVerticalScaling = arg_mode;
		SetTransformDirty();
	}

	void Widget::SetHorizontalScaling(WidgetScalingMode arg_mode)
	{
		mTransform.mHorizontalScaling = arg_mode;
		SetTransformDirty();
	}


	void Widget::SetWidgetScriptClass(const char* arg_script)
	{
		mWidgetScriptClass = arg_script;
	}

	bool Widget::SetupContentFromScript()
	{
		bool canCreate = mWidgetScriptClass != "";
		if (!canCreate)
			return false;

		chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
		std::string createInstanceCall = mWidgetScriptClass + std::string("();");
		try
		{
			mScriptObject = chaiScriptCore->eval(createInstanceCall); // will exist as long as mScriptObject
		}
		catch (std::exception ex)
		{
			LOG_ERROR() << "Failed to create script object for " << mWidgetScriptClass << ". Exception: " << ex.what();
			return false;
		}
		try
		{
			funcCreateContent = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&)>>("CreateContent");
		}
		catch (std::exception ex) {}
		try
		{
			funcOnTick = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&, float)>>("OnTick");
		}
		catch (std::exception ex) {}
		try
		{
			funcOnMouseButtonDown = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&, int)>>("OnMouseButtonDown");
		}
		catch (std::exception ex) {}
		try
		{
			funcOnMouseButtonUp = chaiScriptCore->eval<std::function<void(chaiscript::Boxed_Value&, int)>>("OnMouseButtonUp");
		}
		catch (std::exception ex) {}

		GGameEngine->GetWidgetManager()->RegisterScriptObject(this);

		// Call SetupContent
		try
		{
			funcCreateContent(mScriptObject); // TODO: only call if function exists
		}
		catch (std::exception ex)
		{
			LOG_ERROR() << "Exception caught in Widget::SetupContentFromScript: " << ex.what();
		}

		return true;
	}


	void Widget::BindOnMousePressed(std::function<void()> arg_func)
	{
		mOnMousePressed.push_back(arg_func);
	}

	void Widget::BindOnMouseReleased(std::function<void()> arg_func)
	{
		mOnMouseReleased.push_back(arg_func);
	}


	const WidgetTransform& Widget::GetAbsoluteTransform()
	{
		if (mTransformIsDirty)
		{
			const WidgetTransform* parentTrans = nullptr;
			if (mParentWidget.IsValid())
			{
				parentTrans = &mParentWidget->GetAbsoluteTransform();
			}
			mAbsoluteTransform = mTransform.CreateAbsoluteWidgetTransform(parentTrans);

			mTransformIsDirty = false;
		}

		return mAbsoluteTransform; // return the re-calculated absolute transform of the widget
	}

	const size_t Widget::GetNumChildWidgets() const
	{
		return mChildWidgets.size();
	}

	ObjectPtr<Widget> Widget::GetChildWidgetAt(const size_t& arg_index) const
	{
		return mChildWidgets[arg_index];
	}

	void Widget::OnMouseEnter()
	{
		mIsHovered = true;
	}

	void Widget::OnMouseLeave()
	{
		mIsHovered = false;
	}

	void Widget::OnMouseButtonDown(MouseButtonID arg_button)
	{
		for (auto callbackFunc : mOnMousePressed)
		{
			callbackFunc();
		}
		if (funcOnMouseButtonDown != nullptr)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				funcOnMouseButtonDown(mScriptObject, arg_button); // TODO: only call if function exists
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in Widget::OnMouseButtonDown: " << ex.what();
			}
		}
	}

	void Widget::OnMouseButtonUp(MouseButtonID arg_button)
	{
		for (auto callbackFunc : mOnMouseReleased)
		{
			callbackFunc();
		}
		if (funcOnMouseButtonUp != nullptr)
		{
			chaiscript::ChaiScript* chaiScriptCore = GGameEngine->GetScriptManager()->GetChaiScriptCore();
			try
			{
				funcOnMouseButtonUp(mScriptObject, arg_button); // TODO: only call if function exists
			}
			catch (std::exception ex)
			{
				LOG_ERROR() << "Exception caught in Widget::OnMouseButtonUp: " << ex.what();
			}
		}
	}

}
