#include "../api/ei_utils.h"
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

bool rect_included_in_rect(ei_rect_t rect1, ei_rect_t rect2)
{
    // Check if the top left corner and the bottom right corner of rect1 are inside rect2.
    // If they are, then rect1 is included in rect2
    return rect1.top_left.x >= rect2.top_left.x &&
           rect1.top_left.y >= rect2.top_left.y &&
           rect1.top_left.x + rect1.size.width <= rect2.top_left.x + rect2.size.width &&
           rect1.top_left.y + rect1.size.height <= rect2.top_left.y + rect2.size.height;
}

ei_rect_t get_intersection_rectangle(ei_rect_t rect1, ei_rect_t rect2)
{
    // If the rectangles don't intersect, the intersection is a rectangle of size 0
    if (!rect_intersects_rect(rect1, rect2))
    {
        return ei_rect_zero();
    }

    // Find the right most left coordinate and highest top coordinate
    ei_point_t top_left = ei_point(
        rect1.top_left.x > rect2.top_left.x ? rect1.top_left.x : rect2.top_left.x,
        rect1.top_left.y > rect2.top_left.y ? rect1.top_left.y : rect2.top_left.y);

    // Find the left most right coordinate and lowest bottom coordinate
    ei_point_t bottom_right = ei_point(
        (rect1.top_left.x + rect1.size.width) < (rect2.top_left.x + rect2.size.width) ? (rect1.top_left.x + rect1.size.width) : (rect2.top_left.x + rect2.size.width),
        (rect1.top_left.y + rect1.size.height) < (rect2.top_left.y + rect2.size.height) ? (rect1.top_left.y + rect1.size.height) : (rect2.top_left.y + rect2.size.height));

    return ei_rect(top_left, ei_size(bottom_right.x - top_left.x, bottom_right.y - top_left.y));
}

int get_intersection_percentage(ei_rect_t rect1, ei_rect_t rect2)
{
    ei_rect_t intersection = get_intersection_rectangle(rect1, rect2);
    int intersection_area = intersection.size.width * intersection.size.height;

    // Calculate the total area of the two rectangles minus the intersection area
    // We then return the percentage of the intersection area over that total area
    return ((intersection_area * 100) / ((rect1.size.width * rect1.size.height + rect2.size.width * rect2.size.height) - intersection_area));
}

ei_rect_t merge_rectangles(ei_rect_t rect1, ei_rect_t rect2)
{
    // This uses the same principle as get_intersection_rectangle() but with inverted conditions

    ei_point_t top_left = ei_point(
        rect1.top_left.x < rect2.top_left.x ? rect1.top_left.x : rect2.top_left.x,
        rect1.top_left.y < rect2.top_left.y ? rect1.top_left.y : rect2.top_left.y);

    ei_point_t bottom_right = ei_point(
        (rect1.top_left.x + rect1.size.width) > (rect2.top_left.x + rect2.size.width) ? (rect1.top_left.x + rect1.size.width) : (rect2.top_left.x + rect2.size.width),
        (rect1.top_left.y + rect1.size.height) > (rect2.top_left.y + rect2.size.height) ? (rect1.top_left.y + rect1.size.height) : (rect2.top_left.y + rect2.size.height));

    return ei_rect(top_left, ei_size(bottom_right.x - top_left.x, bottom_right.y - top_left.y));
}