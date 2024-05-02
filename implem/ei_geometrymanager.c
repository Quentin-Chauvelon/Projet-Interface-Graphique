#include <stdio.h>
#include "../api/ei_types.h"
#include "../api/ei_widget.h"
#include <math.h>
#define M_PI       3.14159265358979323846

ei_point_t* arc(ei_point_t* center,
        int radius,
        double  B_angle,
        double E_angle)
{
    /*l' angle est pris par rapport a la l'horizontale.
    Notre but est de trouver maintenat le nombre de point et leur valeurs a mettre
    dans le tableau */
    int number=100; //nombre de points
    
    ei_point_t **tab = malloc( number*sizeof(ei_point_t*));
    ei_point_t **p_tab= tab ;
    //int ** tab_angle = malloc(number*sizeof(int));
    double arc_radius = (E_angle-B_angle) / number;

    int i=0;
    while( i< number ){
        int x = center->x + radius*cos( (double) (B_angle+i*arc_radius));
        int y = center->y + radius*sin( (double) (B_angle+i*arc_radius));
        ei_point_t *point = malloc(sizeof(ei_point_t));
        *point=(ei_point_t) {x,y};
        *p_tab = point;
        p_tab ++;
        i++;
    }
    return tab;
}
