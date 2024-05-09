#include "../api/ei_types.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"

void ei_entry_configure(ei_widget_t widget, int *requested_char_size, const ei_color_t *color, int *border_width, ei_font_t *text_font, ei_color_t *text_color)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // If the color is NULL, override the color only if it's the first time this function is called on the widget
    if (color != NULL)
    {
        entry->widget_appearance.color = *color;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->widget_appearance.color = ei_default_background_color;
        }
    }

    if (border_width != NULL && *border_width >= 2)
    {
        entry->widget_appearance.border_width = *border_width;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->widget_appearance.border_width = 2;
        }
    }

    if (text_font != NULL)
    {
        entry->text.font = *text_font;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->text.font = ei_default_font;
        }
    }

    if (text_color != NULL)
    {
        entry->text.color = *text_color;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->text.color = ei_font_default_color;
        }
    }

    if (requested_char_size != NULL)
    {
        ei_update_requested_char_size(entry, *requested_char_size);
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            ei_update_requested_char_size(entry, 10);
        }
    }

    // Save the widget as instantiated so that any values set by the user won't be overriden
    // later on if another call is made with NULL values
    entry->widget.instantiated = true;

    // Update the geometry of the widget in case the size has changed
    if (ei_widget_is_displayed(widget))
    {
        widget->geom_params->manager->runfunc(widget);
    }

    widget->wclass->geomnotifyfunc(widget);
}