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

#include "uart.h"


/*
 *	Wrapper de inicializaci�n de la UART
 *	
 *  UART0: TXD0 p0.2  RXD0 p0.3
 *	UART2: TxD2 p0.10 RxD2 p0.11
 *  UART3: TxD3 p4.28 RxD3 p4.29
 *
 */
void __uart_init() {
	uart0_init(9600);		
	uart2_init(9600);		
	uart3_init(9600);			
}


/*
 *	Test unitario del modulo UART
 *
 */
void __unittest_UART(void){

	__uart_init();
	
	while(1);
}
