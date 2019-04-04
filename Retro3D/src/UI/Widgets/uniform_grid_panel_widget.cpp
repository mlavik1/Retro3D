#include "uniform_grid_panel_widget.h"

namespace Retro3D
{
	void UniformGridPanelWidget::SetNewColuumnWidths(int arg_fromCol)
	{
		if (mAutoSizeGridCells)
		{
			mGridCellWidth = 1.0f / mNumColumns;
			mCellHorisontalScalingMode = WidgetScalingMode::Relative;
		}

		for (int iCol = 0; iCol < mNumColumns; iCol++)
		{
			mColumnWidths[iCol] = mGridCellWidth;
		}
	}

	void UniformGridPanelWidget::SetNewRowHeights(int arg_fromRow)
	{
		if (mAutoSizeGridCells)
		{
			mGridCellHeight = 1.0f / mNumRows;
			mCellVerticalScalingMode = WidgetScalingMode::Relative;
		}

		for (int iRow = 0; iRow < mNumRows; iRow++)
		{
			mRowHeights[iRow] = mGridCellHeight;
		}
	}

	void UniformGridPanelWidget::SetGridCellSize(float arg_width, float arg_height, WidgetScalingMode arg_horizontalScalingMode, WidgetScalingMode arg_verticalScalingMode)
	{
		if (!mAutoSizeGridCells)
		{
			mCellVerticalScalingMode = arg_verticalScalingMode;
			mCellHorisontalScalingMode = arg_horizontalScalingMode;
			mGridCellWidth = arg_width;
			mGridCellHeight = arg_height;

			UpdateCellTransforms();
		}
	}

	void UniformGridPanelWidget::SetAutoSize(bool arg_autoSize)
	{
		mAutoSizeGridCells = arg_autoSize;

		SetNewColuumnWidths(0);
		SetNewRowHeights(0);

		UpdateCellTransforms();
	}
}
