#include <stdio.h>
#include <stdlib.h>

#include "../api/ei_application.h"
#include "../api/ei_event.h"
#include "../api/hw_interface.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_placer.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_implementation.h"
#include "ei_parser.h"

// process_key --

bool button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    printf("Click !\n");
    return true;
}

bool delete(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (event->param.key_code == SDLK_x)
    {
        ei_widget_destroy(widget);

        return true;
    }

    return false;
}

bool reset_color(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (widget != ei_get_picking_widget())
    {
        ei_frame_set_bg_color(widget, (ei_color_t){0x88, 0x88, 0x88, 0xff});
        ei_unbind(ei_ev_mouse_move, widget, NULL, reset_color, NULL);
        return true;
    }
    else
    {
        return false;
    }
}

bool red(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (widget != ei_get_picking_widget())
    {
        return false;
    }

    if (strcmp(widget->wclass->name, "frame") == 0)
    {
        ei_frame_set_bg_color(widget, (ei_color_t){0xff, 0x00, 0x00, 0xff});
        ei_bind(ei_ev_mouse_move, widget, NULL, reset_color, NULL);
    }
    else if (strcmp(widget->wclass->name, "button") == 0)
    {
        ei_button_set_bg_color(widget, (ei_color_t){0xff, 0x00, 0x00, 0xff});
        ei_bind(ei_ev_mouse_move, widget, NULL, reset_color, NULL);
    }
    else if (strcmp(widget->wclass->name, "toplevel") == 0)
    {
        ei_toplevel_configure(widget, NULL, &(ei_color_t){0xff, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL, NULL);
        ei_bind(ei_ev_mouse_move, widget, NULL, reset_color, NULL);
    }
    else
    {
        return false;
    }

    return true;
}

bool process_key(ei_widget_t widget, ei_event_t *event, void *user_param)
{
    if (event->param.key_code == SDLK_ESCAPE)
    {
        ei_app_quit_request();
        return true;
    }

    return false;
}

/*
 * main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv)
{
    ei_size_t screen_size = {750, 550};
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, false);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (ei_parse_file("tests/tags_parsing.txt") == 0)
        printf("Error while attempting to parse \"parsing.txt\", aborting.\n");
    else
    {

        ei_remove_tag_from_widget(ei_parse_widget_from_name("button2"), "button");
        ei_add_tag_to_widget(ei_parse_widget_from_name("frame"), "red");
        ei_add_tag_to_widget(ei_parse_widget_from_name("frame"), "delete");
        ei_add_tag_to_widget(ei_parse_widget_from_name("button"), "red");

        ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "red", red, NULL);
        ei_bind(ei_ev_keydown, NULL, "delete", delete, NULL);

        /* Run the application's main loop. */
        ei_app_run();
    }

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
