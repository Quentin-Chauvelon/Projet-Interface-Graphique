#include <stdlib.h>
#include <math.h>

#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"

ei_point_t *ei_get_arc_points(ei_point_t center, int radius, int start_angle, int end_angle)
{
    int nb_points = ei_get_nb_points_in_arc(start_angle, end_angle, radius);

    // Using calloc so that the memory is initialized to 0
    // This way, if the number of points in the arc is less than the number of points we allocated,
    // the remaining points will be (0, 0)
    ei_point_t *point_array = calloc(nb_points, sizeof(ei_point_t));

    // If calloc failed, return NULL
    if (point_array == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw arc.\n.\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    // If the radius is 0, we only to draw one point (the center of the arc since radius is 0)
    if (radius == 0)
    {
        point_array[0] = center;
        return point_array;
    }

    int point_array_index = 0;

    // Calculate the position of the point for every degree between start_angle and end_angle
    for (int i = start_angle; i <= end_angle; i++)
    {
        if (point_array_index == nb_points)
        {
            break;
        }

        // i * M_PI / 180 converts the angle from degrees to radians
        point_array[point_array_index].x = center.x + radius * cos(i * M_PI / 180);
        point_array[point_array_index].y = center.y + radius * sin(i * M_PI / 180);
        point_array_index++;
    }

    return point_array;
}

ei_point_t *ei_get_rounded_frame_points(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw)
{
    if (part_to_draw == ei_rounded_frame_top)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(180, 270, radius); // 90° top left angle
        int nb_points_arc_2 = ei_get_nb_points_in_arc(270, 315, radius); // 45° top right angle
        int nb_points_arc_3 = ei_get_nb_points_in_arc(135, 180, radius); // 45° bottom left angle

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // If malloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        ei_point_t *arc_1 = ei_get_arc_points(ei_point(rect.top_left.x + radius, rect.top_left.y + radius), radius, 180, 270);
        ei_point_t *arc_2 = ei_get_arc_points(ei_point(rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius), radius, 270, 315);
        ei_point_t *arc_3 = ei_get_arc_points(ei_point(rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius), radius, 135, 180);

        // If calloc failed on one of the arcs, return NULL
        if (arc_1 == NULL || arc_2 == NULL || arc_3 == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);

            // Free all arcs in case only one of them couldn't be allocated
            free(arc_1);
            free(arc_2);
            free(arc_3);

            return NULL;
        }

        memcpy(point_array, arc_1, nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc_2, nb_points_arc_2 * sizeof(ei_point_t));
        // The two following points are used to get the shape defined in the subject (figure A.1 d))
        point_array[nb_points_arc_1 + nb_points_arc_2] = ei_point(rect.top_left.x + rect.size.width - h, rect.top_left.y + h);
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = ei_point(rect.top_left.x + h, rect.top_left.y + rect.size.height - h);
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc_3, nb_points_arc_3 * sizeof(ei_point_t));

        free(arc_1);
        free(arc_2);
        free(arc_3);

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_bottom)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(0, 90, radius);    // 90° bottom right angle
        int nb_points_arc_2 = ei_get_nb_points_in_arc(90, 135, radius);  // 45° bottom left angle
        int nb_points_arc_3 = ei_get_nb_points_in_arc(315, 360, radius); // 45° top right angle

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // If malloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        ei_point_t *arc_1 = ei_get_arc_points(ei_point(rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius), radius, 0, 90);
        ei_point_t *arc_2 = ei_get_arc_points(ei_point(rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius), radius, 90, 135);
        ei_point_t *arc_3 = ei_get_arc_points(ei_point(rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius), radius, 315, 360);

        // If calloc failed on one of the arcs, return NULL
        if (arc_1 == NULL || arc_2 == NULL || arc_3 == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);

            // Free all arcs in case only one of them couldn't be allocated
            free(arc_1);
            free(arc_2);
            free(arc_3);

            return NULL;
        }

        memcpy(point_array, arc_1, nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc_2, nb_points_arc_2 * sizeof(ei_point_t));
        // The two following points are used to get the shape defined in the subject (figure A.1 d))
        point_array[nb_points_arc_1 + nb_points_arc_2] = ei_point(rect.top_left.x + h, rect.top_left.y + rect.size.height - h);
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = ei_point(rect.top_left.x + rect.size.width - h, rect.top_left.y + h);
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc_3, nb_points_arc_3 * sizeof(ei_point_t));

        free(arc_1);
        free(arc_2);
        free(arc_3);

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_full)
    {
        int nb_points_per_arc = ei_get_nb_points_in_arc(0, 90, radius);

        ei_point_t *point_array = malloc(nb_points_per_arc * 4 * sizeof(ei_point_t));

        // If alloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        ei_point_t *arc_1 = ei_get_arc_points(ei_point(rect.top_left.x + radius, rect.top_left.y + radius), radius, 180, 270);
        ei_point_t *arc_2 = ei_get_arc_points(ei_point(rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius), radius, 270, 360);
        ei_point_t *arc_3 = ei_get_arc_points(ei_point(rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius), radius, 0, 90);
        ei_point_t *arc_4 = ei_get_arc_points(ei_point(rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius), radius, 90, 180);

        // If calloc failed on one of the arcs, return NULL
        if (arc_1 == NULL || arc_2 == NULL || arc_3 == NULL || arc_4 == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);

            // Free all arcs in case only one of them couldn't be allocated
            free(arc_1);
            free(arc_2);
            free(arc_3);
            free(arc_4);

            return NULL;
        }

        memcpy(point_array, arc_1, nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc, arc_2, nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc * 2, arc_3, nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc * 3, arc_4, nb_points_per_arc * sizeof(ei_point_t));

        free(arc_1);
        free(arc_2);
        free(arc_3);
        free(arc_4);

        return point_array;
    }

    return NULL;
}

