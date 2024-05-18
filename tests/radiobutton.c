#include <stdio.h>
#include <stdlib.h>

#include "../api/ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "../implem/headers/ei_radiobutton.h"
#include "../implem/src/ei_widget_configure.c"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
bool button_press(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	printf("Click !\n");
	return true;
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
	ei_widget_t	group;

	ei_radiobutton_t * radiobutton1;
	ei_radiobutton_t * radiobutton2;
	ei_radiobutton_t * radiobutton3;
	ei_radiobutton_t * radiobutton4;

	/* Create the application and change the color of the background. */
	ei_app_create((ei_size_t){600, 600}, false);
	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	group = ei_widget_create	("radiobutton", ei_app_root_widget(), NULL, NULL);
	
	//Initialisation of the radiobuttons to default value
	radiobutton1=ei_radiobutton_setdefaultsfunc( radiobutton1);
	radiobutton2=ei_radiobutton_setdefaultsfunc( radiobutton2);
	radiobutton3=ei_radiobutton_setdefaultsfunc( radiobutton3);
	radiobutton4=ei_radiobutton_setdefaultsfunc( radiobutton4);

	//Building of the group 	
	ei_add_radiobutton(group,radiobutton1);
	ei_add_radiobutton(group,radiobutton2);
	ei_add_radiobutton(group,radiobutton3);
	ei_add_radiobutton(group,radiobutton4);

	//Configuration
	ei_radiobutton_configure	(radiobutton1,
			   			&(ei_color_t){0x30, 0x80, 0x30, 0xff},
                        &(ei_string_t) {" Trés Bien "}, NULL,
					 	NULL,
						&(ei_callback_t){button_press } ,
						true);
	
	ei_radiobutton_configure	(radiobutton2,
			   			&(ei_color_t){0x30, 0x30, 0x80, 0xff},
                        &(ei_string_t) {" Bien "}, NULL,
					 	NULL,
						&(ei_callback_t){button_press} ,
						true);
	
	ei_radiobutton_configure	(radiobutton3,
			   			NULL,
                        &(ei_string_t) {" Assez Bien "}, NULL,
					 	NULL,
						&(ei_callback_t){button_press} ,
						false);

	ei_radiobutton_configure	(radiobutton4,
			   			&(ei_color_t){0x80, 0x30, 0x30, 0xff},
                        &(ei_string_t) {" Passable "}, NULL,
					 	NULL,
						&(ei_callback_t){button_press} ,
						false);
	
	ei_radiobutton_group_configure(group,&(ei_size_t){300,200},NULL,&(ei_color_t){0xff,0xff,0xff,0xff},NULL,NULL);

	ei_place_xy		(group, 150, 200);

	/* Register the default callback to events of interest. */
	ei_bind(ei_ev_keydown,		NULL, "all", default_handler, NULL);
	ei_bind(ei_ev_close,		NULL, "all", default_handler, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_unbind(ei_ev_keydown,	NULL, "all", default_handler, NULL);
	ei_unbind(ei_ev_close,		NULL, "all", default_handler, NULL);

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
