#include "grid_panel_base.h"

/*=============================================================================================
Uniform grid panel widget.
Contains a number of grid cells, with a specified width and height.

==============================================================================================*/

namespace Retro3D
{
	class UniformGridPanelWidget : public GridPanelBase
	{
	private:
		float mGridCellWidth = 0.2f;
		float mGridCellHeight = 0.2f;
		bool mAutoSizeGridCells = false;

	protected:
		virtual void SetNewColuumnWidths(int arg_fromCol) override;
		virtual void SetNewRowHeights(int arg_fromRow) override;

	public:
		/**
		* Sets the size of all grid cells.
		*/
		void SetGridCellSize(float arg_width, float arg_height, WidgetScalingMode arg_horizontalScalingMode, WidgetScalingMode arg_verticalScalingMode);
		
		/**
		* Enables/disables automatic sizing of grid cells.
		* If enabled, the grid cells with be resized to fill the whole grid.
		*/
		void SetAutoSize(bool arg_autoSize);
	};
}
