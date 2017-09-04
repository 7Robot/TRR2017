/* 
 * File:   config.h
 * Author: tchamelot
 *
 * Created on 7 juin 2017, 16:32
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h>

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        80000000UL
#define FCY             SYS_FREQ/2
#define FOSC            FCY

void config_oscillator(void);


#endif	/* CONFIG_H */

