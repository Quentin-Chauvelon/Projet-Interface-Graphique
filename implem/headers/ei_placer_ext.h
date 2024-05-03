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

/**
 * \brief	A function that runs the geometry computation for this widget.
 * 		    Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_placer_runfunc(ei_widget_t widget);

#endif