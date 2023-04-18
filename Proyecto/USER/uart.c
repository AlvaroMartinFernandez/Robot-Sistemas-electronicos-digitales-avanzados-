/* uart.c
 * contiene las funciones:

 1  UART0_IRQHandler(void)
 2  tx_cadena_UART0(char *ptr)
 3  uart0_set_baudrate(unsigned int baudrate) 
 4  uart0_init(int baudrate) 
  
 */
#include <LPC17xx.h>
#include "uart.h"


/*
 * UART0 interrupt handler
 */
void UART0_IRQHandler(void) {
	
    switch(LPC_UART0->IIR&0x0E) {
	
	case 0x04:								 /* RBR, Receiver Buffer Ready */
	*ptr_rx=LPC_UART0->RBR; 	   			 /* lee el dato recibido y lo almacena */
	    if (*ptr_rx++ ==13) 				// Caracter return --> Cadena completa
	    					{
							*ptr_rx=0;		/* Añadimos el caracter null para tratar los datos recibidos como una cadena*/ 
							rx_completa = 1;/* rx completa */
							ptr_rx=buffer;	/* puntero al inicio del buffer para nueva recepción */
	    					}
		break;
	
    
   case 0x02:								/* THRE, Transmit Holding Register empty */
		if (*ptr_tx!=0){
			LPC_UART0->THR=*ptr_tx++;	/* carga un nuevo dato para ser transmitido */
			tx_completa = 0;}
		else tx_completa=1;
		break;

    }
}

// Función para enviar una cadena de texto
// El argumento de entrada es la dirección de la cadena, o
// directamente la cadena de texto entre comillas
void tx_cadena_UART0(char *cadena)
{
ptr_tx=cadena;
tx_completa=0;
LPC_UART0->THR=*ptr_tx++;	 // IMPORTANTE: Introducir un carácter al comienzo para iniciar TX o
}							 // activar flag interrupción por registro transmisor vacio


static int uart0_set_baudrate(unsigned int baudrate) {
    int errorStatus = -1; //< Failure

    // UART clock (FCCO / PCLK_UART0)
   // unsigned int uClk = SystemCoreClock / 4;
    unsigned int uClk =SystemCoreClock/4;
    unsigned int calcBaudrate = 0;
    unsigned int temp = 0;

    unsigned int mulFracDiv, dividerAddFracDiv;
    unsigned int divider = 0;
    unsigned int mulFracDivOptimal = 1;
    unsigned int dividerAddOptimal = 0;
    unsigned int dividerOptimal = 0;

    unsigned int relativeError = 0;
    unsigned int relativeOptimalError = 100000;

    uClk = uClk >> 4; /* div by 16 */

    /*
     *  The formula is :
     * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)
     *
     * The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
     * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15
     */
    for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++) {
        for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++) {
            temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv);

            divider = temp / baudrate;
            if ((temp % baudrate) > (baudrate / 2))
                divider++;

            if (divider > 2 && divider < 65536) {
                calcBaudrate = temp / divider;

                if (calcBaudrate <= baudrate) {
                    relativeError = baudrate - calcBaudrate;
                } else {
                    relativeError = calcBaudrate - baudrate;
                }

                if (relativeError < relativeOptimalError) {
                    mulFracDivOptimal = mulFracDiv;
                    dividerAddOptimal = dividerAddFracDiv;
                    dividerOptimal = divider;
                    relativeOptimalError = relativeError;
                    if (relativeError == 0)
                        break;
                }
            }
        }

        if (relativeError == 0)
            break;
    }

    if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR) / 100)) {

        LPC_UART0->LCR |= DLAB_ENABLE; 	// importante poner a 1
        LPC_UART0->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);
        LPC_UART0->DLL = (unsigned char) dividerOptimal;
        LPC_UART0->LCR &= ~DLAB_ENABLE;	// importante poner a 0

        LPC_UART0->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);

        errorStatus = 0; //< Success
    }

    return errorStatus;
}
 					   					  
void uart0_init(int baudrate) {
    
    LPC_PINCON->PINSEL0|=(1<<4)|(1<<6);// Change P0.2 and P0.3 mode to TXD0 and RXD0
  
    LPC_UART0->LCR &= ~STOP_1_BIT & ~PARITY_NONE; // Set 8N1 mode (8 bits/dato, sin pariad, y 1 bit de stop)
    LPC_UART0->LCR |= CHAR_8_BIT;

    uart0_set_baudrate(baudrate);// Set the baud rate
    
     
    LPC_UART0->IER = THRE_IRQ_ENABLE|RBR_IRQ_ENABLE;// Enable UART TX and RX interrupt (for LPC17xx UART)   
    NVIC_EnableIRQ(UART0_IRQn);// Enable the UART interrupt (for Cortex-CM3 NVIC)
    
}

