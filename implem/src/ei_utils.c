#include "../api/ei_utils.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_types_ext.h"

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
    int area_rect1 = rect1.size.width * rect1.size.height;
    int area_rect2 = rect2.size.width * rect2.size.height;

    if (area_rect1 == 0 || area_rect2 == 0)
    {
        return 0;
    }

    // Calculate the total area of the two rectangles minus the intersection area
    // We then return the percentage of the intersection area over that total area
    return ((intersection_area * 100) / ((area_rect1 + area_rect2) - intersection_area));
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

bool equal_sizes(ei_size_t size1, ei_size_t size2)
{
    return size1.width == size2.width && size1.height == size2.height;
}

ei_point_t get_position_in_parent_from_anchor(ei_rect_t parent, ei_size_t child, ei_anchor_t anchor)
{
    switch (anchor)
    {
    case ei_anc_center:
        return ei_point(parent.top_left.x + parent.size.width / 2 - child.width / 2, parent.top_left.y + parent.size.height / 2 - child.height / 2);
    case ei_anc_north:
        return ei_point(parent.top_left.x + parent.size.width / 2 - child.width / 2, parent.top_left.y);
    case ei_anc_northeast:
        return ei_point(parent.top_left.x + parent.size.width - child.width, parent.top_left.y);
    case ei_anc_east:
        return ei_point(parent.top_left.x + parent.size.width - child.width, parent.top_left.y + parent.size.height / 2 - child.height / 2);
    case ei_anc_southeast:
        return ei_point(parent.top_left.x + parent.size.width - child.width, parent.top_left.y + parent.size.height - child.height);
    case ei_anc_south:
        return ei_point(parent.top_left.x + parent.size.width / 2 - child.width / 2, parent.top_left.y + parent.size.height - child.height);
    case ei_anc_southwest:
        return ei_point(parent.top_left.x, parent.top_left.y + parent.size.height - child.height);
    case ei_anc_west:
        return ei_point(parent.top_left.x, parent.top_left.y + parent.size.height / 2 - child.height / 2);
    case ei_anc_northwest:
        return ei_point(parent.top_left.x, parent.top_left.y);
    default:
        return ei_point(parent.top_left.x, parent.top_left.y);
    }
}

ei_rect_t ei_rect_move(ei_rect_t rect, int x, int y, ei_rect_t *clipper)
{
    // Move the rectangle by x and y
    rect.top_left.x += x;
    rect.top_left.y += y;

    // If the clipper is not NULL, crop the rectangle to fit in the clipper
    if (clipper != NULL)
    {
        rect = get_intersection_rectangle(rect, *clipper);
    }

    return rect;
}

ei_rect_t ei_rect_add(ei_rect_t rect, int x, int y, int width, int height)
{
    rect.top_left.x += x;
    rect.top_left.y += y;
    rect.size.width += width;
    rect.size.height += height;

    return rect;
}

ei_rect_t ei_rect_sub(ei_rect_t rect, int x, int y, int width, int height)
{
    rect.top_left.x -= x;
    rect.top_left.y -= y;
    rect.size.width -= width;
    rect.size.height -= height;

    return rect;
}

ei_hsl_color_t convert_rgb_to_hsl(ei_color_t color)
{
    float r = color.red / 255.0;
    float g = color.green / 255.0;
    float b = color.blue / 255.0;

    // Get the highest component value
    float max = fmax(r, fmax(g, b));
    float min = fmin(r, fmin(g, b));

    float c = max - min;

    float h = 0;
    float s = 0;
    float l = (max + min) / 2;

    if (c == 0)
    {
        h = 0;
    }
    else if (max == r)
    {
        h = fmod((g - b) / c, 6);
    }
    else if (max == g)
    {
        h = ((b - r) / c) + 2;
    }
    else if (max == b)
    {
        h = ((r - g) / c) + 4;
    }

    h *= 60;

    if (l == 1 || l == 0)
    {
        s = 0;
    }
    else
    {
        s = c / (1 - fabs(2 * l - 1));
    }

    return (ei_hsl_color_t){h, s, l};
}

/**
 * @brief   Converts a color from HUE to RGB.
 *
 * @param   p
 * @param   q
 * @param   hue // The hue component of the color.
 */
static float convert_hue_to_rgb(float p, float q, float hue)
{
    if (hue < 0)
    {
        hue += 1;
    }

    if (hue > 1)
    {
        hue -= 1;
    }

    if (hue < 1.0 / 6.0)
    {
        return p + (q - p) * 6 * hue;
    }

    if (hue < 1.0 / 2.0)
    {
        return q;
    }

    if (hue < 2.0 / 3.0)
    {
        return p + (q - p) * (2.0 / 3.0 - hue) * 6;
    }

    return p;
}

ei_color_t convert_hsl_to_rgb(ei_hsl_color_t hsl)
{
    float r = 0;
    float g = 0;
    float b = 0;

    // The hue is in degrees [0,360], for the following code to work, we need in [0,1]
    hsl.hue /= 360;

    if (hsl.saturation == 0)
    {
        r = g = b = hsl.lightness;
    }
    else
    {
        float q = hsl.lightness < 0.5 ? hsl.lightness * (1 + hsl.saturation) : hsl.lightness + hsl.saturation - hsl.lightness * hsl.saturation;
        float p = 2 * hsl.lightness - q;

        r = convert_hue_to_rgb(p, q, hsl.hue + 1.0 / 3.0);
        g = convert_hue_to_rgb(p, q, hsl.hue);
        b = convert_hue_to_rgb(p, q, hsl.hue - 1.0 / 3.0);
    }

    return (ei_color_t){r * 255, g * 255, b * 255, 255};
}