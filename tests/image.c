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
    ei_widget_t button;
    ei_widget_t button2;
    ei_widget_t window;

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){860, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    /* Create, configure and place a toplevel window on screen. */
    window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window, &(ei_size_t){350, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Move/resize to test image redraw"}, &(bool){true}, NULL, NULL);
    ei_place_xy(window, 500, 500);

    ei_surface_t image;
    if ((image = hw_image_load("misc/klimt.jpg", ei_app_root_surface())) == NULL)
    {
        printf("ERROR: could not load image \"misc/klimt.jpg\"");
        return 1;
    }

    ei_surface_t image2;
    if ((image2 = hw_image_load("misc/flag.png", ei_app_root_surface())) == NULL)
    {
        printf("ERROR: could not load image \"misc/flag.png\"");
        return 1;
    }

    ei_rect_t *test_rect = &(ei_rect_t){(ei_point_t){256, 256}, (ei_size_t){128, 128}};
    ei_rect_t *test_rect2 = &(ei_rect_t){(ei_point_t){448, 448}, (ei_size_t){128, 128}};

    // Image, no rect, bigger than widget
    ei_widget_t frame1;
    frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame1, &(ei_size_t){150, 150}, &(ei_color_t){0x00, 0x00, 0x00, 0xff},
                       NULL, NULL, NULL,
                       NULL, NULL, NULL, &image, NULL, &(ei_anchor_t){ei_anc_northeast});
    ei_place_xy(frame1, 10, 10);

    // Image, rect, NE anchor
    ei_widget_t frame2;
    frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame2, &(ei_size_t){150, 150}, &(ei_color_t){0x00, 0x00, 0x00, 0xff},
                       NULL, NULL, NULL,
                       NULL, NULL, NULL, &image, &test_rect, &(ei_anchor_t){ei_anc_northeast});
    ei_place_xy(frame2, 220, 10);

    // Image, rect partly out of image, NE anchor
    ei_widget_t frame3;
    frame3 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame3, &(ei_size_t){150, 150}, &(ei_color_t){0x00, 0x00, 0x00, 0xff},
                       NULL, NULL, NULL,
                       NULL, NULL, NULL, &image, &test_rect2, &(ei_anchor_t){ei_anc_northeast});
    ei_place_xy(frame3, 430, 10);

    // Image, rect fully out of image, NE anchor
    ei_widget_t frame4;
    frame4 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame4, &(ei_size_t){150, 150}, &(ei_color_t){0x00, 0x00, 0x00, 0xff},
                       NULL, NULL, NULL,
                       NULL, NULL, NULL, &image2, &test_rect2, &(ei_anchor_t){ei_anc_northeast});
    ei_place_xy(frame4, 640, 10);

    // Images anchored at all points
    ei_widget_t NW_frame;
    NW_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(NW_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_northwest});
    ei_place_xy(NW_frame, 20, 300);

    ei_widget_t N_frame;
    N_frame = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(N_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_north}, NULL, NULL);
    ei_place_xy(N_frame, 88, 300);

    ei_widget_t NE_frame;
    NE_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(NE_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_northeast});
    ei_place_xy(NE_frame, 156, 300);

    ei_widget_t W_frame;
    W_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(W_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_west});
    ei_place_xy(W_frame, 20, 368);

    ei_widget_t center_frame;
    center_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(center_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_center});
    ei_place_xy(center_frame, 88, 368);

    ei_widget_t E_frame;
    E_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(E_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_east});
    ei_place_xy(E_frame, 156, 368);

    ei_widget_t SW_frame;
    SW_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(SW_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_southwest});
    ei_place_xy(SW_frame, 20, 436);

    ei_widget_t S_frame;
    S_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(S_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_south});
    ei_place_xy(S_frame, 88, 436);

    ei_widget_t SE_frame;
    SE_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(SE_frame, &(ei_size_t){48, 48}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image2, NULL, &(ei_anchor_t){ei_anc_southeast});
    ei_place_xy(SE_frame, 156, 436);

    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
    ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

    ei_app_free();
    hw_surface_free(image);
    hw_surface_free(image2);

    return (EXIT_SUCCESS);
}
