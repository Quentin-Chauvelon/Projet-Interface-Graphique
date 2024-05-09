#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_widget_t ei_entry_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_entry_t));
}

void ei_entry_releasefunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    free(entry);
    entry = NULL;
}

void ei_entry_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_entry_t *entry = (ei_entry_t *)widget;

    // Draw the visible entry
    if (entry->focused)
    {
        ei_draw_straight_frame(surface, widget->screen_location, entry->widget_appearance.border_width - 1, entry->widget_appearance.color, ei_relief_none, clipper);
    }
    else
    {
        ei_draw_straight_frame(surface, widget->screen_location, entry->widget_appearance.border_width, ei_entry_default_focused_border_color, ei_relief_none, clipper);
    }

    // Draw the entry on the offscreen picking surface
    ei_draw_straight_frame(pick_surface, widget->screen_location, 0, *widget->pick_color, ei_relief_none, clipper);

    // Reduce the size of the clipper to the widget's content rect so that children
    // can't be drawn outside the widget's content rect
    ei_rect_t *children_clipper = malloc(sizeof(ei_rect_t));

    // If malloc failed, set the children clipper to the clipper
    if (children_clipper == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for children clipper.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        *children_clipper = *clipper;
    }
    else
    {
        *children_clipper = ei_get_children_clipper(*widget->content_rect, clipper);
    }

    ei_impl_widget_draw_children(widget, surface, pick_surface, children_clipper);

    free(children_clipper);
}

void ei_entry_setdefaultsfunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    entry->widget = *widget;

    entry->widget_appearance.color = ei_default_background_color;
    entry->widget_appearance.border_width = 0;

    entry->text.label = NULL;
    entry->text.font = ei_default_font;
    entry->text.color = ei_font_default_color;
    entry->text.anchor = ei_anc_center;

    entry->text_length = 0;

    entry->previous = NULL;
    entry->next = NULL;

    entry->first_character = NULL;
    entry->last_character = NULL;
    entry->cursor = NULL;
    entry->cursor_visible = false;

    entry->focused = false;

    entry->placeholder_text = NULL;
}

void ei_entry_geomnotifyfunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    int border_width = entry->widget_appearance.border_width;

    // Compute the content rect of the entry (size of the entry without its border and the padding)
    *widget->content_rect = ei_rect_add(widget->screen_location, border_width, border_width, -border_width * 2, -border_width * 2);
    *widget->content_rect = ei_rect_add(widget->screen_location, ei_entry_default_padding, ei_entry_default_padding, -ei_entry_default_padding * 2, -ei_entry_default_padding * 2);
}

ei_size_t ei_entry_get_natural_size(ei_entry_t *entry)
{
    if (&entry->widget == ei_app_root_widget())
    {
        return entry->widget.screen_location.size;
    }

    ei_size_t size = ei_size_zero();

    if (entry->widget_appearance.border_width > 0)
    {
        size.width += entry->widget_appearance.border_width * 2;
        size.height += entry->widget_appearance.border_width * 2;
    }

    return size;
}

void ei_update_requested_char_size(ei_entry_t *entry, int requested_char_size)
{
    ei_surface_t m_surface = hw_text_create_surface("m", entry->text.font, entry->text.color);

    ei_size_t size = hw_surface_get_size(m_surface);

    entry->widget.requested_size.width = size.width * requested_char_size + ei_entry_default_padding * 2;
    entry->widget.requested_size.height = size.height + ei_entry_default_padding * 2;

    entry->widget.screen_location.size = entry->widget.requested_size;
}

void ei_entry_set_text(ei_widget_t widget, ei_const_string_t text)
{
}

ei_const_string_t ei_entry_get_text(ei_widget_t widget)
{
}

void ei_entry_give_focus(ei_widget_t widget)
{
}