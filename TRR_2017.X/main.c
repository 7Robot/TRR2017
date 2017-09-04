/*
 * File:   main.c
 * Author: tchamelot
 *
 * Created on 7 juin 2017, 16:25
 */


#include <xc.h>
#include "config.h"
#include "motor.h"
#include "servo.h"
#include "ultrason.h"
#include "asserv.h"



int main(void) {
    
    int16_t us_av;
    int16_t us_ar;
    
    
    config_oscillator();
    init_PWM();
    init_servo();
    init_ultrason();
    init_asserv();
    
    while(1)
    {
        //us_av = get_distance_US_av();
        //us_ar = get_distance_US_ar();
        //PWM_Moteurs_droit((float)(temp / 10));
        //set_angle_servo(0);
        PWM_Moteurs_droit(-40);
    }
    
    return 0;
}
