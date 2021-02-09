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


/* TIMER3 en modo capture para calcular velocidad del viento a trav�s del anem�metro CAP3.1
Compara valor actual de cuenta en CR1 con el almacenado anteriormente en temp*/
#include "anemometro.h"

void init_TIMER3_anemom(void)
{
	LPC_SC->PCONP|=(1<<23); //Alimentaci�n Timer3
	LPC_PINCON->PINSEL1|=(3<<16); //CAP3.1 por P0.24
	LPC_TIM3->PR = 0;
	LPC_TIM3->MCR=0;    //Match Control Register a 0
	LPC_TIM3->CCR|=(5<<3); //Capture on rising edge e interrupt on CAP3.1 event
	LPC_TIM3->EMR=0;
	LPC_TIM3->TCR=0x01;		//Counter enable
	
	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_SetPriority(TIMER3_IRQn,4);
}

void TIMER3_IRQHandler(void) //CAP3.1
{
	static int N,temp;
	
	LPC_TIM3->IR|=(1<<5);		//Borrar flag
	if (LPC_TIM3->CR1-temp <= 250000)   //Interrumpe con evento pero solo nos interesa subida
		return;
	
	N=LPC_TIM3->CR1-temp;
	veloc_viento=(F_pclk*60)/(2*N); //Dos ciclos por vuelta
	temp=LPC_TIM3->CR1;
	
	viento=1;
}
