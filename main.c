/****************************************************************************
 *   $Id:: tmrtest.c 6098 2011-01-08 02:26:20Z nxp12832                      $
 *   Project: NXP LPC17xx Irrigation system
****************************************************************************/

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "LPC17xx.h"
#include "type.h"
#include "timer.h"
#include "HD44780.h"
#include "irrigazione.h"
#include "ADC.h"


//ora A
uint16_t inizio_oraA = 12; uint16_t inizio_minA = 0;
uint16_t fine_oraA = 13; uint16_t fine_minA = 0;
//ora B
uint16_t inizio_oraB = 5; uint16_t inizio_minB = 0 ;
uint16_t fine_oraB = 6; uint16_t fine_minB = 0;

//tempoA_B

uint32_t tempo_inizio_A; uint32_t tempo_fine_A;
uint32_t tempo_inizio_B; uint32_t tempo_fine_B;

uint32_t tempo_irrigazione_A; uint32_t tempo_irrigazione_B;

uint32_t fine_effettiva_ora_A; uint32_t fine_effettiva_min_A;
uint32_t fine_effettiva_ora_B; uint32_t fine_effettiva_min_B;

uint8_t start_A = 0;
uint8_t start_B = 0;
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t rif_stato;

uint16_t t = 0;

uint16_t t_max = 0;

uint8_t incremento = 0;

uint32_t count_sec = 0;
//////////////////////////////////////////////////////////////////////////////////////


//Global variables definition
uint8_t current_state = 0;
uint8_t key_pressed_event1 = 0; // set
uint8_t key_pressed_event2 = 0; // mode

uint8_t lampeggio = 0;

uint16_t sec_correnti,min_correnti,ore_correnti = 0;
uint16_t sec_quarti = 0;

/* Routine di gesione dell'interrupt esterno
 * EINT3 è in coumune con l'interrupt generato dalla GPIO.
 * Gli if consentono di individiare la causa dell'interruzione */


void TIMER1_IRQHandler (void)
{

  if ( LPC_TIM1->IR & (0x1<<0) )
  {
    LPC_TIM1->IR = 0x1<<0;

    if(current_state==1||current_state==2){
    lampeggio=!lampeggio;
    sec_quarti++;
        if(sec_quarti == 4){
    	    sec_quarti = 0;
    	    sec_correnti++;}
   }

    else{
    	sec_correnti++;}

    if(sec_correnti==60){
    min_correnti++,sec_correnti=0;}
    if(min_correnti==60){
    ore_correnti++,min_correnti=0;}
    if(ore_correnti==24){
    ore_correnti=0;}
  }
}

void EINT3_IRQHandler(void)
{
   if(LPC_GPIOINT->IO0IntStatF & (1<<9)) /* è stato rilevato un fronte negativo su P0.3 */
   {
      key_pressed_event1 = 1; //segnalamento pressione tasto
      delayMs(30); // preve pausa antirimbalzo
      LPC_GPIOINT->IO0IntClr=(1<<9);
   }
   if(LPC_GPIOINT->IO0IntStatF & (1<<8)) /* è stato rilevato un fronte negativo su P0.3 */
   {
      key_pressed_event2 = 1; //segnalamento pressione tasto
      delayMs(30); // preve pausa antirimbalzo
      LPC_GPIOINT->IO0IntClr=(1<<8);
   }
}


/* STATI: */
void state_0()
{
	LPC_TIM1->MR0 = SystemFrequency / PCLKDIV;
	rif_stato = 0;

    visualizza_ora();
    GoToLine(2);
    Write_ndigitsval( t, 2 );
    WriteChar( 0xDF ); WriteChar( 'C' );


	if(key_pressed_event1)		//if input, change state
	{
		//here you can write output as a function of input and state (Mealy machine)
		LPC_TIM1->TCR = 0x00;   //stop
		LPC_TIM1->TCR = 0x02;   //and reset timer
		current_state = 1;		//This is the next state
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event1 = 0;	//event acquired -> reset it
	}

		if(key_pressed_event2){
		current_state = 3;
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event2 = 0;}

	LPC_TIM1->TCR = 0x01;
}

