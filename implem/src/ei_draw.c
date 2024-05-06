#include <stdlib.h>
#include <math.h>

#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"

// Redefine M_PI if it is not defined since we didn't manage to use M_PI from the math or SDL libraries
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ei_point_t *arc(ei_point_t center, int radius, int start_angle, int end_angle)
{
    int nb_points = ei_get_nb_points_in_arc(start_angle, end_angle);

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

ei_point_t *rounded_frame(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw)
{
    if (part_to_draw == ei_rounded_frame_top)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(180, 270); // 90° top left angle
        int nb_points_arc_2 = ei_get_nb_points_in_arc(270, 315); // 45° top right angle
        int nb_points_arc_3 = ei_get_nb_points_in_arc(135, 180); // 45° bottom left angle

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // If malloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        ei_point_t *arc_1 = arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius}, radius, 180, 270);
        ei_point_t *arc_2 = arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 270, 315);
        ei_point_t *arc_3 = arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 135, 180);

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
        point_array[nb_points_arc_1 + nb_points_arc_2] = (ei_point_t){rect.top_left.x + rect.size.width - h, rect.top_left.y + h};
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = (ei_point_t){rect.top_left.x + h, rect.top_left.y + rect.size.height - h};
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc_3, nb_points_arc_3 * sizeof(ei_point_t));

        free(arc_1);
        free(arc_2);
        free(arc_3);

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_bottom)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(0, 90);    // 90° bottom right angle
        int nb_points_arc_2 = ei_get_nb_points_in_arc(90, 135);  // 45° bottom left angle
        int nb_points_arc_3 = ei_get_nb_points_in_arc(315, 360); // 45° top right angle

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // If malloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        ei_point_t *arc_1 = arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 90);
        ei_point_t *arc_2 = arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 90, 135);
        ei_point_t *arc_3 = arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 315, 360);

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
        point_array[nb_points_arc_1 + nb_points_arc_2] = (ei_point_t){rect.top_left.x + h, rect.top_left.y + rect.size.height - h};
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = (ei_point_t){rect.top_left.x + rect.size.width - h, rect.top_left.y + h};
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc_3, nb_points_arc_3 * sizeof(ei_point_t));

        free(arc_1);
        free(arc_2);
        free(arc_3);

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_full)
    {
        int nb_points_per_arc = ei_get_nb_points_in_arc(0, 90);

        ei_point_t *point_array = malloc(nb_points_per_arc * 4 * sizeof(ei_point_t));

        // If alloc failed, return NULL
        if (point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw rounded frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return NULL;
        }

        ei_point_t *arc_1 = arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius}, radius, 180, 270);
        ei_point_t *arc_2 = arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 270, 360);
        ei_point_t *arc_3 = arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 90);
        ei_point_t *arc_4 = arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 90, 180);

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

void ei_draw_rounded_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, int corner_radius, ei_color_t color, ei_relief_t relief, const ei_rect_t *clipper)
{
    // Draw the relief if there is one and if the border width is not 0 (otherwise it
    // would be overriden by the button itself anyway, so it would be useless to draw it)
    if (border_width > 0 && relief != ei_relief_none)
    {
        // Invert the colors based on the relief (raised or sunken)
        ei_color_t color1 = relief == ei_relief_raised ? (ei_color_t){200, 200, 200, 255} : (ei_color_t){100, 100, 100, 255};
        ei_color_t color2 = relief == ei_relief_raised ? (ei_color_t){100, 100, 100, 255} : (ei_color_t){200, 200, 200, 255};

        ei_point_t *top_point_array = rounded_frame(screen_location, corner_radius, ei_rounded_frame_top);
        ei_point_t *bottom_point_array = rounded_frame(screen_location, corner_radius, ei_rounded_frame_bottom);

        // If malloc failed, return
        if (top_point_array == NULL || bottom_point_array == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to draw button.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }

        ei_draw_polygon(surface, top_point_array, 185, color1, clipper);
        ei_draw_polygon(surface, bottom_point_array, 185, color2, clipper);

        free(top_point_array);
        free(bottom_point_array);
    }

    // Create a new smaller rectangle to account for the border width
    ei_rect_t *border_width_rect = malloc(sizeof(ei_rect_t));

    // If malloc failed, return
    if (border_width_rect == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw button.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    border_width_rect->top_left = (ei_point_t){screen_location.top_left.x + border_width, screen_location.top_left.y + border_width};
    border_width_rect->size = (ei_size_t){screen_location.size.width - 2 * border_width, screen_location.size.height - 2 * border_width};

    // If relief is none, we don't want to take the border width into account,
    // otherwise the button would be smaller than what it should be
    ei_rect_t inner_rectangle = relief == ei_relief_none ? screen_location : *border_width_rect;

    ei_point_t *point_array = rounded_frame(inner_rectangle, corner_radius, ei_rounded_frame_full);

    // If malloc failed, return
    if (point_array == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw button.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_draw_polygon(surface, point_array, 364, color, clipper);

    free(point_array);
    free(border_width_rect);
}

int ei_get_nb_points_in_arc(int start_angle, int end_angle)
{
    return abs(end_angle - start_angle) + 1;
}

int ei_copy_surface(ei_surface_t destination, const ei_rect_t *dst_rect, ei_surface_t source, const ei_rect_t *src_rect, bool alpha)
{
    ei_size_t src_size_after_clipping = src_rect == NULL ? hw_surface_get_size(source) : src_rect->size;
    ei_size_t dst_size_after_clipping = dst_rect == NULL ? hw_surface_get_size(destination) : dst_rect->size;

    // If the surfaces after clipping don't have the same size, return 1
    if (!equal_sizes(src_size_after_clipping, dst_size_after_clipping))
    {
        printf("\033[0;33mWarning: The source and destination areas must have the same size to copy.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return 1;
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
        text_surface_rect = get_intersection_rectangle(ei_rect(*where, hw_surface_get_size(text_surface)), *clipper);
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
        image_rect = get_intersection_rectangle(image_rect, *clipper);
    }

    // Limit the size of the subpart to the size of the image,
    // so that the subpart also takes into account the clipper
    image_subpart_clipped->size = image_rect.size;

    hw_surface_lock(image);
    ei_copy_surface(surface, &image_rect, image, image_subpart_clipped, true);
    hw_surface_unlock(image);

    free(image_subpart_clipped);
}