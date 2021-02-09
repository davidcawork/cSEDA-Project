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

#ifndef PWM_H
#define PWM_H

/*	Headers	*/
#include <LPC17xx.H>
#include "../headers.h"
/*	Macros	*/

/*	Functions Declaration */
/* VENTILADOR */
void config_pwm1(void);                 //Config PWM1 (VENTILADOR)
void set_duty_cycle_pwm1(float ciclo);	//Establecer ciclo trabajo PWM1
extern int ventilador;

#endif /* !PWM_H */

