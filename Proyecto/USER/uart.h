/*
 * uart_.h
 *
 *  Created on: 1-Oct-2011
 *  Author: J.M.V.C.
 */

#ifndef UART_H_
#define UART_H_

// Accepted Error baud rate value (in percent unit)
#define UART_ACCEPTED_BAUDRATE_ERROR    3

#define CHAR_8_BIT                      (3 << 0)
#define STOP_1_BIT                      (0 << 2)
#define PARITY_NONE                     (0 << 3)
#define DLAB_ENABLE                     (1 << 7)
#define FIFO_ENABLE                     (1 << 0)
#define RBR_IRQ_ENABLE                  (1 << 0)
#define THRE_IRQ_ENABLE                 (1 << 1)
#define UART_LSR_THRE   				        (1 << 5)
#define RDA_INTERRUPT                   (2 << 1)
#define CTI_INTERRUPT                   (6 << 1)

extern char buffer[30];	// Buffer de recepción
extern char *ptr_rx;	// puntero de recepción
extern char rx_completa;// Flag de recepción de cadena completa que se activa al recibir CR(0x0D)
extern char *ptr_tx;			// puntero de transmisión
extern char tx_completa;		// Flag de transmisión de cadena completa

extern void uart0_init(int baudrate);
extern void tx_cadena_UART0(char *ptr);

#endif /* UART_H_ */
