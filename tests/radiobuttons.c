/**
 * How to use:
 * This file aims at testing the radio button API. There are 2 windows:
 * - The firsrt tests the configuration of radio buttons / goup. This allows us to make sure that 'default values' (the group properties)
 * are applied to the radio buttons if they have not been configured and that is also works depending of the order in which we configured the radio buttons and group
 * - The second one tests all the other functions of the API. When selecting a radio button in the left, it tests getting the selected widget from the group and matches
 * it with the radio buttons on the right (selecting a radio button on the right does not link them to the ones on the left). The "Selected: ..." text tests getting
 * the text of the selected radio button. The "Is first selected ? ..." text tests checking if a given widget is selected, etc...
 */


#include <stdio.h>
#include <stdlib.h>

#include "../api/ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "../implem/headers/api/ei_radio_button.h"
#include "../api/ei_entry.h"

ei_widget_t radio_button_group2;
ei_widget_t radio_button_group3;

ei_widget_t radio_button2_1;
ei_widget_t radio_button2_2;
ei_widget_t radio_button2_3;

ei_widget_t radio_button3_1;
ei_widget_t radio_button3_2;
ei_widget_t radio_button3_3;

ei_widget_t selected_text;
ei_widget_t first_selected;

ei_widget_t add_option_entry;

bool on_add_option_clicked(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_string_t text = (ei_string_t)ei_entry_get_text(add_option_entry);

    ei_widget_t radio_button = ei_widget_create("radio_button", radio_button_group2, NULL, NULL);
    ei_radio_button_set_text(radio_button, &text);

    ei_widget_t radio_button2 = ei_widget_create("radio_button", radio_button_group3, NULL, NULL);
    ei_radio_button_set_text(radio_button2, &text);
}

bool on_clear_selection_clicked(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    clear_selection(radio_button_group2);
    clear_selection(radio_button_group3);
}

