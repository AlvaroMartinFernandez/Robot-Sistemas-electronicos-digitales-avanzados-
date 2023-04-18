#ifndef __I2C_LPC17XX_H
#define __I2C_LPC17XX_H

void I2CSendAddr(unsigned char, unsigned char);
unsigned char I2CGetByte(unsigned char);
void I2CSendStop(void);
void I2CSendByte(unsigned char);
void I2Cdelay(void);

#endif
