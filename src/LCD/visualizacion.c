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

#include "lcd.h"

char array[30];

void representar_const_LCD (void) //Representa todo lo constante (NOMBRES Y LINEAS)
{
	drawString(10, 10, "TEMPERATURA", WHITE, BLACK, MEDIUM);
	drawString(10, 30, "(grados)", WHITE, BLACK, SMALL);
	
	drawString(130, 10, "HUMEDAD", WHITE, BLACK, MEDIUM);
	drawString(130, 30, "(porcentaje)", WHITE, BLACK, SMALL);
	
	drawString(10, 70, "VELOCIDAD VIENTO", WHITE, BLACK, MEDIUM);
	drawString(10, 90, "(rpm)", WHITE, BLACK, SMALL);

	drawString(10, 140, "TEMP EXT", WHITE, BLACK, MEDIUM);
	drawString(10, 160, "(grados)", WHITE, BLACK, SMALL);
	
	drawString(130, 140, "PRESION", WHITE, BLACK, MEDIUM);
	drawString(130, 160, "(porcentaje)", WHITE, BLACK, SMALL);
	
	drawString(10, 250, "Umbral temperatura:", WHITE, BLACK, SMALL);
	drawString(10, 265, "Ventilador: OFF", WHITE, BLACK, SMALL);
	drawString(10, 280, "Alarma: OFF", WHITE, BLACK, SMALL);

	drawHorizontalLine(0,0,235,WHITE);
	drawHorizontalLine(0,45,235,WHITE);
	drawHorizontalLine(0,75,235,WHITE);
	drawHorizontalLine(0,120,235,WHITE);
	drawHorizontalLine(0,150,235,WHITE);
	drawHorizontalLine(0,195,235,WHITE);
	drawHorizontalLine(0,225,235,WHITE);
}

void representar_temp_hum(int ventilador, int alarma) //Representa las variables -> ventilador ON = '1'; alarma ON = '1'
{
	sprintf(array, "%2.2f", temperatura);
	drawString(10, 55, array, WHITE, BLACK, SMALL);			//Muestra valor temperatura

	sprintf(array, "%2.2f", humedad);
	drawString(130, 55, array, WHITE, BLACK, SMALL);		//Muesta valor humedad
	
	sprintf(array, "%2.2f", temperatura2);
	drawString(10, 190, array, WHITE, BLACK, SMALL);			//Muestra valor temperatura
	
	sprintf(array, "%2.2f", presion);
	drawString(130, 190, array, WHITE, BLACK, SMALL);		//Muesta valor humedad

	sprintf(array, "%2.0f", umbral_temperatura);					//Muestra umbral de temperatura
	drawString(180, 250, array, WHITE, BLACK, SMALL);

	if (ventilador == 1) //ventilador ON = '1'
		drawString(10, 265, "Ventilador:  ON", WHITE, BLACK, SMALL);
	else								 //ventilador OFF = '0'
		drawString(10, 265, "Ventilador: OFF", WHITE, BLACK, SMALL);
		
	if (alarma == 1)		 //alarma ON = '1'
		drawString(10, 280, "Alarma:  ON", WHITE, BLACK, SMALL);
	else								 //alarma OFF = '0'
		drawString(10, 280, "Alarma: OFF", WHITE, BLACK, SMALL);
}

void representar_viento(void)
{
	drawString(10, 115, "borrando", BLACK, BLACK, SMALL);		//Muestra valor velocidad viento
	
	sprintf(array, "%3.2f", veloc_viento);
	drawString(10, 115, array, WHITE, BLACK, SMALL);		//Muestra valor velocidad viento
}

