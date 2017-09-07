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
        //50000 on a donc une periode de 10ms
    PR3 = 50000;
    
    //On configure l'interruption sur T3
    _T3IE = 1;
    _T2IP = 3;
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void)
{
    static float erreur_p;
    static float erreur_p_p;
    static float erreur_i = 0;
    static float erreur_d = 0;
    int8_t angle;
    float dist_av;
    float dist_ar;
    
    erreur_p_p = erreur_p;
    
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    
    erreur_p = (dist_av - dist_ar);
    
    //erreur_p = (dist_av+dist_ar)/2 - MOYENNE;
    
    erreur_i += erreur_p;
    
    erreur_d = erreur_p - erreur_p_p ;
    
   // if(dist_av > MOYENNE + E || dist_av < MOYENNE - E )
   //     angle = 0.1 * (dist_av - 300);
   // else
    angle = 10 * erreur_p + 0 * erreur_i + 0* erreur_d; // calcul du PID
    
    if(angle > 35)
        angle = 35;
    if(angle < -35)
        angle = - 35;
    
    set_angle_servo(angle);
    
    _T3IF = 0;
}