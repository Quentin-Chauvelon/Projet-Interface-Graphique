#include "../api/ei_types.h"
#include "../api/ei_widget.h"
#include "ei_widget_attributes.c"

ei_widget_t root = NULL;
//a revoir
void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param)
{
    if (tag==NULL) //if it's a unlocated event
    {
        return callback((ei_widget_t) widget, & eventtype , (ei_user_param_t) user_param);
    }


    if (tag!=NULL) //if it's a located event
    {
        //we consider in a first time that the same type of widget are at the same level in the tree
        ei_widget_t current = ei_widget_get_first_child((ei_widget_t) root );
        
        while (current!=NULL)
        {
            if (tag == (ei_tag_t) (ei_widget_get_class(current) -> name ))
            {
                ei_widget_t widget =current;
                
                while (widget !=NULL)
                {
                    callback((ei_widget_t) current , & eventtype , (ei_user_param_t) user_param);
                    widget=ei_widget_get_next_sibling(widget);
                }
            }
            current=ei_widget_get_first_child(current);
        }
    }
}


void		ei_unbind		(ei_eventtype_t		eventtype,
					 ei_widget_t		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param)
{
    //ei_bind( eventtype,widget,tag,callback,user_param);
}