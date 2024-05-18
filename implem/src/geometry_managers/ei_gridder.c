#include "../api/ei_utils.h"
#include "../api/ei_application.h"
#include "../api/ei_widget_attributes.h"
#include "../api/ei_geometrymanager.h"
#include "../implem/headers/ei_gridder.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"

ei_gridder_data_t *ei_first_gridder_data = NULL;

/**
 * @brief   Create a grid with the given number of rows and columns in the parent widget
 *
 * @param   parent      The widget in which at least one child is managed by the gridder.
 * @param   rows        The number of rows in the grid. Can grow dynamically if a widget is placed outside of the grid.
 * @param   columns     The number of columns in the grid. Can grow dynamically if a widget is placed outside of the grid.
 *
 * @return  The data of the grid
 */
ei_gridder_data_t *ei_create_grid(ei_widget_t parent, int rows, int columns)
{
    ei_gridder_data_t *gridder_data = malloc(sizeof(ei_gridder_data_t));

    if (gridder_data == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the gridder data.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    gridder_data->parent = parent;
    gridder_data->rows = rows;
    gridder_data->columns = columns;
    gridder_data->rows_min_size = ei_int_array_initialize();
    gridder_data->columns_min_size = ei_int_array_initialize();
    gridder_data->rows_current_size = ei_int_array_initialize();
    gridder_data->columns_current_size = ei_int_array_initialize();
    gridder_data->stretched_rows = ei_bool_array_initialize();
    gridder_data->stretched_columns = ei_bool_array_initialize();
    gridder_data->horizontal_spacing = 5;
    gridder_data->vertical_spacing = 5;
    gridder_data->anchor = ei_anc_center;
    gridder_data->margin = 5;
    gridder_data->next = NULL;

    if (ei_first_gridder_data == NULL)
    {
        ei_first_gridder_data = gridder_data;
    }
    else
    {
        ei_gridder_data_t *current = ei_first_gridder_data;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = gridder_data;
    }

    return gridder_data;
}

/**
 * @brief   Return the grid of the parent widget
 *
 * @param   parent      The widget in which at least one child is managed by the gridder.
 *
 * @return  The grid of the parent widget
 */
ei_gridder_data_t *ei_get_grid(ei_widget_t parent)
{
    ei_gridder_data_t *current = ei_first_gridder_data;

    while (current != NULL)
    {
        if (current->parent == parent)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

/**
 * @brief   Return the grid of the parent widget or create it if it doesn't exist
 *
 * @param   parent      The widget in which at least one child is managed by the gridder.
 * @param   rows        The number of rows in the grid. Can grow dynamically if a widget is placed outside of the grid.
 * @param   columns     The number of columns in the grid. Can grow dynamically if a widget is placed outside of the grid.
 *
 * @return  The grid of the parent widget
 */
ei_gridder_data_t *ei_create_or_get_grid(ei_widget_t parent, int rows, int columns)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        gridder_data = ei_create_grid(parent, rows, columns);
    }

    return gridder_data;
}

/**
 * @brief   Return if the widget is currently managed by the gridder
 *
 * @param   widget      The widget to check
 *
 * @return  True if the widget is managed by the gridder, false otherwise
 */
bool ei_is_widget_managed_by_gridder(ei_widget_t widget)
{
    return ei_widget_is_displayed(widget) && strcmp(widget->geom_params->manager->name, "gridder") == 0;
}

/**
 * @brief   Return if the cell at the given row and column is free or already occupied by another widget
 *
 * @param   gridder_data    The grid
 * @param   row             The row to check
 * @param   column          The column to check
 *
 * @return  True if the cell is free, false otherwise
 */
bool ei_is_cell_free(ei_gridder_data_t *gridder_data, int row, int column)
{
    // For each children of the parent widget managed by the gridder, check if the cell is free
    for (ei_widget_t cell_widget = gridder_data->parent->children_head; cell_widget != NULL; cell_widget = cell_widget->next_sibling)
    {
        if (ei_is_widget_managed_by_gridder(cell_widget))
        {
            ei_gridder_t *cell_widget_geom_param = (ei_gridder_t *)ei_widget_get_geom_params(cell_widget);

            // Check if the given row and column are inside the rectangle created by the widget's cell + row/column span
            if (row >= cell_widget_geom_param->row &&
                row < cell_widget_geom_param->row + cell_widget_geom_param->row_span &&
                column >= cell_widget_geom_param->column &&
                column < cell_widget_geom_param->column + cell_widget_geom_param->column_span)
            {
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief   Return the first free cell in the grid. The search is done row by row, from left to right.
 *          The result is stored in the row and column pointers. If no free cell is found, return -1.
 *
 * @param   gridder_data    The grid
 * @param   row             The row of the first free cell or -1
 * @param   column          The column of the first free cell or -1
 */
void ei_get_first_free_cell(ei_gridder_data_t *gridder_data, int *row, int *column)
{
    bool free_cell = true;

    // Iterate over each cell to find the first free cell for the widget
    for (int i = 0; i < gridder_data->rows; i++)
    {
        for (int j = 0; j < gridder_data->columns; j++)
        {
            if (ei_is_cell_free(gridder_data, i, j))
            {
                *row = i;
                *column = j;

                return;
            }
        }
    }

    // If no free cell was found, return -1
    *row = -1;
    *column = -1;
}

void ei_set_cell(ei_widget_t widget, int *row, int *column, int *row_span, int *column_span, bool *fill_row, bool *fill_column, bool *fill_cell, ei_anchor_t *anchor)
{
    ei_gridder_t *gridder_geom_param;

    // If the widget has not been drawn yet, allocate memory
    if (!ei_widget_is_displayed(widget))
    {
        gridder_geom_param = malloc(sizeof(ei_gridder_t));

        // If malloc failed, return
        if (gridder_geom_param == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory for the widget's gridder geom params.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }
    }
    // Otherwise, get the geom params of the widget
    else
    {
        gridder_geom_param = (ei_gridder_t *)ei_widget_get_geom_params(widget);

        // If the widget was managed by another geometry manager, unmap the widget
        if (strcmp(widget->geom_params->manager->name, "gridder") != 0)
        {
            ei_geometrymanager_unmap(widget);
        }
    }

    // Update the geometry manager handling the widget
    char geometrymanager_name[20] = "gridder";
    gridder_geom_param->geom_param.manager = ei_geometrymanager_from_name(geometrymanager_name);

    ei_gridder_data_t *gridder_data = ei_create_or_get_grid(widget->parent, row != NULL ? *row : 0, column != NULL ? *column : 0);

    // If the widget class doesn't exist, return
    if (gridder_geom_param->geom_param.manager == NULL)
    {
        free(gridder_geom_param);
        printf("\033[0;33mWarning: the geometry manager %s doesn't exist.\n\t at %s (%s:%d)\033[0m\n", geometrymanager_name, __func__, __FILE__, __LINE__);
        return;
    }

    // If the widget is not yet displayed, initalize the geom params with the given parameters or their default values
    if (!ei_widget_is_displayed(widget))
    {
        if (row != NULL && column != NULL)
        {
            if (!ei_is_cell_free(gridder_data, *row, *column))
            {
                printf("\033[0;33mWarning: Cell (%d, %d) is already taken by another widget.\n\t at %s (%s:%d)\033[0m\n", *row, *column, __func__, __FILE__, __LINE__);
                return;
            }

            gridder_geom_param->row = *row;
            gridder_geom_param->column = *column;

            // Expand the grid if the widget is placed outside of the grid
            if (*row >= gridder_data->rows)
            {
                gridder_data->rows = *row + 1;
            }

            if (*column >= gridder_data->columns)
            {
                gridder_data->columns = *column + 1;
            }
        }
        // If the row and column are not given, find the first free cell in the grid
        else
        {
            int free_row, free_column = 0;

            ei_get_first_free_cell(gridder_data, &free_row, &free_column);

            if (free_row != -1 && free_column != -1)
            {
                gridder_geom_param->row = free_row;
                gridder_geom_param->column = free_column;
            }
            // If no free cell was found, expand the grid and put the widget in the first column on the new row
            else
            {
                gridder_data->rows++;

                gridder_geom_param->row = gridder_data->rows - 1;
                gridder_geom_param->column = 0;
            }
        }

        gridder_geom_param->row_span = row_span != NULL ? *row_span : 1;
        gridder_geom_param->column_span = column_span != NULL ? *column_span : 1;
        gridder_geom_param->fill_cell = fill_cell != NULL ? *fill_cell : false;

        // if fill_row is not given and the row is not already marked as stretched, initialize the stretch row value to false
        if (fill_row == NULL && !gridder_data->stretched_rows->array[gridder_geom_param->row])
        {
            ei_bool_array_set_at_index(gridder_data->stretched_rows, gridder_geom_param->row, false);
        }

        // if fill_column is not given and the column is not already marked as stretched, initialize the stretch column value to false
        if (fill_column == NULL && !gridder_data->stretched_columns->array[gridder_geom_param->column])
        {
            ei_bool_array_set_at_index(gridder_data->stretched_columns, gridder_geom_param->column, false);
        }

        gridder_geom_param->anchor = anchor != NULL && *anchor != ei_anc_none ? *anchor : ei_anc_northwest;
    }
    else
    {
        if (row != NULL)
        {
            gridder_geom_param->row = *row;

            // Expand the grid if the widget is placed outside of the grid
            if (*row >= gridder_data->rows)
            {
                gridder_data->rows = *row + 1;
            }
        }

        if (column != NULL)
        {
            gridder_geom_param->column = *column;

            if (*column >= gridder_data->columns)
            {
                gridder_data->columns = *column + 1;
            }
        }

        if (row_span != NULL)
        {
            gridder_geom_param->row_span = *row_span;
        }

        if (column_span != NULL)
        {
            gridder_geom_param->column_span = *column_span;
        }

        if (fill_cell != NULL)
        {
            gridder_geom_param->fill_cell = *fill_cell;
        }

        if (anchor != NULL)
        {
            gridder_geom_param->anchor = *anchor;
        }
    }

    if (fill_row != NULL)
    {
        ei_bool_array_set_at_index(gridder_data->stretched_rows, gridder_geom_param->row, *fill_row);
    }

    if (fill_column != NULL)
    {
        ei_bool_array_set_at_index(gridder_data->stretched_columns, gridder_geom_param->column, *fill_column);
    }

    ei_widget_set_geom_params(widget, (ei_geom_param_t)gridder_geom_param);

    gridder_geom_param->geom_param.manager->runfunc(widget);
}

void ei_set_grid_fill_row_at_index(ei_widget_t parent, int index, bool fill)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_bool_array_set_at_index(gridder_data->stretched_rows, index, fill);

    // Recompute the geometry of all children of the parent since the row fill has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_grid_fill_column_at_index(ei_widget_t parent, int index, bool fill)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_bool_array_set_at_index(gridder_data->stretched_columns, index, fill);

    // Recompute the geometry of all children of the parent since the column fill has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_grid_spacing(ei_widget_t parent, int *horizontal_spacing, int *vertical_spacing)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    if (horizontal_spacing != NULL)
    {
        gridder_data->horizontal_spacing = *horizontal_spacing;
    }

    if (vertical_spacing != NULL)
    {
        gridder_data->vertical_spacing = *vertical_spacing;
    }

    // Recompute the geometry of all children of the parent since the spacing has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_grid_anchor(ei_widget_t parent, ei_anchor_t anchor)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    gridder_data->anchor = anchor;

    // Recompute the geometry of all children of the parent since the anchor has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_grid_margin(ei_widget_t parent, int margin)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    gridder_data->margin = margin;

    // Recompute the geometry of all children of the parent since the margin has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_row_min_size(ei_widget_t parent, int row, int size)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_int_array_set_at_index(gridder_data->rows_min_size, row, size);

    // Recompute the geometry of all children of the parent since the row min size has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_set_column_min_size(ei_widget_t parent, int column, int size)
{
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the parent widget has no child managed by the gridder.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_int_array_set_at_index(gridder_data->columns_min_size, column, size);

    // Recompute the geometry of all children of the parent since the column min size has changed
    ei_recompute_geometry_of_grid(parent);
}

void ei_recompute_geometry_of_grid(ei_widget_t parent)
{
    for (ei_widget_t children = parent->children_head; children != NULL; children = children->next_sibling)
    {
        if (ei_is_widget_managed_by_gridder(children))
        {
            children->geom_params->manager->runfunc(children);
        }
    }
}

void ei_gridder_runfunc(ei_widget_t widget)
{
    DEBUG ? printf("Placing widget %d\n", widget->pick_id) : 0;

    ei_gridder_t *gridder_geom_param = (ei_gridder_t *)ei_widget_get_geom_params(widget);
    ei_widget_t parent = widget->parent;
    ei_gridder_data_t *gridder_data = ei_get_grid(parent);

    if (gridder_data == NULL)
    {
        printf("\033[0;33mWarning: the widget %s is not managed by a gridder.\n\t at %s (%s:%d)\033[0m\n", widget->wclass->name, __func__, __FILE__, __LINE__);
        return;
    }

    // If the row only spans one row, update the min size of the row
    if (gridder_geom_param->row_span == 1)
    {
        // If the widget is wider than any other widget in this row, update the min size of the row
        if (widget->requested_size.height > gridder_data->rows_min_size->array[gridder_geom_param->row])
        {
            ei_int_array_set_at_index(gridder_data->rows_min_size, gridder_geom_param->row, widget->requested_size.height);
        }
    }
    // Otherwise, if it spans multiple rows, update the min size of each row
    else
    {
        int size_of_rows = 0;

        // Calculate the total size of the rows the widget spans for
        for (int i = gridder_geom_param->row; i < gridder_geom_param->row + gridder_geom_param->row_span; i++)
        {
            size_of_rows += gridder_data->rows_min_size->array[i];
        }

        size_of_rows += gridder_data->horizontal_spacing * (gridder_geom_param->row_span - 1);

        // If that size is smaller than the widget's height, increase the min size of each row by a proportional amount
        if (size_of_rows < widget->requested_size.height)
        {
            // Calculate the amount by which to increase the min size of each row
            int missing_size_per_row = (widget->requested_size.height - size_of_rows) / gridder_geom_param->row_span;

            // Increase the min size of each row
            for (int i = gridder_geom_param->row; i < gridder_geom_param->row + gridder_geom_param->row_span; i++)
            {
                ei_int_array_set_at_index(gridder_data->rows_min_size, i, gridder_data->rows_min_size->array[i] + missing_size_per_row);
            }
        }
    }

    // Calculate the total height taken by the rows of the grid
    int total_height = ei_int_array_sum(gridder_data->rows_min_size) + gridder_data->horizontal_spacing * (gridder_data->rows - 1) + gridder_data->margin * 2;

    // If the total height is greater than the parent's height (ie: the grid content can't fit in the parent),
    // expand the parent to fit the content. Can't expand the root widget
    if (total_height > parent->content_rect->size.height && parent != ei_app_root_widget())
    {
        ei_widget_set_requested_size(parent, ei_size(parent->content_rect->size.width, total_height));
    }
    else
    {
        int nb_stretched_rows = ei_bool_array_get_occurences(gridder_data->stretched_rows, true);

        // Calculate the amount of space left to fill the parent based on the min size of all rows
        int height_left = (parent->content_rect->size.height - gridder_data->margin * 2 - total_height);

        for (int i = 0; i < gridder_data->stretched_rows->nb_elements; i++)
        {
            // If the row should stretch, add a proportional portion of the space left to its minimum size
            if (gridder_data->stretched_rows->array[i])
            {
                ei_int_array_set_at_index(gridder_data->rows_current_size, i, gridder_data->rows_min_size->array[i] + height_left / nb_stretched_rows);
            }
            // Otherwise, don't change the size of the row
            else
            {
                ei_int_array_set_at_index(gridder_data->rows_current_size, i, gridder_data->rows_min_size->array[i]);
            }
        }
    }

    // If the column only spans one column, update the min size of the column
    if (gridder_geom_param->column_span == 1)
    {
        // If the widget is wider than any other widget in this column, update the min size of the column
        if (widget->requested_size.width > gridder_data->columns_min_size->array[gridder_geom_param->column])
        {
            ei_int_array_set_at_index(gridder_data->columns_min_size, gridder_geom_param->column, widget->requested_size.width);
        }
    }
    // Otherwise, if it spans multiple columns, update the min size of each column
    else
    {
        int size_of_columns = 0;

        // Calculate the total size of the columns the widget spans for
        for (int i = gridder_geom_param->column; i < gridder_geom_param->column + gridder_geom_param->column_span; i++)
        {
            size_of_columns += gridder_data->columns_min_size->array[i];
        }

        size_of_columns += gridder_data->horizontal_spacing * (gridder_geom_param->column_span - 1);

        // If that size is smaller than the widget's width, increase the min size of each column by a proportional amount
        if (size_of_columns < widget->requested_size.width)
        {
            // Calculate the amount by which to increase the min size of each column
            int missing_size_per_column = (widget->requested_size.width - size_of_columns) / gridder_geom_param->column_span;

            // Increase the min size of each column
            for (int i = gridder_geom_param->column; i < gridder_geom_param->column + gridder_geom_param->column_span; i++)
            {
                ei_int_array_set_at_index(gridder_data->columns_min_size, i, gridder_data->columns_min_size->array[i] + missing_size_per_column);
            }
        }
    }

    // Calculate the total width taken by the columns of the grid
    int total_width = ei_int_array_sum(gridder_data->columns_min_size) + gridder_data->horizontal_spacing * (gridder_data->columns - 1) + gridder_data->margin * 2;

    // If the total width is greater than the parent's width (ie: the grid content can't fit in the parent),
    // expand the parent to fit the content. Can't expand the root widget
    if (total_width > parent->content_rect->size.width && parent != ei_app_root_widget())
    {
        ei_widget_set_requested_size(parent, ei_size(total_width, parent->content_rect->size.height));
    }
    else
    {
        int nb_stretched_columns = ei_bool_array_get_occurences(gridder_data->stretched_columns, true);

        // Calculate the amount of space left to fill the parent based on the min size of all columns
        int width_left = (parent->content_rect->size.width - gridder_data->margin * 2 - total_width);

        for (int i = 0; i < gridder_data->stretched_columns->nb_elements; i++)
        {
            // If the column should stretch, add a portion of the space left to its minimum size
            if (gridder_data->stretched_columns->array[i])
            {
                ei_int_array_set_at_index(gridder_data->columns_current_size, i, gridder_data->columns_min_size->array[i] + width_left / nb_stretched_columns);
            }
            // Otherwise, don't change the size of the column
            else
            {
                ei_int_array_set_at_index(gridder_data->columns_current_size, i, gridder_data->columns_min_size->array[i]);
            }
        }
    }

    // Recompute the geometry of all widgets in the grid
    for (ei_widget_t cell_widget = parent->children_head; cell_widget != NULL; cell_widget = cell_widget->next_sibling)
    {
        if (ei_is_widget_managed_by_gridder(cell_widget))
        {
            ei_size_t grid_size = ei_size(
                ei_int_array_sum(gridder_data->columns_current_size) + gridder_data->horizontal_spacing * (gridder_data->columns - 1),
                ei_int_array_sum(gridder_data->rows_current_size) + gridder_data->vertical_spacing * (gridder_data->rows - 1));

            ei_rect_t parent_content_rect = *parent->content_rect;

            // Remove the margin from the content rect
            parent_content_rect = ei_rect_add(parent_content_rect, gridder_data->margin, gridder_data->margin, -gridder_data->margin * 2, -gridder_data->margin * 2);

            parent_content_rect.top_left = ei_get_position_in_parent_from_anchor(parent_content_rect, grid_size, gridder_data->anchor);

            ei_gridder_t *cell_widget_geom_param = (ei_gridder_t *)ei_widget_get_geom_params(cell_widget);

            int cell_top_left_x = parent_content_rect.top_left.x;
            int cell_top_left_y = parent_content_rect.top_left.y;

            // Calculate the coordinates of the top left corner of the cell based on the row and column
            // the cell is at
            for (int i = 0; i < cell_widget_geom_param->column; i++)
            {
                cell_top_left_x += gridder_data->columns_current_size->array[i];

                if (i < gridder_data->columns)
                {
                    cell_top_left_x += gridder_data->horizontal_spacing;
                }
            }

            for (int i = 0; i < cell_widget_geom_param->row; i++)
            {
                cell_top_left_y += gridder_data->rows_current_size->array[i];

                if (i < gridder_data->rows)
                {
                    cell_top_left_y += gridder_data->vertical_spacing;
                }
            }

            int cell_size_x = 0;
            int cell_size_y = 0;

            // If the cell spans multiple row, add the size of each row + the spacing. If it only spanws one row,
            // simply add the size of the row
            for (int i = cell_widget_geom_param->row; i < cell_widget_geom_param->row + cell_widget_geom_param->row_span; i++)
            {
                cell_size_y += gridder_data->rows_current_size->array[i];

                if (i < gridder_data->rows && i != cell_widget_geom_param->row)
                {
                    cell_size_y += gridder_data->vertical_spacing;
                }
            }

            // If the cell spans multiple columns, add the size of each column + the spacing. If it only spanws one column,
            // simply add the size of the column
            for (int i = cell_widget_geom_param->column; i < cell_widget_geom_param->column + cell_widget_geom_param->column_span; i++)
            {
                cell_size_x += gridder_data->columns_current_size->array[i];

                if (i < gridder_data->columns && i != cell_widget_geom_param->column)
                {
                    cell_size_x += gridder_data->horizontal_spacing;
                }
            }

            // Cell size is the size of the cell while widget size is the size of the widget
            // inside that cell. Widget size equals cell size if the cell if fill is set to true
            ei_size_t cell_size = ei_size(cell_size_x, cell_size_y);

            ei_size_t widget_size = cell_widget_geom_param->fill_cell
                                        ? cell_size
                                        : cell_widget->requested_size;

            // By default, the widget has the same rect as the cell
            ei_rect_t widget_rect_in_cell = ei_rect(
                ei_point(cell_top_left_x, cell_top_left_y),
                cell_size);

            // If fill is set to false, position the widget inside the cell based on the anchor and set its size
            // to the size calculated earlier
            if (!cell_widget_geom_param->fill_cell)
            {
                widget_rect_in_cell = ei_rect(
                    ei_get_position_in_parent_from_anchor(widget_rect_in_cell, widget_size, cell_widget_geom_param->anchor),
                    widget_size);
            }

            ei_geometry_run_finalize(cell_widget, &widget_rect_in_cell);
        }
    }
}

void ei_gridder_releasefunc(ei_widget_t widget)
{
    if (widget->children_head != NULL)
    {
        for (ei_widget_t current_child = widget->children_head; current_child != NULL; current_child = current_child->next_sibling)
        {
            if (ei_widget_is_displayed(current_child))
            {
                current_child->geom_params->manager->runfunc(current_child);
            }
        }
    }

    // If the widget is the last children of its parent managed by the gridder, free the gridder data
    ei_gridder_data_t *gridder_data = ei_get_grid(widget->parent);

    if (gridder_data != NULL)
    {
        // Free all the arrays
        ei_int_array_free(gridder_data->rows_min_size);
        ei_int_array_free(gridder_data->columns_min_size);
        ei_int_array_free(gridder_data->rows_current_size);
        ei_int_array_free(gridder_data->columns_current_size);
        ei_bool_array_free(gridder_data->stretched_rows);
        ei_bool_array_free(gridder_data->stretched_columns);

        // Relink the linked list
        if (ei_first_gridder_data == gridder_data)
        {
            ei_first_gridder_data = gridder_data->next;
        }
        else
        {
            ei_gridder_data_t *current = ei_first_gridder_data;

            while (current->next != gridder_data)
            {
                current = current->next;
            }

            current->next = gridder_data->next;
        }

        free(gridder_data);
    }
}