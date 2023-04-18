#include <LPC17xx.H>
#include <glcd.h>
#include <TouchPanel.h>
#include <stdio.h>
#include <string.h>
#include <Math.h>
#include <stdlib.h>

//DECLARAMOS FUNCIONES
void setServo(int8_t grados);
void movServo(void);
void configPWM(void);
void MotorIzquierda(int8_t VelIzquierda);
void MotorDerecha(int8_t VelDerecha);
void PuenteH (void);
void delay_ms(int ms);
