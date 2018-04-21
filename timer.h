/*
 * timer.h
 *
 *  Created on: 10 apr 2016
 *      Author: stefo
 */

#ifndef TIMER_H_
#define TIMER_H_

/* set PCLKDIV as CCLK/PCLK ratio ------------------------------*/
#if PCLKSEL0_Val==0
#define PCLKDIV 4
#endif

#if PCLKSEL0_Val==1
#define PCLKDIV 1
#endif

#if PCLKSEL0_Val==2
#define PCLKDIV 2
#endif

#if PCLKSEL0_Val==3
#define PCLKDIV 8
#endif

void TIMER0_IRQHandler (void);

void delayMs(uint32_t delayInMs);
void delayUs(uint32_t delayInUs);


#endif /* TIMER_H_ */
