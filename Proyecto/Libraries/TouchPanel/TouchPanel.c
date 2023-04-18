/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
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

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h"
#include "TouchPanel.h"

/** SSP status RX FIFO not empty bit */
#define SSP_SR_RNE      ((uint32_t)(1<<2))
/** SSP status SSP Busy bit */
#define SSP_SR_BSY      ((uint32_t)(1<<4))

/** SSP status RX FIFO not empty bit */
#define SSP_STAT_RXFIFO_NOTEMPTY	((uint32_t)(1<<2))
/** SSP status SSP Busy bit */
#define SSP_STAT_BUSY				((uint32_t)(1<<4))

/* Private variables ---------------------------------------------------------*/
Matrix matrix = {18270,-376710,69296370,509895,-21660,-2009881830,-5875458};
Coordinate  display ;

/* DisplaySample LCD struct */	
Coordinate ScreenSample[3];

/* LCD Calibration coordinates */
Coordinate DisplaySample[3] = {{45,45},{45,270}, {190,190}} ;

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 50 /* Be careful: this parameter should be tuned for your board 
                          to avoid measure glitches */

/*******************************************************************************
* Function Name  : TP_Init
* Description    : ADS7843 and SPI config
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void TP_Init(void) 
{ 
	//PINSEL_CFG_Type PinCfg;
//    SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.6  - TP_CS - used as GPIO
	 * P0.7  - TP_SCK
	 * P0.8  - TP_SDO
	 * P0.9  - TP_SDI
	 * P2.13 - TP_IRQ - used as GPIO
	 */
//	PinCfg.Funcnum = 2;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Portnum = 0;
//	PinCfg.Pinnum = 7;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 9;
//	PINSEL_ConfigPin(&PinCfg);
	
	
//	PinCfg.Funcnum = 0;
//	PinCfg.Portnum = 0;
//	PinCfg.Pinnum = 6;
//	PINSEL_ConfigPin(&PinCfg);

	LPC_PINCON->PINSEL0 &= ~(0x3F<<14 | 0x3<<12); // 00 -> PINSEL0 P0.6 .. P0.9
	LPC_PINCON->PINSEL0 |= 0x2A<<14; // P0.7 .. P0.9 -> SSP1


//	PinCfg.Funcnum = 0;
//	PinCfg.Portnum = 2;
//	PinCfg.Pinnum = 13;
//	PINSEL_ConfigPin(&PinCfg);

	LPC_PINCON->PINSEL4 &= ~(0x3<<26); // P2.13 -> GPIO



   /* P2.13 TP_IRQ is Input */
//   GPIO_SetDir(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM), 0);
//	GPIO_SetValue(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM));

	LPC_GPIO2->FIODIR &= ~(1<<13);
	LPC_GPIO2->FIOSET = 1<<13;
	
   /* P0.6 TP_CS is output */
//   GPIO_SetDir(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM), 1);
//	GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM)); 


	LPC_GPIO0->FIODIR |= 1<<6;
	LPC_GPIO0->FIOSET = 1<<6;

//	/* initialize SSP configuration structure to default */
//	SSP_ConfigStructInit(&SSP_ConfigStruct);
//	SSP_ConfigStruct.ClockRate = 500000;
//	/* Initialize SSP peripheral with parameter given in structure above */
//	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

//	/* Enable SSP peripheral */
//	SSP_Cmd(LPC_SSP1, ENABLE);
	
	LPC_SSP1->CR0 = 0x1807;
	LPC_SSP1->CR1 = 2;
} 

