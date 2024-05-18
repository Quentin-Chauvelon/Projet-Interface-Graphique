#include "../api/ei_application.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../api/ei_placer.h"

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
    ei_app_create((ei_size_t){1040, 440}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t NW_frame;
    NW_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(NW_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"NW white text"}, NULL, &(ei_color_t){255, 255, 255, 255}, &(ei_anchor_t){ei_anc_northwest}, NULL, NULL, NULL);
    ei_place_xy(NW_frame, 20, 20);

    ei_widget_t N_frame;
    N_frame = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(N_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, NULL, &(ei_string_t){"N black text"}, NULL, &(ei_color_t){0, 0, 0, 255}, &(ei_anchor_t){ei_anc_north}, NULL, NULL, NULL, NULL, NULL);
    ei_place_xy(N_frame, 370, 20);

    ei_widget_t NE_frame;
    NE_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(NE_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"NE red text"}, NULL, &(ei_color_t){255, 0, 0, 255}, &(ei_anchor_t){ei_anc_northeast}, NULL, NULL, NULL);
    ei_place_xy(NE_frame, 720, 20);

    ei_widget_t E_frame;
    E_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(E_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"E cyan text"}, NULL, &(ei_color_t){0, 255, 255, 255}, &(ei_anchor_t){ei_anc_east}, NULL, NULL, NULL);
    ei_place_xy(E_frame, 720, 170);

    ei_widget_t SE_frame;
    SE_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(SE_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"SE semi-transparent text"}, NULL, &(ei_color_t){0, 0, 0, 50}, &(ei_anchor_t){ei_anc_southeast}, NULL, NULL, NULL);
    ei_place_xy(SE_frame, 720, 320);

    ei_widget_t S_frame;
    S_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(S_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"S yellow text"}, NULL, &(ei_color_t){255, 255, 0, 255}, &(ei_anchor_t){ei_anc_south}, NULL, NULL, NULL);
    ei_place_xy(S_frame, 370, 320);

    ei_widget_t SW_frame;
    SW_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(SW_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"SW magenta text"}, NULL, &(ei_color_t){255, 0, 255, 255}, &(ei_anchor_t){ei_anc_southwest}, NULL, NULL, NULL);
    ei_place_xy(SW_frame, 20, 320);

    ei_widget_t W_frame;
    W_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(W_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"W green text"}, NULL, &(ei_color_t){0, 255, 0, 255}, &(ei_anchor_t){ei_anc_west}, NULL, NULL, NULL);
    ei_place_xy(W_frame, 20, 170);

    ei_widget_t center_frame;
    center_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(center_frame, &(ei_size_t){300, 100}, NULL, NULL, NULL, &(ei_string_t){"Center blue text"}, NULL, &(ei_color_t){0, 0, 255, 255}, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL);
    ei_place_xy(center_frame, 370, 170);

    ei_widget_t overlap_frame;
    overlap_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(overlap_frame, &(ei_size_t){300, 100}, &(ei_color_t){0, 0, 0, 0}, NULL, NULL, &(ei_string_t){"Overlap text"}, NULL, &(ei_color_t){0, 0, 0, 255}, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL);
    ei_place_xy(overlap_frame, 420, 166);

    /* Create, configure and place a toplevel window on screen. */
    ei_widget_t window;
    window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(window, &(ei_size_t){320, 240},
                          &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
                          &(int){2},
                          &(ei_string_t){"Move/resize to test text redraw"}, &(bool){true}, NULL, NULL);
    ei_place_xy(window, 500, 430);

    /* Register the default callback. */
    ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return 0;
}