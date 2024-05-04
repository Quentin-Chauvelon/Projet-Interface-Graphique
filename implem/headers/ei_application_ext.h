/**
 * @file    ei_application_ext.h
 *
 * @brief   Extends the \ref ei_application.h file to add new functions.
 *
 * @see     \ref ei_application.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_APPLICATION_EXT_H
#define EI_APPLICATION_EXT_H

#include "../api/hw_interface.h"

static const bool DEBUG = true;
static const bool TESTING = false;

/**
 * \brief	Returns the offscreen picking surface.
 *
 * @return 			The surface of the offscreen picking surface.
 */
ei_surface_t ei_app_offscreen_picking_surface(void);

#endif