/*******************************************************************************
* Function Name  : DelayUS
* Description    : Delay used for SPI A/D converter
* Input          : - cnt: microseconds
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void DelayUS(uint32_t cnt)
{
  uint32_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12; /* Value for one microsecond delay*/    
     while (us--)     
     {
       ;   
     }
  }
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : ADS7843 Command Write
* Input          : - cmd: command
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
static uint8_t WR_CMD (uint8_t cmd)  
{ 
	/* wait for current SSP activity complete */
//	while (SSP_GetStatus(LPC_SSP1, SSP_STAT_BUSY) ==  SET);

	while(LPC_SSP1->SR & SSP_STAT_BUSY);

	//SSP_SendData(LPC_SSP1, (uint16_t) cmd);
	LPC_SSP1->DR =cmd;

	//while (SSP_GetStatus(LPC_SSP1, SSP_STAT_RXFIFO_NOTEMPTY) == RESET);
	
	while((LPC_SSP1->SR & SSP_STAT_RXFIFO_NOTEMPTY)==0);

	//return (SSP_ReceiveData(LPC_SSP1));
	return(LPC_SSP1->DR);
} 

/*******************************************************************************
* Function Name  : RD_AD
* Description    : ADC read (one measure)
* Input          : None
* Output         : None
* Return         : ADS7843 measure
* Attention		  : None
*******************************************************************************/
static int RD_AD(void)  
{ 
	unsigned short buf,temp; 
	
	temp = WR_CMD(0x00);
	buf = temp << 8; 
	DelayUS(1); 
	temp = WR_CMD(0x00);;
	buf |= temp; 
	buf >>= 3; 
	buf &= 0xfff; 
	return buf; 
} 

/*******************************************************************************
* Function Name  : Read_X
* Description    : Configuration of X coordinate and read ADC
* Input          : None
* Output         : None
* Return         : ADS7843 X value read
* Attention		  : None
*******************************************************************************/
int Read_X(void)  
{  
	int i; 
	TP_CS(0); 
	DelayUS(1); 
	WR_CMD(CHX); 
	DelayUS(1); 
	i = RD_AD(); 
	TP_CS(1); 
	return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Configuration of Y coordinate and read ADC
* Input          : None
* Output         : None
* Return         : ADS7843 Y value read
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
	int i; 
	TP_CS(0); 
	DelayUS(1); 
	WR_CMD(CHY); 
	DelayUS(1); 
	i = RD_AD(); 
	TP_CS(1); 
	return i;     
} 

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read X and Y
* Input          : None
* Output         : None
* Return         : ADS7843 X and Y measure 
* Attention		  : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
	int adx,ady; 
	adx = Read_X(); 
	DelayUS(1); 
	ady = Read_Y(); 
	*x = adx; 
	*y = ady; 
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : Write a 2x2 pixel red square
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
	LCD_SetPoint(Xpos,Ypos,0xf800);     
	LCD_SetPoint(Xpos+1,Ypos,0xf800);
	LCD_SetPoint(Xpos,Ypos+1,0xf800);
	LCD_SetPoint(Xpos+1,Ypos+1,0xf800);	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : Draw a white cross
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
	LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,0xffff);
	LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,0xffff);
	LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,0xffff);
	LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,0xffff);
	
	LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,RGB565CONVERT(184,158,131));
	LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,RGB565CONVERT(184,158,131));
	
	LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,RGB565CONVERT(184,158,131));
	LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,RGB565CONVERT(184,158,131));
	
	LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));
	LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));
	
	LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,RGB565CONVERT(184,158,131));
	LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,RGB565CONVERT(184,158,131));	  	
}	

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Make nine TouchPanel measures, evaluate if there are too
*                  much noise and return the more mean of the more similar measures
* Input          : None
* Output         : None
* Return         : 0 - if no touch detected or a two noise measure 
*                  * Coordinate - if touch detected with X,Y measures.
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};  
  do					       /* Reapeat for 9 measures if touch*/
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	 buffer[0][count]=TP_X[0];  
	 buffer[1][count]=TP_Y[0];
	 count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_IN is low level active if touch */
  
  if(count==9)   /* if 9 measures available¨ */ 
  {  
    /* Obtain three means of three X mesures each */
    temp[0]=(buffer[0][0] + buffer[0][1] + buffer[0][2])/3;
	 temp[1]=(buffer[0][3] + buffer[0][4] + buffer[0][5])/3;
	 temp[2]=(buffer[0][6] + buffer[0][7] + buffer[0][8])/3;
     
	 /* Calculate the distance of X centroids */
	 m0=temp[0]-temp[1];
	 m1=temp[1]-temp[2];
	 m2=temp[2]-temp[0];
     
	 /* Obtain de absolute value */
	 m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
	 m2=m2>0?m2:(-m2);
     
	/* return 0 if three X distances are bigger than THESHOLD. This means no touch */
	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
     
	/* Choose the two more similar centroids and obteain the mean. Choose the best X measures */ 
	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

    /* Obtain three means of three Y mesures each */
    temp[0]=(buffer[1][0] + buffer[1][1] + buffer[1][2])/3;
	 temp[1]=(buffer[1][3] + buffer[1][4] + buffer[1][5])/3;
	 temp[2]=(buffer[1][6] + buffer[1][7] + buffer[1][8])/3;
   
	 /* Calculate the distance of Y centroids */
	 m0=temp[0]-temp[1];
	 m1=temp[1]-temp[2];
	 m2=temp[2]-temp[0];
   
	 /* Obtain de absolute value */
	 m0=m0>0?m0:(-m0);
	 m1=m1>0?m1:(-m1);
	 m2=m2>0?m2:(-m2);
   
	/* return 0 if three Y distances are bigger than THESHOLD. This means no touch */
	 if(m0>THRESHOLD && m1>THRESHOLD && m2>THRESHOLD) return 0;

	/* Choose the two more similar centroids and obteain the mean. Choose the best Y measures */ 
	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

   /* Fill the return struct if a touch detected and return 0 if no tocuch */
	return &screen;
  }  
  return 0; 
}
	 