void ei_draw_rectangle(ei_surface_t surface, ei_rect_t screen_location, ei_color_t background_color, const ei_rect_t *clipper)
{
    ei_point_t *point_array = malloc(4 * sizeof(ei_point_t));

    // If malloc failed, return
    if (point_array == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw rectangle.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    point_array[0] = screen_location.top_left;
    point_array[1] = ei_point(screen_location.top_left.x + screen_location.size.width, screen_location.top_left.y);
    point_array[2] = ei_point(screen_location.top_left.x + screen_location.size.width, screen_location.top_left.y + screen_location.size.height);
    point_array[3] = ei_point(screen_location.top_left.x, screen_location.top_left.y + screen_location.size.height);

    ei_draw_polygon(surface, point_array, 4, background_color, clipper);

    free(point_array);
}

void ei_draw_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, int corner_radius, ei_color_t background_color, ei_relief_t relief, const ei_color_t *border_color, const ei_rect_t *clipper)
{
    //  Draw the border if there is one
    if (border_width > 0)
    {
        ei_color_t color1;
        ei_color_t color2;

        // If border_color is not NULL, use the given color for both border colors
        if (border_color != NULL)
        {
            color1 = color2 = *border_color;
        }
        // Otherwise, compute a lighter and darker color of the background color
        else
        {
            ei_get_border_colors(background_color, relief, &color1, &color2);
        }

        ei_point_t *top_point_array = ei_get_rounded_frame_points(screen_location, corner_radius, ei_rounded_frame_top);
        ei_point_t *bottom_point_array = ei_get_rounded_frame_points(screen_location, corner_radius, ei_rounded_frame_bottom);

        // If malloc failed, return
        if (top_point_array == NULL || bottom_point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }

        ei_draw_polygon(surface, top_point_array, corner_radius != 0 ? 185 : 5, color1, clipper);
        ei_draw_polygon(surface, bottom_point_array, corner_radius != 0 ? 185 : 5, color2, clipper);

        free(top_point_array);
        free(bottom_point_array);
    }

    // Resize the rectangle to account for the border width
    screen_location = ei_rect_add(screen_location, border_width, border_width, border_width * -2, border_width * -2);

    ei_point_t *point_array;

    // If the corner radius is 0, simply fill the rectangle
    if (corner_radius == 0)
    {
        ei_draw_rectangle(surface, screen_location, background_color, clipper);
    }
    else
    {

        point_array = ei_get_rounded_frame_points(screen_location, corner_radius, ei_rounded_frame_full);

        // If malloc failed, return
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }

        ei_draw_polygon(surface, point_array, 364, background_color, clipper);

        free(point_array);
    }
}

