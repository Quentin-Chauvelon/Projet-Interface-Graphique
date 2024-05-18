#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "ei_entry.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "../implem/headers/api/ei_gridder.h"
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

int main(int argc, char **argv)
{

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){900, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t window;
    window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window, &(ei_size_t){850, 520},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Hello World"}, &(bool){true}, NULL, NULL);
    ei_place_xy(window, 25, 25);

    ei_widget_t last_name_text_label;
    last_name_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(last_name_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Nom"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(last_name_text_label, 0, 0);
    ei_set_cell_anchor(last_name_text_label, ei_anc_west);

    ei_widget_t last_name_entry;
    last_name_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(last_name_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(last_name_entry, 0, 1);
    ei_cell_fill_column(last_name_entry);
    ei_cell_fill(last_name_entry);

    ei_widget_t first_name_text_label;
    first_name_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(first_name_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Prénom"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(first_name_text_label, 0, 2);
    ei_set_cell_anchor(first_name_text_label, ei_anc_west);

    ei_widget_t first_name_entry;
    first_name_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(first_name_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(first_name_entry, 0, 3);
    ei_cell_fill(first_name_entry);

    ei_widget_t birth_date_text_label;
    birth_date_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(birth_date_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Date de naissance"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(birth_date_text_label, 1, 0);
    ei_set_cell_anchor(birth_date_text_label, ei_anc_west);

    ei_widget_t birth_date_entry;
    birth_date_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(birth_date_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(birth_date_entry, 1, 1);
    ei_cell_fill(birth_date_entry);

    ei_widget_t address_text_label;
    address_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(address_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Adresse"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(address_text_label, 2, 0);
    ei_set_cell_anchor(address_text_label, ei_anc_west);

    ei_widget_t address_entry;
    address_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(address_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(address_entry, 2, 1);
    ei_set_cell_column_span(address_entry, 3);
    ei_cell_fill(address_entry);

    ei_widget_t zip_code_text_label;
    zip_code_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(zip_code_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Code postal"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(zip_code_text_label, 3, 0);
    ei_set_cell_anchor(zip_code_text_label, ei_anc_west);

    ei_widget_t zip_code_entry;
    zip_code_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(zip_code_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(zip_code_entry, 3, 1);
    ei_cell_fill(zip_code_entry);

    ei_widget_t city_text_label;
    city_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(city_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Ville"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(city_text_label, 3, 2);
    ei_set_cell_anchor(city_text_label, ei_anc_west);

    ei_widget_t city_entry;
    city_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(city_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(city_entry, 3, 3);
    ei_cell_fill(city_entry);

    ei_widget_t tell_us_more_text_label;
    tell_us_more_text_label = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(tell_us_more_text_label, NULL, &(ei_color_t){0x00, 0x00, 0x00, 0x00}, NULL, NULL, &(ei_string_t){"Tell us more"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(tell_us_more_text_label, 4, 0);
    ei_set_cell_anchor(tell_us_more_text_label, ei_anc_east);

    ei_widget_t tell_us_more_entry;
    tell_us_more_entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(tell_us_more_entry, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(tell_us_more_entry, 4, 1);
    ei_set_cell_row_span(tell_us_more_entry, 2);
    ei_set_cell_column_span(tell_us_more_entry, 3);
    ei_cell_fill_row(tell_us_more_entry);
    ei_cell_fill(tell_us_more_entry);

    ei_widget_t ok_button;
    ok_button = ei_widget_create("button", window, NULL, NULL);
    ei_button_configure(ok_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Ok"}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_set_cell_position(ok_button, 5, 0);
    ei_cell_fill(ok_button);

    // ei_set_grid_spacing(window, &(int){20}, &(int){20});
    ei_set_grid_fill_column_at_index(window, 3, true);

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}