/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate the calibration metrix
* Input          : None
* Output         : None
* Return         : Calibration matrix
*                  
* Attention		  : None
*******************************************************************************/
uint8_t setCalibrationMatrix( Coordinate * displayPtr,
                              Coordinate * screenPtr,
                              Matrix * matrixPtr)
{

  uint8_t retTHRESHOLD = 0 ;
   
  /* Divider = (X0­X2)*(Y1­Y2) ­ (X1­X2)*(Y0­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
   
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = 1;
  }
  else
  {
    /* An = ((XD0­XD2)*(Y1­Y2) ­ (XD1­XD2)*(Y0­Y2))	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
     
	 /* Bn = ((X0­X2)*(XD1­XD2) ­ (XD0­XD2)*(X1­X2))	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
     
    /* Cn = (Y0*(X2*XD1 ­ X1*XD2) + Y1*(X0*XD2 ­ X2XD0) + Y2*(X1*XD0 ­ X0*XD1)) */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* Dn = ((YD0­YD2)*(Y1­Y2) ­ (YD1­YD2)*(Y0­Y2)) */
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
     
    /* En = ((X0­X2)*(YD1­YD2)­(YD0­YD2)*(X1­X2))	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
     
    /* Fn = (Y0*(X2YD1­X1YD2) + Y1(X0YD2­X2YD0) + Y2(X1YD0­X0YD1))    */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Translate AD measures to XY coordinates with calibration matrix
* Input          : None
* Output         : None
* Return         : 0 - No valid touch
*                  1 - Valid touch
*                  Coordinate struct with X and Y
* Attention		  : None
*******************************************************************************/
uint8_t getDisplayPoint(Coordinate * displayPtr,
                        Coordinate * screenPtr,
                        Matrix * matrixPtr )
{
  uint8_t retTHRESHOLD = 0 ;

  if( matrixPtr->Divider != 0 )
  {
    /* X = An*x + Bn*y + Cn */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* Y = Dn*x + En*y + Fn */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = 1;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : Draw three crosses, receive the calibration touches and 
*                  calculate calibration matrix
* Input          : None
* Output         : None
* Return         : None
* Attention		  : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
	uint8_t i;
	Coordinate * Ptr;
	
	for(i=0;i<3;i++)
	{
		LCD_Clear(Black);
		GUI_Text(10,10,"Touch crosshair to calibrate",0xffff,Black);
		DelayUS( 500 * 1000 );
		DrawCross(DisplaySample[i].x,DisplaySample[i].y);
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
	}
	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix ) ;     
	LCD_Clear(Black);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