void state_1()
{
	LPC_TIM1->MR0 = SystemFrequency / PCLKDIV / 4; //per lampeggiare più velocemente
	LPC_TIM1->TCR = 0x01;

	if(lampeggio)
		ore_null();

	else
		visualizza_ora();

	if(key_pressed_event1)
	{
		current_state = 2;
		key_pressed_event1 = 0;
	}

	if(key_pressed_event2)
	{
		if(rif_stato == 0){
		ore_correnti++;
		if (ore_correnti == 24)
			ore_correnti=0;}

		if(rif_stato == 4){
		inizio_oraA++;
		if (inizio_oraA == 24)
			inizio_oraA=0;}

		if(rif_stato == 5){
		fine_oraA++;
		if (fine_oraA == 24)
			fine_oraA=0;}

		if(rif_stato == 6){
		inizio_oraB++;
		if (inizio_oraB == 24)
			inizio_oraB=0;}

		if(rif_stato == 7){
		fine_oraB++;
		if (fine_oraB == 24)
			fine_oraB=0;}

		key_pressed_event2 = 0;
	}
}

void state_2()
{
	if(lampeggio)
		min_null();

	else
		visualizza_ora();

	if(key_pressed_event1)
	{
		LPC_TIM1->TCR = 0x00;    //stop
		LPC_TIM1->TCR = 0x02;    //and reset timer
		current_state = rif_stato+1;
		if(rif_stato == 0){current_state = 3;}
		if(rif_stato == 7){current_state = 0;}
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event1 = 0;
	}

	if(key_pressed_event2)
	{
		if(rif_stato == 0){
		min_correnti++;
		if (min_correnti == 60)
			min_correnti=0;}

		if(rif_stato == 4){
		inizio_minA++;
		if (inizio_minA == 60)
			inizio_minA=0;}

		if(rif_stato == 5){
		fine_minA++;
		if (fine_minA == 60)
			fine_minA=0;}

		if(rif_stato == 6){
		inizio_minB++;
		if (inizio_minB == 60)
			inizio_minB=0;}

		if(rif_stato == 7){
		fine_minB++;
		if (fine_minB == 60)
			fine_minB=0;}

		key_pressed_event2 = 0;
	}
}

void state_3()
{
	LPC_TIM1->MR0 = SystemFrequency / PCLKDIV;
	rif_stato = 3;

	PutCommand(RETURN_HOME);
	WriteChar( 'T' );
	WriteChar( 'm' );	WriteChar( 'a' ); WriteChar( 'x' );
	WriteChar( ':' ); WriteChar( ' ' );
	Write_ndigitsval( t_max, 2 );
	WriteChar( 0xDF ); WriteChar( 'C' );


	if(key_pressed_event1){		//if input, change state
	key_pressed_event1 = 0;}

	if(key_pressed_event2){
	current_state = 4;
	PutCommand(DISPLAY_CLEAR);
	key_pressed_event2 = 0;}

	LPC_TIM1->TCR = 0x01;
}

void state_4()
{
	rif_stato = 4;

    visualizza_ora();
    GoToLine(2);
    WriteString((unsigned char*)"Inizio A", 1);

	if(key_pressed_event1)		//if input, change state
	{
		//here you can write output as a function of input and state (Mealy machine)
		LPC_TIM1->TCR = 0x00;   //stop
		LPC_TIM1->TCR = 0x02;   //and reset timer
		current_state = 1;		//This is the next state
		key_pressed_event1 = 0;	//event acquired -> reset it
	}

		if(key_pressed_event2){
		current_state = 5;
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event2 = 0;}

}

void state_5()
{
	rif_stato = 5;

    visualizza_ora();
    if(incremento == 1){
    	WriteChar(' ');WriteChar('+');WriteChar('1');WriteChar('0');WriteChar('%');}
    if(incremento == 2){
    	WriteChar(' ');WriteChar('+');WriteChar('2');WriteChar('0');WriteChar('%');}
    GoToLine(2);
    WriteString((unsigned char*)"Fine A", 1);


	if(key_pressed_event1)		//if input, change state
	{
		//here you can write output as a function of input and state (Mealy machine)
		LPC_TIM1->TCR = 0x00;   //stop
		LPC_TIM1->TCR = 0x02;   //and reset timer
		current_state = 1;		//This is the next state
		key_pressed_event1 = 0;	//event acquired -> reset it
	}

		if(key_pressed_event2){
		current_state = 6;
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event2 = 0;}

}

