#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "ei_entry.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "../implem/headers/ei_gridder.h"

typedef struct
{
    bool stretched;
    int index;
} stretched_t;

int rows = 0;
int columns = 0;

static ei_widget_t grid;
ei_widget_t selected_button;
bool filled = false;

/*
 * default_handler --
 *
 *	Callback called to handle keypress and window close events.
 */
bool default_handler(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if ((event->type == ei_ev_close) ||
        ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE)))
    {
        ei_app_quit_request();
        return true;
    }
    else
        return false;
}

bool toggle_cell_fill(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (selected_button == NULL)
    {
        return false;
    }

    filled = !filled;

    // ei_cell_fill(selected_button);
    ei_set_cell(selected_button, NULL, NULL, NULL, NULL, NULL, NULL, &(bool){filled}, NULL);

    return true;
}

bool select_cell(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (selected_button != NULL)
    {
        ei_button_configure(selected_button, NULL, &ei_default_background_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }

    ei_button_configure(widget, NULL, &(ei_color_t){255, 0, 0, 255}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    selected_button = widget;

    return true;
}

bool stretch_row(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    stretched_t *stretched = (stretched_t *)user_param;
    stretched->stretched = !stretched->stretched;
    ei_set_grid_fill_row_at_index(grid, stretched->index, stretched->stretched);

    return true;
}

bool stretch_column(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    stretched_t *stretched = (stretched_t *)user_param;
    stretched->stretched = !stretched->stretched;
    ei_set_grid_fill_column_at_index(grid, stretched->index, stretched->stretched);

    return true;
}

bool set_cell_anchor(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (selected_button == NULL)
    {
        return false;
    }

    ei_set_cell_anchor(selected_button, (ei_anchor_t)user_param);

    return true;
}

bool set_grid_anchor(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_set_grid_anchor(grid, (ei_anchor_t)user_param);

    return true;
}

bool set_grid_margin(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_set_grid_margin(grid, (int)strtol(ei_entry_get_text((ei_widget_t)user_param), NULL, 10));

    return true;
}

bool set_grid_horizontal_spacing(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_set_grid_spacing(grid, &(int){strtol(ei_entry_get_text((ei_widget_t)user_param), NULL, 10)}, NULL);

    return true;
}

bool set_grid_vertical_spacing(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_set_grid_spacing(grid, NULL, &(int){strtol(ei_entry_get_text((ei_widget_t)user_param), NULL, 10)});

    return true;
}

void instantiate_cell_anchor_button(ei_widget_t parent, int row, int column)
{
    // No good formula to calculate the anchor from the row and column
    ei_anchor_t anchor = ei_anc_south;
    int row_anchor = row - 1;
    int column_anchor = column - 4;

    if (column_anchor == 0)
    {
        anchor = 7 + (2 - row_anchor);
    }
    else if (column_anchor == 1 && row_anchor != 2)
    {
        anchor = 2 - row_anchor;
    }
    else if (column_anchor == 2)
    {
        anchor = 3 + row_anchor;
    }

    ei_widget_t grid_anchor_button;
    grid_anchor_button = ei_widget_create("button", parent, NULL, NULL);
    ei_button_configure(grid_anchor_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){set_cell_anchor}, (ei_user_param_t) & (ei_anchor_t){anchor});
    ei_set_cell_position(grid_anchor_button, row, column);
    ei_cell_fill(grid_anchor_button);

    // Make rows 30 pixels heigh
    ei_set_row_min_size(parent, row, 30);
}

void instantiate_grid_anchor_button(ei_widget_t parent, int row, int column)
{
    // No good formula to calculate the anchor from the row and column
    ei_anchor_t anchor = ei_anc_south;
    int row_anchor = row - 1;

    if (column == 0)
    {
        anchor = 7 + (2 - row_anchor);
    }
    else if (column == 1 && row_anchor != 2)
    {
        anchor = 2 - row_anchor;
    }
    else if (column == 2)
    {
        anchor = 3 + row_anchor;
    }

    ei_widget_t grid_anchor_button;
    grid_anchor_button = ei_widget_create("button", parent, NULL, NULL);
    ei_button_configure(grid_anchor_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){set_grid_anchor}, (ei_user_param_t) & (ei_anchor_t){anchor});
    ei_set_cell_position(grid_anchor_button, row, column);
    ei_cell_fill(grid_anchor_button);

    // Make rows 30 pixels heigh
    ei_set_row_min_size(parent, row, 30);
}

