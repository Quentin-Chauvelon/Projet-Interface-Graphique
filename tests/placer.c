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
    ei_app_create((ei_size_t){860, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t relative_positioning_toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(relative_positioning_toplevel, &(ei_size_t){250, 250},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Relative positionning"}, &(bool){true}, NULL, NULL);
    ei_place_xy(relative_positioning_toplevel, 50, 30);

    ei_widget_t NW_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(NW_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(NW_frame, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL, NULL, &(float){0}, &(float){0}, NULL, NULL);

    ei_widget_t N_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(N_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(N_frame, &(ei_anchor_t){ei_anc_north}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0}, NULL, NULL);

    ei_widget_t NE_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(NE_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(NE_frame, &(ei_anchor_t){ei_anc_northeast}, NULL, NULL, NULL, NULL, &(float){1}, &(float){0}, NULL, NULL);

    ei_widget_t W_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(W_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(W_frame, &(ei_anchor_t){ei_anc_west}, NULL, NULL, NULL, NULL, &(float){0}, &(float){0.5}, NULL, NULL);

    ei_widget_t center_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(center_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(center_frame, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.5}, NULL, NULL);

    ei_widget_t E_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(E_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(E_frame, &(ei_anchor_t){ei_anc_east}, NULL, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    ei_widget_t SW_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(SW_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(SW_frame, &(ei_anchor_t){ei_anc_southwest}, NULL, NULL, NULL, NULL, &(float){0}, &(float){1}, NULL, NULL);

    ei_widget_t S_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(S_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(S_frame, &(ei_anchor_t){ei_anc_south}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){1}, NULL, NULL);

    ei_widget_t SE_frame = ei_widget_create("frame", relative_positioning_toplevel, NULL, NULL);
    ei_frame_configure(SE_frame, &(ei_size_t){30, 30}, &(ei_color_t){0xb5, 0x1b, 0x1b, 0xff}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(SE_frame, &(ei_anchor_t){ei_anc_southeast}, NULL, NULL, NULL, NULL, &(float){1}, &(float){1}, NULL, NULL);

    ei_widget_t anchor_toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(anchor_toplevel, &(ei_size_t){300, 200},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Anchor"}, &(bool){true}, NULL, NULL);
    ei_place_xy(anchor_toplevel, 50, 350);

    ei_widget_t NW_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(NW_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){255, 0, 0, 255}, NULL, NULL, &(ei_string_t){"NW"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(NW_frame_2, &(ei_anchor_t){ei_anc_northwest}, &(int){70}, NULL, NULL, NULL, NULL, &(float){0.5}, NULL, NULL);

    ei_widget_t NE_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(NE_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){0, 255, 0, 255}, NULL, NULL, &(ei_string_t){"NE"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(NE_frame_2, &(ei_anchor_t){ei_anc_northeast}, &(int){70}, NULL, NULL, NULL, NULL, &(float){0.5}, NULL, NULL);

    ei_widget_t SW_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(SW_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){0, 0, 255, 255}, NULL, NULL, &(ei_string_t){"SW"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(SW_frame_2, &(ei_anchor_t){ei_anc_southwest}, &(int){70}, NULL, NULL, NULL, NULL, &(float){0.5}, NULL, NULL);

    ei_widget_t SE_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(SE_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){255, 255, 0, 255}, NULL, NULL, &(ei_string_t){"SE"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(SE_frame_2, &(ei_anchor_t){ei_anc_southeast}, &(int){70}, NULL, NULL, NULL, NULL, &(float){0.5}, NULL, NULL);

    ei_widget_t N_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(N_frame_2, &(ei_size_t){30, 50}, &(ei_color_t){255, 0, 0, 255}, NULL, NULL, &(ei_string_t){"N"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(N_frame_2, &(ei_anchor_t){ei_anc_north}, &(int){-70}, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    ei_widget_t E_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(E_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){0, 255, 0, 255}, NULL, NULL, &(ei_string_t){"E"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(E_frame_2, &(ei_anchor_t){ei_anc_east}, &(int){-70}, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    ei_widget_t S_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(S_frame_2, &(ei_size_t){30, 50}, &(ei_color_t){0, 0, 255, 255}, NULL, NULL, &(ei_string_t){"S"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(S_frame_2, &(ei_anchor_t){ei_anc_south}, &(int){-70}, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    ei_widget_t W_frame_2 = ei_widget_create("frame", anchor_toplevel, NULL, NULL);
    ei_frame_configure(W_frame_2, &(ei_size_t){50, 30}, &(ei_color_t){255, 255, 0, 255}, NULL, NULL, &(ei_string_t){"W"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(W_frame_2, &(ei_anchor_t){ei_anc_west}, &(int){-70}, NULL, NULL, NULL, &(float){1}, &(float){0.5}, NULL, NULL);

    ei_widget_t relative_sizing_toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(relative_sizing_toplevel, &(ei_size_t){300, 200},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Relative sizing"}, &(bool){true}, NULL, NULL);
    ei_place_xy(relative_sizing_toplevel, 450, 200);

    ei_widget_t height_frame = ei_widget_create("frame", relative_sizing_toplevel, NULL, NULL);
    ei_frame_configure(height_frame, &(ei_size_t){70, 30}, &(ei_color_t){0, 255, 0, 255}, NULL, NULL, &(ei_string_t){"0.8 H"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(height_frame, NULL, &(int){30}, NULL, NULL, NULL, NULL, &(float){0.1}, NULL, &(float){0.8});

    ei_widget_t width_frame = ei_widget_create("frame", relative_sizing_toplevel, NULL, NULL);
    ei_frame_configure(width_frame, &(ei_size_t){50, 30}, &(ei_color_t){0, 255, 0, 255}, NULL, NULL, &(ei_string_t){"0.5 Width"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(width_frame, &(ei_anchor_t){ei_anc_southeast}, &(int){-30}, &(int){-30}, NULL, NULL, &(float){1}, &(float){1}, &(float){0.5}, NULL);

    ei_widget_t width_height_frame = ei_widget_create("frame", relative_sizing_toplevel, NULL, NULL);
    ei_frame_configure(width_height_frame, &(ei_size_t){50, 30}, &(ei_color_t){0, 255, 0, 255}, NULL, NULL, &(ei_string_t){"0.3x0.3"}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(width_height_frame, &(ei_anchor_t){ei_anc_northeast}, &(int){-30}, &(int){30}, NULL, NULL, &(float){1}, NULL, &(float){0.3}, &(float){0.3});

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}
