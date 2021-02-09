/* SPDX-License-Identifier: MIT License
 *
 *
 * Project developed for the SEDA course of the Master in Telecommunications Engineering
 * of the UAH. Official repository can be found at (github.com/davidcawork/cSEDA-Project)
 * 
 *  Repository created just for purely academic purposes.
 *
 *	Authors: David Carrascal <david.carrascal@uah.es>
 *           Victoria Noci Luna 
 *           Mohamed Malki
 * 
 *	Date:   30 Jan 2021
 */

#ifndef SENSORES_H
#define SENSORES_H

/*	Headers	*/
#include "../headers.h"

/*	Macros	*/

/*	Functions Declaration */
/* TEMPERATURA Y HUMEDAD */
void init_ADC_sensores(void);		  //Config ADC (SENSORES)
void init_TIMER0_sensores(void);	//Config TIMER0 (SENSORES)
extern float temperatura;         
extern float humedad;				      
extern float umbral_temperatura;	

#endif /* !SENSORES_H */
