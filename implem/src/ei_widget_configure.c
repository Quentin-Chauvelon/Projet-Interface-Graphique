#include "../api/ei_types.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_utils.h"
#include "../api/ei_application.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_widget_configure_ext.h"

void ei_widget_appearance_configure(ei_widget_appearance_t *widget_appearance, bool instantiated, const ei_color_t *color, int *border_width, int default_border_width)
{
    // If the color is NULL, override the color only if it's the first time this function is called on the widget
    if (color != NULL)
    {
        widget_appearance->color = *color;
    }
    else
    {
        if (!instantiated)
        {
            widget_appearance->color = ei_default_background_color;
        }
    }

    if (border_width != NULL)
    {
        widget_appearance->border_width = *border_width;
    }
    else
    {
        if (!instantiated)
        {
            widget_appearance->border_width = default_border_width;
        }
    }
}

void ei_text_configure(ei_text_properties_t *text_properties, bool instantiated, ei_string_t *text, ei_font_t *text_font, ei_color_t *text_color, ei_anchor_t *text_anchor)
{
    if (text != NULL)
    {
        if (text_properties->label != NULL)
        {
            free(text_properties->label);
        }

        text_properties->label = malloc(strlen(*text) + 1);

        // If malloc failed, print
        if (text_properties->label == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory for text.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        }
        else
        {
            strcpy(text_properties->label, *text);
        }
    }
    else
    {
        if (!instantiated)
        {
            text_properties->label = NULL;
        }
    }

    if (text_font != NULL)
    {
        text_properties->font = *text_font;
    }
    else
    {
        if (!instantiated)
        {
            text_properties->font = ei_default_font;
        }
    }

    if (text_color != NULL)
    {
        text_properties->color = *text_color;
    }
    else
    {
        if (!instantiated)
        {
            text_properties->color = ei_font_default_color;
        }
    }

    if (text_anchor != NULL)
    {
        text_properties->anchor = *text_anchor;
    }
    else
    {
        if (!instantiated)
        {
            text_properties->anchor = ei_anc_center;
        }
    }
}

void ei_image_configure(ei_image_properties_t *image_properties, bool instantiated, ei_surface_t *img, ei_rect_ptr_t *img_rect, ei_anchor_t *img_anchor)
{
    if (img != NULL)
    {
        if (image_properties->data != NULL)
        {
            hw_surface_free(image_properties->data);
        }

        ei_surface_t img_copy = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), true);

        hw_surface_lock(*img);
        hw_surface_lock(img_copy);

        ei_copy_surface(img_copy, NULL, *img, NULL, true);

        hw_surface_unlock(*img);
        hw_surface_unlock(img_copy);

        image_properties->data = img_copy;
    }
    else
    {
        if (!instantiated)
        {
            image_properties->data = NULL;
        }
    }

    if (img_rect != NULL)
    {
        image_properties->rect = malloc(sizeof(ei_rect_t));
        image_properties->rect->top_left = (*img_rect)->top_left;
        image_properties->rect->size = (*img_rect)->size;
    }
    else
    {
        if (!instantiated)
        {
            image_properties->rect = NULL;
        }
    }

    if (img_anchor != NULL && *img_anchor != ei_anc_none)
    {
        image_properties->anchor = *img_anchor;
    }
    else
    {
        if (!instantiated)
        {
            image_properties->anchor = ei_anc_center;
        }
    }
}

void ei_widget_configure_finalize(ei_widget_t widget, bool *instantiated)
{
    // Save the widget as instantiated so that any values set by the user won't overriden
    // later on if another call is made with NULL values
    *instantiated = true;

    // Update the geometry of the widget in case the size has changed
    if (ei_widget_is_displayed(widget))
    {
        widget->geom_params->manager->runfunc(widget);
    }
    else
    {
        widget->screen_location.size = widget->requested_size;
    }

    widget->wclass->geomnotifyfunc(widget);
}

/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "optional parameters" protocol: if a parameter is "NULL", the state
 *		of the configured frame for that parameter does not change.
 *		When a frame is created, its parameters are set with the default values specified here.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including the widget's borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed within the widget.
 *				Defines both the anchoring point on the parent and on the widget.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
                Defaults to NULL.
 * @param	img_rect	If not NULL, the rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed within the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void ei_frame_configure(ei_widget_t widget,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        ei_string_t *text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_ptr_t *img_rect,
                        ei_anchor_t *img_anchor)
{
    if (text != NULL && img != NULL)
    {
        printf("\033[0;33mWarning: Frame couldn't be configured. One of the parameter 'text' and 'img' should be NULL.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_frame_t *frame = (ei_frame_t *)widget;

    ei_widget_appearance_configure(&frame->widget_appearance, frame->widget.instantiated, color, border_width, 0);

    if (relief != NULL)
    {
        frame->frame_appearance.relief = *relief;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.relief = ei_relief_none;
        }
    }

    ei_text_configure(&frame->frame_appearance.text, frame->widget.instantiated, text, text_font, text_color, text_anchor);

    ei_image_configure(&frame->frame_appearance.image, frame->widget.instantiated, img, img_rect, img_anchor);

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            widget->requested_size = ei_frame_get_natural_size(frame);
        }
    }

    ei_widget_configure_finalize(widget, &frame->widget.instantiated);
}

