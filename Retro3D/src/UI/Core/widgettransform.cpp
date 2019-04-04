#include "widgettransform.h"

namespace Retro3D
{
	WidgetTransform::WidgetTransform()
	{
		mPosition = glm::vec2(0.0f, 0.0f);
		mSize = glm::vec2(0.0f, 0.0f);
		mPivot = glm::vec2(0.0f, 0.0f);

		mVerticalPositioning = WidgetPositioningMode::Relative;
		mHorizontalPositioning = WidgetPositioningMode::Relative;

		mVerticalScaling = WidgetScalingMode::Relative;
		mHorizontalScaling = WidgetScalingMode::Relative;

	}

	WidgetTransform WidgetTransform::CreateAbsoluteWidgetTransform(const WidgetTransform *arg_parent_absolute)
	{
		WidgetTransform absoluteTransform = *this;

		if (arg_parent_absolute == nullptr)
		{
			return absoluteTransform; // I am the transform of a root widget
		}

		const glm::vec2 &parentPos = arg_parent_absolute->mPosition;
		const glm::vec2 &parentSize = arg_parent_absolute->mSize;

		if (mVerticalPositioning == WidgetPositioningMode::Relative)
		{
			absoluteTransform.mPosition.x = (parentPos.x + parentSize.x) * mPosition.x + parentPos.x * (1.0f - mPosition.x);
		}
		if (mHorizontalPositioning == WidgetPositioningMode::Relative)
		{
			absoluteTransform.mPosition.y = (parentPos.y + parentSize.y) * mPosition.y + parentPos.y * (1.0f - mPosition.y);
		}
		if (mVerticalScaling == WidgetScalingMode::Relative)
		{
			absoluteTransform.mSize.x = parentSize.x * mSize.x;
		}
		if (mHorizontalScaling == WidgetScalingMode::Relative)
		{
			absoluteTransform.mSize.y = parentSize.y * mSize.y;
		}

		const glm::vec2 absPivotPos = (absoluteTransform.mPosition + absoluteTransform.mSize) * mPivot + absoluteTransform.mPosition * (1.0f - mPivot);
		absoluteTransform.mPosition -= (absPivotPos - absoluteTransform.mPosition);

		absoluteTransform.mHorizontalPositioning = WidgetPositioningMode::Absolute;
		absoluteTransform.mVerticalPositioning = WidgetPositioningMode::Absolute;
		absoluteTransform.mHorizontalScaling = WidgetScalingMode::Absolute;
		absoluteTransform.mVerticalScaling = WidgetScalingMode::Absolute;

		return absoluteTransform;
	}

}