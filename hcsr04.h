/*
 * hcsr04.h
 *
 *  Created on: 20.11.2015
 *      Author: forcexx
 */

#include <avr/io.h>

#ifndef HCSR04_H_
#define HCSR04_H_

#define HCSR04_PORT PORTD
#define HCSR04_PORT_DDR DDRD


#define HCSR04_TRIGG 	PD3
#define HCSR04_ECHO		PD2


//Prototypes

int hcsr04_init();
float hcsr04_get_distance();
void hcsr04_start();
void hcsr04_stop();

#endif /* HCSR04_H_ */
