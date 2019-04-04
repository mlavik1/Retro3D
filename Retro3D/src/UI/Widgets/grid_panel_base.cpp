#include "grid_panel_base.h"

namespace Retro3D
{

	void GridPanelBase::SetNewColuumnWidths(int arg_fromCol)
	{
		for (int iCol = arg_fromCol; iCol < mNumColumns; iCol++)
		{
			mColumnWidths[iCol] = 0.2f;
		}
	}

	void GridPanelBase::SetNewRowHeights(int arg_fromRow)
	{
		for (int iRow = arg_fromRow; iRow < mNumRows; iRow++)
		{
			mRowHeights[iRow] = 0.2f;
		}
	}

	void GridPanelBase::AddCells(int arg_fromColumn, int arg_fromRow)
	{
		for (int iRow = arg_fromRow; iRow < mNumRows; iRow++)
		{
			for (int iCol = arg_fromColumn; iCol < mNumColumns; iCol++)
			{
				const int cellIndex = GetGridCellIndex(iCol, iRow);
				GridPanelCellWidget* cellWidget = new GridPanelCellWidget(); // TODO: make static CreateWidget-function?
				mCells[cellIndex] = cellWidget;
				AddChildWidget(cellWidget);
			}
		}
	}

	void GridPanelBase::RemoveCells(int arg_fromColumn, int arg_fromRow)
	{
		for (int iRow = arg_fromRow; iRow < mNumRows; iRow++)
		{
			for (int iCol = arg_fromColumn; iCol < mNumColumns; iCol++)
			{
				const int cellIndex = GetGridCellIndex(iCol, iRow);
				GridPanelCellWidget* cellWidget = mCells[cellIndex];
				RemoveChildWidget(cellWidget);
				delete cellWidget; // TODO: destroy in a nicer way?
				mCells[cellIndex] = nullptr;
			}
		}
	}

	void GridPanelBase::UpdateCellTransforms()
	{
		float currRowY = 0.0f;
		for (int iRow = 0; iRow < mNumRows; iRow++)
		{
			const float currRowHeight = mRowHeights[iRow];
			float currColX = 0.0f;
			for (int iCol = 0; iCol < mNumColumns; iCol++)
			{
				const float currColWidth = mColumnWidths[iCol];

				GridPanelCellWidget* cellWidget = GetGridCell(iCol, iRow);
				cellWidget->SetVerticalScaling(mCellVerticalScalingMode);
				cellWidget->SetHorizontalScaling(mCellHorisontalScalingMode);
				cellWidget->SetVerticalPositioning(mCellVerticalScalingMode == WidgetScalingMode::Absolute ? WidgetPositioningMode::Absolute : WidgetPositioningMode::Relative);
				cellWidget->SetHorizontalPositioning(mCellHorisontalScalingMode == WidgetScalingMode::Absolute ? WidgetPositioningMode::Absolute : WidgetPositioningMode::Relative);
				cellWidget->SetSize(currColWidth, currRowHeight);
				cellWidget->SetPosition(currColX, currRowY);
				
				currColX += currColWidth;
			}
			currRowY += currRowHeight;
		}

		// TODO: Grow grid to child cells size
	}

	void GridPanelBase::SetDimension(int arg_columns, int arg_rows)
	{
		int oldNumRows = mNumRows;
		int oldNumCols = mNumColumns;

		if (oldNumRows < arg_rows)
		{
			RemoveCells(0, oldNumRows);
		}
		if (oldNumCols < arg_columns)
		{
			RemoveCells(oldNumCols, 0);
		}

		mNumRows = arg_rows;
		mNumColumns = arg_columns;
		mRowHeights.resize(arg_rows);
		mColumnWidths.resize(arg_columns);
		mCells.resize(arg_columns * arg_rows);

		if (mNumRows > oldNumRows)
		{
			SetNewRowHeights(oldNumRows);
			AddCells(0, oldNumRows);
		}
		if (mNumColumns > oldNumCols)
		{
			SetNewColuumnWidths(oldNumCols);
			AddCells(oldNumCols, 0);
		}

		UpdateCellTransforms();
	}

	int GridPanelBase::GetGridCellIndex(int arg_col, int arg_row)
	{
		return arg_col + arg_row * mNumColumns;
	}

	GridPanelCellWidget* GridPanelBase::GetGridCell(int arg_col, int arg_row)
	{
		if (arg_col >= 0 && arg_col < mNumColumns && arg_row >= 0 && arg_row < mNumRows)
		{
			return mCells[GetGridCellIndex(arg_col, arg_row)];
		}
		return nullptr;
	}

	void GridPanelBase::AddWidgetToGridCell(Widget* arg_widget, int arg_col, int arg_row)
	{
		GridPanelCellWidget* cellWidget = GetGridCell(arg_col, arg_row);
		if (cellWidget != nullptr)
		{
			cellWidget->AddChildWidget(arg_widget);
		}
	}

}
