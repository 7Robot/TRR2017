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
    
    int16_t temp;
    
    
    config_oscillator();
    init_PWM();
    init_servo();
    init_ultrason();
    init_asserv();
    
    while(1)
    {
        temp = get_distance_US_g();
        PWM_Moteurs_droit((float)(temp / 10));
        //set_angle_servo(0);
    }
    
    return 0;
}
