#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_geometrymanager.h"

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();

    ei_surface_t window_surface = hw_create_window(main_window_size, fullscreen);
}

void ei_app_run(void)
{
    getchar();
}

void ei_app_free(void)
{
}

ei_widget_t ei_app_root_widget(void)
{
}