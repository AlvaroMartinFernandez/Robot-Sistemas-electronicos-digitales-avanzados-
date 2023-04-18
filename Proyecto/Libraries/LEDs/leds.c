#include <LPC17xx.h>
#include "leds.h"


#ifndef CMSIS
   void configLEDs(void)
   {	
	    LPC_PINCON->PINSEL7 = (LPC_PINCON->PINSEL7 & (~(3 << 18))) | (0 << 18);
      LPC_GPIO3->FIODIR |= (1<<25);

      LPC_PINCON->PINSEL7 = (LPC_PINCON->PINSEL7 & (~(3 << 20))) | (0 << 20);
      LPC_GPIO3->FIODIR |= (1<<26);
   }

#else

  void configLEDs(void)
  {
  	/* Configure the LCD Control pins */
    PINSEL_CFG_Type PinCfg;	
	
	  PinCfg.Funcnum = 0;
	  PinCfg.OpenDrain = 0;
	  PinCfg.Pinmode = 0;
	  PinCfg.Portnum = 3;
	  PinCfg.Pinnum = 25;
	  PINSEL_ConfigPin(&PinCfg);
		
	  PinCfg.Funcnum = 0;
	  PinCfg.OpenDrain = 0;
	  PinCfg.Pinmode = 0;
	  PinCfg.Portnum = 3;
	  PinCfg.Pinnum = 26;
	  PINSEL_ConfigPin(&PinCfg);
		
	  GPIO_SetDir(3, (1<<26) | (1<<25), 1);
  }

#endif