bool on_linked_radio_button_selected(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    // This function will not link the radio buttons that have been created using the "Add option" button.
    // This is done on purpose as this file aims at testing all the functions of the API. The linked radio
    // buttons allowed us to test the "ei_radio_button_select" function. The following code compares addresses
    // of the radio buttons from the left with the ones on the right, and thus won't work for any added radio
    // buttons.

    ei_widget_t selected_radio_button = get_selected_radio_button(radio_button_group2);

    if (selected_radio_button == radio_button2_1)
    {
        ei_radio_button_select(radio_button3_1);
    }
    else if (selected_radio_button == radio_button2_2)
    {
        ei_radio_button_select(radio_button3_2);
    }
    else if (selected_radio_button == radio_button2_3)
    {
        ei_radio_button_select(radio_button3_3);
    }

    ei_frame_configure(selected_text, NULL, NULL, NULL, NULL, &(ei_string_t){get_selected_radio_button_text(radio_button_group2)}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_frame_configure(first_selected, NULL, NULL, NULL, NULL, selected_radio_button == radio_button2_1 ? &(ei_string_t){"Is first selected ? Yes"} : &(ei_string_t){"Is first selected ? No"}, NULL, NULL, NULL, NULL, NULL, NULL);
}

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
    ei_widget_t radio_button_group;

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){800, 650}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window, &(ei_size_t){500, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Various configuration tests"}, &(bool){true}, NULL, NULL);

    ei_place(window, &(ei_anchor_t){ei_anc_north}, NULL, &(int){10}, NULL, NULL, &(float){0.5}, NULL, NULL, NULL);

    radio_button_group = ei_widget_create("radio_button_group", window, NULL, NULL);

    ei_widget_t radio_button1 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
    ei_widget_t radio_button2 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
    ei_widget_t radio_button3 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);

    // Configure buton 1 before group
    ei_radio_button_configure(radio_button1, &(ei_string_t){"Configured before group"}, NULL, &(ei_color_t){34, 112, 34, 255}, &(ei_color_t){145, 191, 146, 255}, &(ei_color_t){44, 168, 46, 255}, &(bool){true});

    // No configuration for button 2 apart from text --> group properties will be used
    ei_radio_button_set_text(radio_button2, &(ei_string_t){"No configuration"});

    ei_radio_button_group_configure(radio_button_group, NULL, &(int){2}, NULL, NULL, &(ei_color_t){118, 69, 135, 255}, &(ei_color_t){137, 0, 186, 255}, NULL, NULL);

    // Configure button 3 after group + select it and unselect it right after
    ei_radio_button_select(radio_button3);
    ei_radio_button_configure(radio_button3, &(ei_string_t){"Configured after group"}, NULL, &(ei_color_t){57, 152, 219, 255}, &(ei_color_t){173, 200, 219, 255}, &(ei_color_t){57, 152, 219, 255}, &(bool){false});

    ei_place(radio_button_group, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.5}, NULL, NULL);

    // Create button 4 after everything else is configured and placed + semi-configured
    ei_widget_t radio_button4 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
    ei_radio_button_set_text(radio_button4, &(ei_string_t){"Created after group + semi configuration"});
    ei_radio_button_configure(radio_button4, NULL, NULL, &(ei_color_t){255, 0, 0, 255}, NULL, NULL, NULL);

    // Configure the button 1 once more to select it (shouldn't change the rest of the configuration)
    ei_radio_button_configure(radio_button1, NULL, NULL, NULL, NULL, NULL, &(bool){true});

    ei_widget_t window2 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window2, &(ei_size_t){700, 250},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Linked radio buttons + callback"}, &(bool){true}, NULL, NULL);

    ei_place(window2, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.5}, NULL, NULL);

    radio_button_group2 = ei_widget_create("radio_button_group", window2, NULL, NULL);
    ei_radio_button_group_configure(radio_button_group2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){on_linked_radio_button_selected});

    radio_button2_1 = ei_widget_create("radio_button", radio_button_group2, NULL, NULL);
    radio_button2_2 = ei_widget_create("radio_button", radio_button_group2, NULL, NULL);
    radio_button2_3 = ei_widget_create("radio_button", radio_button_group2, NULL, NULL);

    ei_radio_button_set_text(radio_button2_1, &(ei_string_t){"Option 1"});
    ei_radio_button_set_text(radio_button2_2, &(ei_string_t){"Option 2"});
    ei_radio_button_set_text(radio_button2_3, &(ei_string_t){"Option 3"});

    ei_place(radio_button_group2, NULL, &(int){20}, &(int){20}, NULL, NULL, NULL, NULL, NULL, NULL);

    ei_place(window2, &(ei_anchor_t){ei_anc_south}, NULL, &(int){-50}, NULL, NULL, &(float){0.5}, &(float){1}, NULL, NULL);

    radio_button_group3 = ei_widget_create("radio_button_group", window2, NULL, NULL);

    radio_button3_1 = ei_widget_create("radio_button", radio_button_group3, NULL, NULL);
    radio_button3_2 = ei_widget_create("radio_button", radio_button_group3, NULL, NULL);
    radio_button3_3 = ei_widget_create("radio_button", radio_button_group3, NULL, NULL);

    ei_radio_button_set_text(radio_button3_1, &(ei_string_t){"Option 1 bis"});
    ei_radio_button_set_text(radio_button3_2, &(ei_string_t){"Option 2 bis"});
    ei_radio_button_set_text(radio_button3_3, &(ei_string_t){"Option 3 bis"});

    ei_place(radio_button_group3, &(ei_anchor_t){ei_anc_northeast}, &(int){-20}, &(int){20}, NULL, NULL, &(float){1}, NULL, NULL, NULL);

    selected_text = ei_widget_create("frame", window2, NULL, NULL);
    ei_frame_configure(selected_text, &(ei_size_t){200, 0}, &(ei_color_t){0, 0, 0, 0}, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL);
    ei_place(selected_text, &(ei_anchor_t){ei_anc_north}, NULL, &(int){20}, NULL, NULL, &(float){0.5}, NULL, NULL, NULL);

    first_selected = ei_widget_create("frame", window2, NULL, NULL);
    ei_frame_configure(first_selected, &(ei_size_t){200, 0}, &(ei_color_t){0, 0, 0, 0}, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL);
    ei_place(first_selected, &(ei_anchor_t){ei_anc_north}, NULL, &(int){55}, NULL, NULL, &(float){0.5}, NULL, NULL, NULL);

    add_option_entry = ei_widget_create("entry", window2, NULL, NULL);
    ei_entry_configure(add_option_entry, NULL, NULL, NULL, NULL, NULL);
    ei_place(add_option_entry, &(ei_anchor_t){ei_anc_southwest}, &(int){20}, &(int){-20}, NULL, NULL, NULL, &(float){1}, NULL, NULL);

    ei_widget_t add_option_button = ei_widget_create("button", window2, NULL, NULL);
    ei_button_configure(add_option_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Add option"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){on_add_option_clicked}, NULL);
    ei_place(add_option_button, &(ei_anchor_t){ei_anc_southwest}, &(int){235}, &(int){-20}, NULL, NULL, NULL, &(float){1}, NULL, NULL);

    ei_widget_t clear_button = ei_widget_create("button", window2, NULL, NULL);
    ei_button_configure(clear_button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Clear selection"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){on_clear_selection_clicked}, NULL);
    ei_place(clear_button, &(ei_anchor_t){ei_anc_southeast}, &(int){-20}, &(int){-20}, NULL, NULL, &(float){1}, &(float){1}, NULL, NULL);

    // Error: can't parent a radio button to something other than a radio button group
    printf("Should have an error message because we are trying to create a radio button not a child of radio button group:\n");
    ei_widget_create("radio_button", window2, NULL, NULL);

    printf("Should have an error message because we are trying to create a widget, not a radio button, as a child of a radio button group:\n");
    // Error: all children of a radio button group must be radio buttons
    ei_widget_create("frame", radio_button_group3, NULL, NULL);

    // ei_widget_t window3 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    // ei_toplevel_configure(window3, &(ei_size_t){700, 250},
    //                       &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
    //                       &(int){2},
    //                       &(ei_string_t){"Non grouped radio buttons"}, &(bool){true}, NULL, NULL);

    // ei_place(window3, &(ei_anchor_t){ei_anc_south}, NULL, &(int){-50}, NULL, NULL, &(float){0.5}, &(float){1}, NULL, NULL);

    // ei_widget_t radio_button4_1 = ei_widget_create("radio_button", window3, NULL, NULL);

    // ei_widget_t radio_button_parent = ei_widget_create("frame", window3, NULL, NULL);
    // ei_frame_configure(radio_button_parent, &(ei_size_t){200, 100}, &(ei_color_t){34, 112, 34, 255}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // ei_place(radio_button_parent, &(ei_anchor_t){ei_anc_east}, &(int){-20}, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    // ei_widget_t radio_button4_2 = ei_widget_create("radio_button", radio_button_parent, NULL, NULL);
    // ei_widget_t radio_button4_3 = ei_widget_create("radio_button", radio_button_parent, NULL, NULL);

    // ei_radio_button_set_text(radio_button4_1, &(ei_string_t){"Option 1"});
    // ei_radio_button_set_text(radio_button4_2, &(ei_string_t){"Option 2"});
    // ei_radio_button_set_text(radio_button4_3, &(ei_string_t){"Option 3"});

    /* Register the default callback to events of interest. */
    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
