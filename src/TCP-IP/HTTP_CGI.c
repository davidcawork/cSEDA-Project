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
#include "tcp.h"


/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- cgi_process_var -------------------------------*/

void cgi_process_var (U8 *qs) {
  U8 *var;
  var = (U8 *)alloc_mem (40);

  do {
    /* Loop through all the parameters. */
    qs = http_get_var (qs, var, 40);
    /* Check the returned string, 'qs' now points to the next. */
    if (var[0] != 0) {
      /* Returned string is non 0-length. */
      if (str_scomp(var, (U8 *)"umbral=") == __TRUE) {
			sscanf((const char *)&var[7],"%f", &umbral_temperatura);
	  }
   }
  }while (qs);
				
  free_mem ((OS_FRAME *)var);
}


/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (U8 code, U8 *dat, U16 len) {
 
  switch (code) {
    case 0:
      /* Url encoded form data received. */
      break;

    default:
      /* Ignore all other codes. */
      return;
  }

 }


/*--------------------------- cgi_func --------------------------------------*/

U16 cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi) {
	
  U32 len = 0;

  switch (env[0]) {
	  
    case 'n' :
          len = sprintf((char *)buf,(const char *)&env[2],MY_IP[0],MY_IP[1], MY_IP[2],MY_IP[3]);
		break;
	case 's':
      switch (env[2]) {
        case '1':
          len = sprintf((char *)buf,(const char *)&env[4],temperatura);
          break;
		
        case '2':
          len = sprintf((char *)buf,(const char *)&env[4],humedad);
          break;
		
        case '3':         
          len = sprintf((char *)buf,(const char *)&env[4], veloc_viento);
          break;
		
        case '4':
          len = sprintf((char *)buf,(const char *)&env[4],temperatura2);
          break;
		
		 case '5':
          len = sprintf((char *)buf,(const char *)&env[4],presion);
          break;
		 
        default:
           break;
      }
      break;
	  
	case 'u' :
		len = sprintf((char *)buf,(const char *)&env[2],umbral_temperatura);
       break;
      
    default:
       break;

  }
  return ((U16)len);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

