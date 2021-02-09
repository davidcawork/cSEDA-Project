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

/* Grabaci�n y reproducci�n de la se�al de alarma a trav�s de DMA.
ADC + TIMER0 (inicio de conversi�n). Transferencia DMA de perif�rico a memoria SIN modo Linked (4 transferencias de 4000 datos)
DAC + TIMER1 (REQ nuevo bloque de datos al DMA). Transferencia DMA de memoria a perif�rico SIN modo Linked (4 transferencias de 4000 datos) 
Transferencias de 8 bits que interrumpen con el fin de transferencia de cada bloque (hasta llegar al cuarto)
Inicialmente y sin nada grabado, se reproduce como alarma las muestras almacenadas en <audio_muestras.h>
Si hemos grabado una se�al de alarma, se indica con flag existe_grabacion=1 y se reproduce lo almacenado en muestras_grab */

#include "audio.h"
#include "audio_muestras.h"

uint8_t muestras_grab[16000];     //Array de muestras convertidas ADC
int existe_grabacion=0;						//No hay grabacion inicial
int pulsado_ADC=0;						    //No hay grabacion inicial

void EINT0_IRQHandler()
{
	LPC_SC->EXTINT=(1<<0); 					//Borrar flag Externa 0
	pulsado=1;
	pulsado_ADC=1;
}

void init_ADC_audio(void)
{
	LPC_SC->PCONP|=(1<<12); 				//Alimentaci�n ADC
	LPC_PINCON->PINSEL3|=(3<<28); 	//P1.30 como AD0.4 = MICRO
	LPC_PINCON->PINMODE3|=(2<<28);	//Sin pullup-down
	LPC_SC->PCLKSEL0&=~(3<<24); 	  //CCLK/4 (Fpclk despu�s del reset) (100 Mhz/4 = 25Mhz)
	LPC_ADC->ADCR= 0;
	LPC_ADC->ADCR=(1<<4)| 					//CANAL 4
								(1<<8)|						//CLKDIV=1 -> FclkADC=12.5MHz -> Tconv=5.2us
								(1<<21)|    	 	  //PDN=1
								(4<<24);					//Inicio de conversi�n con MAT0.1-> TIMER0
	
	LPC_ADC->ADINTEN=(1<<4);	      //Interrupci�n para fin conversi�n canal 4
}

void init_DMA_ADC(int i)		//4 tx de 4000 datos
{
	LPC_SC->PCONP|=(1<<29); 				//Alimentaci�n DMA
	LPC_GPDMA->DMACConfig|=(1<<0); 	//DMA enabled
	
	LPC_GPDMACH0->DMACCDestAddr=(uint32_t)&muestras_grab[i*4000];  //Dir dest SI var�a (4 transf de 4000)
	LPC_GPDMACH0->DMACCSrcAddr=(uint32_t)&LPC_ADC->ADDR4+1;   //8 bits de mayor peso de ADDR4
	
	LPC_GPDMACH0->DMACCLLI=0; 										 //Modo linked = 0
	
	//Periferico de origen: ADC y Transfer type - Desde periferico a memoria (P2M)	
	                              // | S.BURST=1  | D.Burst=1   | S.Width     | D.Width     |Incr. Dest    | TC interrupt enable.
	LPC_GPDMACH0->DMACCControl= 4000 | (0 << 12 ) | ( 0 << 15 ) | ( 0 << 18 ) | ( 0 << 21 ) | ( 1U << 27 ) | (1U << 31); 	 		
  							          //ORIGEN: ADC |Dest=mem. |  P2M	     | Inicio
	LPC_GPDMACH0->DMACCConfig= ( 4 << 1 ) | (0 << 6) | (2 << 11) | (1 << 15) | (1 << 14)| 1;	
}

void init_DMA_DAC(int i)		//4 tx de 4000 datos
{
	LPC_SC->PCONP|=(1<<29); 											//Alimentaci�n DMA
	LPC_GPDMA->DMACConfig|=(1<<0); 								//DMA enabled
	
	LPC_GPDMACH0->DMACCDestAddr=(uint32_t)&LPC_DAC->DACR+1;   //( bits de DACR
	if (existe_grabacion == 0) 	//NO HAY -> Reproduccion muestras (.h)
		LPC_GPDMACH0->DMACCSrcAddr=(uint32_t)&muestras[i*4000];  //Dir dest SI var�a (4 transf de 4000)
	if (existe_grabacion == 1)	//SI HAY -> Reproduccion muestras_grab
		LPC_GPDMACH0->DMACCSrcAddr=(uint32_t)&muestras_grab[i*4000];  //Dir dest SI var�a (4 transf de 4000)
	
	LPC_GPDMACH0->DMACCLLI=0; 										 //Modo linked = 0
	
	//Periferico de origen: ADC y Transfer type - Desde periferico a memoria (P2M)	
	                              // | S.BURST=1  | D.Burst=1   | S.Width     | D.Width     |Incr. Dest   | TC interrupt enable.
	LPC_GPDMACH0->DMACCControl= 4000 | (0 << 12 ) | ( 0 << 15 ) | ( 0 << 18 ) | ( 0 << 21 ) | ( 1U << 26 ) | (1U << 31); 	 		
  							          //ORIGEN: MAT1.0 |Dest=mem. |  P2M	     | Inicio
	LPC_GPDMACH0->DMACCConfig= ( 10 << 1 ) | (0 << 6) | (2 << 11) | (1 << 15) | (1 << 14)| 1;	
	LPC_SC->DMAREQSEL|=(1<<2); 	//MAT1.0 para REQ
}

