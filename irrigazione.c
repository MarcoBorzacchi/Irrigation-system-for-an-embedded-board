/*
 * irrigazione.c
 */

#include "irrigazione.h"

#include "LPC17xx.h"
#include "type.h"
#include "timer.h"
#include "HD44780.h"

extern uint16_t sec_correnti,min_correnti,ore_correnti;

extern uint16_t inizio_oraA,inizio_minA;

extern uint16_t fine_oraA,fine_minA;

extern uint16_t inizio_oraB,inizio_minB;

extern uint16_t fine_oraB,fine_minB;

extern uint8_t current_state;

extern uint8_t rif_stato;

///////////////////////////////////////////////////////////////////////////////////////////////////
extern uint32_t tempo_fine_A; extern uint32_t tempo_inizio_A;
extern uint32_t tempo_fine_B; extern uint32_t tempo_inizio_B;
extern uint32_t tempo_irrigazione_A; extern uint32_t tempo_irrigazione_B;

extern uint32_t fine_effettiva_ora_A; extern uint32_t fine_effettiva_min_A;
extern uint32_t fine_effettiva_ora_B; extern uint32_t fine_effettiva_min_B;

extern uint8_t start_A; extern uint8_t start_B;
//////////////////////////////////////////////////////////////////////////////////////////////////

