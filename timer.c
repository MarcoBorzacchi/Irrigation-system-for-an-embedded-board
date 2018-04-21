/*
 * timer.c
 *
 *  Created on: 10 apr 2016
 *      Author: stefo
 */

#include "LPC17xx.h"
#include "type.h"
#include "timer.h"

extern uint8_t sample;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
  if ( LPC_TIM0->IR & (0x1<<0) )
  {
    LPC_TIM0->IR = 0x1<<0;		/* clear interrupt flag */

	  // segnala tick
	  // sample = 1;
  }
  return;
}

/******************************************************************************
** Function name:		delayMs
**
** Descriptions:		Timer used to generate a pause in ms
**
** parameters:			delayInMs (pause value in ms)
** Returned value:		None
**
******************************************************************************/
void delayMs(uint32_t delayInMs)
{
  LPC_TIM0->TCR = 0x02;		/* reset timer */
  LPC_TIM0->PR  = 0x00;		/* set prescaler to zero */
  LPC_TIM0->MR0 = delayInMs * (SystemFrequency / PCLKDIV / 1000); //pclk = 18MHz
  LPC_TIM0->IR  = 0x0F;		/* reset all interrrupts */
  LPC_TIM0->MCR = 0x04;		/* stop timer on match */
  LPC_TIM0->TCR = 0x01;		/* start timer */

  /* wait until delay time has elapsed */
  while (LPC_TIM0->TCR & 0x01);

  return;
}

/******************************************************************************
** Function name:		delayUs
**
** Descriptions:		Timer used to generate a pause in us
**
** parameters:			delayInUs (pause value in us)
** Returned value:		None
**
******************************************************************************/
void delayUs(uint32_t delayInUs)
{
  LPC_TIM0->TCR = 0x02;		/* reset timer */
  LPC_TIM0->PR  = 0x00;		/* set prescaler to zero */
  LPC_TIM0->MR0 = delayInUs * (SystemFrequency / PCLKDIV / 1000000); //pclk = 18MHz
  LPC_TIM0->IR  = 0x0F;		/* reset all interrrupts */
  LPC_TIM0->MCR = 0x04;		/* stop timer on match */
  LPC_TIM0->TCR = 0x01;		/* start timer */

  /* wait until delay time has elapsed */
  while (LPC_TIM0->TCR & 0x01);

  return;
}
