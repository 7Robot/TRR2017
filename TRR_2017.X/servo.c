#include <xc.h>
#include <timer.h>
#include "servo.h"

volatile uint16_t duty_cycle_servo;

void init_servo()
{
    //On configure la pin en sortie
    TRIS_SERVO = 0;
    
    //On configure le timer 2
        //T2 actif, gate off, prescaler 1/8, mode 16 bits, source int
        //F = 5MHz, T = 0.2µs
    T2CON = 0x8010;
    //OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_8 & T2_SOURCE_INT, 500);
    
    //On configure la valeur max du timer = periode
        //500 on a donc une période de 0.1ms
    PR2 = 500;
    
    //On configure l'interruption sur T2
    _T2IE = 1;
    _T2IP = 2;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    static uint16_t pwm = 0;
    
    
    if(pwm < duty_cycle_servo)
    {
        PIN_SERVO = 1;
    }
    else
    {
        PIN_SERVO = 0;
    }
    
    if(pwm > 200)
        pwm = 0;
    
    pwm++;
    
    _T2IF = 0;
}

void set_angle_servo(float angle)
{
    if(angle > 45)
        angle = 45;
    
    if(angle < -45)
        angle = -45;
    
    duty_cycle_servo = (uint16_t)(angle/4.5 + 15);
}