void DMA_IRQHandler(void)
{
	static int i=1;

	if (LPC_GPDMA->DMACIntTCStat & 1){      //Terminal Count Interrupt Request? (Bloque de tama�o TAM_BLOCK_DMA  transferido !!!) 
		LPC_GPDMA->DMACIntTCClear |= 1; 		  //Borramos interrupcion;
		if (grabacion_completa ==0 && pulsado_ADC == 1){	    //ADQUISICION ADC
			init_DMA_ADC(i++);	                //Repetimos la adquisici�n
			if (i==4)                           //Hemos terminado la transferencia todos los bloques
			{			
				i=1; 															//Lo dejamos preparado para la siguiente vez
				LPC_TIM0->TCR=0x02; 							//Timer0 Reset/Stop para parar conversi�n (hasta volver a pulsar bot�n)
				grabacion_completa=1;							//Flag de grabaci�n completa
				existe_grabacion=1;								//Hemos grabado audio
				pulsado_ADC=0;
			}
		}
		else if (reproducir == 1){	//REPRODUCCION DAC
			init_DMA_DAC(i++);	                //Repetimos la adquisici�n
			if (i==4)                           //Hemos terminado la transferencia todos los bloques
			{			
				i=1; 															//Lo dejamos preparado para la siguiente vez
				grabacion_completa=0;							//Flag de grabaci�n completa
				LPC_TIM1->TCR = 0x02;  						//Timer1 startt para empezar con DAC
				pulsado=0;
				fin_reproduccion=1;
			}
		}		
	}
}	

void init_DAC_audio(void)
{
	LPC_PINCON->PINSEL1|= (2<<20); 	 	       //DAC output = P0.26 (AOUT)
	LPC_PINCON->PINMODE1|= (2<<20); 	       //Deshabilita pullup/pulldown
	LPC_DAC->DACCTRL=0;								 
}

void init_TIMER0_audio(void)  //MAT0.1 para conversion ADC
{
	LPC_SC->PCONP|=(1<<1); 					         //Alimentaci�n TIMER0
	LPC_TIM0->PR=0;									         //PR=0
	LPC_TIM0->MCR=0x10;							         //Reset on MR1
	LPC_TIM0->MR1=(F_pclk/F_muestreo/2)-1;   //Cada 2 match se hace flanco de subida para inicio conversi�n
	LPC_TIM0->EMR=0x00C2; 					         //MAT0.1 do TOGGLE
	LPC_TIM0->TCR=0x02;							         //RESET hasta EINT0 lo inicie	
	
	NVIC_DisableIRQ(TIMER0_IRQn);
}

void init_TIMER1_audio(void)	//MAT1.0 para conversion DAC
{
	  LPC_SC->PCONP|=(1<<2);					      	//Alimentacion
    LPC_TIM1->PR = 0x00;     	 				      //PR=0
    LPC_TIM1->MCR = 0x03;							      //Reset e interrupc en MR0  
    LPC_TIM1->MR0 = (F_pclk/F_muestreo)-1;  //El timer 1 interrumpe cada 125us=8kHz
    LPC_TIM1->TCR = 0x02;							      //Timer STOP y RESET-> Inicia despu�s de ADC terminal   
}

void init_Externas(void)      //EINT0 por flanco de bajada
{
	// Configuraci�n interrupciones externas
	LPC_PINCON->PINSEL4|=(0x01<<20); 		      // P2.10 es entrada interrup. EXT 0 (pulsador ISP en MIni-DK2)
	LPC_SC->EXTMODE|=(1<<0);   					      // Por Flanco,	
	LPC_SC->EXTPOLAR=0;				  				      // de bajada
	NVIC_SetPriority(EINT0_IRQn,4);			      // Menor prioritaria!!!
	NVIC_EnableIRQ(EINT0_IRQn); 		
}
