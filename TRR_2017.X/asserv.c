#include <xc.h>
#include "ultrason.h"
#include "asserv.h"


void init_asserv()
{
    //On configure le timer 3
        //T3 actif, gate off, prescaler 1/8, mode 16 bits, source int
        //F = 5MHz, T = 0,2µs
    T3CON = 0x8030;
    
    //On configure la valeur max du timer = periode
        //5000 on a donc une periode de 50ms
    PR3 = 10000;
    
    //On configure l'interruption sur T3
    _T3IE = 1;
    _T2IP = 3;
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void)
{
    static float erreur_m;
    static float erreur_a;
    static float erreur_i_a = 0;
    static float erreur_i_m = 0;
    float angle;
    int32_t dist_av;
    int32_t dist_ar;
    
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    
    erreur_m = (dist_av + dist_ar)/2 - 300;
    erreur_a = (dist_av - dist_ar);
    
    erreur_i_m += erreur_m;
    erreur_i_a += erreur_a;
    
    if(erreur_i_a > 50)
        erreur_i_a = 50;
    if(erreur_i_a < -50)
        erreur_i_a = -50;
 
    angle = 0.05 * erreur_m + 0.01 * erreur_a + 0.01 * erreur_i_a;
    
    /*if(dist_av < 250)
        angle = 10 * (dist_av - 250);
    if(dist_av > 320)
        angle = 15 * (dist_av - 320);
    */
    
    if(angle > 30)
        angle = 30;
    if(angle < -30)
        angle = -30;
    
    set_angle_servo(angle);
    
    _T3IF = 0;
}