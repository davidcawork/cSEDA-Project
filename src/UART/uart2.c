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

/*	Var gloables de la UART2 */ 
int uart2_state = UART_TX_INIT;			// Id de estados de la UART2
char uart2_tx_buffer[150];		// Buffer de trasnmisi�n de 150 caracteres
char uart2_rx_buffer[30];		// Buffer de recepci�n de 30 caracteres
char *uart2_ptr_rx;				// puntero de recepci�n
char *uart2_ptr_tx;				// puntero de transmisi�n
char uart2_rx_completa;			// Flag de recepci�n de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char uart2_tx_completa;			// Flag de transmisi�n de cadena que se activa al transmitir el caracter null (fin de cadena)



/*
 * UART2 - Manejador de interrupci�n
 */
void UART2_IRQHandler(void) {
	
    switch(LPC_UART2->IIR&0x0E) {
	
	case 0x04:								 	 						/* RBR, Receiver Buffer Ready */
		*uart2_ptr_rx=LPC_UART2->RBR; 	   			 					/* lee el dato recibido y lo almacena */
	    if (*uart2_ptr_rx++ ==13){
			*uart2_ptr_rx=0;											/* A�adimos el caracter null para tratar los datos recibidos como una cadena*/ 
			uart2_rx_completa = 1;					 					/* rx completa */
			uart2_getCommand(uart2_rx_buffer);
			uart2_ptr_rx=uart2_rx_buffer;								/* puntero al inicio del buffer para nueva recepci�n */
	    }	
		break;
	
    
   case 0x02:															/* THRE, Transmit Holding Register empty */
		if (*uart2_ptr_tx!=0) 											/* carga un nuevo dato para ser transmitido */
			LPC_UART2->THR=*uart2_ptr_tx++;			
		else uart2_tx_completa=1;
		break;

    }
}


/*
 * Funci�n para procesar los comandos desde UART2
 *
 */
void uart2_getCommand(char * buffer_RX){

	switch(uart2_state) {
		case UART_TX_INIT:
			strcpy(uart2_tx_buffer, CMD_INIT);
			uart2_state = UART_TX_CMD;
			break;
		case UART_TX_CMD:
			memset(uart2_tx_buffer, 0, sizeof uart2_tx_buffer);
			if(!strcmp(buffer_RX, CMD1)){
				sprintf(uart2_tx_buffer, CMD1_FRMT, temperatura, 176, humedad, veloc_viento, temperatura2, 176,presion);
			}
			else if(!strcmp(buffer_RX, CMD2)){
				sprintf(uart2_tx_buffer, CMD2_FRMT, umbral_temperatura, 176);
				
			}
			else if(!strcmp(buffer_RX, CMD3)){
				strcpy(uart2_tx_buffer,CMD3_FRMT);
				uart2_state = UART_RX_UMBRAL;
			}
			else if(!strcmp(buffer_RX, CMD4)){
				strcpy(uart2_tx_buffer, CMD4_FRMT);
				
			}
			else {
				sprintf(uart2_tx_buffer, CMD_ERR);
			}
			break;
		case UART_RX_UMBRAL:
			sscanf(buffer_RX,"%f",&umbral_temperatura);
			strcpy(uart2_tx_buffer, RX_UMBRAL);
			uart2_state = UART_TX_CMD;
			break;
	}
	tx_cadena_UART2(uart2_tx_buffer);

}

/*
 * Funci�n para enviar una cadena de texto desde UART2
 *
 */
void tx_cadena_UART2(char *cadena){
	uart2_ptr_tx=cadena;
	uart2_tx_completa=0;
	LPC_UART2->THR=*uart2_ptr_tx++;	 
}

/*
 * Funci�n para establecer una tasa de baudios en especifico en la UART2
 *
 */
static int uart2_set_baudrate(unsigned int baudrate) {
    int errorStatus = -1; //< Failure
    unsigned int uClk =SystemCoreClock/4;
    unsigned int calcBaudrate = 0;
    unsigned int temp = 0;
    unsigned int mulFracDiv, dividerAddFracDiv;
    unsigned int divider = 0;
    unsigned int mulFracDivOptimal = 1;
    unsigned int dividerAddOptimal = 0;
    unsigned int dividerOptimal = 0;
    unsigned int relativeError = 0;
    unsigned int relativeOptimalError = 100000;

    uClk = uClk >> 4; /* div by 16 */

    for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++) {
        for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++) {
            temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv);

            divider = temp / baudrate;
            if ((temp % baudrate) > (baudrate / 2))
                divider++;

            if (divider > 2 && divider < 65536) {
                calcBaudrate = temp / divider;

                if (calcBaudrate <= baudrate) {
                    relativeError = baudrate - calcBaudrate;
                } else {
                    relativeError = calcBaudrate - baudrate;
                }

                if (relativeError < relativeOptimalError) {
                    mulFracDivOptimal = mulFracDiv;
                    dividerAddOptimal = dividerAddFracDiv;
                    dividerOptimal = divider;
                    relativeOptimalError = relativeError;
                    if (relativeError == 0)
                        break;
                }
            }
        }

        if (relativeError == 0)
            break;
    }

    if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR) / 100)) {

        LPC_UART2->LCR |= DLAB_ENABLE; 	// importante poner a 1
        LPC_UART2->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);
        LPC_UART2->DLL = (unsigned char) dividerOptimal;
        LPC_UART2->LCR &= ~DLAB_ENABLE;	// importante poner a 0
        LPC_UART2->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);
        errorStatus = 0; //< Success
    }

    return errorStatus;
}
 		

/*
 * Funci�n para iniciar la UART2
 *
 */
void uart2_init(int baudrate) {
	LPC_SC->PCONP |= (1 << 24);
    LPC_PINCON->PINSEL0 |= (1<<20)|(1<<22);
	uart2_ptr_rx=uart2_rx_buffer;	                		// inicializa el puntero de recepci�n al comienzo del buffer
	
    LPC_UART2->LCR &= ~STOP_1_BIT & ~PARITY_NONE; 			// Set 8N1 mode (8 bits/dato, sin pariad, y 1 bit de stop)
    LPC_UART2->LCR |= CHAR_8_BIT;

    uart2_set_baudrate(baudrate);							// Set the baud rate
    
     
    LPC_UART2->IER = THRE_IRQ_ENABLE|RBR_IRQ_ENABLE;		// Enable UART TX and RX interrupt (for LPC17xx UART)   
    NVIC_EnableIRQ(UART2_IRQn);								// Enable the UART interrupt (for Cortex-CM3 NVIC)

    
}

