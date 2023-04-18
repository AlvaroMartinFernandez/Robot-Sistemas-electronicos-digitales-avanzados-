/**********************************/
/*Funciones de control del bus I2C*/
/* SDA=P0.0 y SCL=P0.1            */
/**********************************/
#include <LPC17xx.h>
#include "i2c_lpc17xx.h"

#define SDA 0 //pin 0
#define SCL 1 //pin 1
void I2Cdelay(void)//retardo minimo de 4.7 us
{
 unsigned char i;    
 for(i=0;i<100;i++); //Modificar límite para garantizar los tiempos (Bus standar -->F_max=100kHz)
}

void I2Cdelay1(void)//retardo minimo de 4.7 us
{
 unsigned char i;    
 for(i=0;i<50;i++); //Modificar límite para garantizar los tiempos (Bus standar -->F_max=100kHz)
}

//Genera un pulso de reloj (1 ciclo)
void pulso_SCL(void)
{
	I2Cdelay();
	 LPC_GPIO0->FIOSET=(1<<SCL);	// Genera pulso de reloj (nivel alto)
	 I2Cdelay();
	 LPC_GPIO0->FIOCLR=(1<<SCL);	// Nivel bajo
	 //I2Cdelay1();
}

void I2CSendByte(unsigned char byte)
{
 unsigned char i;
 for(i=0;i<8;i++){
	 
	 if (byte &0x80)  LPC_GPIO0->FIOSET=(1<<SDA); // envia cada bit, comenzando por el MSB
	 else  LPC_GPIO0->FIOCLR=(1<<SDA);
	 byte = byte <<1;		// siguiente bit
	 pulso_SCL();
	}

//Leer ACK que envía el Slave (el Master ha de enviar un pulso de reloj)	
// CONFIGURAR PIN SDA COMO ENTRADA;        //espera ACK(config. pin como entrada)
 LPC_GPIO0->FIODIR&=~(1<<SDA);
 pulso_SCL();
// CONFIGURA PIN SDA COMO SALIDA;
	LPC_GPIO0->FIOSET=(1<<SDA);
 LPC_GPIO0->FIODIR|=(1<<SDA);			// Dejamos SDA de nuevo como salida 
	I2Cdelay();
}


//Función que envía START + Byte de dirección del Slave (con bit LSB inicando R/W)	
void I2CSendAddr(unsigned char addr, unsigned char rw)
{
 //CONFIGURAR PINs SDA, SCL COMO SALIDAS;  // Por si se nos olvidada en la conf. general.

	LPC_GPIO0->FIOSET|=(1<<SDA)|(1<<SCL);	   // SDA y SCL a nivel alto para garantizar el
										   // nivel de reposo del bus + tiempo.

	LPC_GPIO0->FIODIR|=(1<<SDA)|(1<<SCL);	
 
 I2Cdelay();
 LPC_GPIO0->FIOCLR=(1<<SDA);			   //condicion de START: Bajar SDA y luego SCL
 I2Cdelay();								
 LPC_GPIO0->FIOCLR=(1<<SCL);
 I2Cdelay();
 I2CSendByte((addr=addr<<1) + rw); //envia byte de direccion 
																	//addr, direccion (7bits) 
																	//rw=1, lectura
																	//rw=0, escritura
}


// Función para leer un Byte del Slave. El Master envía al final de la lectura
// el bit ACK o NACK (si es último byte leído) que se pasa como argumento de la función.
unsigned char I2CGetByte(unsigned char ACK)	
{
// ACK = 0, para cualquier byte que no sea el ultimo.
// ACK = 1 (NACK), despues de leer el ultimo byte
 unsigned char i, byte;
 //CONFIGURAR PIN SDA COMO ENTRADA;	//configura pin SDA como entrada
 LPC_GPIO0->FIODIR &= ~(1<<SDA);
 for(i=0;i<8;i++){					//lee un bit comenzando por el MSB
		 I2Cdelay();
		 LPC_GPIO0->FIOSET=(1<<SCL);//mientras SCL=1
		 I2Cdelay();
		 byte=byte<<1;				
		 if(LPC_GPIO0->FIOPIN&(1<<SDA)) byte++;			//Si leemos "1" sumamos para introducir el "1"
		 LPC_GPIO0->FIOCLR=(1<<SCL);					//Si leemos "0" solo desplazamos (se introduce un "0")
		 I2Cdelay();
	}

 //CONFIGURAR PIN SDA COMO SALIDA;    // Master envía un ACK por cada byte leído.		
	if(ACK)LPC_GPIO0->FIOSET=(1<<SDA);	  // ACK o (NACK) es funcion del último byte leído
	else LPC_GPIO0->FIOCLR=(1<<SDA);

	LPC_GPIO0->FIODIR|=(1<<SDA);	
 
 pulso_SCL();   				      // Pulso de reloj para su envío
 return (byte);
}

void I2CSendStop(void)
{
 LPC_GPIO0->FIOCLR=(1<<SDA);
 I2Cdelay();
 LPC_GPIO0->FIOSET=(1<<SCL);		  // Subir SCL, y después SDA!! para dejar el bus en reposo
 I2Cdelay();
 LPC_GPIO0->FIOSET=(1<<SDA);						
 I2Cdelay();
}
