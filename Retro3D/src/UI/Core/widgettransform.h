#ifndef RETRO3D_WIDGETTRANSFORM_H
#define RETRO3D_WIDGETTRANSFORM_H

/*=============================================================================================
									Widget Transform

Defines the location, size and other replated properties of a Widget.
Each Widget has a local transform, which needs to be turned into an absolute transform
 before rendering.
==============================================================================================*/

#include "glm/glm.hpp"

namespace Retro3D
{
	enum class WidgetPositioningMode // TODO: Merge with WidgetScalingMode?
	{
		Absolute,
		Relative
	};

	enum class WidgetScalingMode // TODO: Merge with WidgetPositioningMode?
	{
		Absolute,
		Relative
	};

	class WidgetTransform
	{
	public:
		glm::vec2 mPosition;
		glm::vec2 mSize;
		glm::vec2 mPivot;

		WidgetPositioningMode mVerticalPositioning;
		WidgetPositioningMode mHorizontalPositioning;

		WidgetScalingMode mVerticalScaling;
		WidgetScalingMode mHorizontalScaling;

	public:
		WidgetTransform();

		/**
		* Creates an absolute WidgetTransform from this (local) WidgetTransform.
		* IMPORANT:
		*   - this must be a local WidgetTransform (Widget::mTransform is local)
		*   - arg_parent_absolute must be the absolute WidgetTransform of the parent Widget (or NULL)
		*/
		WidgetTransform CreateAbsoluteWidgetTransform(const WidgetTransform *arg_parent_absolute);

	};
}

#endif
