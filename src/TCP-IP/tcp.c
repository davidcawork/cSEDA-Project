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
 
#include "tcp.h"


/*
 *	Test unitario del modulo TCP - Servidor HTTP
 *
 */
void __init_TCP(void) {

  init_TcpNet();
   
  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
  
}

/*
 *	Test unitario del modulo TCP - Servidor HTTP
 *
 */
void timer_poll(void) {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
  }
}


/*
 *	Test unitario del modulo TCP - Servidor HTTP
 *
 */
void __unittest_TCP(void){

	__init_TCP();
	
	while (1){
      timer_poll ();
      main_TcpNet ();
	}
}
