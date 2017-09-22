#include <xc.h>
#include <stdint.h>
#include <timer.h>
#include "ultrason.h"

volatile etat_US etat = SEND_RDY;

volatile uint16_t debut_av;
volatile uint16_t fin_av;
volatile uint16_t debut_ar;
volatile uint16_t fin_ar;

volatile uint16_t distance_av;
volatile uint16_t distance_ar;

void init_ultrason()
{
    //On désactive les interruptions sur changement
    IE_US_AR = 0;
    IE_US_AV = 0;
    
    //On met les PIN en input par précaution
    TRIS_US_AR = 1;
    TRIS_US_AV = 1;
    
    //On configure le timer 4 pour gérer les capteurs
        //T4 actif, gate off, prescaler 1/8, mode 16 bits, source int
        //F = 5 MHz, T = 0.2µs
    T4CON = 0x8010;
    
    //On configure la valeur max du timer = periode
        //65535 on à donc un max de 13.107 ms ce qui correspond à un overshoot
    PR4 = 0xFFFF;
    
    //On configure les interruptions pout T4
        //On autorise les interruption sur T4
    _T4IE = 1;
        //On mets la priorité de l'interruption sur T4
    _T4IP = 0x2;
    
    //On configure les interruptions notification de changement
        //On met la priorité de l'interruption sur notification de changement
    _CNIP = 6;
        //On clear une éventuelle interruption
    _CNIF = 0;
        //On autorise les interruptions sur notification de changement
    _CNIE = 1;
}

void __attribute__((interrupt, auto_psv)) _T4Interrupt(void)
{    
    uint32_t temp;
    static uint8_t cpt_restart;
    
    switch(etat)
    {
    //On peut envoyer une impulsion
    case SEND_RDY:
        //On met les pins en output et on les met à 1
        TRIS_US_AV = 0;
        TRIS_US_AR = 0;
        CAPT_US_AV = 1;
        CAPT_US_AR = 1;
        
        //On prépare l'attente de 40µs
        PR4 = 200; 
        
        etat = SENDING;
        
        break;
    //L'impulsion a été envoyée
    case SENDING:
        //On met les pins à 0 puis en input
        CAPT_US_AR = 0;
        CAPT_US_AV = 0;
        TRIS_US_AR = 1;
        TRIS_US_AV = 1;
        
        //On reset le timer
        PR4 = 0xFFFF;
        //TMR4 = 0;        
        
        //On autorise les interruptions sur notification de changement pour les capteurs
        _CNIF = 0;
        IE_US_AR = 1;
        IE_US_AV = 1;
        
        etat = WAITING;
        
        break;
    //On à attendu 13.107 ms. On regarde les resultats
    case WAITING:
        //On interdit les interruptions sur notification de changement pour les capteurs
        IE_US_AR = 0;
        IE_US_AV = 0;
        
        //On calcul la durée de chaque impulsion
            /**
             * à base de vitesse du son (/2 pour l'aller-retour)  340.29 m/s
             *=> 1 coup = 34 us
             *pour avoir distance en mm, il faut diviser par 29.39
             *donc multiplication par 1115 puis division par 32768 (2^15)
             *passage obligé en 32 bits
             */
        if(fin_av)
        {
            temp = fin_av - debut_av;
            temp = (temp * 1115) >> 15;
        }
        else
        {
            temp = 2000;
        }
        
        
        if (temp > 30){
            distance_av = temp;//moy_us(val_av, temp);
        }
        
        if(fin_ar)
        {
            temp = fin_ar - debut_ar;
            temp = (temp * 1115) >> 15;
        }
        else
        {
            temp = 2000;
        }
        
        if (temp > 30){
            distance_ar = temp; //moy_us(val_ar, temp);
        }
        //On remet les mesures de temps à 0;
        debut_av = 0;
        fin_av = 0;
        debut_ar = 0;
        fin_ar = 0;
        
        etat = RESTART;
        //Avec une periode a 30710, il faut compter 6 fois pour avoir 50ms pour un cycle
        //Si t'es maso tu refera les calculs 
        PR4 = 30710;
        cpt_restart = 0;        
        
        break; 
        
    case RESTART:
        if(cpt_restart < 5)
            cpt_restart++;
        else
        {
            etat = SEND_RDY;
            PR4 = 1;
        }
            break;
        
    }
    
    //On baisse le flag d'interruption
    _T4IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{    
    if(etat == WAITING)
    {   
        //Test capteur gauche 
        //debut_g et fin_g sont remis à 0 dans l'interruption sur T4 etat WAITING
        if((CAPT_US_AV == 1) && (debut_av == 0))
        {
            debut_av = TMR4;
        }
        
        if((CAPT_US_AV == 0) && (debut_av != 0) && (fin_av == 0))
        {
            fin_av = TMR4;
        }
        
        //Test capteur droit 
        //debut_d et fin_d sont remis à 0 dans l'interruption sur T4 etat WAITING
        if((CAPT_US_AR == 1) && (debut_ar == 0))
        {
            debut_ar = TMR4;
        }
        
        if((CAPT_US_AR == 0) && (debut_ar != 0) && (fin_ar == 0))
        {
            fin_ar = TMR4;
        }
    }
    _CNIF = 0;
}

uint16_t get_distance_US_AR()
{
    return distance_ar;
}

uint16_t get_distance_US_AV()
{
    return distance_av;
}


float moy_us(uint16_t* valeurs, uint32_t new_val)
{
    uint8_t i;
    float moy = 0;
    float coeff[3] = {0.7, 0.25, 0.05};
    
    for(i = 2; i > 0; i--)
        valeurs[i] = valeurs[i -1];
    
    valeurs[0] = new_val;
    
    for(i = 0; i < 3; i++)
        moy += coeff[i] * valeurs[i];
    
    return moy;
}