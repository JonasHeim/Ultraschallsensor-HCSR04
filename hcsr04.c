#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hcsr04.h"

static int toggle_flanks = -1;
static float distance = -1;
static uint16_t temp1 = -1;
static uint16_t temp2 = -1;



int hcsr04_init(){
	int status = -1;

	//DDRs
	HCSR04_PORT_DDR |= (1<<HCSR04_TRIGG);
	HCSR04_PORT_DDR &= ~(1<<HCSR04_ECHO);

	//external interrupt on INT0 on rising edge
	MCUCR |= (1<<ISC01) | (1<<ISC00);

	//enable INT0 interrupt
	GICR |= (1<<INT0);

	sei();

	return status;
}

float hcsr04_get_distance(){
	distance = -1;

	//reset timer
	TCNT1 = 0x0000;
	//start timer, no prescaler
	TCCR1B |= (1<<CS10);

	//exspecting rising edge
	toggle_flanks = 1;

	//send trigger
	hcsr04_start();

	_delay_ms(20);

	/*
	 * Berechne Abstand
	 * ( Zeit(ms)*32,4(cm) )/2 = Abstand(cm)
	 *
	 */

	distance = (abs(temp1-temp2)*0.343F)/2;

	temp1 = 0;
	temp2 = 0;

	return distance;
}

void hcsr04_start(){
	HCSR04_PORT |= (1<<HCSR04_TRIGG);
	_delay_us(10);
	HCSR04_PORT &= ~(1<<HCSR04_TRIGG);
}

void hcsr04_stop(){
	HCSR04_PORT &= ~(1<<HCSR04_TRIGG);
	_delay_us(10);
	HCSR04_PORT |= (1<<HCSR04_TRIGG);
}

//Interrupt-Routine für Echo-Signal INT0
ISR(INT0_vect){
	//Low to High
	if(toggle_flanks){
		//read timer
		temp1 = TCNT1;

		//exspecting falling edge
		MCUCR &= ~(1<<ISC00);
		MCUCR |= (1<<ISC01);

		toggle_flanks = 0;


	}
	//High to Low
	else{
		//read timer
		temp2 = TCNT1;

		//exspecting rising edge
		MCUCR |= (1<<ISC01) | (1<<ISC00);

	}
}
