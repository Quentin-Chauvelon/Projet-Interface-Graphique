#include "../implem/headers/ei_utils_ext.h"

ei_color_t get_color_from_id(int id)
{
    ei_color_t color = {0, 0, 0, 255};

    // This code is inspired from https://stackoverflow.com/a/48596185
    // Knowing that each color is coded on 8 bits (0-255). For the blue, we take the 8 least significant bits.
    // Then, for the green, we take the 8 following bits, and for the red, the 8 last bits.
    // This is what the & 0xFF does (it only keeps the 8 least significant bits of the number and sets the others to 0).
    // But, since we don't always want the 8 least significant bits, we shift the bits to the right of the green and red
    // color, so that the 8 least significant bits are the ones we want.
    color.red = (id >> 16) & 0xFF;
    color.green = (id >> 8) & 0xFF;
    color.blue = id & 0xFF;

    return color;
}

int get_id_from_color(ei_color_t color)
{
    // To convert a color back to an id, we can do the opposite of the previous function.
    // By shifting the bits left, we put each component at the right position with 0s added to the right.
    // This way, we can simply add the 3 components together to get the id.
    return (color.red << 16) + (color.green << 8) + color.blue;
}

bool rect_intersects_rect(ei_rect_t rect1, ei_rect_t rect2)
{
    // Inspired from https://stackoverflow.com/a/306332
    // This works by checking if the left edge of rect1 is to the right edge of rect2,
    // in which case the 2 rectangles don't intersect.
    // We then apply the same principle for the other edges.
    // If at least one of the checks is true, then the rectangles don't intersect.
    // And so, by using De Morgan's law and inverting the conditions, we can find
    // if the two rectangles intersect.
    return rect1.top_left.x < rect2.top_left.x + rect2.size.width &&
           rect1.top_left.x + rect1.size.width > rect2.top_left.x &&
           rect1.top_left.y < rect2.top_left.y + rect2.size.height &&
           rect1.top_left.y + rect1.size.height > rect2.top_left.y;
}