bool add_row(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    rows++;

    stretched_t *stretched = malloc(sizeof(stretched_t));
    stretched->stretched = false;
    stretched->index = rows;

    ei_widget_t column_button;
    column_button = ei_widget_create("button", grid, NULL, NULL);
    ei_button_configure(column_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Row"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){stretch_row}, (ei_user_param_t)&stretched);
    ei_set_cell_anchor(column_button, ei_anc_center);
    ei_set_cell_position(column_button, rows, 0);
    ei_cell_fill(column_button);

    for (size_t i = 1; i < columns + 1; i++)
    {
        ei_widget_t button;
        button = ei_widget_create("button", grid, NULL, NULL);
        ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Select"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){select_cell}, NULL);
        ei_set_cell_position(button, rows, i);
    }

    return true;
}

bool add_column(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    columns++;

    stretched_t *stretched = malloc(sizeof(stretched_t));
    stretched->stretched = false;
    stretched->index = columns;

    ei_widget_t row_button;
    row_button = ei_widget_create("button", grid, NULL, NULL);
    ei_button_configure(row_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Column"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){stretch_column}, (ei_user_param_t)&stretched);
    ei_set_cell_anchor(row_button, ei_anc_center);
    ei_set_cell_position(row_button, 0, columns);
    ei_cell_fill(row_button);

    for (size_t i = 1; i < rows + 1; i++)
    {
        ei_widget_t button;
        button = ei_widget_create("button", grid, NULL, NULL);
        ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Select"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){select_cell}, NULL);
        ei_set_cell_position(button, i, columns);
    }

    return true;
}

int main(int argc, char **argv)
{

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){1200, 800}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    grid = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(grid, NULL,
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Grid playground"}, &(bool){true}, &(ei_axis_set_t){ei_axis_none}, NULL);
    ei_set_cell(grid, &(int){0}, &(int){0}, NULL, NULL, &(bool){true}, &(bool){true}, NULL, NULL);
    ei_cell_fill(grid);

    // Grid control grid background (on the right)
    ei_widget_t grid_control;
    grid_control = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(grid_control, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(grid_control, 0, 1);
    ei_cell_fill(grid_control);

    // Cell control grid background (at the bottom)
    ei_widget_t cell_control;
    cell_control = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(cell_control, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(cell_control, 1, 0);
    ei_cell_fill(cell_control);

    ei_widget_t grid_anchor_label;
    grid_anchor_label = ei_widget_create("frame", grid_control, NULL, NULL);
    ei_frame_configure(grid_anchor_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Grid anchor"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(grid_anchor_label, 3);
    ei_set_cell_anchor(grid_anchor_label, ei_anc_center);
    ei_set_cell_position(grid_anchor_label, 0, 0);

    // Instantiate all buttons for the grid anchor (3x3 pattern)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            instantiate_grid_anchor_button(grid_control, i + 1, j);
        }
    }

    // MARGIN CONTROL

    // Leave a space between the grid anchor and the margin control
    ei_set_row_min_size(grid_control, 4, 20);

    ei_widget_t margin_label;
    margin_label = ei_widget_create("frame", grid_control, NULL, NULL);
    ei_frame_configure(margin_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Grid margin"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(margin_label, 3);
    ei_set_cell_anchor(margin_label, ei_anc_center);
    ei_set_cell_position(margin_label, 5, 0);

    ei_widget_t margin_entry;
    margin_entry = ei_widget_create("entry", grid_control, NULL, NULL);
    ei_entry_configure(margin_entry, &(int){3}, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(margin_entry, 2);
    ei_cell_fill(margin_entry);
    ei_set_cell_position(margin_entry, 6, 0);

    ei_widget_t margin_button;
    margin_button = ei_widget_create("button", grid_control, NULL, NULL);
    ei_button_configure(margin_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){set_grid_margin}, (ei_user_param_t)&margin_entry);
    ei_set_cell_position(margin_button, 6, 2);
    ei_cell_fill(margin_button);

    // HORIZONTAL SPACING CONTROL

    ei_set_row_min_size(grid_control, 7, 20);

    ei_widget_t horizontal_spacing_label;
    horizontal_spacing_label = ei_widget_create("frame", grid_control, NULL, NULL);
    ei_frame_configure(horizontal_spacing_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Horizontal spacing"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(horizontal_spacing_label, 3);
    ei_set_cell_position(horizontal_spacing_label, 8, 0);

    ei_widget_t horizontal_spacing_entry;
    horizontal_spacing_entry = ei_widget_create("entry", grid_control, NULL, NULL);
    ei_entry_configure(horizontal_spacing_entry, &(int){3}, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(horizontal_spacing_entry, 2);
    ei_cell_fill(horizontal_spacing_entry);
    ei_set_cell_position(horizontal_spacing_entry, 9, 0);

    ei_widget_t horizontal_spacing_button;
    horizontal_spacing_button = ei_widget_create("button", grid_control, NULL, NULL);
    ei_button_configure(horizontal_spacing_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){set_grid_horizontal_spacing}, (ei_user_param_t)&horizontal_spacing_entry);
    ei_set_cell_position(horizontal_spacing_button, 9, 2);
    ei_cell_fill(horizontal_spacing_button);

    // VERTICAL SPACING CONTROL

    ei_set_row_min_size(grid_control, 10, 20);

    ei_widget_t vertical_spacing_label;
    vertical_spacing_label = ei_widget_create("frame", grid_control, NULL, NULL);
    ei_frame_configure(vertical_spacing_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Vertical spacing"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(vertical_spacing_label, 3);
    ei_set_cell_position(vertical_spacing_label, 11, 0);

    ei_widget_t vertical_spacing_entry;
    vertical_spacing_entry = ei_widget_create("entry", grid_control, NULL, NULL);
    ei_entry_configure(vertical_spacing_entry, &(int){3}, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(vertical_spacing_entry, 2);
    ei_cell_fill(vertical_spacing_entry);
    ei_set_cell_position(vertical_spacing_entry, 12, 0);

    ei_widget_t vertical_spacing_button;
    vertical_spacing_button = ei_widget_create("button", grid_control, NULL, NULL);
    ei_button_configure(vertical_spacing_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){set_grid_vertical_spacing}, (ei_user_param_t)&vertical_spacing_entry);
    ei_set_cell_position(vertical_spacing_button, 12, 2);
    ei_cell_fill(vertical_spacing_button);

    ei_set_grid_margin(grid_control, 20);

    ei_widget_t directions_label;
    directions_label = ei_widget_create("frame", cell_control, NULL, NULL);
    ei_frame_configure(directions_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Controls for selected cell:"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_row_span(directions_label, 4);
    ei_set_cell_anchor(directions_label, ei_anc_west);
    ei_set_cell_position(directions_label, 0, 0);

    ei_set_column_min_size(cell_control, 1, 20);

    ei_widget_t toggle_fill_button;
    toggle_fill_button = ei_widget_create("button", cell_control, NULL, NULL);
    ei_button_configure(toggle_fill_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Toggle fill"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){toggle_cell_fill}, NULL);
    ei_set_cell_row_span(toggle_fill_button, 4);
    ei_set_cell_anchor(toggle_fill_button, ei_anc_center);
    ei_set_cell_position(toggle_fill_button, 0, 2);

    // CELL ANCHOR CONTROL

    ei_set_column_min_size(cell_control, 3, 20);

    ei_widget_t cell_anchor_label;
    cell_anchor_label = ei_widget_create("frame", cell_control, NULL, NULL);
    ei_frame_configure(cell_anchor_label, NULL, NULL, NULL, NULL, &(ei_string_t){"Cell anchor"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_column_span(cell_anchor_label, 3);
    ei_set_cell_anchor(cell_anchor_label, ei_anc_center);
    ei_set_cell_position(cell_anchor_label, 0, 4);

    // Instantiate all buttons for the grid anchor (3x3 pattern)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            instantiate_cell_anchor_button(cell_control, i + 1, j + 4);
        }
    }

    ei_set_grid_margin(cell_control, 20);

    ei_set_grid_spacing(ei_app_root_widget(), &(int){15}, &(int){15});
    ei_set_grid_margin(ei_app_root_widget(), 10);

    add_row(grid, NULL, NULL);
    add_row(grid, NULL, NULL);
    add_column(grid, NULL, NULL);
    add_column(grid, NULL, NULL);

    ei_widget_t add_row_button;
    add_row_button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(add_row_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Add row"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){add_row}, NULL);
    ei_place(add_row_button, &(ei_anchor_t){ei_anc_southeast}, &(int){-40}, &(int){-87}, NULL, NULL, &(float){1.0f}, &(float){1.0f}, NULL, NULL);

    ei_widget_t add_column_button;
    add_column_button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(add_column_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Add column"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){add_column}, NULL);
    ei_place(add_column_button, &(ei_anchor_t){ei_anc_southeast}, &(int){-40}, &(int){-40}, NULL, NULL, &(float){1.0f}, &(float){1.0f}, NULL, NULL);

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}
