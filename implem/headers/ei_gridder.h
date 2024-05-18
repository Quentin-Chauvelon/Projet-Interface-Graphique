/**
 *  @file	ei_gridder.h
 *  @brief	Defines functions to manage the positioning and sizing of widgets on the
 *          screen in a grid
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