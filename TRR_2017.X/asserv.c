#include <xc.h>
#include <math.h>
#include "ultrason.h"
#include "asserv.h"
#include "odometrie.h"
#include "motor.h"
void init_asserv()
{
    //On configure le timer 3
        //T3 actif, gate off, prescaler 1/8, mode 16 bits, source int
        //F = 5MHz, T = 0,2µs
    T3CON = 0x8030;
    
    //On configure la valeur max du timer = periode
        //5000 on a donc une periode de 50ms
    PR3 = 50000;
    
    //On configure l'interruption sur T3
    _T3IE = 1;
    _T2IP = 3;
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void)
{
    //test
    int16_t test;
    //err actuelle
    static float erreur_m;
    static float erreur_a;
    
    //err precedente
    static float erreur_p_m = 0;
    static float erreur_p_a = 0;
    
    //erreur integral
    static float erreur_i_a = 0;
    static float erreur_i_m = 0;
    
    //erreur dérivée
    static float erreur_d_a = 0;
    static float erreur_d_m = 0;
    
    //angle 
    float angle_servo;
    float consigne_a;
    
    static uint16_t dist_av;
    static uint16_t dist_ar;
    
    uint16_t temp;
    
    temp = get_distance_US_AV();
    if(temp < 1300)
        dist_av = temp;
    
    
    temp = get_distance_US_AR();
    if(temp < 1300)
        dist_ar = temp;
  
     /*
     * PID en distance = chassi a Xcm de la piste
     */
    
    erreur_p_m = erreur_m;
    erreur_m = get_delta() - MOYENNE;//(dist_av + dist_ar)/2 - MOYENNE;//
    erreur_i_m += erreur_m;
    erreur_d_m = erreur_m - erreur_p_m;

    if(erreur_i_m > 50)
        erreur_i_m = 50;
    if(erreur_i_m < -50)
        erreur_i_m = -50;
    
    consigne_a = -1*(0.07 * erreur_m + 0.04 * erreur_i_m + 0.08 * erreur_d_m);
    
    /*
     * PID en angle = chassi paralelle à la piste
     */
    
    erreur_p_a = erreur_a;
    erreur_a = (float)dist_av - (float)dist_ar - consigne_a;
    erreur_i_a += erreur_a;
    erreur_d_a =  erreur_a + erreur_p_a;
    
    if(erreur_i_a > 50)
        erreur_i_a = 50;
    if(erreur_i_a < -50)
        erreur_i_a = -50;
    
    //kc = 2.75
    
    angle_servo = 1.0 * erreur_a;

    test = angle_servo;
    
    if(angle_servo > 30)
        angle_servo = 30;
    if(angle_servo < -30)
        angle_servo = -30;
    
    
    set_angle_servo(angle_servo);
    PWM_Moteurs_droit(-70 + absolu(angle_servo)*0);
    
    _T3IF = 0;
}

float absolu(float a)
{
    if(a < 0)
        a =-a;
    
    return a;
}