/**
 * @file    ei_widget_ext.h
 *
 * @brief   Extends the \ref ei_widget.h file to add new functions.
 *
 * @see     \ref ei_widget.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_WIDGET_EXT_H
#define EI_WIDGET_EXT_H

#include "../api/ei_widget.h"
#include "../api/ei_types.h"
#include "../implem/headers/ei_types_ext.h"

/**
 * \brief	Text properties.
 */
typedef struct ei_text_properties_t
{
    ei_string_t label;  ///< The text of the widget.
    ei_font_t font;     ///< The font of the text.
    ei_color_t color;   ///< The color of the text.
    ei_anchor_t anchor; ///< The anchor of the text.
} ei_text_properties_t;

/**
 * \brief	Image properties.
 */
typedef struct ei_image_properties_t
{
    ei_surface_t data;  ///< The image of the widget.
    ei_rect_t *rect;    ///< The rectangle of the image.
    ei_anchor_t anchor; ///< The anchor of the image.
} ei_image_properties_t;

/**
 * \brief	Fields common to frames, buttons, toplevels, and entries.
 */
typedef struct ei_widget_appearance_t
{
    ei_color_t color; ///< The background color of the widget.
    int border_width; ///< The width of the border of the widget.
} ei_widget_appearance_t;

/**
 * \brief	Properties common to a frame and a button, mainly related to the appearance.
 */
typedef struct ei_frame_appearance_t
{
    ei_relief_t relief;          ///< The relief of the widget.
    ei_text_properties_t text;   ///< The properties of the text.
    ei_image_properties_t image; ///< The properties of the image.
} ei_frame_appearance_t;

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		    an existing widget. This function is called by \ref ei_widget_create.
 *          The main difference is that this function allows for parent to be NULL (used for the root widget).
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		When no more needed, the widget must be released by calling \ref ei_widget_destroy.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can be NULL.
 * @param	user_data	A pointer provided by the programmer for private use. May be NULL.
 * @param	destructor	A pointer to a function to call before destroying a widget structure. May be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
ei_widget_t ei_widget_create_internal(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor);

/**
 * @brief	Returns the widget matching the given pick id
 *          This implements a tree preorder traversal algorithm.
 *
 * @param	widget	The current widget
 * @param	pick_id	The pick id to search
 *
 * @return	The widget matching the given pick id
 */
ei_widget_t ei_get_widget_from_pick_id(ei_widget_t *widget, int pick_id);

/**
 * @brief	A function that allocates a block of memory that is big enough to store the
 *		    attributes of a widget of a class, given by widget_size.
 *		    After allocation, the function *must* initialize the memory to 0.
 *
 * @param	widget_size	The size of the widget to allocate
 *
 * @return	The widget allocated
 */
ei_widget_t ei_widget_allocfunc(size_t widget_size);

/**
 * @brief   Draws the frame appearance (text and image) of the frame and button widgets.
 *
 * @param   surface    Where to draw the rounded frame. The surface must be *locked* by \ref hw_surface_lock.
 * @param   widget     The widget to draw
 * @param   text       The text properties
 * @param   image      The image properties
 * @param   clipper    If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_frame_appearance(ei_surface_t surface, ei_widget_t widget, ei_text_properties_t text, ei_image_properties_t image, const ei_rect_t *clipper);

/**
 * @brief   Returns a clipper that corresponds to the intersection of the content rect
 *          and the given clipper if it is not NULL.
 *
 * @param   content_rect    The content rect of the widget
 * @param   clipper         The clipper. May be NULL.
 *
 * @return  The intersection of the content rect and the clipper if it is not NULL.
 */
ei_rect_t ei_get_children_clipper(ei_rect_t content_rect, const ei_rect_t *clipper);

/**
 * @brief   Calculates the natural size of a widget containing a ei_frame_appearance_t and a border.
 *          It takes into account the text, image and border_width and writes the result to the
 *          given size pointer.
 *
 * @param   frame_appearance    The frame appearance
 * @param   border_width        The border width
 * @param   size                The size pointer to write the result to
 */
void ei_calculate_frame_appearance_natural_size(ei_frame_appearance_t frame_appearance, int border_width, ei_size_t *size);

/**
 * @brief   Adds the given tag to the given widget.
 *
 * @param   widget  The widget to add the tag to
 * @param   tag     The tag to add to the widget
 */
void ei_add_tag_to_widget(ei_widget_t widget, ei_tag_t tag);

/**
 * @brief   Removes the given tag from the given widget.
 *
 * @param   widget  The widget to remove the tag from
 * @param   tag     The tag to remove from the widget
 */
void ei_remove_tag_from_widget(ei_widget_t widget, ei_tag_t tag);

#endif