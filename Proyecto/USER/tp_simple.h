#include <LPC17xx.H>
#include <glcd.h>
#include <TouchPanel.h>
#include <stdio.h>
#include <string.h>

/* Estructura que define una zona de la pantalla */
struct t_screenZone
{
   uint16_t x;         
	uint16_t y;
	uint16_t size_x;
	uint16_t size_y;
	uint8_t  pressed;
};

void checkTouchPanel(void);
int8_t zonePressed(struct t_screenZone* zone);
int8_t zoneNewPressed(struct t_screenZone* zone);
void PantallaDepuracion_Nunchuck(void);
void PantallaDepuracion(void);
void PantallaAutomatico(void);
void PantallaManual(void);
void PantallaInicio(void);
void Dato_motorDer(void);
void Dato_motorIzq(void);
void Dato_motorServo(void);
void Dato_Distanciacm(void);
void Dato_sensorLuz1(void);
void Dato_sensorLuz2(void);
void Dato_promedioSensores(void);
void modoDepuracion(void);
void screenMessageIP(void);
