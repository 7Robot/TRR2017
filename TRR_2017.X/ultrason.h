/* 
 * File:   ultrason.h
 * Author: tchamelot
 *
 * Created on 8 juin 2017, 15:12
 */

#ifndef ULTRASON_H
#define	ULTRASON_H

#define CAPT_US_D   _RC3
#define TRIS_US_D   _TRISC3
#define IE_US_D     _CN28IE

#define CAPT_US_G   _RB7
#define TRIS_US_G   _TRISB7
#define IE_US_G     _CN23IE

#include <stdint.h>

typedef enum etat_US etat_US;
enum etat_US{
    SEND_RDY, SENDING, WAITING, RESTART
};


/**
 * Initialise les capteurs US
 * Utilise T4 et les interruptions sur notification de changement
 */
void init_ultrason();

uint16_t get_distance_US_d();
uint16_t get_distance_US_g();

#endif	/* ULTRASON_H */



