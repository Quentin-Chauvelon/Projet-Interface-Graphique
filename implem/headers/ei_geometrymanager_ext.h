/**
 * @file    ei_geometrymanager_ext.h
 *
 * @brief   Extends the \ref ei_geometrymanager.h file and add functions to handle geometry.
 *
 * @see     \ref ei_geometrymanager.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_GEOMETRYMANAGER_EXT_H
#define EI_GEOMETRYMANAGER_EXT_H

/**
 * @brief   Registers all geometry managers (placer, ...).
 */
void ei_geometrymanager_register_all();

/**
 * @brief   Frees all geometry managers (placer, ...).
 */
void ei_geometrymanager_free_all();

/**
 * @brief   Recomputes the geometry of all the descendants of the given widget.
 *          This implements a tree preorder traversal algorithm.
 *
 * @param   widget  The widget.
 */
void ei_recompute_geometry_of_all_descendants(ei_widget_t *widget);

#endif