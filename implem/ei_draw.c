#include <stdlib.h>
#include <math.h>

#include "ei_draw_ext.h"
#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/ei_implementation.h"

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
    int point_array_size = 0;

    for (int i = start_angle; i <= end_angle; i++)
    {
        if (point_array_size == nb_points)
        {
            break;
        }

        point_array[point_array_size].x = center.x + radius * cos(i * M_PI / 180);
        point_array[point_array_size].y = center.y + radius * sin(i * M_PI / 180);
        point_array_size++;
    }

    return point_array;
}

ei_point_t *rounded_frame(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw)
{
    if (part_to_draw == ei_rounded_frame_top)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(180, 270);
        int nb_points_arc_2 = ei_get_nb_points_in_arc(270, 315);
        int nb_points_arc_3 = ei_get_nb_points_in_arc(135, 180);

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        memcpy(point_array, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius}, radius, 180, 270), nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 270, 315), nb_points_arc_2 * sizeof(ei_point_t));
        point_array[nb_points_arc_1 + nb_points_arc_2] = (ei_point_t){rect.top_left.x + rect.size.width - h, rect.top_left.y + h};
        point_array[nb_points_arc_1 + nb_points_arc_2 + 1] = (ei_point_t){rect.top_left.x + h, rect.top_left.y + rect.size.height - h};
        memcpy(point_array + nb_points_arc_1 + nb_points_arc_2 + 2, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 135, 180), nb_points_arc_3 * sizeof(ei_point_t));

        return point_array;
    }
    else if (part_to_draw == ei_rounded_frame_bottom)
    {
        int nb_points_arc_1 = ei_get_nb_points_in_arc(0, 90);
        int nb_points_arc_2 = ei_get_nb_points_in_arc(90, 135);
        int nb_points_arc_3 = ei_get_nb_points_in_arc(315, 360);

        ei_point_t *point_array = malloc((nb_points_arc_1 + nb_points_arc_2 + nb_points_arc_3 + 2) * sizeof(ei_point_t));

        // Calculate the minimum between half the width and half the height of the rectangle
        int h = rect.size.width / 2 < rect.size.height / 2 ? rect.size.width / 2 : rect.size.height / 2;

        memcpy(point_array, arc((ei_point_t){rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 90), nb_points_arc_1 * sizeof(ei_point_t));
        memcpy(point_array + nb_points_arc_1, arc((ei_point_t){rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 90, 135), nb_points_arc_2 * sizeof(ei_point_t));
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

int ei_get_nb_points_in_arc(int start_angle, int end_angle)
{
    return abs(end_angle - start_angle) + 1;
}