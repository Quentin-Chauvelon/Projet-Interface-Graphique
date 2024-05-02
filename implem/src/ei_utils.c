#include "../implem/headers/ei_utils_ext.h"

ei_color_t get_color_from_id(int id)
{
    ei_color_t color = {0, 0, 0, 255};

    // This code is inspired from https://stackoverflow.com/a/48596185
    // Knowing that each color is coded on 8 bits (0-255). For the red, we take the 8 least significant bits.
    // Then, for the green, we take the 8 following bits, and for the blue, the 8 last bits.
    // This is what the & 0xFF does (it only keeps the 8 least significant bits of the number and sets the others to 0).
    // But, since we don't always want the 8 least significant bits, we shift the bits to the right of the green and blue
    // color, so that the 8 least significant bits are the ones we want.
    color.red = id & 0xFF;
    color.green = (id >> 8) & 0xFF;
    color.blue = (id >> 16) & 0xFF;

    return color;
}