void ei_draw_circle(ei_surface_t surface, ei_point_t center, int radius, ei_color_t color, const ei_rect_t *clipper)
{
    int nb_points = ei_get_nb_points_in_arc(0, 360, radius);

    ei_point_t *point_array = ei_get_arc_points(center, radius, 0, 360);

    // If malloc failed, return
    if (point_array == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw circle.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_draw_polygon(surface, point_array, nb_points, color, clipper);

    free(point_array);
}

int ei_get_nb_points_in_arc(int start_angle, int end_angle, int radius)
{
    return radius != 0 ? abs(end_angle - start_angle) + 1 : 1;
}

void ei_fill(ei_surface_t surface, const ei_color_t *color, const ei_rect_t *clipper)
{
    // building of a square whith the specified surface if clipper is None
    ei_rect_t rec = hw_surface_get_rect((ei_surface_t)surface);
    ei_size_t dim = rec.size;
    int h = dim.height;
    int l = dim.width;

    // We create a rect from the top-left of the surface
    ei_point_t *point_array = malloc(4 * sizeof(ei_point_t));
    ei_point_t *p = point_array;

    *point_array = rec.top_left;
    point_array++;
    *point_array = ei_point(rec.top_left.x, rec.top_left.y + h);
    point_array++;
    *point_array = ei_point(rec.top_left.x + l, rec.top_left.y + h);
    point_array++;
    *point_array = ei_point(rec.top_left.x + l, rec.top_left.y);

    ei_draw_polygon(surface, p, 4, *color, clipper);

    // free(p);
}

int ei_copy_surface(ei_surface_t destination, const ei_rect_t *dst_rect, ei_surface_t source, const ei_rect_t *src_rect, bool alpha)
{
    ei_size_t src_size_after_clipping = src_rect == NULL ? hw_surface_get_size(source) : src_rect->size;
    ei_size_t dst_size_after_clipping = dst_rect == NULL ? hw_surface_get_size(destination) : dst_rect->size;

    // If the surfaces after clipping don't have the same size, resize them
    if (!ei_equal_sizes(src_size_after_clipping, dst_size_after_clipping))
    {
        src_size_after_clipping.width = src_size_after_clipping.width < dst_size_after_clipping.width ? src_size_after_clipping.width : dst_size_after_clipping.width;
        dst_size_after_clipping.width = src_size_after_clipping.width < dst_size_after_clipping.width ? src_size_after_clipping.width : dst_size_after_clipping.width;
    }

    // Calculate the rectangles represensenting the areas to copy/paste after clipping
    src_rect = src_rect != NULL ? src_rect : &(ei_rect_t){ei_point_zero(), src_size_after_clipping};
    dst_rect = dst_rect != NULL ? dst_rect : &(ei_rect_t){ei_point_zero(), dst_size_after_clipping};

    ei_size_t src_size = hw_surface_get_size(source);
    ei_size_t dst_size = hw_surface_get_size(destination);

    // Since get_buffer returns a pointer to the first pixel, we need to calculate the offset
    // needed to get a pointer to the pixel in the top left of the rectangle after clipping.
    // To do this, we can simply move the pointer by the number of lines before the rectangle * the width of the surface,
    // which gives us a pointer to the first pixel of the correct line, and so, we only need to move by the number of columns
    // before the rectangle. Since a color has 4 components, we also need to multiply the result by the size of the color (4)
    uint8_t *src_top_left_pixel = hw_surface_get_buffer(source) + (src_rect->top_left.y * src_size.width + src_rect->top_left.x) * sizeof(ei_color_t);
    uint8_t *dst_top_left_pixel = hw_surface_get_buffer(destination) + (dst_rect->top_left.y * dst_size.width + dst_rect->top_left.x) * sizeof(ei_color_t);

    int src_line_length = src_size.width * sizeof(ei_color_t);
    int dst_line_length = dst_size.width * sizeof(ei_color_t);

    // Iterate over each pixel to copy
    for (int y = 0; y < src_size_after_clipping.height; y++)
    {
        // memcpy(hw_surface_get_buffer(destination) + dst_offset + y * dst_line_length, hw_surface_get_buffer(source) + src_offset + y * src_line_length, dst_size_after_clipping.width * sizeof(ei_color_t));
        for (int x = 0; x < src_size_after_clipping.width * sizeof(ei_color_t); x += 4)
        {
            // Get a pointer to the first component of the pixel
            uint8_t *src_buffer = src_top_left_pixel + y * src_line_length + x;
            uint8_t *dst_buffer = dst_top_left_pixel + y * dst_line_length + x;

            // If alpha is true, compute a combination of source and destination pixels weighted by the source alpha channel
            if (alpha)
            {
                dst_buffer[0] = (src_buffer[3] * src_buffer[0] + (255 - src_buffer[3]) * dst_buffer[0]) / 255;
                dst_buffer[1] = (src_buffer[3] * src_buffer[1] + (255 - src_buffer[3]) * dst_buffer[1]) / 255;
                dst_buffer[2] = (src_buffer[3] * src_buffer[2] + (255 - src_buffer[3]) * dst_buffer[2]) / 255;
                dst_buffer[3] = 255;
            }
            // Otherwise, we can simply copy the 4 components
            else
            {
                dst_buffer[0] = src_buffer[0];
                dst_buffer[1] = src_buffer[1];
                dst_buffer[2] = src_buffer[2];
                dst_buffer[3] = src_buffer[3];
            }
        }
    }

    // Reset origin
    hw_surface_set_origin(destination, ei_point_zero());

    return 0;
}

void ei_draw_text(ei_surface_t surface, const ei_point_t *where, ei_const_string_t text, ei_font_t font, ei_color_t color, const ei_rect_t *clipper)
{
    ei_surface_t text_surface = hw_text_create_surface(text, font, color);

    ei_rect_t text_surface_rect;
    // If the clipper is NULL, limit the surface to the size of the text
    if (clipper == NULL)
    {
        text_surface_rect = ei_rect(*where, hw_surface_get_size(text_surface));
    }
    // Otherwise, limit the surface to the intersection of the size of the text and the clipper
    else
    {
        text_surface_rect = ei_get_intersection_rectangle(ei_rect(*where, hw_surface_get_size(text_surface)), *clipper);
    }

    hw_surface_lock(text_surface);
    ei_copy_surface(surface, &text_surface_rect, text_surface, NULL, true);
    hw_surface_unlock(text_surface);

    hw_surface_free(text_surface);
}

void ei_draw_image(ei_surface_t surface, ei_surface_t image, ei_rect_t *image_subpart, const ei_point_t *where, const ei_rect_t *clipper)
{
    // If the subpart is not NULL, limit the surface to the size of the subpart, otherwise use the whole image
    ei_rect_t *image_subpart_clipped = malloc(sizeof(ei_rect_t));
    image_subpart_clipped->top_left = image_subpart != NULL ? image_subpart->top_left : ei_point_zero();
    image_subpart_clipped->size = image_subpart != NULL ? image_subpart->size : hw_surface_get_size(image);

    ei_rect_t image_rect = ei_rect(*where, image_subpart_clipped->size);

    // If the clipper is not NULL, limit the surface to the intersection of the size of the text and the clipper
    if (clipper != NULL)
    {
        image_rect = ei_get_intersection_rectangle(image_rect, *clipper);
    }

    // Limit the size of the subpart to the size of the image,
    // so that the subpart also takes into account the clipper
    image_subpart_clipped->size = image_rect.size;

    hw_surface_lock(image);
    ei_copy_surface(surface, &image_rect, image, image_subpart_clipped, true);
    hw_surface_unlock(image);

    free(image_subpart_clipped);
}

void ei_get_border_colors(ei_color_t background_color, ei_relief_t relief, ei_color_t *lighter_color, ei_color_t *darker_color)
{
    switch (relief)
    {
    case ei_relief_raised:
        *lighter_color = ei_lighten_color(background_color);
        *darker_color = ei_darken_color(background_color);
        break;
    case ei_relief_sunken:
        *lighter_color = ei_darken_color(background_color);
        *darker_color = ei_lighten_color(background_color);
        break;
    default:
        *lighter_color = *darker_color = ei_darken_color(background_color);
        break;
    }
}

ei_color_t ei_lighten_color(ei_color_t color)
{
    // Our first approach to lighten the color was to simply increase
    // each of the components by a set value, which worked well for
    // gray colors, but not too much for others. Then, we tried to
    // multiply each component by a set value in order to keep the
    // ratio between the components, which worked much better for
    // most colors. However, when implementing the close button
    // of toplevels, we noticed that the lighter color was the
    // same as the background color, which actually makes sense
    // since the r component is already at 255 and g and b are at 0,
    // so even if we multiply them, they are still at 0, thus we were
    // getting the same color after lightening it.
    // After that, we decided to research a bit on how it was usually
    // done and found two approaches that could work well.
    // The first one was to redistribute the components when they reached
    // 255 (eg: for red, let's say we multiply 255 * 1.5, which gives us
    // 382.5, but the r component is capped at 255, so we take 382.5 - 255
    // and redistribute equally among g and b, which gives us
    // g = b = 0 + (382.5 - 255) / 2 = 63.75). This would have worked well
    // but did seem a bit complicated with a lot of cases to consider and
    // since this is not a very important function, we didn't want to spend
    // too much time on it.
    // Instead, we decided to use the second approach, which is to convert
    // the color to HSL, increase the lightness and convert it back to RGB.

    ei_hsl_color_t hsl = ei_convert_rgb_to_hsl(color);

    hsl.lightness = hsl.lightness * 1.3 < 1 ? hsl.lightness * 1.3 : 1;

    return ei_convert_hsl_to_rgb(hsl);
}

ei_color_t ei_darken_color(ei_color_t color)
{
    ei_hsl_color_t hsl = ei_convert_rgb_to_hsl(color);

    hsl.lightness = hsl.lightness * 0.6 > 0 ? hsl.lightness * 0.6 : 0;

    return ei_convert_hsl_to_rgb(hsl);
}