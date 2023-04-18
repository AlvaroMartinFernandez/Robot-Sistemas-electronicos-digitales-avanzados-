#include <tp_simple.h>
#include <stdbool.h>
#include "Net_Config.h"

#define MY_IP localm[NETIF_ETH].IpAdr
extern LOCALM localm[];

/* Variable que contiene el dato del programa */
extern int8_t VelDer;
extern int8_t VelIzq;
extern int8_t Posserv;
extern int distancia;
extern uint8_t Estado;

//Variables mando nunchuck
extern int8_t x;		// coordenada x
extern int8_t y;		// coordenada y
extern int8_t acelerometro_x;
extern int8_t acelerometro_y;
extern int8_t acelerometro_z;
extern bool flagContadorC;
extern bool flagContadorZ;

//Variables sensores
uint32_t SLuz1 = 0;
uint32_t SLuz2 = 0;
uint32_t SPromedio= 0;
//Variables auxiliares
uint8_t messageText[25+1] = {"Esto es una prueba 1000"};
int8_t aux1,aux2,aux3, aux4, aux5,aux6,aux7,aux8,aux9,aux10,aux11,aux12,aux13,aux14;
bool dep_nunchuck= true;

/* Definicion de las diferentes zonas de la pantalla */
//Modo MAnual & Automatico
struct t_screenZone Cabecera = { 10,  20, 220,  40, 0}; /*Mensaje "Bienvenida "*/
struct t_screenZone subCabecera = { 10,  60, 220,  40, 0}; /*Elija el modo*/
struct t_screenZone Manual = { 10,  100, 220,  40, 0}; /*Modo Manual*/
struct t_screenZone Automatico = { 10,  140, 220,  40, 0}; /*Modo Automatico*/
struct t_screenZone Depuracion = { 10,  180, 220,  40, 0}; /*Modo Depuracion*/
struct t_screenZone Infor = { 10,  60, 220,  30, 0}; /*Informacion*/
struct t_screenZone VDer = { 10,  90, 150,  30, 0}; /*Vel Derecha*/
struct t_screenZone DatDer = { 160,  90, 70,  30, 0}; /*Datos Vel Derecha*/
struct t_screenZone VIzq = { 10, 120,  150,  30, 0}; /*Vel Inquierda             */
struct t_screenZone DatIzq = { 160, 120,  70, 30, 0}; /*Datos Vel Izquierda   */
struct t_screenZone PServ = { 10, 150,  150,  30, 0}; /*Posicion Servo   */
struct t_screenZone DatServ = { 160, 150,  70, 30, 0}; /*Datos posicion servo   */
struct t_screenZone Dist = { 10, 180,  150,  30, 0}; /*Umbral Distancia   */
struct t_screenZone DatDist = { 160, 180,  70,  30, 0}; /*Datos Umbral Distancia   */
struct t_screenZone Luz1 = { 10, 210,  150,  30, 0}; /*Intensidad Luz sensor 1   */
struct t_screenZone DatLuz1 = { 160, 210,  70,  30, 0}; /*Datos Intensidad Luz sensor 1   */
struct t_screenZone Luz2 = { 10, 240,  150,  30, 0}; /*Intensidad Luz sensor 2   */
struct t_screenZone DatLuz2 = { 160, 240,  70,  30, 0}; /*Datos Intensidad Luz sensor 2   */
struct t_screenZone Promedio = { 10, 270,  150,  30, 0}; /*Promedio Intensidad Luz   */
struct t_screenZone DatPromedio = { 160, 270,  70,  30, 0}; /*Datos Promedio   */


