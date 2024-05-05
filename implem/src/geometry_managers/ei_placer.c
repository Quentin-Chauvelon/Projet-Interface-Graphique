#include "../api/ei_placer.h"
#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_application_ext.h"

static ei_anchor_t ei_placer_get_anchor_default_value()
{
    return ei_anc_northwest;
}

static int ei_placer_get_x_default_value()
{
    return 0;
}

static int ei_placer_get_y_default_value()
{
    return 0;
}

static int ei_placer_get_width_default_value(ei_widget_t widget, int *width, float *rel_width)
{
    // If width is NULL and rel_width is NULL, use the requested width
    // If the requested width is NULL, use the default width
    if (width != NULL)
    {
        return *width;
    }
    else
    {
        if (rel_width == NULL)
        {
            if (widget->requested_size.width != 0)
            {
                return widget->requested_size.width;
            }
            else
            {
                return widget->preferred_size.width;
            }
        }
        else
        {
            return 0;
        }
    }
}

static int ei_placer_get_height_default_value(ei_widget_t widget, int *height, float *rel_height)
{
    // If height is NULL and rel_height is NULL, use the requested height
    // If the requested height is NULL, use the default height
    if (height != NULL)
    {
        return *height;
    }
    else
    {
        if (rel_height == NULL)
        {
            if (widget->requested_size.height != 0)
            {
                return widget->requested_size.height;
            }
            else
            {
                return widget->preferred_size.height;
            }
        }
        else
        {
            return 0;
        }
    }
}

static float ei_placer_get_rel_x_default_value()
{
    return 0.0;
}

static float ei_placer_get_rel_y_default_value()
{
    return 0.0;
}

static float ei_placer_get_rel_width_default_value()
{
    return 0.0;
}

static float ei_placer_get_rel_height_default_value()
{
    return 0.0;
}

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
    ei_placer_t *placer_geom_param;

    // If the widget has not been drawn yet, allocate memory
    if (!ei_widget_is_displayed(widget))
    {
        placer_geom_param = malloc(sizeof(ei_placer_t));

        // If malloc failed, return
        if (placer_geom_param == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory for the widget's placer geom params.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }
    }
    // Otherwise, get the geom params of the widget
    else
    {
        placer_geom_param = (ei_placer_t *)ei_widget_get_geom_params(widget);
    }

    // Update the geometry manager handling the widget
    char geometrymanager_name[20] = "placer";
    placer_geom_param->geom_param.manager = ei_geometrymanager_from_name(geometrymanager_name);

    // If the widget class doesn't exist, return
    if (placer_geom_param->geom_param.manager == NULL)
    {
        free(placer_geom_param);
        printf("\033[0;33mWarning: the geometry manager %s doesn't exist.\n\t at %s (%s:%d)\033[0m\n", geometrymanager_name, __func__, __FILE__, __LINE__);
        return;
    }

    // Set the geom params to the given parameter if not NULL or the default value otherwise
    placer_geom_param->anchor = anchor != NULL ? *anchor : ei_placer_get_anchor_default_value();
    placer_geom_param->x = x != NULL ? *x : ei_placer_get_x_default_value();
    placer_geom_param->y = y != NULL ? *y : ei_placer_get_y_default_value();
    placer_geom_param->width = ei_placer_get_width_default_value(widget, width, rel_width);
    placer_geom_param->height = ei_placer_get_height_default_value(widget, height, rel_height);
    placer_geom_param->rel_x = rel_x != NULL ? *rel_x : ei_placer_get_rel_x_default_value();
    placer_geom_param->rel_y = rel_y != NULL ? *rel_y : ei_placer_get_rel_y_default_value();
    placer_geom_param->rel_width = rel_width != NULL ? *rel_width : ei_placer_get_rel_width_default_value();
    placer_geom_param->rel_height = rel_height != NULL ? *rel_height : ei_placer_get_rel_height_default_value();

    ei_widget_set_geom_params(widget, (ei_geom_param_t)placer_geom_param);
}

void ei_placer_runfunc(ei_widget_t widget)
{
    DEBUG ? printf("Placing widget %d\n", widget->pick_id) : 0;

    ei_placer_t *widget_geom_params = (ei_placer_t *)ei_widget_get_geom_params(widget);

    ei_rect_t new_screen_location = ei_rect_zero();

    new_screen_location.top_left.x = widget_geom_params->x;
    new_screen_location.top_left.y = widget_geom_params->y;
    new_screen_location.size.width = widget_geom_params->width;
    new_screen_location.size.height = widget_geom_params->height;

    // Must be the last function call before returning
    ei_geometry_run_finalize(widget, &new_screen_location);
}

void ei_placer_releasefunc(ei_widget_t widget)
{
}