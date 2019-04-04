#ifndef RETRO3D_GRIDPANELWIDGET_H
#define RETRO3D_GRIDPANELWIDGET_H

#include "panel_widget.h"

/*=============================================================================================
Base class for GridPanel Widgets, containing other widgets inside a grid.
The grid may be uniform or non uniform.

==============================================================================================*/

#include <vector>

namespace Retro3D
{
	class GridPanelCellWidget : public VisualWidget
	{
	};

	class GridPanelBase : public PanelWidget
	{
	protected:
		std::vector<GridPanelCellWidget*> mCells;
		std::vector<float> mColumnWidths;
		std::vector<float> mRowHeights;
		int mNumRows = 0;
		int mNumColumns = 0;
		WidgetScalingMode mCellVerticalScalingMode = WidgetScalingMode::Absolute;
		WidgetScalingMode mCellHorisontalScalingMode = WidgetScalingMode::Absolute;

		/** Setup column width of newly added column(s) */
		virtual void SetNewColuumnWidths(int arg_fromCol);

		/** Setup row height of newly added row(s) */
		virtual void SetNewRowHeights(int arg_fromRow);

		/** Initialise new cells starting at the specified row and column. */
		void AddCells(int arg_fromColumn, int arg_fromRow);

		/** Remove all cells starting at the specified column and row index, towards the end. */
		void RemoveCells(int arg_fromColumn, int arg_fromRow);

		/** Update transform (position and size) of all cells. */
		void UpdateCellTransforms();

		int GetGridCellIndex(int arg_col, int arg_row);

	public:
		/**
		* Sets the nubmer of columns and rows.
		*/
		void SetDimension(int arg_columns, int arg_rows);

		/**
		* Gets the grid cell (GridPanelCellWidget) at the specified location.
		*/
		GridPanelCellWidget* GetGridCell(int arg_col, int arg_row);

		/**
		* Adds a Widget as the child of the GridPanelCellWidget at the specified location.
		*/
		void AddWidgetToGridCell(Widget* arg_widget, int arg_col, int arg_row);
	};
}

#endif