//zona modo Depuracion
struct t_screenZone DVDer = { 10,  90, 90,  30, 0}; /*Vel Derecha*/
struct t_screenZone DDatDer = { 100,  90, 40,  30, 0}; /*Datos Vel Derecha*/
struct t_screenZone zone_1 = { 140,  90, 45,  30, 0}; /*Incrementar Vel Derecha*/
struct t_screenZone zone_2 = { 185,  90, 45,  30, 0}; /*Decrementar Vel Derecha*/
struct t_screenZone DVIzq = { 10, 120,  90,  30, 0}; /*Vel Inquierda             */
struct t_screenZone DDatIzq = { 100, 120,  40, 30, 0}; /*Datos Vel Izquierda   */
struct t_screenZone zone_3 = { 140,  120, 45,  30, 0}; /*Incrementar Vel Izquierda*/
struct t_screenZone zone_4 = { 185,  120, 45,  30, 0}; /*Decrementar Vel Izquierda*/
struct t_screenZone DPServ = { 10, 150,  90,  30, 0}; /*Posicion Servo   */
struct t_screenZone DDatServ = { 100, 150,  40, 30, 0}; /*Datos posicion servo   */
struct t_screenZone zone_5 = { 140,  150, 45,  30, 0}; /*Mover serv Derecha*/
struct t_screenZone zone_6 = { 185,  150, 45,  30, 0}; /*Mover servo Izquierda*/
struct t_screenZone DDist = { 10, 180,  170,  30, 0}; /*Umbral Distancia   */
struct t_screenZone DDatDist = { 180, 180,  50,  30, 0}; /*Datos Umbral Distancia   */
struct t_screenZone DLuz1 = { 10, 210,  170,  30, 0}; /*Intensidad Luz sensor 1   */
struct t_screenZone DDatLuz1 = { 180, 210,  50,  30, 0}; /*Datos Intensidad Luz sensor 1   */
struct t_screenZone DLuz2 = { 10, 240,  170,  30, 0}; /*Intensidad Luz sensor 2   */
struct t_screenZone DDatLuz2 = { 180, 240,  50,  30, 0}; /*Datos Intensidad Luz sensor 2   */
struct t_screenZone DPromedio = { 10, 270,  170,  30, 0}; /*Promedio Intensidad Luz   */
struct t_screenZone DDatPromedio = { 180, 270,  50,  30, 0}; /*Datos Promedio   */
struct t_screenZone IP = { 10, 60,  70,  30, 0}; /*Ip   */
struct t_screenZone DatIP = { 80, 60,  150,  30, 0}; /*Valor IP   */
struct t_screenZone zone_7 = { 140,  90, 90,  90, 0}; /*Mover servo Izquierda*/


/* Flag que indica si se detecta una pulsación válida */
uint8_t pressedTouchPanel = 0;


/* Variable temporal donde almacenar cadenas de caracteres */
char texto1[25];
char texto2[25];
char texto3[25];
char texto4[25];
char texto5[25];
char texto6[25];
char texto7[25];

