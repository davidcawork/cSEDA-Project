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


#ifndef UART_H
#define UART_H

/*	Headers	*/
#include <LPC17xx.h>
#include <stdio.h>
#include <string.h>
#include "command.h"

/*	Macros */
#define UART_ACCEPTED_BAUDRATE_ERROR    3
#define CHAR_8_BIT                      (3 << 0)
#define STOP_1_BIT                      (0 << 2)
#define PARITY_NONE                     (0 << 3)
#define DLAB_ENABLE                     (1 << 7)
#define FIFO_ENABLE                     (1 << 0)
#define RBR_IRQ_ENABLE                  (1 << 0)
#define THRE_IRQ_ENABLE                 (1 << 1)
#define UART_LSR_THRE   				(1 << 5)
#define RDA_INTERRUPT                   (2 << 1)
#define CTI_INTERRUPT                   (6 << 1)

/*	Vars Declaration */
extern float umbral_temperatura;
extern float humedad;
extern float temperatura;
extern float veloc_viento;
extern float temperatura2;
extern float presion;


/*	Functions Declaration */
void __uart_init(void);
void __unittest_UART(void);
void uart0_init(int baudrate);
void uart2_init(int baudrate);
void uart3_init(int baudrate);

void tx_cadena_UART0(char *ptr);
void uart0_getCommand(char *);

void tx_cadena_UART2(char *ptr);
void uart2_getCommand(char *);

void tx_cadena_UART3(char *ptr);
void uart3_getCommand(char *);

#endif /* !UART_H */
