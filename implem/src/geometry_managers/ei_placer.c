#include "../api/ei_placer.h"
#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"

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

    // If the widget has not been drawn yet, allocate memory and set the geom params for the widget
    if (widget->geom_params == NULL)
    {
        ei_impl_geom_param_t *geom_params = malloc(ei_geom_param_size());

        char geometrymanager_name[20] = "placer";
        geom_params->manager = ei_geometrymanager_from_name(geometrymanager_name);
        geom_params->anchor = anchor != NULL ? *anchor : ei_anc_northwest;
        geom_params->x = x != NULL ? *x : 0;
        geom_params->y = y != NULL ? *y : 0;

        // If width is NULL and rel_width is NULL, use the requested width
        // If the requested width is NULL, use the default width
        if (width != NULL)
        {
            geom_params->width = *width;
        }
        else
        {
            if (rel_width != NULL)
            {
                if (widget->requested_size.width != 0)
                {
                    geom_params->width = widget->requested_size.width;
                }
                else
                {
                    geom_params->width = widget->preferred_size.width;
                }
            }
            else
            {
                geom_params->width = 0;
            }
        }

        // If height is NULL and rel_height is NULL, use the requested height
        // If the requested height is NULL, use the default height
        if (height != NULL)
        {
            geom_params->height = *height;
        }
        else
        {
            if (rel_height != NULL)
            {
                if (widget->requested_size.height != 0)
                {
                    geom_params->height = widget->requested_size.height;
                }
                else
                {
                    geom_params->height = widget->preferred_size.height;
                }
            }
            else
            {
                geom_params->height = 0;
            }
        }

        geom_params->rel_x = rel_x != NULL ? *rel_x : 0.0;
        geom_params->rel_y = rel_y != NULL ? *rel_y : 0.0;
        geom_params->rel_width = rel_width != NULL ? *rel_width : 0.0;
        geom_params->rel_height = rel_height != NULL ? *rel_height : 0.0;

        ei_widget_set_geom_params(widget, geom_params);

        return;
    }

    // If the widget has already been drawn, update the geom params
    if (anchor != NULL)
    {
        widget->geom_params->anchor = *anchor;
    }

    if (x != NULL)
    {
        widget->geom_params->x = *x;
    }

    if (y != NULL)
    {
        widget->geom_params->y = *y;
    }

    if (width != NULL)
    {
        widget->geom_params->width = *width;
    }

    if (height != NULL)
    {
        widget->geom_params->height = *height;
    }

    if (rel_x != NULL)
    {
        widget->geom_params->rel_x = *rel_x;
    }

    if (rel_y != NULL)
    {
        widget->geom_params->rel_y = *rel_y;
    }

    if (rel_width != NULL)
    {
        widget->geom_params->rel_width = *rel_width;
    }

    if (rel_height != NULL)
    {
        widget->geom_params->rel_height = *rel_height;
    }
}

void ei_placer_runfunc(ei_widget_t widget)
{
    widget->screen_location.top_left.x = widget->geom_params->x;
    widget->screen_location.top_left.y = widget->geom_params->y;
}