/*******************************************************************************
* Function Name  : squareButton
* Description    : Dibuja un cuadrado en las coordenadas especificadas colocando 
*                  un texto en el centro del recuadro
* Input          : zone: zone struct
*                  text: texto a representar en el cuadro
*                  textColor: color del texto
*                  lineColor: color de la línea
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void squareButton(struct t_screenZone* zone, char * text, uint16_t textColor, uint16_t lineColor)
{
   LCD_DrawLine( zone->x, zone->y, zone->x + zone->size_x, zone->y, lineColor);
   LCD_DrawLine( zone->x, zone->y, zone->x, zone->y + zone->size_y, lineColor);
   LCD_DrawLine( zone->x, zone->y + zone->size_y, zone->x + zone->size_x, zone->y + zone->size_y, lineColor);
   LCD_DrawLine( zone->x + zone->size_x, zone->y, zone->x + zone->size_x, zone->y + zone->size_y, lineColor);
	GUI_Text(zone->x + zone->size_x/2 - (strlen(text)/2)*8, zone->y + zone->size_y/2 - 8,
            (uint8_t*) text, textColor, Black);	
}

/*******************************************************************************
* Function Name  : drawMinus
* Description    : Draw a minus sign in the center of the zone
* Input          : zone: zone struct
*                  lineColor
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void drawMinus(struct t_screenZone* zone, uint16_t lineColor)
{
   LCD_DrawLine( zone->x + 5 , zone->y + zone->size_y/2 - 1, 
                 zone->x + zone->size_x-5, zone->y + zone->size_y/2 - 1,
                 lineColor);
   LCD_DrawLine( zone->x + 5 , zone->y + zone->size_y/2, 
                 zone->x + zone->size_x-5, zone->y + zone->size_y/2,
                 lineColor);
   LCD_DrawLine( zone->x + 5 , zone->y + zone->size_y/2 + 1, 
                 zone->x + zone->size_x-5, zone->y + zone->size_y/2 + 1,
                 lineColor);
}

/*******************************************************************************
* Function Name  : drawMinus
* Description    : Draw a minus sign in the center of the zone
* Input          : zone: zone struct
*                  lineColor
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void drawAdd(struct t_screenZone* zone, uint16_t lineColor)
{
   drawMinus(zone, lineColor);
   
   LCD_DrawLine( zone->x + zone->size_x/2 - 1,  zone->y + 5 ,
                 zone->x + zone->size_x/2 - 1,  zone->y + zone->size_y - 5, 
                 lineColor);
   LCD_DrawLine( zone->x + zone->size_x/2 ,  zone->y + 5 ,
                 zone->x + zone->size_x/2 ,  zone->y + zone->size_y - 5, 
                 lineColor);
   LCD_DrawLine( zone->x + zone->size_x/2 + 1,  zone->y + 5 ,
                 zone->x + zone->size_x/2 + 1,  zone->y + zone->size_y - 5, 
                 lineColor);
}
/*******************************************************************************
* Function Name  : PantallaInicio
* Description    : Visualiza el menu de inicio
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void PantallaInicio(void)
{
  squareButton(&Cabecera, "BIENVENIDO USUARIO", White, Blue);
  squareButton(&subCabecera, "Eliga el modo:", White, Blue);
	squareButton(&Manual, "Manual", White, Blue);
	squareButton(&Automatico, "Automatico", White, Blue);
	squareButton(&Depuracion, "Depuracion", White, Blue);
}
/*******************************************************************************
* Function Name  : PantallaManual
* Description    : Visualiza la informacion a mostra en el modo manual
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void PantallaManual(void)
{
	//Dibujamos Zona de pantalla.
  squareButton(&Cabecera, "MODO MANUAL", White, Blue);
  squareButton(&Infor, "Datos:", White, Blue);
  squareButton(&VDer, "Vel Derecha", White, Blue);
	squareButton(&DatDer, "     ", White, Blue);
	squareButton(&VIzq, "Vel Izquierda", White, Blue);
	squareButton(&DatIzq, "     ", White, Blue);
	squareButton(&PServ, "Pos. Servo", White, Blue);
	squareButton(&DatServ, "     ", White, Blue);
	squareButton(&Dist, "Distancia", White, Blue);
	squareButton(&DatDist, "     ", White, Blue);
	squareButton(&Luz1, "Intensidad Luz1", White, Blue);
	squareButton(&DatLuz1, "     ", White, Blue);
	squareButton(&Luz2, "Intensidad Luz2", White, Blue);
	squareButton(&DatLuz2, "     ", White, Blue);
	squareButton(&Promedio, "Prom. Intensidad", White, Blue);
	squareButton(&DatPromedio, "     ", White, Blue);
	//Dibujamos los Valores de los parametros
	sprintf(texto1,"%2d", VelDer);
	sprintf(texto2,"%2d", VelIzq);
	sprintf(texto3,"%2d", Posserv);
	sprintf(texto4,"%2d", distancia);
	sprintf(texto5,"%2d", SLuz1);
	sprintf(texto6,"%2d", SLuz2);
	SPromedio= (SLuz1+SLuz2)/2;
	sprintf(texto7,"%2d", SPromedio);
		 
     GUI_Text(DatDer.x + DatDer.size_x/2 - (strlen(texto1)/2)*8, DatDer.y + DatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);		
		 GUI_Text(DatIzq.x + DatIzq.size_x/2 - (strlen(texto2)/2)*8, DatIzq.y + DatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		 GUI_Text(DatServ.x + DatServ.size_x/2 - (strlen(texto3)/2)*8, DatServ.y + DatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		 GUI_Text(DatDist.x + DatDist.size_x/2 - (strlen(texto4)/2)*8, DatDist.y + DatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		 GUI_Text(DatLuz1.x + DatLuz1.size_x/2 - (strlen(texto5)/2)*8, DatLuz1.y + DatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);
		 GUI_Text(DatLuz2.x + DatLuz2.size_x/2 - (strlen(texto6)/2)*8, DatLuz2.y + DatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);
		 GUI_Text(DatPromedio.x + DatPromedio.size_x/2 - (strlen(texto7)/2)*8, DatPromedio.y + DatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);

}
/*******************************************************************************
* Function Name  : PantallaAutomatico
* Description    : Visualiza la informacion a mostra en el modo automatico
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void PantallaAutomatico(void)
{
	//Dibujamos Zona de pantalla.
  squareButton(&Cabecera, "MODO AUTOMATICO", White, Blue);
  squareButton(&Infor, "Datos:", White, Blue);
  squareButton(&VDer, "Vel Derecha", White, Blue);
	squareButton(&DatDer, "     ", White, Blue);
	squareButton(&VIzq, "Vel Izquierda", White, Blue);
	squareButton(&DatIzq, "     ", White, Blue);
	squareButton(&PServ, "Pos. Servo", White, Blue);
	squareButton(&DatServ, "     ", White, Blue);
	squareButton(&Dist, "Distancia", White, Blue);
	squareButton(&DatDist, "     ", White, Blue);
	squareButton(&Luz1, "Intensidad Luz1", White, Blue);
	squareButton(&DatLuz1, "     ", White, Blue);
	squareButton(&Luz2, "Intensidad Luz2", White, Blue);
	squareButton(&DatLuz2, "     ", White, Blue);
	squareButton(&Promedio, "Prom. Intensidad", White, Blue);
	squareButton(&DatPromedio, "     ", White, Blue);
	//Dibujamos los Valores de los parametros
	sprintf(texto1,"%2d", VelDer);
	sprintf(texto2,"%2d", VelIzq);
	sprintf(texto3,"%2d", Posserv);
	sprintf(texto4,"%2d", distancia);
	sprintf(texto5,"%2d", SLuz1);
	sprintf(texto6,"%2d", SLuz2);
	SPromedio= (SLuz1+SLuz2)/2;
	sprintf(texto7,"%2d", SPromedio);
		 
     GUI_Text(DatDer.x + DatDer.size_x/2 - (strlen(texto1)/2)*8, DatDer.y + DatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);		
		 GUI_Text(DatIzq.x + DatIzq.size_x/2 - (strlen(texto2)/2)*8, DatIzq.y + DatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		 GUI_Text(DatServ.x + DatServ.size_x/2 - (strlen(texto3)/2)*8, DatServ.y + DatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		 GUI_Text(DatDist.x + DatDist.size_x/2 - (strlen(texto4)/2)*8, DatDist.y + DatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		 GUI_Text(DatLuz1.x + DatLuz1.size_x/2 - (strlen(texto5)/2)*8, DatLuz1.y + DatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);
		 GUI_Text(DatLuz2.x + DatLuz2.size_x/2 - (strlen(texto6)/2)*8, DatLuz2.y + DatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);
		 GUI_Text(DatPromedio.x + DatPromedio.size_x/2 - (strlen(texto7)/2)*8, DatPromedio.y + DatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);

}

/*******************************************************************************
* Function Name  : PantallaDepuracion
* Description    : Visualiza la informacion a mostra en el modo depuracion
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void PantallaDepuracion(void)
{
	//Dibujamos Zona de pantalla.
  squareButton(&Cabecera, "MODO DEPURACION", White, Blue);
  squareButton(&IP, "Dir. IP:", White, Blue);
	squareButton(&DatIP, "     ", White, Blue);
  squareButton(&DVDer, "V.Derecha", White, Blue);
	squareButton(&DDatDer, "     ", White, Blue);
	squareButton(&DVIzq, "V.Izquierd", White, Blue);
	squareButton(&DDatIzq, "     ", White, Blue);
	squareButton(&DPServ, "Pos.Servo", White, Blue);
	squareButton(&DDatServ, "     ", White, Blue);
	squareButton(&DDist, "Distancia", White, Blue);
	squareButton(&DDatDist, "     ", White, Blue);
	squareButton(&DLuz1, "Intensidad Luz1", White, Blue);
	squareButton(&DDatLuz1, "     ", White, Blue);
	squareButton(&DLuz2, "Intensidad Luz2", White, Blue);
	squareButton(&DDatLuz2, "     ", White, Blue);
	squareButton(&DPromedio, "Promedio Int.", White, Blue);
	squareButton(&DDatPromedio, "     ", White, Blue);
	squareButton(&zone_7, "     ", White, Blue);
	drawAdd(&zone_1,White);
	drawAdd(&zone_3,White);
	drawAdd(&zone_5,White);
	drawMinus(&zone_2,White);
	drawMinus(&zone_4,White);
	drawMinus(&zone_6,White);
	//Dibujamos los Valores de los parametros
	sprintf(texto1,"%2d", VelDer);
	sprintf(texto2,"%2d", VelIzq);
	sprintf(texto3,"%2d", Posserv);
	sprintf(texto4,"%2d", distancia);
	sprintf(texto5,"%2d", SLuz1);
	sprintf(texto6,"%2d", SLuz2);
	SPromedio= (SLuz1+SLuz2)/2;
	sprintf(texto7,"%2d", SPromedio);
		 
     GUI_Text(DDatDer.x + DDatDer.size_x/2 - (strlen(texto1)/2)*8, DDatDer.y + DDatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);		
		 GUI_Text(DDatIzq.x + DDatIzq.size_x/2 - (strlen(texto2)/2)*8, DDatIzq.y + DDatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		 GUI_Text(DDatServ.x + DDatServ.size_x/2 - (strlen(texto3)/2)*8, DDatServ.y + DDatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		 GUI_Text(DDatDist.x + DDatDist.size_x/2 - (strlen(texto4)/2)*8, DDatDist.y + DDatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		 GUI_Text(DDatLuz1.x + DDatLuz1.size_x/2 - (strlen(texto5)/2)*8, DDatLuz1.y + DDatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);
		 GUI_Text(DDatLuz2.x + DDatLuz2.size_x/2 - (strlen(texto6)/2)*8, DDatLuz2.y + DDatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);
		 GUI_Text(DDatPromedio.x + DDatPromedio.size_x/2 - (strlen(texto7)/2)*8, DDatPromedio.y + DDatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);
}
/*******************************************************************************
* Function Name  : PantallaDepuracion_Nunchuck
* Description    : Visualiza la informacion de los valores del NunChuck
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void PantallaDepuracion_Nunchuck(void)
{
	//Dibujamos Zona de pantalla.
  squareButton(&Cabecera, "MODO DEPURACION", White, Blue);
  squareButton(&IP, "Dir. IP:", White, Blue);
	squareButton(&DatIP, "     ", White, Blue);
  squareButton(&VDer, "Eje X", White, Blue);
	squareButton(&DatDer, "     ", White, Blue);
	squareButton(&VIzq, "Eje Y", White, Blue);
	squareButton(&DatIzq, "     ", White, Blue);
	squareButton(&PServ, "Acelerometro X", White, Blue);
	squareButton(&DatServ, "     ", White, Blue);
	squareButton(&Dist, "Acelerometro Y", White, Blue);
	squareButton(&DatDist, "     ", White, Blue);
	squareButton(&Luz1, "Acelerometro Z", White, Blue);
	squareButton(&DatLuz1, "     ", White, Blue);
	squareButton(&Luz2, "Boton C", White, Blue);
	squareButton(&DatLuz2, "     ", White, Blue);
	squareButton(&Promedio, "Boton Z", White, Blue);
	squareButton(&DatPromedio, "     ", White, Blue);
	
	//Dibujamos los Valores de los parametros
	sprintf(texto1,"%4d", x);
	sprintf(texto2,"%4d", y);
	sprintf(texto3,"%4d", acelerometro_x);
	sprintf(texto4,"%4d", acelerometro_y);
	sprintf(texto5,"%4d", acelerometro_z);
	sprintf(texto6,"%4d", flagContadorC);
	sprintf(texto7,"%4d", flagContadorZ);
		 
     GUI_Text(DatDer.x + DatDer.size_x/2 - (strlen(texto1)/2)*8, DatDer.y + DatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);		
		 GUI_Text(DatIzq.x + DatIzq.size_x/2 - (strlen(texto2)/2)*8, DatIzq.y + DatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		 GUI_Text(DatServ.x + DatServ.size_x/2 - (strlen(texto3)/2)*8, DatServ.y + DatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		 GUI_Text(DatDist.x + DatDist.size_x/2 - (strlen(texto4)/2)*8, DatDist.y + DDatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		 GUI_Text(DatLuz1.x + DatLuz1.size_x/2 - (strlen(texto5)/2)*8, DatLuz1.y + DatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);
		 GUI_Text(DatLuz2.x + DatLuz2.size_x/2 - (strlen(texto6)/2)*8, DatLuz2.y + DatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);
		 GUI_Text(DatPromedio.x + DatPromedio.size_x/2 - (strlen(texto7)/2)*8, DatPromedio.y + DatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);
}
/*******************************************************************************
* Function Name  : checkTouchPanel
* Description    : Lee el TouchPanel y almacena las coordenadas si detecta pulsación
* Input          : None
* Output         : Modifica pressedTouchPanel
*                    0 - si no se detecta pulsación
*                    1 - si se detecta pulsación
*                        En este caso se actualizan las coordinadas en la estructura display
* Return         : None
* Attention		  : None
*******************************************************************************/
void checkTouchPanel(void)
{
	Coordinate* coord;
	
	coord = Read_Ads7846();
	
	if (coord > 0) {
	  getDisplayPoint(&display, coord, &matrix );
     pressedTouchPanel = 1;
   }   
   else
   {   
     pressedTouchPanel = 0;
      
     // Esto es necesario hacerlo si hay dos zonas diferentes en 
     // dos pantallas secuenciales que se solapen      
     zone_1.pressed = 1;
     zone_2.pressed = 1;
     zone_3.pressed = 1;
     zone_4.pressed = 1;
     zone_5.pressed = 1;
     zone_6.pressed = 1;
		 zone_7.pressed = 1;
		 Cabecera.pressed = 1;
		 subCabecera.pressed = 1;
		 Manual.pressed = 1;
		 Automatico.pressed = 1;
		 Depuracion.pressed = 1;
     Infor.pressed = 1;
     VDer.pressed = 1;
     DatDer.pressed = 1;
     VIzq.pressed = 1;
     DatIzq.pressed = 1;
     PServ.pressed = 1;
     DatServ.pressed = 1;
     Dist.pressed = 1;
     DatDist.pressed = 1;
     Luz1.pressed = 1;
     DatLuz1.pressed = 1;
     Luz2.pressed = 1;
     DatLuz2.pressed = 1;
     Promedio.pressed = 1;
		 DatPromedio.pressed = 1;
     DVDer.pressed = 1;
     DDatDer.pressed = 1;
     DVIzq.pressed = 1;
     DDatIzq.pressed = 1;
     DPServ.pressed = 1;
     DDatServ.pressed = 1;
     DDist.pressed = 1;
     DDatDist.pressed = 1;
     DLuz1.pressed = 1;
     DDatLuz1.pressed = 1;
     DLuz2.pressed = 1;
     DDatLuz2.pressed = 1;
     DPromedio.pressed = 1;
		 DDatPromedio.pressed = 1;
		 IP.pressed = 1;
		 DatIP.pressed = 1;
     
   }  
}
/*******************************************************************************
* Function Name  : Dato_motorDer
* Description    : Imprime el dato del motor derecho
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_motorDer(void) 
{
		if (Estado== 3 || Estado==5)
		{
			if (aux1!=VelDer)
			squareButton(&DatDer, "      ", White, Blue);
			sprintf(texto1,"%2d", VelDer);
			GUI_Text(DatDer.x + DatDer.size_x/2 - (strlen(texto1)/2)*8, DatDer.y + DatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);	
			aux1=VelDer;
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if (aux1!=VelDer)
		squareButton(&DDatDer, "    ", White, Blue);
		sprintf(texto1,"%2d", VelDer);
    GUI_Text(DDatDer.x + DDatDer.size_x/2 - (strlen(texto1)/2)*8, DDatDer.y + DDatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);
		aux1=VelDer;
	}
}
/*******************************************************************************
* Function Name  : Dato_motorIzq
* Description    : Imprime el dato del motor derecho
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_motorIzq(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if (aux2!=VelIzq)
		squareButton(&DatIzq, "      ", White, Blue);
		sprintf(texto2,"%2d", VelIzq);
    GUI_Text(DatIzq.x + DatIzq.size_x/2 - (strlen(texto2)/2)*8, DatIzq.y + DatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		aux2=VelIzq;			
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if (aux2!=VelIzq)
		squareButton(&DDatIzq, "    ", White, Blue);
		sprintf(texto2,"%2d", VelIzq);
    GUI_Text(DDatIzq.x + DDatIzq.size_x/2 - (strlen(texto2)/2)*8, DDatIzq.y + DDatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);
		aux2=VelIzq;	
	}
}
/*******************************************************************************
* Function Name  : Dato_motorServo
* Description    : Imprime el dato del motor servo
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_motorServo(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if(aux3!=Posserv)
		squareButton(&DatServ, "      ", White, Blue);
		sprintf(texto3,"%2d",Posserv);
    GUI_Text(DatServ.x + DatServ.size_x/2 - (strlen(texto3)/2)*8, DatServ.y + DatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		aux3=Posserv;				
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if(aux3!=Posserv)
			squareButton(&DDatServ, "   ", White, Blue);
			sprintf(texto3,"%2d", Posserv);
			GUI_Text(DDatServ.x + DDatServ.size_x/2 - (strlen(texto3)/2)*8, DDatServ.y + DDatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);
		
		aux3=Posserv;	
		
	}
}

/*******************************************************************************
* Function Name  : Dato_Distanciacm
* Description    : Imprime el dato del sensor ultrasonidos
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_Distanciacm(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if(aux4!=distancia)
		squareButton(&DatDist, "      ", White, Blue);
		if (distancia>400)
		sprintf(texto4,"max");
		else
		sprintf(texto4,"%4d",distancia);
    GUI_Text(DatDist.x + DatDist.size_x/2 - (strlen(texto4)/2)*8, DatDist.y + DatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		aux4=distancia;				
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if(aux4!=distancia)
			squareButton(&DDatDist, "     ", White, Blue);
			if (distancia>400)
			sprintf(texto4,"max");
			else
			sprintf(texto4,"%4d", distancia);
			GUI_Text(DDatDist.x + DDatDist.size_x/2 - (strlen(texto4)/2)*8, DDatDist.y + DDatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);
		
		aux4=distancia;	
		
	}
}
/*******************************************************************************
* Function Name  : Dato_sensorLuz1
* Description    : Imprime el dato del sensor luz1
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_sensorLuz1(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if(aux5!=SLuz1)
		squareButton(&DatLuz1, "      ", White, Blue);
		sprintf(texto5,"%3d",SLuz1);
    GUI_Text(DatLuz1.x + DatLuz1.size_x/2 - (strlen(texto5)/2)*8, DatLuz1.y + DatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);	
		aux5=SLuz1;				
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if(aux5!=SLuz1)
			squareButton(&DDatLuz1, "      ", White, Blue);
			sprintf(texto5,"%3d", SLuz1);
			GUI_Text(DDatLuz1.x + DDatLuz1.size_x/2 - (strlen(texto5)/2)*8, DDatLuz1.y + DDatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);
		
		aux5=SLuz1;	
		
	}
}
/*******************************************************************************
* Function Name  : Dato_sensorLuz2
* Description    : Imprime el dato de luz 2
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_sensorLuz2(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if(aux6!=SLuz2)
		squareButton(&DatLuz2, "      ", White, Blue);
		sprintf(texto6,"%3d",SLuz2);
    GUI_Text(DatLuz2.x + DatLuz2.size_x/2 - (strlen(texto6)/2)*8, DatLuz2.y + DatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);	
		aux6=SLuz2;				
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if(aux6!=SLuz2)
			squareButton(&DDatLuz2, "     ", White, Blue);
			sprintf(texto6,"%3d", SLuz2);
			GUI_Text(DDatLuz2.x + DDatLuz2.size_x/2 - (strlen(texto6)/2)*8, DDatLuz2.y + DDatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);
		
		aux6=SLuz2;	
		
	}
}
/*******************************************************************************
* Function Name  : Dato_promedioSensores
* Description    : Imprime el dato promedio de los sensores de luz
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void Dato_promedioSensores(void) 
{
		if (Estado== 3 || Estado==5)
		{
		if(aux7!=SPromedio)
		squareButton(&DatPromedio, "      ", White, Blue);
		sprintf(texto7,"%3d",SPromedio);
    GUI_Text(DatPromedio.x + DatPromedio.size_x/2 - (strlen(texto7)/2)*8, DatPromedio.y + DatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);	
		aux7=SPromedio;				
		}
	else if(Estado==7 && dep_nunchuck == false)
	{
		if(aux7!=SPromedio)
			squareButton(&DDatPromedio, "      ", White, Blue);
			sprintf(texto7,"%3d", SPromedio);
			GUI_Text(DDatPromedio.x + DDatPromedio.size_x/2 - (strlen(texto7)/2)*8, DDatPromedio.y + DDatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);
		
		aux7=SPromedio;	
		
	}
}
/*******************************************************************************
* Function Name  : screenMessageIP
* Description    : Visualiza la pantalla de mensajes
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void screenMessageIP(void)
{     
	
   sprintf((char *)messageText," %d.%d.%d.%d ", MY_IP[0], MY_IP[1],
                                                       MY_IP[2], MY_IP[3]);
   squareButton(&DatIP, (char*)messageText, Red , Blue);
}
/*******************************************************************************
* Function Name  : zonePressed
* Description    : Detecta si se ha producido una pulsación en una zona contreta
* Input          : zone: Estructura con la información de la zona
* Output         : Modifica zone->pressed
*                    0 - si no se detecta pulsación en la zona
*                    1 - si se detecta pulsación en la zona
* Return         : 0 - si no se detecta pulsación en la zona
*                  1 - si se detecta pulsación en la zona
* Attention		  : None
*******************************************************************************/
int8_t zonePressed(struct t_screenZone* zone)
{
	if (pressedTouchPanel == 1) {

		if ((display.x > zone->x) && (display.x < zone->x + zone->size_x) && 
			  (display.y > zone->y) && (display.y < zone->y + zone->size_y))
      {
         zone->pressed = 1;
		   return 1;
      }   
	}
   
	zone->pressed = 0;
	return 0;
}

