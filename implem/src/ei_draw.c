#include <stdlib.h>
#include <math.h>

#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_implementation.h"

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

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        memcpy(point_array, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius}, radius, 180, 270), nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 270, 315), nb_points_arc_2 * sizeof(ei_point_t));
        // The two following points are used to get the shape defined in the subject (figure A.1 d))
        point_array[nb_points_arc_1 + nb_points_arc_2] = (ei_point_t){rect.top_left.x + rect.size.width - h, rect.top_left.y + h};
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = (ei_point_t){rect.top_left.x + h, rect.top_left.y + rect.size.height - h};
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 135, 180), nb_points_arc_3 * sizeof(ei_point_t));

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_bottom)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(0, 90);    // 90° bottom right angle
        int nb_points_arc_2 = ei_get_nb_points_in_arc(90, 135);  // 45° bottom left angle
        int nb_points_arc_3 = ei_get_nb_points_in_arc(315, 360); // 45° top right angle

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        memcpy(point_array, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 90), nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 90, 135), nb_points_arc_2 * sizeof(ei_point_t));
        // The two following points are used to get the shape defined in the subject (figure A.1 d))
        point_array[nb_points_arc_1 + nb_points_arc_2] = (ei_point_t){rect.top_left.x + h, rect.top_left.y + rect.size.height - h};
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = (ei_point_t){rect.top_left.x + rect.size.width - h, rect.top_left.y + h};
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 315, 360), nb_points_arc_3 * sizeof(ei_point_t));

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_full)
    {
        int nb_points_per_arc = ei_get_nb_points_in_arc(0, 90);

        ei_point_t *point_array = malloc(nb_points_per_arc * 4 * sizeof(ei_point_t));

        memcpy(point_array, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius}, radius, 180, 270), nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 270, 360), nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc * 2, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 90), nb_points_per_arc * sizeof(ei_point_t));
        memcpy(point_array + nb_points_per_arc * 3, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 90, 180), nb_points_per_arc * sizeof(ei_point_t));

        return point_array;
    }

    return NULL;
}

void ei_draw_button(ei_button_t *button, ei_surface_t surface, const ei_rect_t *clipper, int border_width, ei_color_t color)
{
    // Draw the relief if there is one and if the border width is not 0 (otherwise it
    // would be overriden by the button itself anyway, so it would be useless to draw it)
    if (border_width > 0 && button->relief != ei_relief_none)
    {
        // Invert the colors based on the relief (raised or sunken)
        ei_color_t color1 = button->relief == ei_relief_raised ? (ei_color_t){200, 200, 200, 255} : (ei_color_t){100, 100, 100, 255};
        ei_color_t color2 = button->relief == ei_relief_raised ? (ei_color_t){100, 100, 100, 255} : (ei_color_t){200, 200, 200, 255};

        ei_draw_polygon(surface, rounded_frame(button->widget.screen_location, button->corner_radius, ei_rounded_frame_top), 185, color1, clipper);
        ei_draw_polygon(surface, rounded_frame(button->widget.screen_location, button->corner_radius, ei_rounded_frame_bottom), 185, color2, clipper);
    }

    // Create a new smaller rectangle to account for the border width
    ei_rect_t *border_width_rect = malloc(sizeof(ei_rect_t));
    border_width_rect->top_left = (ei_point_t){button->widget.screen_location.top_left.x + border_width, button->widget.screen_location.top_left.y + border_width};
    border_width_rect->size = (ei_size_t){button->widget.screen_location.size.width - 2 * border_width, button->widget.screen_location.size.height - 2 * border_width};

    // If relief is none, we don't want to take the border width into account,
    // otherwise the button would be smaller than what it should be
    ei_rect_t inner_rectangle = button->relief == ei_relief_none ? button->widget.screen_location : *border_width_rect;
    ei_draw_polygon(surface, rounded_frame(inner_rectangle, button->corner_radius, ei_rounded_frame_full), 364, color, clipper);

    free(border_width_rect);
}

int ei_get_nb_points_in_arc(int start_angle, int end_angle)
{
    return abs(end_angle - start_angle) + 1;
}