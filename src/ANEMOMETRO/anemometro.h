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

#ifndef ANEMOMETRO_H
#define ANEMOMETRO_H

/*	Headers	*/
#include "../headers.h"


/*	Macros	*/

/*	Functions Declaration */
/* ANEMOMETRO */
void init_TIMER3_anemom(void);  //Config TIMER3 modo capture (ANEMOMETRO)
extern float veloc_viento;
extern int viento;

#endif /* !ANEMOMETRO_H */

