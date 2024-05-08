/**
 * @file	ei_placer_ext.h
 *
 * @brief 	Definition of a placer geometry manager.
 *          Defines custom functions from \ref ei_geometrymanager.h
 *
 * @see    \ref ei_placer.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_PLACER_EXT_H
#define EI_PLACER_EXT_H

#include "../api/ei_types.h"

typedef struct
{
    ei_impl_geom_param_t geom_param; ///< The common geom param of the geometry managers
    ei_anchor_t anchor;              ///< The anchor where the widget is attached.
    int x;                           ///< The x absolute position
    int y;                           ///< The y absolute position
    int width;                       ///< The width of the widget
    int height;                      ///< The height of the widget
    float rel_x;                     ///< The relative x position
    float rel_y;                     ///< The relative y position
    float rel_width;                 ///< The relative width of the widget
    float rel_height;                ///< The relative height of the widget
} ei_placer_t;

/**
 * \brief	A function that runs the geometry computation for this widget.
 * 		    Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_placer_runfunc(ei_widget_t widget);

/**
 * \brief	A function called when a widget cease to be managed by its geometry manager.
 *		    Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		    only provided to trigger recomputation when the disappearance of a widget has an
 *		    effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the geometry manager.
 */
void ei_placer_releasefunc(ei_widget_t widget);

#endif