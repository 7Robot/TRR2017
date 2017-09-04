/* 
 * File:   servo.h
 * Author: tchamelot
 *
 * Created on 9 juin 2017, 15:00
 */

#ifndef SERVO_H
#define	SERVO_H

#include <stdint.h>

#define TRIS_SERVO  _TRISC4
#define PIN_SERVO   _RC4

/**
 * Initialise le servomoteur
 * Utilise T2
 */
void init_servo();

/**
 * Permet de choisir l'angle du servomoteur
 * -45 < angle < +45
 */
void set_angle_servo(float angle);

#endif	/* SERVO_H */

