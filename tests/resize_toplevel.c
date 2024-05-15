#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"

#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_toplevel.h"

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
    ei_app_create((ei_size_t){800, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t toplevel1;
    toplevel1 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(toplevel1, &(ei_size_t){320, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Resize none"}, &(bool){true}, &(ei_axis_set_t){ei_axis_none}, NULL);
    ei_place_xy(toplevel1, 10, 10);

    ei_widget_t toplevel2;
    toplevel2 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(toplevel2, &(ei_size_t){320, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Resize x"}, &(bool){true}, &(ei_axis_set_t){ei_axis_x}, NULL);
    ei_place_xy(toplevel2, 400, 10);

    ei_widget_t toplevel3;
    toplevel3 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(toplevel3, &(ei_size_t){320, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Resize y"}, &(bool){true}, &(ei_axis_set_t){ei_axis_y}, NULL);
    ei_place_xy(toplevel3, 10, 300);

    ei_widget_t toplevel4;
    toplevel4 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(toplevel4, &(ei_size_t){320, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Resize both"}, &(bool){true}, &(ei_axis_set_t){ei_axis_both}, NULL);
    ei_place_xy(toplevel4, 400, 300);

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}
