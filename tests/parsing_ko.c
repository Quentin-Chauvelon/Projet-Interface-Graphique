#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_parser.h"

#include "../implem/headers/ei_implementation.h"

char *filename = "tests/parsing_ko.txt";
FILE *file;
ei_size_t screen_size = {750, 550};
ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

// process_key --

bool process_key(ei_widget_t widget, ei_event_t *event, void *user_param)
{
    if (event->param.key_code == SDLK_ESCAPE)
    {
        ei_app_quit_request();
        return true;
    }

    return false;
}

void test_parsing_ko(char *text)
{
    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, false);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error while opening file\n");
        return;
    }

    fputs(text, file);

    fclose(file);

    if (ei_parse_file(filename) == 0)
        printf("Error while attempting to parse \"parsing_ko.txt\", aborting.\n\n");
    // else
    //     /* Run the application's main loop. */
    //     ei_app_run();

    remove(filename);

    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();
}

/*
 * main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv)
{
    test_parsing_ko("what_is_this_type toplevel root\n");
    // test_parsing_ko("toplevel toplevel no_root\n");
    // test_parsing_ko("toplevel toplevel root");
    // test_parsing_ko("toplevel toplevel root what_is_this_property=0\n");
    // test_parsing_ko("toplevel toplevel root color={200 300}\n");
    // test_parsing_ko("place toplevel\n");
    // test_parsing_ko("toplevel toplevel root\n\n\n\n place toplevel abc=0\n");

    return (EXIT_SUCCESS);
}
