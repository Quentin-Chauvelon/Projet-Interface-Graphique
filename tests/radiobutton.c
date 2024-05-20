#include <stdio.h>
#include <stdlib.h>

#include "../api/ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "../implem/headers/api/ei_radio_button.h"

bool on_radio_button_select(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
	printf("Selected: %s\n", get_selected_radio_button_text(widget));
}

/*
 * default_handler --
 *
 *	Callback called to handle keypress and window close events.
 */
bool default_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE))) {
		ei_app_quit_request();
		return true;
	} else
		return false;
}

int main(int argc, char** argv)
{
	ei_widget_t radio_button_group;

	/* Create the application and change the color of the background. */
	ei_app_create((ei_size_t){600, 600}, false);
	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	ei_widget_t window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure(window, &(ei_size_t){320, 240},
						  &(ei_color_t){0xA0, 0xA0, 0xA0, 0xff},
						  &(int){2},
						  &(ei_string_t){"Radio buttons"}, &(bool){true}, NULL, NULL);

	ei_place(window, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.5}, NULL, NULL);

	radio_button_group = ei_widget_create("radio_button_group", window, NULL, NULL);

	ei_widget_t radio_button1 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
	ei_widget_t radio_button2 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
	ei_widget_t radio_button3 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
	ei_widget_t radio_button4 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);
	ei_widget_t radio_button5 = ei_widget_create("radio_button", radio_button_group, NULL, NULL);

	ei_radio_button_set_text(radio_button1, &(ei_string_t){"Great"});
	ei_radio_button_set_text(radio_button2, &(ei_string_t){"Good"});
	ei_radio_button_set_text(radio_button3, &(ei_string_t){"OK"});
	ei_radio_button_set_text(radio_button4, &(ei_string_t){"Poor"});
	ei_radio_button_set_text(radio_button5, &(ei_string_t){"Awful"});

	ei_radio_button_select(radio_button3);

	ei_radio_button_group_configure(radio_button_group, NULL, NULL, NULL, NULL, &(ei_color_t){0xff, 0xff, 0xff, 0xff}, NULL, NULL, &(ei_callback_t){on_radio_button_select});

	ei_place(radio_button_group, &(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL, &(float){0.5}, &(float){0.5}, NULL, NULL);

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
