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

        return false;
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
    ei_app_create((ei_size_t){750, 550}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t frame;
    frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame, &(ei_size_t){300, 200},
                       &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                       &(int){6},
                       &(ei_relief_t){ei_relief_raised}, &(ei_string_t){"Red on hover"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place_xy(frame, 50, 50);

    ei_widget_t frame2;
    frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame2, &(ei_size_t){300, 200},
                       &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                       &(int){6},
                       &(ei_relief_t){ei_relief_raised}, &(ei_string_t){"Nothing"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place_xy(frame2, 400, 50);

    ei_widget_t button;
    button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(button, &(ei_size_t){300, 200},
                        &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                        &(int){6}, &(int){25},
                        &(ei_relief_t){ei_relief_raised}, &(ei_string_t){"Red on hover"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){button_press}, NULL);
    ei_place_xy(button, 50, 300);

    ei_widget_t button2;
    button2 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(button2, &(ei_size_t){300, 200},
                        &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                        &(int){6}, &(int){25},
                        &(ei_relief_t){ei_relief_raised}, &(ei_string_t){"No sunken effect"}, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_callback_t){button_press}, NULL);
    ei_place_xy(button2, 400, 300);

    ei_widget_t frame3;
    frame3 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame3, NULL,
                       &(ei_color_t){0x88, 0x88, 0x88, 0x00},
                       &(int){6},
                       &(ei_relief_t){ei_relief_raised}, &(ei_string_t){"Press X to delete all widgets"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(frame3, &(ei_anchor_t){ei_anc_center}, NULL, &(int){-15}, NULL, NULL, &(float){0.5}, &(float){1}, NULL, NULL);

    ei_remove_tag_from_widget(button2, "button");

    ei_add_tag_to_widget(frame, "red");
    ei_add_tag_to_widget(button, "red");

    ei_add_tag_to_widget(frame, "delete");
    ei_add_tag_to_widget(frame2, "delete");
    ei_add_tag_to_widget(button, "delete");
    ei_add_tag_to_widget(button2, "delete");

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_mouse_move, NULL, "red", red, NULL);
    ei_bind(ei_ev_keydown, NULL, "delete", delete, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
