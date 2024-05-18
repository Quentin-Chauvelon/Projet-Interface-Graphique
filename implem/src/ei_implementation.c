#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"

void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_widget_t current = widget->children_head;

    while (current != NULL)
    {
        // If there is no intersection between the clipper (representing the widget that has had an event)
        // and the current widget, don't redraw it
        if (clipper == NULL || ei_rect_intersects_rect(current->screen_location, *clipper))
        {
            ei_rect_t children_clipper = ei_get_intersection_rectangle(current->screen_location, *clipper);
            current->wclass->drawfunc(current, surface, pick_surface, &children_clipper);
        }

        current = current->next_sibling;
    }
}


/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */

uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color)
{
    uint32_t converted_color = 0 ;
    int ir, ig , ib, ia ;

    // get the channel indices of the surface
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

    // position each byte R, G, B, A in the right place in the 32-bit integer
    converted_color += color.red << 32 - (ir + 1) * 8;
    converted_color += color.green << 32 - (ig + 1) * 8;
    converted_color += color.blue << 32 - (ib + 1) * 8;

    // case where the surface has an alpha chanel
    if (ia = !-1)
    {
        converted_color += color.alpha << 32 - (ia + 1) * 8;
    }

    return converted_color;
}
