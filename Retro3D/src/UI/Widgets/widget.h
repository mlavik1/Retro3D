#ifndef RETRO3D_WIDGET_H
#define RETRO3D_WIDGET_H

/*=============================================================================================
Base class for Widgets

All GUI is rendered through Widgets.
Widgets can contain other Widgets, as well as Visuals (that do the actual rendering).

You can also specify a ChaiScript class to be used for constructing and updating the Widget.
The ChaiScript can implement the following functions:
  def CreateContent()
  def OnTick(deltatime)

==============================================================================================*/

#include "UI/Core/widgettransform.h"
#include <vector>
#include "Math/rectangle.h"
#include "Object/object.h"
#include "Object/objectptr.h"
#include "Object/weak_objectptr.h"
#include "UI/Visual/visual.h"
#include "Engine/input_listener.h"
#include "chaiscript/chaiscript.hpp"
#include <functional>

namespace Retro3D
{
	class IWidgetRenderer;

	class Widget : public Object, public IInputListener
	{
		friend class WidgetManager;

	protected:
		/**
		* Local transform of the Widget.
		*/
		WidgetTransform mTransform;

		/**
		* Cached absolute transform of the Widget.
		* If mTransformIsDirty == true, this needs to be recalculated.
		*/
		WidgetTransform mAbsoluteTransform;

		/** Does the absolute transform need to be recalculated? */
		bool mTransformIsDirty;

		bool mHasCreatedContent = false;

		bool mIsHovered = false;

		/** Child Widgets */
		std::vector<ObjectPtr<Widget>> mChildWidgets;
		
		/** Parent of this Widget */
		ObjectPtr<Widget> mParentWidget;

		/** Visuals to render */
		std::vector<ObjectPtr<Visual>> mVisuals;


		/** ChaiScript class name. */
		std::string mWidgetScriptClass;

		/** Instance of the ChaiScript class, owned by this Widget. */
		chaiscript::Boxed_Value mScriptObject;

		std::function<void(chaiscript::Boxed_Value&)> funcCreateContent;
		std::function<void(chaiscript::Boxed_Value&, float)> funcOnTick;
		std::function<void(chaiscript::Boxed_Value&, int)> funcOnMouseButtonDown;
		std::function<void(chaiscript::Boxed_Value&, int)> funcOnMouseButtonUp;


		// callback functions
		std::vector<std::function<void()>> mOnMousePressed;
		std::vector<std::function<void()>> mOnMouseReleased;

	public:
		Widget();
		~Widget();

		/** Sets up the Widget. Will also tell the ChaiScript object (if it has one) to create content. */
		virtual void CreateContent();

		/**
		* Called once, when the Widget has been created and is initialised.
		*/
		virtual void OnInitialise();

		/**
		* Called every frame.
		*/
		virtual void OnTick(float arg_deltatime);


		//virtual void OnInput(InputEvent arg_event);


		/**
		* Adds a pointer to a visual to be rendered.
		* Usually you want to call this from Widget::CreateContent()
		*/
		void AddVisual(Visual* arg_visual);


		/**
		* Marks the absolute transform as dirty.
		* This is called when doing transformations on the widget.
		*/
		void SetTransformDirty();

		/** Adds a child Widget.
		* The child Widget will be owned by this Widget, and is rendered inside it.
		*/
		void AddChildWidget(Widget* arg_widget);

		/**
		* Removes the specified child Widget.
		* This may cause in the child Widget being destroyed (when the strong reference count becomes 0).
		*/
		bool RemoveChildWidget(Widget* arg_widget);


		/***/
		/***** Modifiers ******/

		void SetPosition(glm::vec2 arg_pos);
		void SetSize(glm::vec2 arg_size);
		void SetPivot(glm::vec2 arg_pivot);

		inline void SetPosition(const float& x, const float& y) { SetPosition(glm::vec2(x, y)); }
		inline void SetSize(const float& w, const float& h) { SetSize(glm::vec2(w, h)); }
		inline void SetPivot(const float& x, const float& y) { SetPivot(glm::vec2(x, y)); }

		void SetVerticalPositioning(WidgetPositioningMode arg_mode);
		void SetHorizontalPositioning(WidgetPositioningMode arg_mode);
		void SetVerticalScaling(WidgetScalingMode arg_mode);
		void SetHorizontalScaling(WidgetScalingMode arg_mode);

		void SetWidgetScriptClass(const char* arg_script);
		bool SetupContentFromScript();

		void BindOnMousePressed(std::function<void()> arg_func);
		void BindOnMouseReleased(std::function<void()> arg_func);

		/***/
		/***** Accessors ******/

		const glm::vec2& GetPosition() const { return mTransform.mPosition; }
		const glm::vec2& GetSize() const { return mTransform.mSize; };
		const glm::vec2& GetPivot() const { return mTransform.mPivot; };

		const WidgetPositioningMode& GetVerticalPositioning() const { return mTransform.mVerticalPositioning; };
		const WidgetPositioningMode& GetHorizontalPositioning() const { return mTransform.mHorizontalPositioning; };
		const WidgetScalingMode& GetVerticalScaling() const { return mTransform.mVerticalScaling; };
		const WidgetScalingMode& GetHorizontalScaling() const { return mTransform.mHorizontalScaling; };

		/**
		* Gets the cached absolute transform of the Widget.
		* Note: Only valid if mTransformIsDirty == true
		*/
		const WidgetTransform& GetCachedAbsoluteTransform() const { return mAbsoluteTransform; };
		
		/**
		* Gets the absolute transform of the Widget.
		* This will re-calculate the absolute transform if it's dirty.
		*/
		const WidgetTransform& GetAbsoluteTransform();

		const size_t GetNumChildWidgets() const;
		ObjectPtr<Widget> GetChildWidgetAt(const size_t& arg_index) const;

		std::string GetWidgetScriptClass() { return mWidgetScriptClass; };

		virtual void OnMouseEnter();
		virtual void OnMouseLeave();

		/***/
		/***** Interface functions ******/

		virtual void OnMouseButtonDown(MouseButtonID arg_button) override;
		virtual void OnMouseButtonUp(MouseButtonID arg_button) override;

	};
}

#endif
