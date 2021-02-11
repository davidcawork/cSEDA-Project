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

/*	Var gloables de la UART0 */ 
int  uart0_state = UART_TX_INIT;			// Id de estados de la UART0
char uart0_tx_buffer[150] ;		// Buffer de trasnmisi�n de 150 caracteres
char uart0_rx_buffer[30];		// Buffer de recepci�n de 30 caracteres
char *uart0_ptr_rx;				// Puntero de recepci�n
char *uart0_ptr_tx;				// Puntero de transmisi�n
char uart0_rx_completa;			// Flag de recepci�n de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char uart0_tx_completa;			// Flag de transmisi�n de cadena que se activa al transmitir el caracter null (fin de cadena)

/*
 * UART0 - Manejador de interrupci�n
 */
void UART0_IRQHandler(void) {
	
    switch(LPC_UART0->IIR&0x0E) {
	
	case 0x04:								 	 		/* RBR, Receiver Buffer Ready */
		*uart0_ptr_rx=LPC_UART0->RBR; 	   			 	/* lee el dato recibido y lo almacena */
	    if (*uart0_ptr_rx++ ==13){
			*uart0_ptr_rx=0;							/* A�adimos el caracter null para tratar los datos recibidos como una cadena*/ 
			uart0_rx_completa = 1;					 	/* rx completa */
			uart0_getCommand(uart0_rx_buffer);
			uart0_ptr_rx=uart0_rx_buffer;				/* puntero al inicio del buffer para nueva recepci�n */
	    }	
		break;
	
    
   case 0x02:													/* THRE, Transmit Holding Register empty */
		if (*uart0_ptr_tx!=0) LPC_UART0->THR=*uart0_ptr_tx++;	/* carga un nuevo dato para ser transmitido */
		else uart0_tx_completa=1;
		break;

    }
}

/*
 * Funci�n para procesar los comandos desde UART0
 *
 */
void uart0_getCommand(char * buffer_RX){

	switch(uart0_state) {
		case UART_TX_INIT:
			strcpy(uart0_tx_buffer, CMD_INIT);
			uart0_state=UART_TX_CMD;
			break;
		case UART_TX_CMD:
			memset(uart0_tx_buffer, 0, sizeof uart0_tx_buffer);
			if(!strcmp(buffer_RX, CMD1)){
				sprintf(uart0_tx_buffer, CMD1_FRMT, temperatura, 176, humedad, veloc_viento, temperatura2, 176,presion);
			}
			else if(!strcmp(buffer_RX, CMD2)){
				sprintf(uart0_tx_buffer, CMD2_FRMT,umbral_temperatura, 176);
				
			}
			else if(!strcmp(buffer_RX, CMD3)){
				strcpy(uart0_tx_buffer,CMD3_FRMT);
				uart0_state = UART_RX_UMBRAL;
			}
			else if(!strcmp(buffer_RX, CMD4)){
				strcpy(uart0_tx_buffer, CMD4_FRMT);
				
			}
			else {
				sprintf(uart0_tx_buffer,CMD_ERR);
			}
			break;
		case UART_RX_UMBRAL:
			sscanf(buffer_RX,"%f",&umbral_temperatura);
			strcpy(uart0_tx_buffer,RX_UMBRAL);
			uart0_state = UART_TX_CMD;
			break;
	}
	tx_cadena_UART0(uart0_tx_buffer);

}


/*
 * Funci�n para enviar una cadena de texto desde UART0
 *
 */
void tx_cadena_UART0(char *cadena){
	uart0_ptr_tx=cadena;
	uart0_tx_completa=0;
	LPC_UART0->THR=*uart0_ptr_tx++;	 
}							 	 	


/*
 * Funci�n para establecer una tasa de baudios en especifico en la UART0
 *
 */
static int uart0_set_baudrate(unsigned int baudrate) {
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

        LPC_UART0->LCR |= DLAB_ENABLE; 	// importante poner a 1
        LPC_UART0->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);
        LPC_UART0->DLL = (unsigned char) dividerOptimal;
        LPC_UART0->LCR &= ~DLAB_ENABLE;	// importante poner a 0

        LPC_UART0->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);

        errorStatus = 0; //< Success
    }

    return errorStatus;
}
 	
/*
 * Funci�n para iniciar la UART0
 *
 */
void uart0_init(int baudrate) {
    
    LPC_PINCON->PINSEL0|=(1<<4)|(1<<6);					
	uart0_ptr_rx=uart0_rx_buffer;	                 	// Inicializa el puntero de recepci�n al comienzo del buffer
    LPC_UART0->LCR &= ~STOP_1_BIT & ~PARITY_NONE; 		// Set 8N1 mode (8 bits/dato, sin pariad, y 1 bit de stop)
    LPC_UART0->LCR |= CHAR_8_BIT;

    uart0_set_baudrate(baudrate);						// Establecemos el baudrate
    
     
    LPC_UART0->IER = THRE_IRQ_ENABLE|RBR_IRQ_ENABLE;	// Enable UART TX and RX interrupt (for LPC17xx UART)   
    NVIC_EnableIRQ(UART0_IRQn);							// Enable the UART interrupt (for Cortex-CM3 NVIC)
    
}
