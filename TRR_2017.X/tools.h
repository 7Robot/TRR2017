/* 
 * File:   tools.h
 * Author: tchamelot
 *
 * Created on 7 juin 2017, 17:16
 */

#ifndef TOOLS_H
#define	TOOLS_H

#define PI 3.14159265359

/* Fonctions génériques pouvant servir à plusieurs endroits */

// fonction min pour float
float fmin(float x, float y);

// Renvoyer une valeur comprise entre inf et sup
float limit_float(float valeur, float inf, float sup);
long int limit_int(long int valeur, long int inf, long int sup);

// angle principal
float principal_angle(float angle);

#endif	/* TOOLS_H */

