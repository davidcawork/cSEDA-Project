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

/* ADC modo BURST para convertir datos del sensor de temperatura LM35 y de humedad HIH4000.
TIMER0 nos permite establecer tiempo de consulta de valores 1s -> F_muestreo_anemom=1 Hz. <cabeceras.h>
Con interrupci�n del TIMER0 por match consultamos velocidad del aire y cnt++
Con cnt=15, es decir, han pasado 15s recuperamos los valores de las medidas del resto de sensores 
ACTUALIZACI�N VELOCIDAD VIENTO = 1s
ACTUALIZACI�N TEMPERATURA Y HUMEDAD = 15s */

#include "sensores.h"
int cnt=0;

void TIMER0_IRQHandler (void) // BURST=1
{
	LPC_TIM0->IR|= (1<<1); 			//Borrar flag de interrup por MR1
	cnt++;
	update_viento=1;
	if (viento)
		viento=0;
	else
		veloc_viento=0;
	
	if(cnt==15){
		temperatura=((LPC_ADC->ADDR0 >>4)&0xFFF) * 3.3/4095 * 100;     // se borra automat. el flag DONE al leer ADCGDR y *100 por 10 mV para pasar a grados
		humedad=((((LPC_ADC->ADDR2 >>4)&0xFFF)*3.3/4095)-0.869)/0.029;  // se borra automat. el flag DONE al leer ADCGDR y ecuaci�n recta

		update_flag=1;  //Actualizar valores de la pantalla
		cnt=0;
	}
}

void init_ADC_sensores(void) 
{
	LPC_SC->PCONP |= (1<<12);  								// Power on
  LPC_SC->PCLKSEL0|= (0x00 << 8); 					// CCLK/4 (Fpclk despu�s del reset) (100 Mhz/4 = 25Mhz) bits [25:24]
	LPC_PINCON->PINSEL1 |= (1<<14)|(1<<18);		// AD0.0-P0.23 y AD0.2-P0.25 - Canales 0 y 2
	LPC_PINCON->PINMODE1 |= (2<<14)|(2<<18); 	// Deshabilitar pull up y down para ambos
	LPC_ADC->ADCR=0;
	LPC_ADC->ADCR |= (1<<0)|(1<<2)|	    			// Canal 0 y canal 2
									 (0xFF<<8)|								// CLKDIV=FF para ir lo m�s lento posible
									 (1<<16)|									// Modo burst cte
									 (1<<21);     						// bit PDN (power bit)
}

void init_TIMER0_sensores(void) //Modo match con MAT0.1 = interrumpe cada 1 seg
{
	LPC_SC->PCONP |= (1<<1);   //Power on TIMER0
	LPC_PINCON->PINSEL3 |= 0x0C000000; 		//P1.29 como MAT0.1 -> LED
	LPC_TIM0->PR = 0x00;
	LPC_TIM0->MCR = 0x0018;   //Reset e interrupcion con MR1
	LPC_TIM0->MR1 =(F_pclk/F_muestreo_anemom)-1;
	LPC_TIM0->EMR = 0x00C2;   //MATn.1 y toggle
	LPC_TIM0->TCR = 0x01;     //Enable counter
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_SetPriority(TIMER0_IRQn,2);
}