void visualizza_ora()
{
	PutCommand(RETURN_HOME);
	if(rif_stato == 0){
	Write_ndigitsval( ore_correnti, 2 );
	WriteChar(':');
    Write_ndigitsval0( min_correnti, 2 );
    WriteChar(':');
	Write_ndigitsval0( sec_correnti, 2 );

	    if(current_state == 1){
		GoToLine(2);
		WriteString((unsigned char*)"set ora corrente", 1);}
	}

	if(rif_stato == 4){
	Write_ndigitsval( inizio_oraA, 2 );
	WriteChar(':');
    Write_ndigitsval0( inizio_minA, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 5){
	Write_ndigitsval( fine_oraA, 2 );
	WriteChar(':');
    Write_ndigitsval0( fine_minA, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 6){
	Write_ndigitsval( inizio_oraB, 2 );
	WriteChar(':');
    Write_ndigitsval0( inizio_minB, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 7){
	Write_ndigitsval( fine_oraB, 2 );
	WriteChar(':');
    Write_ndigitsval0( fine_minB, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}
}

void ore_null(){
	PutCommand(RETURN_HOME);
	if(rif_stato == 0){
	WriteChar(' ');WriteChar(' ');
	WriteChar(':');
    Write_ndigitsval0( min_correnti, 2 );
    WriteChar(':');
	Write_ndigitsval0( sec_correnti, 2 );}

	if(rif_stato == 4){
	WriteChar(' ');WriteChar(' ');
	WriteChar(':');
    Write_ndigitsval0( inizio_minA, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 5){
	WriteChar(' ');WriteChar(' ');
	WriteChar(':');
    Write_ndigitsval0( fine_minA, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 6){
	WriteChar(' ');WriteChar(' ');
	WriteChar(':');
    Write_ndigitsval0( inizio_minB, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 7){
	WriteChar(' ');WriteChar(' ');
	WriteChar(':');
    Write_ndigitsval0( fine_minB, 2 );
    WriteChar(':');
	WriteChar('0');WriteChar('0');}
}

void min_null(){
	PutCommand(RETURN_HOME);
	if(rif_stato == 0){
	Write_ndigitsval( ore_correnti, 2 );
	WriteChar(':');
	WriteChar(' ');WriteChar(' ');
    WriteChar(':');
	Write_ndigitsval0( sec_correnti, 2 );}

	if(rif_stato == 4){
	Write_ndigitsval( inizio_oraA, 2 );
	WriteChar(':');
	WriteChar(' ');WriteChar(' ');
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 5){
	Write_ndigitsval( fine_oraA, 2 );
	WriteChar(':');
	WriteChar(' ');WriteChar(' ');
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 6){
	Write_ndigitsval( inizio_oraB, 2 );
	WriteChar(':');
	WriteChar(' ');WriteChar(' ');
    WriteChar(':');
	WriteChar('0');WriteChar('0');}

	if(rif_stato == 7){
	Write_ndigitsval( fine_oraB, 2 );
	WriteChar(':');
	WriteChar(' ');WriteChar(' ');
    WriteChar(':');
	WriteChar('0');WriteChar('0');}
}

void incrementa_tempo_A_B (int incr){
	uint32_t plus_tempo_irrigazione_A, plus_tempo_irrigazione_B;

	tempo_inizio_A = (inizio_oraA * 3600 ) + (inizio_minA * 60 );
    tempo_fine_A = (fine_oraA * 3600 ) + (fine_minA * 60 );
    tempo_inizio_B = (inizio_oraB * 3600 ) + (inizio_minB * 60 );
	tempo_fine_B = (fine_oraB * 3600 ) + (fine_minB * 60 );

	if (tempo_fine_A > tempo_inizio_A){
		tempo_irrigazione_A	= tempo_fine_A - tempo_inizio_A;}
	else
		tempo_irrigazione_A	= (24*3600 - tempo_inizio_A) + tempo_fine_A;


	if (tempo_fine_B > tempo_inizio_B){
		tempo_irrigazione_B	= tempo_fine_B - tempo_inizio_B;}
	else
		tempo_irrigazione_B	= (24*3600 - tempo_inizio_B) + tempo_fine_B;

	plus_tempo_irrigazione_A = (tempo_irrigazione_A*incr)/100;
	fine_effettiva_ora_A = plus_tempo_irrigazione_A/3600 + fine_oraA;
	if (fine_effettiva_ora_A >= 24){fine_effettiva_ora_A -= 24;}
	fine_effettiva_min_A = (plus_tempo_irrigazione_A%3600)/60 + fine_minA;

	plus_tempo_irrigazione_B = (tempo_irrigazione_B*incr)/100;
	fine_effettiva_ora_B = plus_tempo_irrigazione_B/3600 + fine_oraB;
	if (fine_effettiva_ora_B >= 24){fine_effettiva_ora_B -= 24;}
	fine_effettiva_min_B = (plus_tempo_irrigazione_B%3600)/60 + fine_minB;
}

void irriga_campo_A(void){
	if(fine_effettiva_ora_A >= inizio_oraA ){
		if(((ore_correnti == inizio_oraA && min_correnti >= inizio_minA
				|| ore_correnti > inizio_oraA)
			  && ore_correnti <= fine_effettiva_ora_A)
			|| start_A == 1){
			start_A = 1;
			LPC_GPIO0->FIOCLR = 1 << 22;
			if(ore_correnti >= fine_effettiva_ora_A){
				if (ore_correnti == fine_effettiva_ora_A &&
					min_correnti >= fine_effettiva_min_A){
				    start_A = 0;
				    LPC_GPIO0->FIOSET = 1 << 22;}

				else if(ore_correnti > fine_effettiva_ora_A){
					start_A = 0;
			        LPC_GPIO0->FIOSET = 1 << 22;}
			}
		}
		else
			LPC_GPIO0->FIOSET = 1 << 22;
	}

	if(fine_effettiva_ora_A < inizio_oraA ){
		if(((ore_correnti == inizio_oraA && min_correnti >= inizio_minA )
				|| ore_correnti > inizio_oraA)
				|| ore_correnti < inizio_oraA && ore_correnti <= fine_oraA
			|| start_A == 1){
			start_A = 1;
			LPC_GPIO0->FIOCLR = 1 << 22;
			if(ore_correnti < inizio_oraA &&
			   ore_correnti >= fine_effettiva_ora_A){
				if (ore_correnti == fine_effettiva_ora_A &&
					min_correnti >= fine_effettiva_min_A){
				    start_A = 0;
				    LPC_GPIO0->FIOSET = 1 << 22;}

				else if(ore_correnti > fine_effettiva_ora_A){
					start_A = 0;
			        LPC_GPIO0->FIOSET = 1 << 22;}
			}
		}
		else
			LPC_GPIO0->FIOSET = 1 << 22;
	}
}

void irriga_campo_B(void){
	if(fine_effettiva_ora_B >= inizio_oraB ){
		if(((ore_correnti == inizio_oraB && min_correnti >= inizio_minB
				|| ore_correnti > inizio_oraB)
			  && ore_correnti <= fine_effettiva_ora_B)
			|| start_B == 1){
			start_B = 1;
			LPC_GPIO3->FIOCLR = 1 << 26;
			if(ore_correnti >= fine_effettiva_ora_B){
				if (ore_correnti == fine_effettiva_ora_B &&
					min_correnti >= fine_effettiva_min_B){
				    start_B = 0;
				    LPC_GPIO3->FIOSET = 1 << 26;}

				else if(ore_correnti > fine_effettiva_ora_B){
					start_B = 0;
			        LPC_GPIO3->FIOSET = 1 << 26;}
			}
		}
		else
			LPC_GPIO3->FIOSET = 1 << 26;
	}

	if(fine_effettiva_ora_B < inizio_oraB ){
		if(((ore_correnti == inizio_oraB && min_correnti >= inizio_minB )
				|| ore_correnti > inizio_oraB)
				|| ore_correnti < inizio_oraB && ore_correnti <= fine_oraB
			|| start_B == 1){
			start_B = 1;
			LPC_GPIO3->FIOCLR = 1 << 26;
			if(ore_correnti < inizio_oraB &&
			   ore_correnti >= fine_effettiva_ora_B){
				if (ore_correnti == fine_effettiva_ora_B &&
					min_correnti >= fine_effettiva_min_B){
				    start_B = 0;
				    LPC_GPIO3->FIOSET = 1 << 26;}

				else if(ore_correnti > fine_effettiva_ora_B){
					start_B = 0;
			        LPC_GPIO3->FIOSET = 1 << 26;}
			}
		}
		else
			LPC_GPIO3->FIOSET = 1 << 26;
	}
}
