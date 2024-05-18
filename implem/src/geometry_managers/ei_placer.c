#include "../api/ei_placer.h"
#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_application_ext.h"

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

        // If the widget was managed by another geometry manager, unmap the widget
        if (strcmp(widget->geom_params->manager->name, "placer") != 0)
        {
            ei_geometrymanager_unmap(widget);
        }
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
    if (!ei_widget_is_displayed(widget))
    {
        placer_geom_param->anchor = anchor != NULL && *anchor != ei_anc_none ? *anchor : ei_anc_northwest;
        placer_geom_param->x = x != NULL ? *x : 0;
        placer_geom_param->y = y != NULL ? *y : 0;
        placer_geom_param->width = width != NULL ? *width : 0;
        placer_geom_param->height = height != NULL ? *height : 0;
        placer_geom_param->rel_x = rel_x != NULL ? *rel_x : 0.0;
        placer_geom_param->rel_y = rel_y != NULL ? *rel_y : 0.0;
        placer_geom_param->rel_width = rel_width != NULL ? *rel_width : 0.0;
        placer_geom_param->rel_height = rel_height != NULL ? *rel_height : 0.0;
    }
    else
    {
        if (anchor != NULL && *anchor != ei_anc_none)
        {
            placer_geom_param->anchor = *anchor;
        }

        if (x != NULL)
        {
            placer_geom_param->x = *x;
        }

        if (y != NULL)
        {
            placer_geom_param->y = *y;
        }

        if (width != NULL)
        {
            placer_geom_param->width = *width;
        }

        if (height != NULL)
        {
            placer_geom_param->height = *height;
        }

        if (rel_x != NULL)
        {
            placer_geom_param->rel_x = *rel_x;
        }

        if (rel_y != NULL)
        {
            placer_geom_param->rel_y = *rel_y;
        }

        if (rel_width != NULL)
        {
            placer_geom_param->rel_width = *rel_width;
        }

        if (rel_height != NULL)
        {
            placer_geom_param->rel_height = *rel_height;
        }
    }

    ei_widget_set_geom_params(widget, (ei_geom_param_t)placer_geom_param);

    placer_geom_param->geom_param.manager->runfunc(widget);
}

void ei_placer_runfunc(ei_widget_t widget)
{
    DEBUG ? printf("Placing widget %d\n", widget->pick_id) : 0;

    ei_placer_t *widget_geom_params = (ei_placer_t *)ei_widget_get_geom_params(widget);

    ei_rect_t new_screen_location;

    // If the widget is the close button of the toplevel, don't use the widget's content_rect,
    // instead, we want to use the screen location
    if (strcmp(widget->parent->wclass->name, "toplevel") == 0 &&
        ((ei_toplevel_t *)widget->parent)->close_button != NULL &&
        (ei_widget_t *)((ei_toplevel_t *)widget->parent)->close_button == (ei_widget_t *)widget)
    {
        new_screen_location.top_left = widget->parent->screen_location.top_left;
    }
    else
    {
        new_screen_location.top_left = widget->parent->content_rect->top_left;
    }

    new_screen_location.size = ei_size_zero();

    // Add the relative position
    new_screen_location.top_left.x += widget_geom_params->rel_x * widget->parent->content_rect->size.width;
    new_screen_location.top_left.y += widget_geom_params->rel_y * widget->parent->content_rect->size.height;

    // Add the absolute position
    new_screen_location.top_left.x += widget_geom_params->x;
    new_screen_location.top_left.y += widget_geom_params->y;

    // Add the relative size
    new_screen_location.size.width += widget_geom_params->rel_width * widget->parent->content_rect->size.width;
    new_screen_location.size.height += widget_geom_params->rel_height * widget->parent->content_rect->size.height;

    // Add the absolute size
    // If width is NULL and rel_width is NULL, use the requested width
    // If the requested width is NULL, use the default width (which is stored
    // in the widget's rquested_size)
    if (widget_geom_params->width != 0)
    {
        new_screen_location.size.width += widget_geom_params->width;
    }
    else
    {
        if (widget_geom_params->rel_width == 0)
        {
            new_screen_location.size.width += widget->requested_size.width;
        }
    }

    // If height is NULL and rel_height is NULL, use the requested height
    // If the requested height is NULL, use the default height (which is stored
    // in the widget's requested_size)
    if (widget_geom_params->height != 0)
    {
        new_screen_location.size.height += widget_geom_params->height;
    }
    else
    {
        if (widget_geom_params->rel_height == 0)
        {
            new_screen_location.size.height += widget->requested_size.height;
        }
    }

    // Move the widget on the x axis based on the anchor
    switch (widget_geom_params->anchor)
    {
    case ei_anc_north:
    case ei_anc_center:
    case ei_anc_south:
        new_screen_location.top_left.x -= widget->screen_location.size.width / 2;
        break;
    case ei_anc_northeast:
    case ei_anc_east:
    case ei_anc_southeast:
        new_screen_location.top_left.x -= widget->screen_location.size.width;
        break;
    default:
        break;
    }

    // Move the widget on the y axis based on the anchor
    switch (widget_geom_params->anchor)
    {
    case ei_anc_west:
    case ei_anc_center:
    case ei_anc_east:
        new_screen_location.top_left.y -= widget->screen_location.size.height / 2;
        break;
    case ei_anc_southwest:
    case ei_anc_south:
    case ei_anc_southeast:
        new_screen_location.top_left.y -= widget->screen_location.size.height;
        break;
    default:
        break;
    }

    // Must be the last function call before returning
    ei_geometry_run_finalize(widget, &new_screen_location);
}

void ei_placer_releasefunc(ei_widget_t widget)
{
    if (widget->children_head != NULL)
    {
        for (ei_widget_t current_child = widget->children_head; current_child != NULL; current_child = current_child->next_sibling)
        {
            if (ei_widget_is_displayed(current_child))
            {
                current_child->geom_params->manager->runfunc(current_child);
            }
        }
    }
}