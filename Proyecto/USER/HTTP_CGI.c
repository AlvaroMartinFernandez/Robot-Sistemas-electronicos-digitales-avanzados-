/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_CGI.C
 *      Purpose: HTTP Server CGI Module
 *      Rev.:    V4.22
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <Net_Config.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stdbool.h"
#include "stdlib.h"

/* ---------------------------------------------------------------------------
 * The HTTP server provides a small scripting language.
 *
 * The script language is simple and works as follows. Each script line starts
 * with a command character, either "i", "t", "c", "#" or ".".
 *   "i" - command tells the script interpreter to "include" a file from the
 *         virtual file system and output it to the web browser.
 *   "t" - command should be followed by a line of text that is to be output
 *         to the browser.
 *   "c" - command is used to call one of the C functions from the this file.
 *         It may be followed by the line of text. This text is passed to
 *         'cgi_func()' as a pointer to environment variable.
 *   "#' - command is a comment line and is ignored (the "#" denotes a comment)
 *   "." - denotes the last script line.
 *
 * --------------------------------------------------------------------------*/

// VARIABLES NUESTRO PROGRAMA
extern int8_t VelDer;
extern int8_t VelIzq;
extern int8_t Posserv;
extern int distancia;
extern uint8_t Estado;
extern uint32_t SLuz1,SLuz2,SPromedio;

/* http_demo.c */
extern U16 AD_in (U32 ch);
extern U8  get_button (void);

/* at_System.c */
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

/* Net_Config.c */
extern struct tcp_cfg   tcp_config;
extern struct http_cfg  http_config;
#define tcp_NumSocks    tcp_config.NumSocks
#define tcp_socket      tcp_config.Scb
#define http_EnAuth     http_config.EnAuth
#define http_auth_passw http_config.Passw

extern BOOL LEDrun;
extern void LED_out (U32 val);
extern BOOL LCDupdate;
extern U8   lcd_text[2][16+1];
int auxhttp;
/* Local variables. */
static char const state[][9] = {
  "FREE",
  "CLOSED",
  "LISTEN",
  "SYN_REC",
  "SYN_SENT",
  "FINW1",
  "FINW2",
  "CLOSING",
  "LAST_ACK",
  "TWAIT",
  "CONNECT"};

/* My structure of CGI status U32 variable. This variable is private for */
/* each HTTP Session and is not altered by HTTP Server. It is only set to  */
/* zero when the cgi_func() is called for the first time.                  */
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- cgi_process_var -------------------------------*/

void cgi_process_var (U8 *qs) {
  /* This function is called by HTTP server to process the Querry_String   */
  /* for the CGI Form GET method. It is called on SUBMIT from the browser. */
  /*.The Querry_String.is SPACE terminated.                                */
  U8 *var;
  var = (U8 *)alloc_mem (40);
	
  do {
    /* Loop through all the parameters. */
    qs = http_get_var (qs, var, 40);
    /* Check the returned string, 'qs' now points to the next. */
    if (var[0] != 0) {
      /* Returned string is non 0-length. */
      if (str_scomp (var, "modo=manual") == __TRUE) {
         Estado=3;
      }
      if (str_scomp (var, "modo=automatico") == __TRUE) {
				 Estado=5;
      }
      else if (str_scomp (var, "modo=depuracion") == __TRUE) {
         Estado=7;

      }	
				if(str_scomp(var,"VelDer=")==__TRUE){
					sscanf((const char*)&var[7], "%d", &auxhttp);
					VelDer =auxhttp;				
		}
				else if(str_scomp(var,"VelIzq=")==__TRUE){
					sscanf((const char*)&var[7], "%d", &auxhttp);
					VelIzq =auxhttp;				
		}
				else if(str_scomp(var,"Posserv=")==__TRUE){
					sscanf((const char*)&var[8], "%d", &auxhttp);
					Posserv =auxhttp;				
		}
      }
  }while (qs);				

  free_mem ((OS_FRAME *)var);
}


