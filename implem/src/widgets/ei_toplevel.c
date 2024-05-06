#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_size_t *ei_toplevel_get_natural_size(ei_toplevel_t *toplevel)
{
    ei_size_t natural_size = ei_size(toplevel->widget.requested_size.width, toplevel->widget.requested_size.height);

    // The top level must have a minimum size of (160, 120)
    if (natural_size.width < 160)
    {
        natural_size.width = 160;
    }

    if (natural_size.height < 120)
    {
        natural_size.height = 120;
    }

    return &natural_size;
}