void state_6()
{
	rif_stato = 6;

    visualizza_ora();
    GoToLine(2);
    WriteString((unsigned char*)"Inizio B", 1);

	if(key_pressed_event1)		//if input, change state
	{
		//here you can write output as a function of input and state (Mealy machine)
		LPC_TIM1->TCR = 0x00;   //stop
		LPC_TIM1->TCR = 0x02;   //and reset timer
		current_state = 1;		//This is the next state
		key_pressed_event1 = 0;	//event acquired -> reset it
	}

		if(key_pressed_event2){
		current_state = 7;
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event2 = 0;}

}

void state_7()
{
	rif_stato = 7;

    visualizza_ora();
    if(incremento == 1){
    	WriteChar(' ');WriteChar('+');WriteChar('1');WriteChar('0');WriteChar('%');}
    if(incremento == 2){
    	WriteChar(' ');WriteChar('+');WriteChar('2');WriteChar('0');WriteChar('%');}
    GoToLine(2);
    WriteString((unsigned char*)"Fine B", 1);

	if(key_pressed_event1)		//if input, change state
	{
		//here you can write output as a function of input and state (Mealy machine)
		LPC_TIM1->TCR = 0x00;   //stop
		LPC_TIM1->TCR = 0x02;   //and reset timer
		current_state = 1;		//This is the next state
		key_pressed_event1 = 0;	//event acquired -> reset it
	}

		if(key_pressed_event2){
		current_state = 0;
		PutCommand(DISPLAY_CLEAR);
		key_pressed_event2 = 0;}

}


/*
 * Tabella dei possibili stati:
 * la tabella punta alle funzioni in elenco (stati)
 */
void(*state_table[])(void) = {state_0, state_1, state_2,state_3, state_4, state_5,state_6, state_7}; /* l'asterisco indica che
l'indirizzo è di una funzione, è un puntatore a funzione*/

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{
	uint32_t i;
	/* SystemClockUpdate() updates the SystemFrequency variable */
	SystemClockUpdate();

	i = SystemFrequency;

	uint8_t n = 3;

	//Enable interrupt
	LPC_GPIOINT->IO0IntEnF =(1<<9)+(1<<8);
	NVIC_EnableIRQ(EINT3_IRQn);

    LPC_GPIO0->FIODIR = 1 << 22;
    LPC_GPIO3->FIODIR = 1 << 26;
    LPC_GPIO0->FIOSET = 1 << 22;
    LPC_GPIO3->FIOSET = 1 << 26;

	InitLCD(); //inizializza LCD

	ADC_Init(5);

	visualizza_ora();

	  LPC_TIM1->TCR = 0x02;	         /* reset timer */
	  LPC_TIM1->PR  = 0x00;	         /* set prescaler to 0 */
	  LPC_TIM1->MCR = (1<<0|1<<1);
	  LPC_TIM1->IR  = 0x0F;		/* reset all interrrupts */
      NVIC_EnableIRQ(TIMER1_IRQn);


	/* Loop forever */
	while(1)
	{
		t = 0;
		for( i=0; i < (1<<n) ; i++)
		{
			t += ADC_Read(5);
		}
		t >>= n; //media su 2^n valori: 0 - 4095
		// fondo scala: 2297

		t = (t*5000) >> 12 ;
		t = ((t/10)+5)/10;
		if(t > t_max){t_max = t;}

		state_table[current_state]();

		if(t_max > 30 && t_max <=35){
			incremento = 1;
			incrementa_tempo_A_B (10);}

		if(t_max > 35 ){
			incremento = 2;
			incrementa_tempo_A_B (20);}

		if(t_max <= 30 ){
			fine_effettiva_ora_A = fine_oraA;
		    fine_effettiva_min_A = fine_minA;
			fine_effettiva_ora_B = fine_oraB;
			fine_effettiva_min_B = fine_minB;}

		irriga_campo_A();
		irriga_campo_B();

		if((fine_effettiva_ora_A >= 8) && (inizio_oraA < 8)){
			if ((ore_correnti == fine_effettiva_ora_A)&&(min_correnti == fine_effettiva_min_A)){
				t_max = 0;
				incremento = 0;}

		    else{;}
		  }

		else if((fine_effettiva_ora_B >= 8) && (inizio_oraB < 8)){
			if ((ore_correnti == fine_effettiva_ora_B)&&(min_correnti == fine_effettiva_min_B)){
				t_max = 0;
				incremento = 0;}

		    else{;}
		  }

		else if(ore_correnti == 8 && min_correnti == 0){
			t_max = 0;
			incremento = 0;
		}
	}

	//se arriva qui c'è stato un crash!!
	return 0;
}
/*****************************************************************************
**                            End Of File
******************************************************************************/
