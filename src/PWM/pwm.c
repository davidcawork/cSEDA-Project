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

/* Ventilador de 5V controlado por PWM1.2 por P1.20
Si la temperatura sobrepasa el umbral, el ventilador se activa con un ciclo de trabajo del 50% (se activa)
Si la temperatura vuelve a estar por debajo del umbral, establecemos un ciclo de trabajo del 0% (lo paramos) */
#include "pwm.h"

void config_pwm1(void)
  {
		LPC_PINCON->PINSEL3|=(2<<8); 			// P1.20 como salida para PWM1.2
		LPC_SC->PCONP|=(1<<6);       			// Encendemos el perif�rico = ALIMENTACI�N PWM
		LPC_PWM1->MR0=F_pclk*Tpwm-1;			// Establecemos periodo para se�al PWM -> 375 kHz
		LPC_PWM1->PCR|=(1<<10); 					// Habilitamos PWM1.2
		LPC_PWM1->MCR|=(1<<1);						// Reset en MR0 (Sin interrupci�n)
		LPC_PWM1->TCR|=(1<<0)|(1<<3);			// Counter ENABLE y PWM Mode
	}

void set_duty_cycle_pwm1 (float ciclo)
{
	LPC_PWM1->MR2=LPC_PWM1->MR0*ciclo/100;
	LPC_PWM1->LER|=(1<<2)|(1<<0);
}
