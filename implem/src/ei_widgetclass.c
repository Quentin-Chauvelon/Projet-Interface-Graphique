#include <stdlib.h>

#include "../api/ei_widgetclass.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_radiobutton.h"

// Keep a pointer to the first widget class registered
// Other widget classes are linked using the next pointer
ei_widgetclass_t *first_widgetclass = NULL;

size_t ei_widget_struct_size()
{
    return sizeof(ei_impl_widget_t);
}

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    // If a widget class already exists with the same name, return
    if (ei_widgetclass_from_name(widgetclass->name) != NULL)
    {
        printf("\033[0;31mError: A widget class called \"%s\" already exists.\n\t at %s (%s:%d)\033[0m\n", widgetclass->name, __func__, __FILE__, __LINE__);
        return;
    }

    // If no widget class have been registered yet, set the first one
    if (first_widgetclass == NULL)
    {
        first_widgetclass = widgetclass;
    }
    // Otherwise, link the new widget class to the previous one
    else
    {
        ei_widgetclass_t *current = first_widgetclass;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = widgetclass;
    }
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name)
{
    // If no widget class have been registered yet, return NULL
    if (first_widgetclass == NULL)
    {
        return NULL;
    }

    ei_widgetclass_t *current = first_widgetclass;

    // Traverse the linked list to find the widget class with the given name
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void ei_widgetclass_register_all()
{
    ei_widgetclass_t *frame = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (frame == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the frame widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(frame->name, "frame");
    frame->allocfunc = &ei_frame_allocfunc;
    frame->releasefunc = &ei_frame_releasefunc;
    frame->drawfunc = &ei_frame_drawfunc;
    frame->setdefaultsfunc = &ei_frame_setdefaultsfunc;
    frame->geomnotifyfunc = &ei_frame_geomnotifyfunc;
    frame->next = NULL;

    ei_widgetclass_t *button = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (button == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the button widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(button->name, "button");
    button->allocfunc = &ei_button_allocfunc;
    button->releasefunc = &ei_button_releasefunc;
    button->drawfunc = &ei_button_drawfunc;
    button->setdefaultsfunc = &ei_button_setdefaultsfunc;
    button->geomnotifyfunc = &ei_button_geomnotifyfunc;
    button->next = NULL;

    ei_widgetclass_t *toplevel = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (toplevel == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the toplevel widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(toplevel->name, "toplevel");
    toplevel->allocfunc = &ei_toplevel_allocfunc;
    toplevel->releasefunc = &ei_toplevel_releasefunc;
    toplevel->drawfunc = &ei_toplevel_drawfunc;
    toplevel->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
    toplevel->geomnotifyfunc = &ei_toplevel_geomnotifyfunc;
    toplevel->next = NULL;

    ei_widgetclass_t *entry = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (entry == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the entry widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(entry->name, "entry");
    entry->allocfunc = &ei_entry_allocfunc;
    entry->releasefunc = &ei_entry_releasefunc;
    entry->drawfunc = &ei_entry_drawfunc;
    entry->setdefaultsfunc = &ei_entry_setdefaultsfunc;
    entry->geomnotifyfunc = &ei_entry_geomnotifyfunc;
    entry->next = NULL;

    ei_widgetclass_t* radiobutton=malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (radiobutton==NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the entry widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(radiobutton->name,"radiobutton");
    radiobutton->allocfunc = & ei_radiobutton_allocfunc_group ;
    radiobutton->releasefunc= & ei_radiobutton_releasefunc_group;
    radiobutton->drawfunc= & ei_radiobutton_drawfunc_group;
    radiobutton->setdefaultsfunc= & ei_radiobutton_setdefaultsfunc_group;
    radiobutton->geomnotifyfunc= & ei_radiobutton_geomnotifyfunc;
    radiobutton->next=NULL;


    ei_widgetclass_register(frame);
    ei_widgetclass_register(button);
    ei_widgetclass_register(toplevel);
    ei_widgetclass_register(entry);
    ei_widgetclass_register(radiobutton);
}

void ei_widgetclass_free_all()
{
    ei_widgetclass_t *current = first_widgetclass;
    ei_widgetclass_t *next;

    while (current != NULL)
    {
        next = current->next;

        // Only free the widget classes that were created by the library,
        // it is up to the programmer to free any widget class they
        // would have added
        if (strcmp(current->name, "frame") == 0 ||
            strcmp(current->name, "button") == 0 ||
            strcmp(current->name, "toplevel") == 0 ||
            strcmp(current->name, "entry") == 0||
            strcmp(current->name, "radiobutton")==0 )
        {
            free(current);
        }

        current = next;
    }
}