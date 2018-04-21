/*
 * adc.c
 *
 *  Created on: 12/feb/2015
 *      Author: stefo
 */

#include "LPC17xx.h"
#include "type.h"
#include "timer.h"

/*****************************************************************************
** Function name:		ADC_Init
**
** Descriptions:		Initialize ADC for software controlled conversion
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void ADC_Init(uint8_t channel)
{
	/* to be updated: now only channels n. 4 and 5 */

    LPC_SC->PCONP |= (1 << 12);           //ADC on

    LPC_ADC->CR   =
         ( 0x1 << channel ) |  /* sel ADC0.channel */
		 ((4 - 1) << 8) |      /* set div 4 -> 4.5 MHz */
		 ( 0x0 << 16 ) |       /* BURST = 0, no BURST, software controlled */
		 ( 0x1 << 21 ) |       /* ADC oparational */
		 ( 0x0 << 24 ) |       /* START = 0 A/D conversion stops */
		 ( 0x0 << 27 ) ;       /* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */;

    if(channel == 4)
    {
        LPC_PINCON->PINSEL3 &= 0xCFFFFFFF;  //reset b28 e b29
        LPC_PINCON->PINSEL3 |= 0x30000000;  //set ADC0.4 at P1.30

        LPC_PINCON->PINMODE3 &= 0xCFFFFFFF; //reset b28 e b29
        LPC_PINCON->PINMODE3 |= 0x20000000; //nessun pull-up o pull-down a P1.30
    }

    if(channel == 5)
    {
        LPC_PINCON->PINSEL3 &= 0x3FFFFFFF;  //reset b30 e b31
        LPC_PINCON->PINSEL3 |= 0xC0000000;  //set ADC0.5 at P1.31

        LPC_PINCON->PINMODE3 &= 0x3FFFFFFF; //reset b30 e b31
        LPC_PINCON->PINMODE3 |= 0x80000000; //nessun pull-up o pull-down a P1.31
    }
}

/*****************************************************************************
** Function name:		ADC_Read
**
** Descriptions:		Start a software controlled conversion
** 						and return the value for specified channel
**
** parameters:			channel: channel number to be acquired
**
** Returned value:		adcValue: ADC output
**
*****************************************************************************/
uint32_t ADC_Read(uint8_t channel)
{
	uint32_t adcValue;

	LPC_ADC->CR &= 0xF0FFFF00;
    LPC_ADC->CR |= (1 << channel);   //select channel
    LPC_ADC->CR |= (1 << 24);  // start conversion

    //aspetta fine conversione con Global Register
    adcValue = LPC_ADC->GDR;
    while ((adcValue & 0x80000000) == 0) //aspetta finché b31 (DONE) non va a 1
    {
    	adcValue = LPC_ADC->GDR;
    }

    adcValue = ( LPC_ADC->GDR >> 4 ) & 0xFFF;

    return (adcValue);
}


