/**
 *  @file	ei_gridder.h
 *  @brief	Manages the positioning and sizing of widgets on the screen in a grid.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_GRIDDER_H
#define EI_GRIDDER_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"

typedef struct ei_gridder_t
{
    ei_impl_geom_param_t geom_param; ///< The common geom param of the geometry managers
    int row;                         ///< The row in which to place the widget
    int column;                      ///< The column in which to place the widget
    int row_span;                    ///< The number of rows the widget takes
    int column_span;                 ///< The number of columns the widget takes
    bool fill_cell;                  ///< If true, the widget will stretch to fill its cell
    ei_anchor_t anchor;              ///< The point to which the widget will be anchored relative to the cell
} ei_gridder_t;

/**
 * \brief	Struct to store data about a grid common to all widgets in the grid
 *          (hence why it has a reference to the parent widget).
 */
typedef struct ei_gridder_data_t
{
    ei_widget_t parent;                ///< The parent widget in which children are placed using the gridder
    int rows;                          ///< The number of rows in the grid
    int columns;                       ///< The number of columns in the grid
    int_array_t *rows_min_size;        ///< An array containing the minimum size of each row
    int_array_t *columns_min_size;     ///< An array containing the minimum size of each column
    int_array_t *rows_current_size;    ///< An array containing the current size of each row
    int_array_t *columns_current_size; ///< An array containing the current size of each column
    bool_array_t *stretched_rows;      ///< An array containing a boolean indicating if the row should stretch to fill the parent
    bool_array_t *stretched_columns;   ///< An array containing a boolean indicating if the column should stretch to fill the parent
    int horizontal_spacing;            ///< The space between each column. Defaults to 5
    int vertical_spacing;              ///< The space between each row. Defaults to 5
    ei_anchor_t anchor;                ///< Where the grid is anchored in the parent widget. Defaults to ei_anc_center
    int margin;                        ///< The margin around the grid. Defaults to 5
    struct ei_gridder_data_t *next;    ///< A pointer to the next gridder data
} ei_gridder_data_t;

/**
 * \brief	Set the given widget in the cell specified by the parameters. The values remains unchanged when the
 *          parameters are NULL except at initialization where the default values are used.
 *          Each cell parameters are managed by the widget in the cell while grid parameters (common to all cells)
 *          are managed by the parent widget. A parent manages a grid if at least one of its children is managed
 *          by the gridder.
 *          If no column has fill set to true, each column will be the exact size of the biggest widget in the column, and
 *          thus, the grid may not fill the parent widget.
 *          If at least one column has fill set to true, these columns will stretch to fill the parent widget. If more
 *          than one column has fill set to true, the space to fill will be shared equally between the columns.
 *          If the total minimum size of the columns is greater than the parent widget's width, the overflow is divided
 *          equally by the number of columns and each column will be shrunk by this value.
 *          The same criterias apply for rows as well.
 *
 * @param	widget		The widget to place in the grid.
 * @param	row			The row in which to place the widget. If this value is greater than the number of rows,
 *                      the grid will expand to add the required number of rows. Defaults to the first free cell
 * @param	column		The column in which to place the widget. If this value is greater than the number of columns,
 *                      the grid will expand to add the required number of columns. Defaults to the first free cell.
 * @param	row_span	The number of rows the widget takes. Defaults to 1.
 * @param	column_span	The number of columns the widget takes. Defaults to 1.
 * @param	fill_row	If true, the row will stretch to fill the remaining space in the parent widget . Defaults to false.
 * @param	fill_column	If true, the column will stretch to fill the remaining space in the parent widget. Defaults to false.
 * @param	fill_cell	If true, the widget will stretch to fill its cell, otherwise the widget will be its natural size. Defaults to false.
 * @param	anchor		The point to which the widget will be anchored relative to the cell. Defaults to ei_anc_northwest.
 */
void ei_set_cell(ei_widget_t widget, int *row, int *column, int *row_span, int *column_span, bool *fill_row, bool *fill_column, bool *fill_cell, ei_anchor_t *anchor);

/**
 * @brief	Set the given widget to the cell matching the given row and column
 */
