/* 
 * File:   ultrason.h
 * Author: tchamelot
 *
 * Created on 8 juin 2017, 15:12
 */

#ifndef ULTRASON_H
#define	ULTRASON_H

#define CAPT_US_AR   _RC3
#define TRIS_US_AR   _TRISC3
#define IE_US_AR     _CN28IE

#define CAPT_US_AV  _RB7
#define TRIS_US_AV  _TRISB7
#define IE_US_AV    _CN23IE

#define NB_MOY 5

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

uint16_t get_distance_US_AR();
uint16_t get_distance_US_AV();
float moy_us(uint16_t* valeurs, uint32_t new_val);


#endif	/* ULTRASON_H */