#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "ei_entry.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"

#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_toplevel.h"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
bool button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    printf("Click !\n");

    ei_widget_t root = ei_app_root_widget();

    ((ei_toplevel_t *)root->children_head)->closable = true;
    // ((ei_toplevel_t *)root->children_head)->title = "Title changed !";

    return true;
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
    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){700, 400}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window, &(ei_size_t){600, 300},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Content rect visualization"}, &(bool){true}, NULL, NULL);
    ei_place_xy(window, 50, 50);

    ei_widget_t window_content_rect = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(window_content_rect, &(ei_size_t){30, 30}, &(ei_color_t){255, 143, 133, 255}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(window_content_rect, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, NULL, NULL, &(float){1}, &(float){1});

    ei_widget_t frame = ei_widget_create("frame", window, NULL, NULL);
    ei_frame_configure(frame, &(ei_size_t){30, 30}, NULL, &(int){5}, &(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(frame, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, &(float){0.1}, &(float){0.1}, &(float){0.3}, &(float){0.5});

    ei_widget_t frame_content_rect = ei_widget_create("frame", frame, NULL, NULL);
    ei_frame_configure(frame_content_rect, &(ei_size_t){30, 30}, &(ei_color_t){158, 187, 255, 255}, NULL, NULL, &(ei_string_t){"This is a frame"}, NULL, &(ei_color_t){17, 76, 214, 255}, NULL, NULL, NULL, NULL);
    ei_place(frame_content_rect, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, NULL, NULL, &(float){1}, &(float){1});

    ei_widget_t button = ei_widget_create("button", window, NULL, NULL);
    ei_button_configure(button, &(ei_size_t){30, 30}, NULL, &(int){6}, &(int){10}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(button, &(ei_anchor_t){ei_anc_northeast}, NULL, NULL, NULL, NULL, &(float){0.9}, &(float){0.1}, &(float){0.3}, &(float){0.5});

    ei_widget_t button_content_rect = ei_widget_create("frame", button, NULL, NULL);
    ei_frame_configure(button_content_rect, &(ei_size_t){30, 30}, &(ei_color_t){153, 255, 125, 255}, NULL, NULL, &(ei_string_t){"This is a button"}, NULL, &(ei_color_t){42, 143, 14, 255}, NULL, NULL, NULL, NULL);
    ei_place(button_content_rect, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, NULL, NULL, &(float){1}, &(float){1});

    ei_widget_t entry = ei_widget_create("entry", window, NULL, NULL);
    ei_entry_configure(entry, NULL, NULL, NULL, NULL, NULL);
    ei_place(entry, &(ei_anchor_t){ei_anc_south}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.9}, &(float){0.8}, &(float){0.2});

    ei_widget_t entry_content_rect = ei_widget_create("frame", entry, NULL, NULL);
    ei_frame_configure(entry_content_rect, &(ei_size_t){30, 30}, &(ei_color_t){255, 253, 110, 255}, NULL, NULL, &(ei_string_t){"This is an entry"}, NULL, &(ei_color_t){166, 127, 0, 255}, NULL, NULL, NULL, NULL);
    ei_place(entry_content_rect, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, NULL, NULL, &(float){1}, &(float){1});

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}
