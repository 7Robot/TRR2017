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
    
    
    config_oscillator();
    init_PWM();
    init_servo();
    init_ultrason();
    init_asserv();
    
    //set_angle_servo(20);
    while(1)
    {
        //PWM_Moteurs_droit(-40);
        //PWM_Moteurs_droit(-60);
    }
    
    return 0;
}
