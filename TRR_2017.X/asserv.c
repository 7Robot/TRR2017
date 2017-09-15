#include <xc.h>
#include <math.h>
#include "ultrason.h"
#include "asserv.h"
#include "odometrie.h"

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
    static float erreur_p_m = 0;
    static float erreur_p_a = 0;
    static float erreur_i_a = 0;
    static float erreur_i_m = 0;
    static float erreur_d_a = 0;
    static float erreur_d_m = 0;
    float angle_servo;
    float consigne_a; // angle que doit prendre le chassis de la voiture
    int32_t dist_av;
    int32_t dist_ar;
    
    dist_av = get_distance_US_AV();
    dist_ar = get_distance_US_AR();
    
    /* step 1
     on determine l'angle que doit prendre la voiture avec le bord de la piste pour atteindre la consigne
     */
    erreur_p_m = erreur_m;
    erreur_m = (dist_av + dist_ar)/2 - MOYENNE;//get_delta() - MOYENNE;//
    erreur_i_m += erreur_m;
    erreur_d_m = erreur_m - erreur_p_m;

    if(erreur_i_m > 50)
        erreur_i_m = 50;
    if(erreur_i_m < -50)
        erreur_i_m = -50;
 
    // pid pour determiner l'angle que doit prendre la voiture
    consigne_a = 0.1 * erreur_m;
    
    if(consigne_a > 100)  
        consigne_a = 100; 
    if(consigne_a < -100) 
        consigne_a = -100;   
    
    /* step 2
     on determine l'angle du servo pour obtenir l'angle consigne
     */
    
    erreur_p_a = erreur_d_a;
    erreur_a = (dist_av - dist_ar) - consigne_a; //get_angle()*(180/3.1415);
    erreur_i_a += erreur_a;
    erreur_d_a = erreur_a - erreur_p_a;
    
    if(erreur_i_a > 50)
        erreur_i_a = 50;
    if(erreur_i_a < -50)
        erreur_i_a = -50;

    angle_servo = 0.732 * erreur_a + 0.66 * erreur_i_a + 0.167 * erreur_d_a;

    //angle = 0.732 * erreur_a + 0.66 * erreur_i_a + 0.167 * erreur_d_a;
            /*0.45 * erreur_m + 1.5 * erreur_a + 
            0.06 * erreur_i_a + 0.018 * erreur_i_m + 
            9.38 * erreur_d_a + 2.81 * erreur_d_m; */
    
    /*if(dist_av < 250)
        angle = 10 * (dist_av - 250);
    if(dist_av > 320)
        angle = 15 * (dist_av - 320);
    */
    
    if(angle_servo > 30)
        angle_servo = 30;
    if(angle_servo < -30)
        angle_servo = -30;
    
    set_angle_servo(angle_servo);
    
    _T3IF = 0;
}