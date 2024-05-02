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

#endif