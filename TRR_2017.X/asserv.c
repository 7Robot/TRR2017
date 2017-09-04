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
    static float erreur_i = 0;
    static float angle;
    
    erreur_p = get_distance_US_d() - get_distance_US_g();
    
    erreur_i += erreur_p;
    
    angle = 0.6*erreur_p;
    
    set_angle_servo(angle);
    
    
    
    _T3IF = 0;
}