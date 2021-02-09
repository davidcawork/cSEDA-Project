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

#ifndef AUDIO_H
#define AUDIO_H

/*	Headers	*/
#include "../headers.h"

/*	Macros	*/

/*	Functions Declaration */
/* AUDIO */
void init_ADC_audio(void);     //Config ADC (AUDIO)
void init_DAC_audio(void);	   //Config DAC (AUDIO)
void init_DMA_ADC(int i);		   //Config DMA-ADC (AUDIO) = 4 tx de 4000 datos
void init_DMA_DAC(int i);		   //Config DMA-DAC (AUDIO) = 4 tx de 4000 datos
void init_TIMER0_audio(void);  //Config TIMER0-ADC (AUDIO) = MAT0.1 sin salida, sin interrupción, empieza parado e indica inicio conversión ADC
void init_TIMER1_audio(void);	 //Config TIMER1-DAC (AUDIO) = MAT1.0
void init_Externas(void);      //Config EINT0 por flanco de bajada (AUDIO) = pulsación botón
extern int reproducir;
extern int alarma;
extern int fin_reproduccion;
extern int pulsado;
extern int grabacion_completa;

#endif /* !AUDIO_H */

