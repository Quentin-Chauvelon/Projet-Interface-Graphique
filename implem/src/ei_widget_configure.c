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
#include "../implem/headers/ei_radiobutton.h"
#include "../implem/src/widgets/ei_radiobutton.c"
#include "../implem/headers/ei_event_ext.h"

ei_frame_t* common_frame_button_configure(ei_widget_t widget,
                                   ei_size_t *requested_size,
                                   const ei_color_t *color,
                                   ei_string_t *text,
                                   ei_font_t *text_font,
                                   ei_color_t *text_color,
                                   ei_anchor_t *text_anchor,
                                   ei_surface_t *img,
                                   ei_rect_ptr_t *img_rect,
                                   ei_anchor_t *img_anchor)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    widget->requested_size = requested_size != NULL ? *requested_size : *ei_frame_get_natural_size(frame);
    widget->screen_location.size = widget->requested_size;
    color != NULL ? ei_frame_set_color(frame, *color) : ei_frame_set_color(frame, ei_default_background_color);
    text != NULL ? ei_frame_set_text_label(frame, *text) : ei_frame_set_text_label(frame, NULL);
    text_font != NULL ? ei_frame_set_text_font(frame, *text_font) : ei_frame_set_text_font(frame, ei_default_font);
    text_color != NULL ? ei_frame_set_text_color(frame, *text_color) : ei_frame_set_text_color(frame, ei_font_default_color);
    text_anchor != NULL ? ei_frame_set_text_anchor(frame, *text_anchor) : ei_frame_set_text_anchor(frame, ei_anc_center);
    img != NULL ? ei_frame_set_image_data(frame, *img) : ei_frame_set_image_data(frame, NULL);
    img_rect != NULL ? ei_frame_set_image_rect(frame, *img_rect) : ei_frame_set_image_rect(frame, NULL);
    img_anchor != NULL ? ei_frame_set_image_anchor(frame, *img_anchor) : ei_frame_set_image_anchor(frame, ei_anc_center);

    return frame;
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

    // If the color is NULL, override the color only if it's the first time this function is called on the widget
    if (color != NULL)
    {
        frame->widget_appearance.color = *color;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->widget_appearance.color = ei_default_background_color;
        }
    }

    if (border_width != NULL)
    {
        frame->widget_appearance.border_width = *border_width;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->widget_appearance.border_width = 0;
        }
    }

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

    if (text != NULL)
    {
        if (frame->frame_appearance.text.label != NULL)
        {
            free(frame->frame_appearance.text.label);
        }

        frame->frame_appearance.text.label = malloc(strlen(*text) + 1);

        // If malloc failed, print
        if (frame->frame_appearance.text.label == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory for text.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        }
        else
        {
            strcpy(frame->frame_appearance.text.label, *text);
        }
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.text.label = NULL;
        }
    }

    if (text_font != NULL)
    {
        frame->frame_appearance.text.font = *text_font;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.text.font = ei_default_font;
        }
    }

    if (text_color != NULL)
    {
        frame->frame_appearance.text.color = *text_color;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.text.color = ei_font_default_color;
        }
    }

    if (text_anchor != NULL)
    {
        frame->frame_appearance.text.anchor = *text_anchor;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.text.anchor = ei_anc_center;
        }
    }

    if (img != NULL)
    {
        if (frame->frame_appearance.image.data != NULL)
        {
            hw_surface_free(frame->frame_appearance.image.data);
        }

        ei_surface_t img_copy = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), true);

        hw_surface_lock(*img);
        hw_surface_lock(img_copy);

        ei_copy_surface(img_copy, NULL, *img, NULL, true);

        hw_surface_unlock(*img);
        hw_surface_unlock(img_copy);

        frame->frame_appearance.image.data = img_copy;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.image.data = NULL;
        }
    }

    if (img_rect != NULL)
    {
        frame->frame_appearance.image.rect = malloc(sizeof(ei_rect_t));
        frame->frame_appearance.image.rect->top_left.x = (*img_rect)->top_left.x;
        frame->frame_appearance.image.rect->top_left.y = (*img_rect)->top_left.y;
        frame->frame_appearance.image.rect->size.width = (*img_rect)->size.width;
        frame->frame_appearance.image.rect->size.height = (*img_rect)->size.height;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.image.rect = NULL;
        }
    }

    if (img_anchor != NULL && *img_anchor != ei_anc_none)
    {
        frame->frame_appearance.image.anchor = *img_anchor;
    }
    else
    {
        if (!frame->widget.instantiated)
        {
            frame->frame_appearance.image.anchor = ei_anc_center;
        }
    }

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

    // Save the widget as instantiated so that any values set by the user won't overriden
    // later on if another call is made with NULL values
    frame->widget.instantiated = true;

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
    ei_button_t *button = (ei_button_t *)widget;

    if (color != NULL)
    {
        button->widget_appearance.color = *color;
    }

    if (border_width != NULL)
    {
        button->widget_appearance.border_width = *border_width;
    }

    if (corner_radius != NULL)
    {
        button->corner_radius = *corner_radius;
    }

    if (button->widget_appearance.border_width != 0)
    {
        if (relief != NULL)
        {
            button->frame_appearance.relief = *relief;
        }
    }

    if (text != NULL)
    {
        button->frame_appearance.text.label = *text;

        if (text_font != NULL)
        {
            button->frame_appearance.text.font = *text_font;
        }

        if (text_color != NULL)
        {
            button->frame_appearance.text.color = *text_color;
        }
        if (text_anchor != NULL)
        {
            button->frame_appearance.text.anchor = *text_anchor;
        }
    }

    if (img != NULL)
    {
        button->frame_appearance.image.data = *img;

        if (img_rect != NULL)
        {
            button->frame_appearance.image.rect = *img_rect;
        }

        if (img_anchor != NULL)
        {
            button->frame_appearance.image.anchor = *img_anchor;
        }
    }

    if (callback != NULL)
    {
        button->callback = *callback;
    }

    if (user_param != NULL)
    {
        button->user_param = *user_param;
    }

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
        widget->screen_location.size = *requested_size;
    }
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

    if (color != NULL)
    {
        toplevel->widget_appearance.color = *color;
    }

    if (border_width != NULL)
    {
        toplevel->widget_appearance.border_width = *border_width;
    }

    if (title != NULL)
    {
        toplevel->title = *title;
    }

    if (closable != NULL)
    {
        toplevel->closable = *closable;
    }

    if (resizable != NULL)
    {
        toplevel->resizable = *resizable;
    }

    if (min_size != NULL)
    {
        toplevel->min_size = *min_size;
    }

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
        widget->screen_location.size = *requested_size;
    }
}