/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (U8 code, U8 *dat, U16 len) {
  /* This function is called by HTTP server to process the returned Data    */
  /* for the CGI Form POST method. It is called on SUBMIT from the browser. */
  /* Parameters:                                                            */
  /*   code  - callback context code                                        */
  /*           0 = www-url-encoded form data                                */
  /*           1 = filename for file upload (0-terminated string)           */
  /*           2 = file upload raw data                                     */
  /*           3 = end of file upload (file close requested)                */
  /*           4 = any xml encoded POST data (single or last stream)        */
  /*           5 = the same as 4, but with more xml data to follow          */
  /*               Use http_get_content_type() to check the content type    */  
  /*   dat   - pointer to POST received data                                */
  /*   len   - received data length                                         */
	/**************************************************************************/
	
 
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
  /* This function is called by HTTP server script interpreter to make a    */
  /* formated output for 'stdout'. It returns the number of bytes written   */
  /* to the output buffer. Hi-bit of return value (len is or-ed with 0x8000)*/
  /* is a repeat flag for the system script interpreter. If this bit is set */
  /* to 1, the system will call the 'cgi_func()' again for the same script  */
  /* line with parameter 'pcgi' pointing to a 4-byte buffer. This buffer    */
  /* can be used for storing different status variables for this function.  */
  /* It is set to 0 by HTTP Server on first call and is not altered by      */
  /* HTTP server for repeated calls. This function should NEVER write more  */
  /* than 'buflen' bytes to the buffer.                                     */
  /* Parameters:                                                            */
  /*   env    - environment variable string                                 */
  /*   buf    - HTTP transmit buffer                                        */
  /*   buflen - length of this buffer (500-1400 bytes - depends on MSS)     */
  /*   pcgi   - pointer to session local buffer used for repeated loops     */
  /*            This is a U32 variable - size is 4 bytes. Value is:         */
  /*            - on 1st call = 0                                           */
  /*            - 2nd call    = as set by this function on first call       */
  U32 len = 0;

  switch (env[0]) {
    /* Analyze the environment string. It is the script 'c' line starting */
    /* at position 2. What you write to the script file is returned here. */
		  case '1' :
      switch (env[2]) {
     case 'a':
          /* Write the local IP address. The format string is included */
          /* in environment string of the script line.                 */
		 len = sprintf((char *)buf,(const char *)&env[4],(Estado==2)? "Manual":(Estado==3)? "Manual":(Estado==4)? "Automatico":(Estado==5)? "Automatico":(Estado==6)? "Depuracion":(Estado==7)? "Depuracion": "Espera");
          break;
			case 'b':
          /* Write the local IP address. The format string is included */
          /* in environment string of the script line.                 */
          len = sprintf((char *)buf,(const char *)&env[4],VelDer);
          break;
       case 'c':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],VelIzq);
          break;
       case 'd':
          /* Write the local IP address. The format string is included */
          /* in environment string of the script line.                 */
          len = sprintf((char *)buf,(const char *)&env[4],Posserv);
          break;
       case 'f':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],SLuz1);
          break;
			case 'g':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],SLuz2);
          break;
			case 'h':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],SPromedio);
          break;
			case 'i':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],distancia);
          break;
			case 'j':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],(Estado==3)? "checked='checked'":"");
          break;
			case 'k':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],(Estado==5)? "checked='checked'":"");
          break;
			case 'l':
          /* Write local Net mask. */
          len = sprintf((char *)buf,(const char *)&env[4],(Estado==7)? "checked='checked'":"");
          break;
			case 'm':
				len=sprintf((char *)buf,(const char *)&env[4],(Estado==7)?"<form action='/index.cgi' method='get'><input style='text-align: center' name='VelDer' size='3' type='test' value=' '> <input type='submit' value='Actualizar'/></form>":"");
   break;       
			case 'n':
				len=sprintf((char *)buf,(const char *)&env[4],(Estado==7)?"<form action='/index.cgi' method='get'><input style='text-align: center' name='VelIzq' size='3' type='test' value=' '> <input type='submit' value='Actualizar'/></form>":"");
					 break;
			case 'o':
				len=sprintf((char *)buf,(const char *)&env[4],(Estado==7)?"<form action='/index.cgi' method='get'> <input style='text-align: center' name='Posserv' size='3' type='test' value=' '>&ordm;C <input type='submit' value='Actualizar'/></form>":"");
           break;
      default:
           break;
      }
    break;
    default:
       break;

  }
  return ((U16)len);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

