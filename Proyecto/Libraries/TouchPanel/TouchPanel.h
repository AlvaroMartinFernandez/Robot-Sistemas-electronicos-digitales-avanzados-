/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
//#include "lpc17xx_ssp.h"
//#include "lpc17xx_gpio.h"
//#include "lpc17xx_pinsel.h"

#include <LPC17xx.H>

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/
/* ADS7846 configuration commands */
#define	CHX 	        0x90 	/* CH_X 12 bits Differential measure command*/	
#define	CHY 	        0xd0	/* CH_Y 12 bits Differential measure command*/

/* Physical level marcos */
/* PORT number that TP_IRQ pin assigned on */
#define TP_IRQ_PORT_NUM		2
/* PIN number that  TP_IRQ pin assigned on */
#define TP_IRQ_PIN_NUM		13
/* PORT number that TP_CS pin assigned on */
#define TP_CS_PORT_NUM		0
/* PIN number that  TP_CS pin assigned on */
#define TP_CS_PIN_NUM		6

//#define TP_CS(x)   ((x) ? GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM) ) : GPIO_ClearValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM) ) );
#define TP_CS(x)	((x) ? (LPC_GPIO0->FIOSET = (1<<TP_CS_PIN_NUM)) : (LPC_GPIO0->FIOCLR = (1<<TP_CS_PIN_NUM)));

//#define TP_INT_IN   (GPIO_ReadValue(TP_IRQ_PORT_NUM)&(1<<TP_IRQ_PIN_NUM))
#define TP_INT_IN   ( LPC_GPIO2->FIOPIN & (1<<TP_IRQ_PIN_NUM) ) 

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
uint8_t setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
uint8_t getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
