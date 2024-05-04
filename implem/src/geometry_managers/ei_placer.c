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
        ei_placer_t *placer_geom_param = malloc(sizeof(ei_placer_t));

        char geometrymanager_name[20] = "placer";
        placer_geom_param->geom_param.manager = ei_geometrymanager_from_name(geometrymanager_name);

        placer_geom_param->anchor = anchor != NULL ? *anchor : ei_anc_northwest;

        placer_geom_param->anchor = anchor != NULL ? *anchor : ei_anc_northwest;
        placer_geom_param->x = x != NULL ? *x : 0;
        placer_geom_param->y = y != NULL ? *y : 0;

        // If width is NULL and rel_width is NULL, use the requested width
        // If the requested width is NULL, use the default width
        if (width != NULL)
        {
            placer_geom_param->width = *width;
        }
        else
        {
            if (rel_width != NULL)
            {
                if (widget->requested_size.width != 0)
                {
                    placer_geom_param->width = widget->requested_size.width;
                }
                else
                {
                    placer_geom_param->width = widget->preferred_size.width;
                }
            }
            else
            {
                placer_geom_param->width = 0;
            }
        }

        // If height is NULL and rel_height is NULL, use the requested height
        // If the requested height is NULL, use the default height
        if (height != NULL)
        {
            placer_geom_param->height = *height;
        }
        else
        {
            if (rel_height != NULL)
            {
                if (widget->requested_size.height != 0)
                {
                    placer_geom_param->height = widget->requested_size.height;
                }
                else
                {
                    placer_geom_param->height = widget->preferred_size.height;
                }
            }
            else
            {
                placer_geom_param->height = 0;
            }
        }

        placer_geom_param->rel_x = rel_x != NULL ? *rel_x : 0.0;
        placer_geom_param->rel_y = rel_y != NULL ? *rel_y : 0.0;
        placer_geom_param->rel_width = rel_width != NULL ? *rel_width : 0.0;
        placer_geom_param->rel_height = rel_height != NULL ? *rel_height : 0.0;

        ei_widget_set_geom_params(widget, (ei_geom_param_t)placer_geom_param);

        return;
    }

    ei_placer_t *widget_geom_params = (ei_placer_t *)ei_widget_get_geom_params(widget);

    // If the widget has already been drawn, update the geom params
    if (anchor != NULL)
    {
        widget_geom_params->anchor = *anchor;
    }

    if (x != NULL)
    {
        widget_geom_params->x = *x;
    }

    if (y != NULL)
    {
        widget_geom_params->y = *y;
    }

    if (width != NULL)
    {
        widget_geom_params->width = *width;
    }

    if (height != NULL)
    {
        widget_geom_params->height = *height;
    }

    if (rel_x != NULL)
    {
        widget_geom_params->rel_x = *rel_x;
    }

    if (rel_y != NULL)
    {
        widget_geom_params->rel_y = *rel_y;
    }

    if (rel_width != NULL)
    {
        widget_geom_params->rel_width = *rel_width;
    }

    if (rel_height != NULL)
    {
        widget_geom_params->rel_height = *rel_height;
    }

    ei_widget_set_geom_params(widget, (ei_geom_param_t)widget_geom_params);
}

void ei_placer_runfunc(ei_widget_t widget)
{
    ei_placer_t *widget_geom_params = (ei_placer_t *)ei_widget_get_geom_params(widget);

    widget->screen_location.top_left.x = widget_geom_params->x;
    widget->screen_location.top_left.y = widget_geom_params->y;
}