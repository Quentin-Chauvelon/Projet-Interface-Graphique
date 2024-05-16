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

ei_widget_t ei_radiobutton_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_radiobutton_t));
}

void ei_radiobutton_releasefunc(ei_widget_t widget)
{
    ei_radiobutton_t *radiobutton =(ei_radiobutton_t*) widget;

    if (radiobutton->text.label!=NULL){
        free(radiobutton->text.label!=NULL);
    }

    //free(radiobutton->widget);
    free(radiobutton);
    radiobutton=NULL; //not usefull but also did in the others release functions
}

void ei_radiobutton_setdefaultsfunc(ei_widget_t widget)
{
    ei_radiobutton_t *radiobutton =(ei_radiobutton_t*) widget;

    radiobutton->widget= *widget;
    radiobutton->widget.content_rect= &(widget->screen_location);

    radiobutton->text.anchor=ei_anc_west;
    radiobutton->text.color=ei_font_default_color;
    radiobutton->text.font=ei_default_font;
    radiobutton->text.label= NULL;

    radiobutton->actif=true;

    radiobutton->next_sibling=NULL;

    radiobutton->previous_sibling=NULL;

    radiobutton->callback=NULL;
}

void ei_add_radiobutton( ei_radiobutton_t *Group,ei_radiobutton_t* radiobuttons)
{
    ei_radiobutton_t* run= Group;
    while (run->next_sibling!=NULL){run=run->next_sibling;}
    run->next_sibling=radiobuttons;
    radiobuttons->previous_sibling=run;
    radiobuttons->widget.screen_location=Group->widget.screen_location;
}


void ei_radiobutton_drawfunc_group(
                ei_widget_t widget, 
                ei_surface_t surface, 
                ei_surface_t pick_surface,
                ei_rect_t *clipper)
{
    true ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_radiobutton_t *radiobutton =(ei_radiobutton_t*) widget;

    //Searching of the first radiobutton of the group
    while(radiobutton->previous_sibling!=NULL){radiobutton=radiobutton->previous_sibling;}

    // Draw the visible frame
    int  border_width=5;
    ei_draw_straight_frame(surface,widget->screen_location, border_width,(ei_color_t){0x44, 0x88, 0x88, 0xff}, ei_relief_none, clipper);

    //Draw the radiobuttons
    ei_image_properties_t image ;
    image.data=NULL; //to avoid to draw a image

    int number=ei_nb_radiobutton_group(radiobutton)+1;

    ei_size_t rec_size=clipper->size;
    ei_point_t top = clipper->top_left;
    ei_point_t end =(ei_point_t) {clipper->top_left.x ,clipper->top_left.y +rec_size.height/ number};

    //We have to add the border_width
    top.x+=border_width;
    top.y+=border_width;
    end.x+=border_width;
    end.y+=border_width;

    //Draw a title
    //to do

    while (radiobutton!=NULL)
    {
        //Draw of a button before the radiobutton to know if he is selected ot not
        ei_color_t color= radiobutton->actif ==false ? (ei_color_t){0,0xff,0,0xff} : (ei_color_t){0xff,0x0,0x0,0xff};
        int length_rect=( end.y-top.y)/2;
        ei_point_t center= (ei_point_t) {top.x+length_rect/4, top.y+length_rect/2 };
        ei_rect_t *cir_rect= &(ei_rect_t) { center, {length_rect,length_rect}};//We multiply to put the button on the center

        ei_button_t* button=(ei_button_t*) radiobutton->widget.children_head;
        ei_button_setdefaultsfunc(button);

        ei_button_configure((ei_widget_t) button, NULL,
					    	&color,
						&(int){2}, NULL,
						&(ei_relief_t){ei_relief_raised},
						NULL, NULL,NULL, NULL, NULL, NULL, NULL,
						& radiobutton->callback, NULL);
        button->widget.screen_location=*cir_rect;

        ei_button_drawfunc((ei_widget_t) button, surface, pick_surface, cir_rect);
    
    
        //Draw the frame appearance (text)
        ei_size_t size=(ei_size_t) {rec_size.width - length_rect ,rec_size.height/number};
        ei_rect_t* rb_clipper= &(ei_rect_t){ {top.x+length_rect, top.y}, size};
        widget->content_rect= rb_clipper;
        ei_draw_frame_appearance(surface, widget, radiobutton->text,(ei_image_properties_t) image, rb_clipper);
    
    
        //Advance to the next radiobuttton
        radiobutton=radiobutton->next_sibling;
        top.y+=rec_size.height / number;
        end.y+=rec_size.height /number;
    }

    // Draw the frame on the offscreen picking surface
    ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, 0, *widget->pick_color, ei_relief_none, clipper);

}

int ei_nb_radiobutton_group(ei_radiobutton_t* radiobutton)
{
    int compt= (int) 0 ;

    ei_radiobutton_t* bis = (ei_radiobutton_t*) radiobutton;
    while ( bis !=NULL)
    {
        compt++;
        bis=bis->previous_sibling;
    }

    bis = (ei_radiobutton_t*) radiobutton->next_sibling;
    while (bis!=NULL)
    {
        compt++;
        bis=bis->next_sibling;
    }

    return compt;
}

bool ei_check_change_radiobutton_state( ei_radiobutton_t* radiobutton, bool actif )
{
    ei_radiobutton_t* part1 = radiobutton->previous_sibling;
    ei_radiobutton_t* part2 = radiobutton->next_sibling;
    bool end=false;//To avoid to advance uselessly in the chaine
    
    //Checking and changing
    if (actif)
    {
        //Change of the current one state
        radiobutton->actif=actif;

        //We consider there are only one activated radiobutton

        //First part of the chain
        while (part1!=NULL)
        {
            if (part1->actif)
            {
                part1->actif=false;
                end=true;
                break;
            }
            part1=part1->previous_sibling;
        }

        //Second part of the chain
        while ( part2!=NULL)
        {
            if (part2->actif)
            {
                part2->actif=false;
                break;
            }
            part2=part2->next_sibling;
        }
    }

    else{
        //Only for checking if everything is okay

        ei_radiobutton_t* part1 = radiobutton->previous_sibling;
        ei_radiobutton_t* part2 = radiobutton->next_sibling;
        int num=0; //number of activated radiobutton

        //First part of the chain
        while (part1!=NULL)
        {
            if (part1->actif){
                if(num!=1){return false;} 
                num++;
            }
            part1=part1->previous_sibling;
        }

        // Second part of the chain 
        while (part2!=NULL)
        {
            if (part2->actif){
                if(num!=1){return false;}
                num++;
            }
            part2=part2->next_sibling;
        }
    }

    return true;

}


void ei_radiobutton_geomnotifyfunc(ei_widget_t widget)
{
    
}
