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


#ifndef CMD_H
#define CMD_H

/*	Macros */
#define UART_TX_INIT 0
#define UART_TX_CMD 1
#define UART_RX_UMBRAL 2

#define CMD_ERR "Comando erroneo\r"
#define CMD_INIT "Bienvenido a la estacion meteorologica de la UAH\r\rIntroduzca un comando: (Escriba HELP para ver los comandos)\r"

#define CMD1 "GET DATA\r"
#define CMD1_FRMT "[+] Temperatura: %g %c C\r[+] Humedad: %g %%\r[+] Viento: %g rpm\r"
#define CMD2 "GET ALARM\r"
#define CMD2_FRMT "[+] Umbral temperatura: %g %c C\r"
#define CMD3 "SET ALARM\r"
#define CMD3_FRMT "Intruduzca el umbral de alarma por temperatura: \r"
#define CMD4 "HELP\r"
#define CMD4_FRMT "[+] HELP: ver los comandos disponibles\r[+] GET DATA: ver los datos de los sensores\r[+] GET ALARM: ver el umbral\r[+] SET ALARM: poner el umbral\r"

#define RX_UMBRAL "Umbral de alarma por temperatura modificado.\n"


#endif /* !CMD_H */
