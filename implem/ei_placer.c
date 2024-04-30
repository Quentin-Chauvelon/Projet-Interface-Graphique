#include "../api/ei_placer.h"
#include "../api/ei_types.h"
#include "../implem/ei_implementation.h"

void ei_place(ei_widget_t widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height)
{
    if (x != NULL)
    {
        widget->screen_location.top_left.x = *x;
    }
    else
    {
        widget->screen_location.top_left.x = 0;
    }

    if (y != NULL)
    {
        widget->screen_location.top_left.y = *y;
    }
    else
    {
        widget->screen_location.top_left.y = 0;
    }
}