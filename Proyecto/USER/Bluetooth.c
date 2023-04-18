
#include <LPC17xx.H>
#include "uart.h"
#include <stdio.h>
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"





char buffer[30];		// Buffer de recepción de 30 caracteres --> Utilizada en la ISR de recepcion de la uart
char buffer_tx[30];		// Buffer de recepción de 30 caracteres
char *ptr_rx;			// puntero de recepción
char *ptr;			// puntero de recepción
char rx_completa;		// Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;			// puntero de transmisión
char tx_completa;		// Flag de transmisión de cadena que se activa al transmitir el caracter null (fin de cadena)
char fin=0;
long auxbth=0;
int auxmuestras = 0;
int estado_uart =1;
uint8_t orden_preparada = 0;

//variables externas 
extern uint8_t Estado;
extern int8_t VelDer;
extern int8_t VelIzq;
extern int8_t Posserv;
extern bool Giro90;
extern bool Giromenos90;
extern uint32_t SLuz1,SLuz2,SPromedio;
extern int distancia;
extern int bluetooth;
extern int tiempo;
int milisegundos= 0;
bool modobluetooth= false;

/*Funcion encargada de ejecutar la maquina de estados para el envio de mensajes*/
void MaquinaEstadosBluetooth(){
	switch(estado_uart){
		case 1: //envio del mensaje inicial 
			ptr_rx=buffer;
			tx_cadena_UART0("Bienvenido al modo Bluetooth/USB indique su nombre\n\r");
			estado_uart = 2; //una vez que se ha indicado la trama a transmitir cambiamos de estado para que no la vuelva a repetir
		  break;
		
		  case 2: 
			if(tx_completa == 1) //TRANSMISION DE LAS CADENAS(ESTADO SOLO PARA LA PRINCIPAL)
				estado_uart = 3; //en caso de que se haya transmitido toda empieza la recepcion
			
			break;
			
		case 3: //RECEPCION DE LAS CADENAS(ESTADO SOLO PARA LA PRINCIPAL)
			if(rx_completa == 0) //hasta que no se reciba todo nos mantenemos en este estado
				estado_uart = 3;
				
				
			if(rx_completa == 1){ //una vez que se reibe todo procedemos al estado general transmite-envia con ordenes en donde actuamos sobre el HW
				rx_completa = 0; //borramos flag
				estado_uart =4; //estado general
			}
			
			break;
			
		case 4: //PREPARAMOS LOS MENSAJES DE TRANSMISION GENERICOS
				
				if(orden_preparada == 0 ){
					tx_cadena_UART0("Elige modo:\n\r MOVER \n\r VISUALIZAR VALORES \n\r FIN \n\r"); 
					orden_preparada = 1;
				}
						
			
			estado_uart = 5;
			break;
		
		case 5: // TRANSMISION DE MENSAJES GENERICOS
			if(tx_completa == 1 && fin == 0 )
				estado_uart = 6; //una vez transmitido todo pasamos a la recepcion
      else if(tx_completa == 1 && fin ==1)
				estado_uart = 0; //ninguno
			break;
			
		case 6: //RECEPCION MENSAJES GENERICOS
			if(rx_completa == 0){  
				 estado_uart = 6; //se mantiene hasta que se completa 
			}
			else if (rx_completa ==1){
				estado_uart = 4; //volvemos a preparar los mensajes para luego transmitirlos
				orden_preparada = 0;
				rx_completa = 0; //borramos flag
				
				//Segun lo recibido se actua sobre el HW
				if (strcmp (buffer, "MOVER\r") == 0) {
					estado_uart=7; //Realizamos ordenes
					Estado = 2;
					modobluetooth =true;
				}
				else if (strcmp (buffer, "VISUALIZAR VALORES\r") == 0)  
				 estado_uart =9; //Elegimos valores a visualizar
				else if (strcmp (buffer, "FIN\r") == 0){
					fin = 1;
					tx_cadena_UART0("FIN DEL PROGRAMA\n\r");
					estado_uart = 5;
				}
				else{ //en caso de ser un comando no reconocido
					tx_cadena_UART0("COMANDO ERRONEO\n\r");
					estado_uart = 5;
				}
			}
			break;
			
		case 7: //ELIGE MOVIMIENTO DEL COCHE
			tx_cadena_UART0("Elige modo:\n\r ADELANTE \n\r ATRAS \n\r DERECHA \n\r IZQUIERDA \n\r VOLVER AL MENU PREVIO\n\r "); 
			Posserv=0;
			estado_uart = 8;
			break;

		case 8://PROCESAMOR ORDEN DE MOVIMIENTO
			Posserv=0;
			if(rx_completa == 0){  
				 estado_uart = 8; //se mantiene hasta que se completa 
			}
			else if (rx_completa ==1){
				estado_uart = 7; //volvemos a preparar los mensajes para luego transmitirlos
				orden_preparada = 0;
				rx_completa = 0; //borramos flag
				
				//Segun lo recibido se actua sobre el HW
				if (strcmp (buffer, "ADELANTE\r") == 0)
				{
					VelDer=75;
					VelIzq=75;
					estado_uart=7; //Realizamos ordenes
				} 
				else if (strcmp (buffer, "ATRAS\r") == 0)  
				{
						VelDer=-75;
						VelIzq=-75;
						estado_uart=7; //Realizamos ordenes
				}
	  		else if (strcmp (buffer, "DERECHA\r") == 0){
					estado_uart = 7;
					Giromenos90 =true;
				}
	  		else if (strcmp (buffer, "IZQUIERDA\r") == 0){
					estado_uart = 7;
					Giro90 =true;
				}
				else if (strcmp (buffer, "VOLVER AL MENU PREVIO\r") == 0){
					estado_uart = 4;
					modobluetooth=false;
				}
				else{ //en caso de ser un comando no reconocido
					tx_cadena_UART0("COMANDO ERRONEO\n\r");
					estado_uart = 7;
				}
			}
			break;
		case 9: //ELIGE DATO A VISUALIZAR
			if(tx_completa == 0){ 				
				 estado_uart = 9; //se mantiene hasta que se completa 
			}
			else if (tx_completa ==1){
			tx_cadena_UART0("INDIQUE EL NUMERO DE MUESTRAS A TOMAR:\n\r "); 
			estado_uart = 10;
					}
			break;
		case 10://PROCESAMOR ORDEN DE MOVIMIENTO
			if(rx_completa == 0){  
				 estado_uart = 10; //se mantiene hasta que se completa 
			}
			else if (rx_completa ==1){
				estado_uart = 9; //volvemos a preparar los mensajes para luego transmitirlos
				orden_preparada = 0;
				rx_completa = 0; //borramos flag
				//Segun lo recibido se actua sobre el HW
				auxbth =strtol(buffer,&ptr,10);

				if (auxbth > 0 && auxbth <100){
					sprintf(buffer_tx, "TOMA DE MUESTRAS:\n\r");
					tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
					estado_uart = 11;
					auxmuestras= 8;
					tiempo =0;
					}
				else{ //en caso de ser un comando no reconocido
					tx_cadena_UART0("COMANDO ERRONEO\n\r");
					estado_uart = 9;
					tiempo =0;
				}
			}
			break;
			case 11: //imprimimos muestras
			if(tx_completa == 0){ 				
				 estado_uart = 11; //se mantiene hasta que se completa 
			}
			else if (tx_completa ==1){
				if (auxmuestras==8)
					{
						sprintf(buffer_tx, "Motor Derecha: %3d \n\r", VelDer);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==7)
					{
						sprintf(buffer_tx, "Motor Izquierda: %3d \n\r", VelIzq);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==6)
					{
						sprintf(buffer_tx, "Posicion Servo: %3d º\n\r", Posserv);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==5)
					{
						sprintf(buffer_tx, "Sensor Luz1: %3d \n\r",SLuz1);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==4)
					{
						sprintf(buffer_tx, "Sensor Luz2: %3d \n\r",SLuz2);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==3)
					{
						sprintf(buffer_tx, "Promedio: %3d \n\r",SPromedio);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==2)
					{
						sprintf(buffer_tx, "Distancia: %3d cm \n\r",distancia);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras--;
					}
			else if (auxmuestras==1)
					{
						milisegundos=tiempo*10;
						auxbth--;
						sprintf(buffer_tx, "Tiempo de toma muestras:%d milisegundos \n\r",milisegundos);
						tx_cadena_UART0(buffer_tx); //enviamos la cadena para visualizarla
						auxmuestras=8;
					}
			 if(auxbth>0)
			 	estado_uart = 11;
			 else
				 estado_uart = 9;
		 }
				break;
	}
}
