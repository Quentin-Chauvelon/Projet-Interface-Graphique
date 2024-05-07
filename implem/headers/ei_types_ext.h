/**
 * @file    ei_types_ext.h
 *
 * @brief   Extends the \ref ei_types.h file to add new types.
 *
 * @see     \ref ei_types.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_TYPES_EXT_H
#define EI_TYPES_EXT_H

/**
 * @brief	Part of rounded frame to draw.
 */
typedef enum
{
    ei_rounded_frame_top = 0, ///< Top part of the rounded frame.
    ei_rounded_frame_bottom,  ///< Bottom part of the rounded frame.
    ei_rounded_frame_full,    ///< Full rounded frame.
} ei_rounded_frame_part_t;

/**
 * @brief   A color in HSL format with transparency.
 */
typedef struct ei_hsl_color_t
{
    float hue;           ///< The hue component of the color.
    float saturation;    ///< The saturation of the color.
    float lightness;     ///< The lightness of the color.
    unsigned char alpha; ///< The transparency of the color. 0 is invisible, 255 is totally opaque.
} ei_hsl_color_t;

#endif