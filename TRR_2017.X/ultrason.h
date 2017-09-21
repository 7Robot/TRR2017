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

#define CAPT_US_VIR _RC5
#define TRIS_US_VIR _TRISB5
#define IE_US_VIR   _CN26IE


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
uint16_t get_distance_US_VIR();
uint16_t moy_US(uint16_t tab_val[], uint8_t* index, uint8_t valeur);


#endif	/* ULTRASON_H */



