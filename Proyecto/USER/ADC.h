#include <stdio.h>
#include <string.h>
#include <Math.h>
#include <LPC17xx.H>
#include <GLCD.h>
#include <TouchPanel.h>

void init_ADC_LDRs(void);
void init_ADC_grabar(void);
void ADC_IRQHandler(void);
void Audio(int Opcion);
