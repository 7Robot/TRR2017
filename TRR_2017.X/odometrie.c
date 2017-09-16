#include <math.h>
#include <stdint.h>
#include "odometrie.h"

#define DISTANCE_INTER_US 250

/**
 * mesure l'angle (radian) entre la voiture et la piste en se basant sur la différence 
 * des capteurs
 * @return 
 */
float get_angle(){
    int32_t dist_av;
    int32_t dist_ar;
    float ratio;
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    ratio = (((float)(dist_ar-dist_av)/(float)DISTANCE_INTER_US));
    return 90.0-atan(ratio);//ratio - ((ratio*ratio*ratio)/3.0); // DL de l'atan
}

/**
 * mesure la distance entre le milieu de la voiture et le bord de la piste
 * contrairement a la moyenne cette mesure est faite perpendiculairement au bord
 * de la piste et non perpendiculairement a la trajectoire de la voiture (pour 
 * la moyenne)
 * @return 
 */
float get_delta(){
    int32_t dist_av;
    int32_t dist_ar;
    float result;
    float sin_alpha = fabs(sin(get_angle()));
    float dist_m;
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    dist_m = (float)(((float)( dist_av + dist_ar )) / ((float)2.0));
    result = (float)dist_m * (float)sin_alpha;
    return result;//(alpha - ((alpha*alpha*alpha)/6.0)); // DL du sin //
}
