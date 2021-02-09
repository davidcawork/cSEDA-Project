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

#ifndef LCD_H
#define LCD_H

/*	Headers	*/
#include "../headers.h"
#include "lcddriver.h"

/*	Macros	*/

/*	Functions Declaration */
/* REPRESENTACIÓN LCD */
void representar_temp_hum(int ventilador, int alarma); //Representar valores medidas sensores 
void representar_const_LCD (void);                 //Representar todo lo constante
void representar_viento(void);
extern int update_flag;
extern int update_viento;
extern int irq_cnt;

#endif /* !LCD_H */