/*******************************************************************************
* Function Name  : zoneNewPressed
* Description    : Detecta si se ha producido el flanco de una nueva pulsación en 
*                  una zona contreta
* Input          : zone: Estructura con la información de la zona
* Output         : Modifica zone->pressed
*                    0 - si no se detecta pulsación en la zona
*                    1 - si se detecta pulsación en la zona
* Return         : 0 - si no se detecta nueva pulsación en la zona
*                  1 - si se detecta una nueva pulsación en la zona
* Attention		  : None
*******************************************************************************/
int8_t zoneNewPressed(struct t_screenZone* zone)
{
	if (pressedTouchPanel == 1) {

		if ((display.x > zone->x) && (display.x < zone->x + zone->size_x) && 
			  (display.y > zone->y) && (display.y < zone->y + zone->size_y))
      {
         if (zone->pressed == 0)
         {   
            zone->pressed = 1;
            return 1;
         }
		   return 0;
      }
	}

   zone->pressed = 0;
	return 0;
}

/*******************************************************************************
* Function Name  : modoDepuracion
* Description    : Incrementa valores servo en pantalla
* Input          : 
* Output         : 
* Return         : 
* Attention		  : None
*******************************************************************************/
void modoDepuracion(void)
{
	//Cambio para visualizar datos del nunchuck
	if(zonePressed(&Cabecera))
	{
		Estado= 6;
	}
	//Actualizamos datos del nunchuck
	if(dep_nunchuck== true)
	{
		if(aux8!=x)
		squareButton(&DatDer, "      ", White, Blue);
		sprintf(texto1,"%4d",x);
    GUI_Text(DatDer.x + DatDer.size_x/2 - (strlen(texto1)/2)*8, DatDer.y + DatDer.size_y/2 - 8,
             (uint8_t*) texto1, White, Black);	
		aux8=x;
		
		
		if(aux9!=y)
		squareButton(&DatIzq, "      ", White, Blue);
		sprintf(texto2,"%4d",y);
    GUI_Text(DatIzq.x + DatIzq.size_x/2 - (strlen(texto2)/2)*8, DatIzq.y + DatIzq.size_y/2 - 8,
             (uint8_t*) texto2, White, Black);	
		aux9=y;
				
		if(aux10!=acelerometro_x)
		squareButton(&DatServ, "      ", White, Blue);
		sprintf(texto3,"%4d",acelerometro_x);
    GUI_Text(DatServ.x + DatServ.size_x/2 - (strlen(texto3)/2)*8, DatServ.y + DatServ.size_y/2 - 8,
             (uint8_t*) texto3, White, Black);	
		aux10=acelerometro_x;
		
		if(aux11!=acelerometro_y)
		squareButton(&DatDist, "      ", White, Blue);
		sprintf(texto4,"%4d",acelerometro_y);
    GUI_Text(DatDist.x + DatDist.size_x/2 - (strlen(texto4)/2)*8, DatDist.y + DatDist.size_y/2 - 8,
             (uint8_t*) texto4, White, Black);	
		aux11=acelerometro_y;
		
		if(aux12!=acelerometro_z)
		squareButton(&DatLuz1, "      ", White, Blue);
		sprintf(texto5,"%4d",acelerometro_z);
    GUI_Text(DatLuz1.x + DatLuz1.size_x/2 - (strlen(texto5)/2)*8, DatLuz1.y + DatLuz1.size_y/2 - 8,
             (uint8_t*) texto5, White, Black);	
		aux12=acelerometro_z;
		
		if(aux13!=flagContadorC)
		squareButton(&DatLuz2, "      ", White, Blue);
		sprintf(texto6,"%4d",flagContadorC);
    GUI_Text(DatLuz2.x + DatLuz2.size_x/2 - (strlen(texto6)/2)*8, DatLuz2.y + DatLuz2.size_y/2 - 8,
             (uint8_t*) texto6, White, Black);	
		aux13=flagContadorC;
	
		if(aux14!=flagContadorZ)
		squareButton(&DatPromedio, "      ", White, Blue);
		sprintf(texto7,"%4d",flagContadorZ);
    GUI_Text(DatPromedio.x + DatPromedio.size_x/2 - (strlen(texto7)/2)*8, DatPromedio.y + DatPromedio.size_y/2 - 8,
             (uint8_t*) texto7, White, Black);	
		aux14=flagContadorZ;
	}
	else
	{
		if (zonePressed(&zone_1))
					VelDer++;
		if (zonePressed(&zone_3))
					VelIzq++;
		if (zonePressed(&zone_5))
					Posserv++;
		if (zonePressed(&zone_2))
					VelDer--;
		if (zonePressed(&zone_4))
					VelIzq--;
		if (zonePressed(&zone_6))
					Posserv--;
		Dato_motorDer();
		Dato_motorIzq();
		Dato_motorServo();
	
	}
}
