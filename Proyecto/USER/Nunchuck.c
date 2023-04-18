#include <LPC17xx.H>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c_lpc17xx.h"
#include "Nunchuck.h"
#include "PWM.h"


// Variable de velocidad de la ruedas
extern uint8_t VelDer;
extern uint8_t VelIzq;
extern int8_t Posserv;
int ejex,ejey;

//Variable de datos del mando
int8_t x;		// coordenada x
int8_t y;		// coordenada y
int8_t acelerometro_x, acelerometro_y, acelerometro_z;
int8_t resto;
extern bool flagContadorC, flagContadorZ, flagCambioMas2segPulsadores, flagCambioMenos2segPulsadores; 
extern int	contadorZ, contadorC, contadorCambioMas2segPulsadores, contadorCambioMenos2segPulsadores;
extern bool flagCambioMas2segBotones, flagCambioMenos2segBotones;
extern int contadorCambioMas2segPulsadores, contadorCambioMenos2segPulsadores;
extern bool flagCambioPulsandoC;
extern int contadorCambioPulsandoC;
extern bool modobluetooth;

//---------------------------------------------------------------------------

void Nunchuck_Init(){
	I2Cdelay();
	I2Cdelay();
	
	// Enviar commando 0x52 para configurar el Nunchuck
	I2CSendAddr(0x52,0);		 	// I2C Address del Nunchuck, WRITE
	I2CSendByte(0xF0);				// Access Config Command
	I2CSendByte(0x55);					// Continuous Conversion	
	I2CSendStop();

	I2Cdelay();								// Para propositos de simulacion
	I2Cdelay();

	// Enviar commando 0xFB para arrancar conversion del Nunchuck
	I2CSendAddr(0x52,0);		 	// I2C Address del Nunchuck, WRITE
	I2CSendByte(0xFB);				// Start Conversion 
	I2CSendByte(0x00);
	I2CSendStop();
}

//---------------------------------------------------------------------------

void Nunchuck_read(){
	
	// Enviar commando 0x52 para leer Nunchuck
	I2CSendAddr(0x52,0); 				// I2C Address del Nunchuck, WRITE
	I2CSendByte(0x0);					  // Read 
	I2CSendStop();

	I2Cdelay();

	// Leer datos mando
	I2CSendAddr(0x52,1); 				// I2C Address, READ
	x = I2CGetByte(0);		      // Read MSB Byte, ACK : Esto es la coordenada x
	y = I2CGetByte(0);		
	acelerometro_x = I2CGetByte(0);
	acelerometro_y = I2CGetByte(0);
	acelerometro_z = I2CGetByte(0); 
	resto = I2CGetByte(1); //NACK
	
	if ((resto & 00000001) == 0x0) {  // Boton Z
		flagContadorZ = true;
	}
	else {
		flagContadorZ = false;
	}
	if ((resto & 0x02) == 0x0) {  // Boton C
		flagContadorC = true;
	}
	else {
		flagContadorC = false;
	}
	
	I2CSendStop();
}

/*******************************************************************************
* Function Name  : MovimientoNunchuck
* Description    : Saca los valores recogidos por el Nunchuck para controlar el robot 
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void Movimiento_Nunchuck() 
	{
	if(modobluetooth==false)
		{
			if(y<0)
			{
				ejey= (128+y)*100/127;
			}
			else if( y>= 0)
			{
				ejey= (y-128)*100/128;
			}
			
			if(x<0)
			{
				ejex= (x+129)*100/128;
			}
			else if( x>= 0)
			{
				ejex= (x-127)*100/127;
			}
			
			
			if (ejey==0 && ejex==0)
			{
				VelDer = 0;
				VelIzq= 0;
			}
			else if(ejey!=0 &&ejex==0)
			{
				VelDer=ejey;
				VelIzq=ejey;
			}
			else if(ejey==0 &&ejex!=0)
			{
				VelDer=-ejex;
				VelIzq=ejex;
			}
			else if(ejey>0 &&ejex!=0)
			{
				VelDer=ejey/2-ejex/2;
				VelIzq=ejey/2+ejex/2;
			}
			else if(ejey<0 &&ejex!=0)
			{
				VelDer=ejey/2+ejex/2;
				VelIzq=ejey/2-ejex/2;
			}
		}
	}

/*******************************************************************************
* Function Name  : Servo_Nunchuck
* Description    : Mueve el servo con la inclinacion del mando
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void Servo_Nunchuck() 
	{ 
	 if (acelerometro_x>0)
    Posserv=(127-acelerometro_x)*90/62;
	 
	 else 
		Posserv=-(128+acelerometro_x)*90/63;
 }
