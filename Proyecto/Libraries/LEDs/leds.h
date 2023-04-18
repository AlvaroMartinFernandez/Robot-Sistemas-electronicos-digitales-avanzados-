#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>

#define CMSIS


#ifndef CMSIS
   #define LED1_ON   LPC_GPIO3->FIOCLR = (1<<25)
   #define LED1_OFF  LPC_GPIO3->FIOSET = (1<<25)
   #define LED2_ON   LPC_GPIO3->FIOCLR = (1<<26)
   #define LED2_OFF  LPC_GPIO3->FIOSET = (1<<26)
#else
   #define LED1_ON   GPIO_ClearValue(3,1<<25)
   #define LED1_OFF  GPIO_SetValue(3, 1<<25)
   #define LED2_ON   GPIO_ClearValue(3,1<<26)
   #define LED2_OFF  GPIO_SetValue(3, 1<<26)
#endif

void configLEDs(void);
