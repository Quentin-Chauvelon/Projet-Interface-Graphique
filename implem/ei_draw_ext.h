#ifndef EI_DRAW_EXT_H
#define EI_DRAW_EXT_H

#include "../api/ei_types.h"
#include "../implem/ei_types_ext.h"
#include "../implem/ei_button.h"

/// @brief
/// @return
ei_point_t *arc(ei_point_t center, int radius, int start_angle, int end_angle);

ei_point_t *rounded_frame(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw);

void ei_draw_button(ei_button_t *button, ei_surface_t surface, const ei_rect_t *clipper);

int ei_get_nb_points_in_arc(int start_angle, int end_angle);

#endif