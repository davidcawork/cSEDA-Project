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

#include "headers.h"

/* MAQUINA DE ESTADOS MAIN (while(1)) */
#define IDLE 0			  //ESTADO POR DEFECTO
#define UPDATE_LCD 1	  //ACTUALIZAR PANTALLA CON VALORES
#define ALARMA 2		  //INICIA ALARMA SONORA
#define CONFIG_SENSORES 3 //CONFIGURA ADC Y TIMER0 PARA ADQUISICION SENSORES
#define GRABAR 4		  //GRABA NUEVA AUDIO PARA ALARMA (2s)

/*	VARIABLES CON DATOS MEDIDOS	*/
float umbral_temperatura = 25.0;
float humedad = 50.0;
float temperatura = 24.45;
float veloc_viento = 123.0;
float temperatura2 = 30.0;
float presion = 80;

/* VARIABLES UTILIZADAS POR LOS ESTADOS */
int alarma = 0;				//Flag alarma
int ventilador = 0;			//Flag ventilador
int state = 0;				//ESTADO EN EL QUE NOS ENCONTRAMOS
int update_flag;			//Flag actualizar pantalla (temperatura y humedad) cada 15s
int update_viento;			//Flag actualizar pantalla (solo viento) cada 1s
int irq_cnt;				//Contador entradas ADC_IRQHandler de sensores (pinta a la 2 vez)
int reproducir = 0;			//Flag para reproducir alarma
int fin_reproduccion = 1;	//Flag fin reproducci�n
int sensor = 1;				//Flag para configurar ADC y TIMER0 para sensores (Inicialmente a 1 para configurarlos al empezar)
int pulsado = 0;			//Flag de haber pulsado para grabar
int grabacion_completa = 0; //Flag de grabaci�n completa
int viento = 0;				//Flag indica viento

int main(void)
{
	configure();
	main_loop();
}

/* CONFIGURACI�N INICIAL */
void configure(void)
{
	NVIC_SetPriorityGrouping(4);

	/* ANEMOMETRO */
	init_TIMER3_anemom();

	/* VENTILADOR */
	config_pwm1();
	set_duty_cycle_pwm1(0);

	/* GRABACION Y REPRODUCCION AUDIO */
	init_Externas();
	init_TIMER1_audio();
	init_DAC_audio();
	NVIC_SetPriority(DMA_IRQn, 0);
	NVIC_EnableIRQ(DMA_IRQn);

	/* LCD */
	lcdInitDisplay();
	fillScreen(BLACK);
	representar_const_LCD();

	/* UART */
	__uart_init();

	/* TCP */
	__init_TCP();
}

/* M�QUINA DE ESTADOS */
void main_loop(void)
{
	while (1)
	{
		timer_poll();
		main_TcpNet();
		switch (state)
		{
		case IDLE: //ESTADO POR DEFECTO
			if (update_flag || update_viento)
			{						//�Hay que actualizar los valores de la pantalla?
				state = UPDATE_LCD; //estado = ACTUALIZAR PANTALLA
				break;
			}

			if (sensor || grabacion_completa)
			{							 //�Hay que reconfigurar el ADC y TIMER0 para adquisici�n de sensores?
				state = CONFIG_SENSORES; //estado = CONFIGURAR ADC Y TIMER0
				sensor = 0;
				break;
			}

			if (alarma)
			{ //�Hay alarma activa?
				if (fin_reproduccion)
				{ //Si hemos terminado la reproducci�n
					fin_reproduccion = 0;
					state = ALARMA; //estado = REPRODUCIR ALARMA
					break;
				}
			}

			if (pulsado)
			{					//�Se ha pulsado el bot�n para grabar nueva alarma?
				state = GRABAR; //estado = GRABAR ALARMA
				pulsado = 0;
				break;
			}

			if (temperatura > umbral_temperatura)
			{ //�Se ha sobrepasado umbral? -> ALARMA+VENTILADOR ON
				ventilador = 1;
				alarma = 1;
				set_duty_cycle_pwm1(70); //Inicia ventilador
				update_flag = 1;		 //Actualiza LCD
				reproducir = 1;
				break;
			}

			if (temperatura < umbral_temperatura && ventilador == 1)
			{ //�Ya no se sobrepasa el umbral? -> ALARMA+VENTILADOR OFF
				ventilador = 0;
				alarma = 0;
				set_duty_cycle_pwm1(0); //Paramos ventilador
				update_flag = 1;		//Actualiza LCD
				break;
			}
			break;

		case UPDATE_LCD: //ACTUALIZAR PANTALLA CON VALORES
			if (update_flag)
			{
				update_flag = 0;
				representar_temp_hum(ventilador, alarma); //Funci�n representar por pantalla temp y humedad
			}
			if (update_viento)
			{
				update_viento = 0;
				representar_viento(); //Funci�n representar por pantalla viento
			}
			state = IDLE; //estado = DEFECTO
			break;

		case ALARMA:			  //INICIA ALARMA SONORA
			init_DMA_DAC(0);	  //Inicia DAC para reproducir alarma
			LPC_TIM1->TCR = 0x01; //Start TIMER1 para inicio conversi�n DAC
			state = IDLE;		  //estado = DEFECTO
			break;

		case CONFIG_SENSORES:		//CONFIGURA ADC Y TIMER0 PARA ADQUISICION SENSORES
			init_TIMER0_sensores(); //TIMER0 para sensores
			init_ADC_sensores();	//ADC para sensores
			state = IDLE;			//estado = DEFECTO
			if (grabacion_completa)
				grabacion_completa = 0;
			break;

		case GRABAR:				   //GRABA NUEVA AUDIO PARA ALARMA (2s)
			init_TIMER0_audio();	   //TIMER0 para audio
			init_ADC_audio();		   //ADC para audio
			NVIC_DisableIRQ(ADC_IRQn); //Deshabilita interrupci�n para DMA
			LPC_TIM0->TCR = 0x01;	   //Start TIMER0 para inicio conversi�n ADC
			init_DMA_ADC(0);		   //Adquisici�n ADC por DMA
			state = IDLE;			   //estado = DEFECTO
			break;
		}
	}
}
