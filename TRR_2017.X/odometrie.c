#include <math.h>
#include <stdint.h>
#include "odometrie.h"

#define DISTANCE_INTER_US 250

/**
 * mesure l'angle entre la voiture et la piste en se basant sur la différence 
 * des capteurs
 * @return 
 */
double get_angle(){
    int32_t dist_av;
    int32_t dist_ar;
    double ratio;
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    ratio = ((double)DISTANCE_INTER_US) / ((double)(dist_av-dist_ar));
    return ratio - ((ratio*ratio*ratio)/3.0); // DL de l'atan //atan(ratio);
}

/**
 * mesure la distance entre le milieu de la voiture et le bord de la piste
 * contrairement a la moyenne cette mesure est faite perpendiculairement au bord
 * de la piste et non perpendiculairement a la trajectoire de la voiture (pour 
 * la moyenne)
 * @return 
 */
double get_delta(){
    int32_t dist_av;
    int32_t dist_ar;
    double alpha = get_angle();
    double dist_m;
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    dist_m = ( dist_av + dist_ar ) / 2.0;
    return dist_m * (alpha - ((alpha*alpha*alpha)/6.0)); // DL du sin //sin(alpha);//
}