/**
 * @brief                 Configures the attributes of widgets of the class "radiobutton".
 * 
 * @param widget          The widget to configure.
 * 
 * @param text_color      The color of the text. Defaults to \ref ei_font_default_color.
 * 
 * @param requested_size  The size requested for this widget, including the widget's borders.
 * 
 * @param text            The text to write in the radiobutton. Defaults nothing is written.
 * 
 * @param text_font       The font used to display the text. Defaults to \ref ei_default_font.
 * 
 * @param text_anchor     The anchor of the text, i.e. where it is placed within the widget.
 *				Defines both the anchoring point on the parent and on the widget.
 *				Defaults to \ref ei_anc_west.
 * 
 * @param callback        The callback function to call when the user clicks on the button.
 *				         Defaults to NULL (no callback).
 * 
 * @param actif The state of the radiobutton
 * 
*/

void ei_radiobutton_configure(
                        ei_widget_t widget,
                        const ei_color_t *text_color,
                        ei_size_t *requested_size,
                        ei_string_t *text,
                        ei_font_t *text_font,
                        ei_anchor_t *text_anchor,
                        ei_callback_t *callback,
                        bool actif
                        )
{
    ei_radiobutton_t* radiobutton = (ei_radiobutton_t *) widget;

    if (text_color!=NULL)
    {
        radiobutton->text.color=*text_color;
    }

    if (text!=NULL)
    {
        radiobutton->text.label=*text;
    }

    if (text_font!=NULL)
    {
        radiobutton->text.font=text_font;
    }
    

    if (text_anchor!=NULL)
    {
        radiobutton->text.anchor=*text_anchor;
    }

    if (requested_size!=NULL)
    {
        widget->requested_size=*requested_size;
    }

    if (callback!=NULL)
    {
        radiobutton->callback=*callback;
    }
    else
    {
        if (!radiobutton->actif)
        {
            radiobutton->callback=NULL;
        }
    }

    if (actif)
    {
        ei_check_change_radiobutton_state(radiobutton, actif);
    }
}