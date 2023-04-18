#include <tp_simple.h>
#include "Nunchuck.h"
#include "stdbool.h"
#include "timer.h"
#include "PWM.h"

/* Definición de los estados */
// Maquina de estados global
#define Inicio          				0
#define Inicio_Espera 			    1
#define Modo_Manual         		2
#define Modo_Manual_Espera      3
#define Modo_Automatico       	4
#define Modo_Automatico_Espera  5
#define Modo_Depuracion       	6
#define Modo_Depuracion_Espera  7
uint8_t Estado;
extern int umbralDistancia, distancia;
extern bool Barrido_Completo,Barrido;
extern int8_t Posserv;
//Variables Nunchuck

extern int8_t x;		// coordenada x
extern int8_t y;		// coordenada y
extern int8_t acelerometro_x, acelerometro_y, acelerometro_z;
extern int8_t resto;
extern bool flagContadorC, flagContadorZ, flagCambioMas2segPulsadores, flagCambioMenos2segPulsadores; 
extern int	contadorZ, contadorC, contadorCambioMas2segPulsadores, contadorCambioMenos2segPulsadores;
extern bool flagCambioMas2segBotones, flagCambioMenos2segBotones;
extern int contadorKey1,contadorKey2;
extern int contadorCambioMas2segPulsadores, contadorCambioMenos2segPulsadores;
extern bool flagCambioPulsandoC;
extern int contadorCambioPulsandoC;
extern bool dep_nunchuck;

//Definicion de las pulsacion de pantalla
extern struct t_screenZone Manual;
extern struct t_screenZone Automatico;
extern struct t_screenZone Depuracion;
extern struct t_screenZone zone_1;
extern struct t_screenZone zone_2;
extern struct t_screenZone zone_3;
extern struct t_screenZone zone_4;
extern struct t_screenZone zone_5;
extern struct t_screenZone zone_6;


void MaquinaEstados (void){
	checkTouchPanel();	
		switch (Estado) 
	{ 
		case Inicio :  // Inicializamos  contadores y cargamos pantalla de inicio y pasamos a modo espera
		  LCD_Clear(Black);
		  PantallaInicio();
		  Estado = Inicio_Espera;
		break;
		
		case Inicio_Espera : 	 //Elegimos modo en funcion de la zona que pulsemos
      if (zonePressed(&Manual)) { 
				Estado = Modo_Manual;
			}
			if (zonePressed(&Automatico)) { 
				Estado = Modo_Automatico;
			}
			if (zonePressed(&Depuracion)) { 
				Estado = Modo_Depuracion;
			}
	  break; 
		
		case Modo_Manual : 
			Posserv=0;									//iniciamos la posicion de servo 
			setServo(Posserv);
		  contadoresreset();					//Ponemos contadores de botones del mando nunchuck y pulsadores a 0
			LCD_Clear(Black);						//Limpiamos pantalla
		  PantallaManual();						//Imprimimos pantalla del modo manual
		  Estado = Modo_Manual_Espera;//cambiamos a modo espera manual
		  Barrido_Completo =false;
		  Barrido= false;
	  break; 
		
		case Modo_Manual_Espera: 
			if( distancia> umbralDistancia)
			Movimiento_Nunchuck();		//Funcion movimiento ruedas con el mando.
			Servo_Nunchuck(); 				//Funcion de movimiento del servo en funcion de la inclinacion del mando.
			controladorPulsadores();	//Funcion que controla Pulsadores Key1 y Key2
		  if (flagCambioMas2segPulsadores || flagCambioMas2segBotones) {		//Cambiamos de modo en funcion de los pulsadores y botones
				Estado = Modo_Depuracion;
				Posserv=0;
			}
			if (flagCambioMenos2segPulsadores|| flagCambioMenos2segBotones) {
				Estado = Modo_Automatico;
			}
	  break; 
		
		case Modo_Automatico : 
			Posserv=0;											//iniciamos la posicion de servo 
			setServo(Posserv);							
			contadoresreset();							//Ponemos contadores de botones del mando nunchuck y pulsadores a 0
			LCD_Clear(Black);								//Limpiamos pantalla
		  PantallaAutomatico();						//Imprimimos pantalla del modo automatico
		  Estado = Modo_Automatico_Espera;//cambiamos a modo espera automatico
	  	Barrido_Completo =false;
		  Barrido= false;
	  break; 
		
		case Modo_Automatico_Espera : 
			controladorPulsadores();				//Funcion que controla Pulsadores Key1 y Key2
		if (flagCambioMas2segPulsadores) {//cambiamos de modo en funcion de las keys y los pulsadores
			Estado = Modo_Depuracion;
			Posserv=0;
		}
		if(flagCambioPulsandoC)
			Estado = Modo_Manual;
	  break;
		
		case Modo_Depuracion :
			Posserv=0;				
			setServo(Posserv);
			contadoresreset();
			LCD_Clear(Black);
		  if(dep_nunchuck==false)				//En funcion de la variable mostramos el modo depuracion con los datos de los sensores o bien con los valores de mando nunchuck
		{
			PantallaDepuracion_Nunchuck();
			dep_nunchuck=true;
			screenMessageIP();
		}
		else
		{
			PantallaDepuracion();
			dep_nunchuck=false;
			screenMessageIP();
		}
			Estado = Modo_Depuracion_Espera;
			Barrido_Completo =false;
		  Barrido= false;
	  break;
		
		case Modo_Depuracion_Espera: 
			controladorPulsadores();	//controla los pulsadores
			modoDepuracion();					//funcion que cambia los valores a visualizar cada vez que tocamos la cabecera
		  if (flagCambioMas2segPulsadores) { // cambiamos de modo en funcion de los pulsadores.
				Estado = Modo_Manual;
			}
			if (flagCambioMenos2segPulsadores) {
				Estado = Modo_Automatico;
			}
	  break;
		default:
			break;
	}
}
