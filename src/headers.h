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

#ifndef HEADERS_H
#define HEADERS_H

/*	Headers	*/
#include <LPC17xx.H>
#include <stdio.h>
#include <Math.h>
#include <string.h>
#include "LCD/lcddriver.h"
#include "LCD/lcd.h"
#include "ANEMOMETRO/anemometro.h"
#include "AUDIO/audio.h"
#include "SENSORES/sensores.h"
#include "PWM/pwm.h"
#include "UART/uart.h"
#include "TCP-IP/tcp.h"

/* Macros */
#define F_cpu 100e6         // Defecto Keil (xtal=12Mhz)
#define F_pclk F_cpu / 4    // Defecto despues del reset
#define Tpwm 15e-3          // Perido de la se�al PWM (15ms)
#define F_muestreo_anemom 1 // Cada 1s -> cnt=15 para muestreo temp y humedad
#define F_muestreo 8000     // Audio
#define seg_grabacion 2     // Audio

/* Funciones main */
void main_loop(void);
void configure(void);

#endif /* !HEADERS_H */