/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void ei_button_configure(ei_widget_t widget,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         ei_string_t *text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_ptr_t *img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         ei_user_param_t *user_param)
{
    if (text != NULL && img != NULL)
    {
        printf("\033[0;33mWarning: Frame couldn't be configured. One of the parameter 'text' and 'img' should be NULL.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    ei_button_t *button = (ei_button_t *)widget;

    ei_widget_appearance_configure(&button->widget_appearance, button->widget.instantiated, color, border_width, k_default_button_border_width);

    if (corner_radius != NULL)
    {
        button->corner_radius = *corner_radius;
    }
    else
    {
        if (!button->widget.instantiated)
        {
            button->corner_radius = k_default_button_corner_radius;
        }
    }

    if (relief != NULL)
    {
        button->frame_appearance.relief = *relief;
    }
    else
    {
        if (!button->widget.instantiated)
        {
            button->frame_appearance.relief = ei_relief_raised;
        }
    }

    ei_text_configure(&button->frame_appearance.text, button->widget.instantiated, text, text_font, text_color, text_anchor);

    ei_image_configure(&button->frame_appearance.image, button->widget.instantiated, img, img_rect, img_anchor);

    if (callback != NULL)
    {
        button->callback = *callback;
    }
    else
    {
        if (!button->widget.instantiated)
        {
            button->callback = NULL;
        }
    }

    if (user_param != NULL)
    {
        button->user_param = *user_param;
    }
    else
    {
        if (!button->widget.instantiated)
        {
            button->user_param = NULL;
        }
    }

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
    }
    else
    {
        if (!button->widget.instantiated)
        {
            widget->requested_size = ei_button_get_natural_size(button);
        }
    }

    ei_widget_configure_finalize(widget, &button->widget.instantiated);
}

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title displayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to true.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size of its content.
 *				The default minimum size of a toplevel is (160, 120).
 *				If *min_size is NULL, this requires the min_size to be configured to
 *				the default size.
 */

void ei_toplevel_configure(ei_widget_t widget,
                           ei_size_t *requested_size,
                           const ei_color_t *color,
                           int *border_width,
                           ei_string_t *title,
                           bool *closable,
                           ei_axis_set_t *resizable,
                           ei_size_ptr_t *min_size)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    ei_widget_appearance_configure(&toplevel->widget_appearance, toplevel->widget.instantiated, color, border_width, 4);

    if (title != NULL)
    {
        if (toplevel->title != NULL)
        {
            free(toplevel->title);
        }

        toplevel->title = malloc(strlen(*title) + 1);

        toplevel->title = strcpy(toplevel->title, *title);
    }
    else
    {
        if (!toplevel->widget.instantiated)
        {
            toplevel->title = malloc(sizeof(char) * 9);
            strcpy(toplevel->title, "Toplevel");
        }
    }

    if (closable != NULL)
    {
        toplevel->closable = *closable;
    }
    else
    {
        if (!toplevel->widget.instantiated)
        {
            toplevel->closable = true;
        }
    }

    // Instantiate the close button if closable became true
    if (toplevel->closable && toplevel->close_button == NULL)
    {
        toplevel->close_button = ei_toplevel_instantiate_close_button(toplevel);
    }

    if (resizable != NULL)
    {
        toplevel->resizable = *resizable;
    }
    else
    {
        if (!toplevel->widget.instantiated)
        {
            toplevel->resizable = ei_axis_both;
        }
    }

    if (min_size != NULL)
    {
        *toplevel->min_size = **min_size;
    }
    else
    {
        if (toplevel->min_size == NULL)
        {
            *toplevel->min_size = ei_size(160, 120);
        }
    }

    int width = 0;
    int height = 0;
    ei_toplevel_get_min_size(widget, &width, &height);

    // If the user set the min_size too low, set it to the default min_size
    if (toplevel->min_size->width < width)
    {
        toplevel->min_size->width = width;
    }

    if (toplevel->min_size->height < height)
    {
        toplevel->min_size->height = height;
    }

    if (requested_size != NULL || !toplevel->widget.instantiated)
    {
        // Increase the requested size to take into account the decorations (title bar and border)
        ei_size_t toplevel_size = requested_size != NULL
                                      ? *requested_size
                                      : ei_size(320, 240);

        ei_size_t decorations_size = ei_size(2 * toplevel->widget_appearance.border_width, ei_toplevel_get_title_bar_rect(toplevel).size.height + toplevel->widget_appearance.border_width);
        toplevel_size = ei_size_add(toplevel_size, decorations_size);
        widget->requested_size = toplevel_size;

        // Resize the requested size to be at least the min size
        if (widget->requested_size.width < toplevel->min_size->width)
        {
            widget->requested_size.width = toplevel->min_size->width;
        }

        if (widget->requested_size.height < toplevel->min_size->height)
        {
            widget->requested_size.height = toplevel->min_size->height;
        }
    }

    ei_widget_configure_finalize(widget, &toplevel->widget.instantiated);
}