static inline void ei_set_cell_position(ei_widget_t widget, int row, int column)
{
    ei_set_cell(widget, &row, &column, NULL, NULL, NULL, NULL, NULL, NULL);
}

/**
 * @brief	Make the widget span the given number of rows
 */
static inline void ei_set_cell_row_span(ei_widget_t widget, int row_span)
{
    ei_set_cell(widget, NULL, NULL, &row_span, NULL, NULL, NULL, NULL, NULL);
}

/**
 * @brief	Make the widget span the given number of columns
 */
static inline void ei_set_cell_column_span(ei_widget_t widget, int column_span)
{
    ei_set_cell(widget, NULL, NULL, NULL, &column_span, NULL, NULL, NULL, NULL);
}

/**
 * @brief	Make the row containing the widget's cell stretch to fill the parent widget
 */
static inline void ei_cell_fill_row(ei_widget_t widget)
{
    ei_set_cell(widget, NULL, NULL, NULL, NULL, &(bool){true}, NULL, NULL, NULL);
}

/**
 * @brief	Make the column containing the widget's cell stretch to fill the parent widget
 */
static inline void ei_cell_fill_column(ei_widget_t widget)
{
    ei_set_cell(widget, NULL, NULL, NULL, NULL, NULL, &(bool){true}, NULL, NULL);
}

/**
 * @brief	Make the widget stretch to fill its cell
 */
static inline void ei_cell_fill(ei_widget_t widget)
{
    ei_set_cell(widget, NULL, NULL, NULL, NULL, NULL, NULL, &(bool){true}, NULL);
}

/**
 * @brief	Set the anchor of the widget in its cell if fill is set to false
 */
static inline void ei_set_cell_anchor(ei_widget_t widget, ei_anchor_t anchor)
{
    ei_set_cell(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &anchor);
}

/**
 * @brief	If fill is true, the row at the given index will stretch to fill the parent widget
 *
 * @param	parent	The widget in which at least one child is managed by the gridder.
 * @param	index	The index of the row to fill
 * @param	fill	True if the row should stretch to fill the parent widget, false otherwise
 */
void ei_set_grid_fill_row_at_index(ei_widget_t parent, int index, bool fill);

/**
 * @brief	If fill is true, the column at the given index will stretch to fill the parent widget
 *
 * @param	parent	The widget in which at least one child is managed by the gridder.
 * @param	index	The index of the column to fill
 * @param	fill	True if the column should stretch to fill the parent widget, false otherwise
 */
void ei_set_grid_fill_column_at_index(ei_widget_t parent, int index, bool fill);

/**
 * \brief	Set the spacing between each row and column in the grid. If one of the parameters is NULL,
 *          the value remains unchanged.
 *
 * @param	parent				The widget in which at least one child is managed by the gridder.
 * @param	horizontal_spacing	The space between each column
 * @param	vertical_spacing	The space between each row
 */
void ei_set_grid_spacing(ei_widget_t parent, int *horizontal_spacing, int *vertical_spacing);

/**
 * @brief	Set the anchor of the grid in the parent widget
 *
 * @param	parent	The widget in which at least one child is managed by the gridder.
 * @param	anchor	The point to which the grid will be anchored
 */
void ei_set_grid_anchor(ei_widget_t parent, ei_anchor_t anchor);

/**
 * @brief	Set the margin around the grid
 *
 * @param	parent	The widget in which at least one child is managed by the gridder.
 * @param	margin	The number of pixels that will be left between the grid and the parent widget
 *                  on each side
 */
void ei_set_grid_margin(ei_widget_t parent, int margin);

/**
 * @brief	Recompute the geometry of all children of the parent widget
 *          that are managed by the gridder
 *
 * @param	widget	The widget to remove from the gridder
 */
void ei_recompute_geometry_of_grid(ei_widget_t parent);

/**
 * \brief	A function that runs the geometry computation for this widget.
 * 		    Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_gridder_runfunc(ei_widget_t widget);

/**
 * \brief	A function called when a widget cease to be managed by its geometry manager.
 *		    Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		    only provided to trigger recomputation when the disappearance of a widget has an
 *		    effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the geometry manager.
 */
void ei_gridder_releasefunc(ei_widget_t widget);

#endif