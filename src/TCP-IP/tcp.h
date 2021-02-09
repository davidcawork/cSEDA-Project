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

#ifndef TCP_H
#define TCP_H

/*	Headers	*/
#include <LPC17xx.h>
#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>

/*	Macros */
#define LocM   localm[NETIF_ETH]
#define MY_IP localm[NETIF_ETH].IpAdr
#define tcp_NumSocks    tcp_config.NumSocks
#define tcp_socket      tcp_config.Scb
#define http_EnAuth     http_config.EnAuth
#define http_auth_passw http_config.Passw
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

/*	Vars Declaration */
extern struct tcp_cfg   tcp_config;
extern struct http_cfg  http_config;
extern  LOCALM localm[];
extern float umbral_temperatura;
extern float humedad;
extern float temperatura;
extern float veloc_viento;
extern float temperatura2;
extern float presion;

/*	Functions Declaration */
void __init_TCP(void);
void __unittest_TCP(void);
void timer_poll(void);

#endif /* !TCP_H */
