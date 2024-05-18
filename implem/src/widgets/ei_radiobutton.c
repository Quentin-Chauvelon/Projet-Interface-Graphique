#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_radiobutton.h"

ei_widget_t ei_radiobutton_allocfunc_group()
{
    return ei_widget_allocfunc(sizeof(ei_radiobutton_group_t));
}

ei_radiobutton_t* ei_radiobutton_allocfun()
{
    ei_radiobutton_t* radiobutton = (ei_radiobutton_t* ) malloc(sizeof(ei_radiobutton_t));

    // If malloc failed, exit since the program will crash if the user tries to use the widget
    if (radiobutton == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the radiobutton.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    memset(radiobutton, 0, sizeof(ei_radiobutton_t));

    return radiobutton;
}


void ei_radiobutton_releasefunc_group(ei_widget_t widget)
{
    ei_radiobutton_group_t * group =(ei_radiobutton_group_t*) widget;
    ei_radiobutton_t* radiobutton = group->radiobutton;

    while (radiobutton!=NULL)
    {
        if (radiobutton->text.label!=NULL){
            free(radiobutton->text.label);}
        if (radiobutton->button!=NULL){
            free(radiobutton->button);
        }
        radiobutton=radiobutton->next_sibling;
    }
    
    free(group);
}


void ei_radiobutton_setdefaultsfunc_group(ei_widget_t widget)
{
    ei_radiobutton_group_t *group =(ei_radiobutton_group_t*) widget;

    group->radiobutton=NULL;

    group->relief=ei_relief_none;

    group->window.border_width=5;
    group->window.color=(ei_color_t){0x88, 0x88, 0x88, 0xff};

    group->widget=*widget;

    group->widget.content_rect= &(widget->screen_location);
}

ei_radiobutton_t* ei_radiobutton_setdefaultsfunc( ei_radiobutton_t* radiobutton)
{
    radiobutton= ei_radiobutton_allocfun();
    radiobutton->button=NULL;

    radiobutton->text.anchor=ei_anc_west;
    radiobutton->text.color=ei_font_default_color;
    radiobutton->text.font=ei_default_font;
    radiobutton->text.label= NULL;

    radiobutton->selected = true;

    radiobutton->next_sibling=NULL;

    radiobutton->previous_sibling=NULL;

    radiobutton->callback=NULL;

    return radiobutton;
}


void ei_add_radiobutton( ei_widget_t widget,ei_radiobutton_t* to_add_radiobutton)
{
    ei_radiobutton_group_t* Group=(ei_radiobutton_group_t*) widget;
    ei_radiobutton_t *rb;
    //If radiobutton is NULL it means it is the first radiobutton to add  
    if (Group->radiobutton==NULL){
        //to_add_radiobutton= ei_radiobutton_setdefaultsfunc(rb);
        Group->radiobutton=to_add_radiobutton;
        return;
    }

    ei_radiobutton_t* radiobutton=(ei_radiobutton_t*) Group->radiobutton;

    while (radiobutton->next_sibling!=NULL){
        radiobutton=(ei_radiobutton_t*) radiobutton->next_sibling;
        //printf("Ici de add\n");
        }

    radiobutton->next_sibling=to_add_radiobutton;
    to_add_radiobutton->previous_sibling=radiobutton;
    to_add_radiobutton->next_sibling=NULL;
}


void ei_radiobutton_drawfunc_group(
                ei_widget_t widget, 
                ei_surface_t surface, 
                ei_surface_t pick_surface,
                ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_radiobutton_group_t *group =(ei_radiobutton_group_t*) widget;
    ei_radiobutton_t* radiobutton = group->radiobutton; //First radiobutton of the group

    // Draw the visible frame
    ei_draw_straight_frame(surface, group->widget.screen_location, group->window.border_width,group->window.color,group->relief, clipper);

    //Draw the radiobuttons
    ei_image_properties_t image ;
    image.data=NULL; //to avoid to draw a image

    int number=ei_nb_radiobutton_group(group)+1;

    ei_size_t rec_size=clipper->size;
    ei_point_t top = clipper->top_left;
    ei_point_t end =(ei_point_t) {clipper->top_left.x ,clipper->top_left.y +rec_size.height/ number};

    //We have to add the border_width
    top.x+=group->window.border_width;
    top.y+=group->window.border_width;
    end.x+=group->window.border_width;
    end.y+=group->window.border_width;

    //Draw a title
    //to do

    while (radiobutton!=NULL)
    {
       //printf("ici de draw");
        //Draw of a button before the radiobutton to know if he is selected ot not
       ei_color_t color = radiobutton->selected == false ? (ei_color_t){0x55, 0x55, 0x55, 0xff} : (ei_color_t){0xff, 0x0, 0x0, 0xff};
       int length_rect = (end.y - top.y) / 2;
       ei_point_t center = (ei_point_t){top.x + length_rect / 4, top.y + length_rect / 2};
       ei_rect_t *cir_rect = &(ei_rect_t){center, {length_rect, length_rect}}; //

       radiobutton->button = (ei_button_t *)ei_widget_create("button", group, NULL, NULL);
       ;
       ei_button_t *button = (ei_button_t *)radiobutton->button;
       ei_button_setdefaultsfunc(button);

       ei_button_configure((ei_widget_t)button, NULL,
                           &color,
                           &(int){2}, NULL,
                           &(ei_relief_t){ei_relief_raised},
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                           &radiobutton->callback, NULL);
       button->widget.screen_location = *cir_rect;

       ei_button_drawfunc((ei_widget_t)button, surface, pick_surface, cir_rect);

       // Draw the frame appearance (text)
       ei_size_t size = (ei_size_t){rec_size.width - length_rect, rec_size.height / number};
       ei_rect_t *rb_clipper = &(ei_rect_t){{top.x + length_rect, top.y}, size};
       widget->content_rect = rb_clipper;
       ei_draw_frame_appearance(surface, widget, radiobutton->text, (ei_image_properties_t)image, rb_clipper);

       // Advance to the next radiobuttton
       radiobutton = radiobutton->next_sibling;
       top.y += rec_size.height / number;
       end.y += rec_size.height / number;
    }

    // Draw the frame on the offscreen picking surface
    ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, 0, *widget->pick_color, ei_relief_none, clipper);

}

int ei_nb_radiobutton_group(ei_radiobutton_group_t * group)
{
    int compt= (int) 0 ;
    //printf("ici de nb\n");
    ei_radiobutton_t* run = (ei_radiobutton_t*) group->radiobutton;
    while ( run !=NULL)
    {
        //printf("ici de nb\n");
        compt++;
        run=run->next_sibling;
    }
    return compt;
}

bool ei_check_change_radiobutton_state(ei_radiobutton_t *radiobutton, bool selected)
{
    ei_radiobutton_t* part1 = radiobutton->previous_sibling;
    ei_radiobutton_t* part2 = radiobutton->next_sibling;
    bool end=false;//To avoid to advance uselessly in the chaine
    
    //Checking and changing
    if (selected)
    {
        //Change of the current one state
        radiobutton->selected = selected;

        //We consider there are only one activated radiobutton

        //First part of the chain
        while (part1!=NULL)
        {
            if (part1->selected)
            {
                part1->selected = false;
                end=true;
                break;
            }
            part1=part1->previous_sibling;
        }

        //Second part of the chain
        while (!end && part2!=NULL)
        {
            if (part2->selected)
            {
                part2->selected = false;
                break;
            }
            part2=part2->next_sibling;
        }
    }

    return true;

}


void ei_radiobutton_geomnotifyfunc(ei_widget_t widget)
{
    
}
