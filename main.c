#include <uart.h>
#include <stdlib.h>
#include <util/delay.h>
#include "hcsr04.h"

#ifndef F_CPU
#define F_CPU 1000000
#endif


int main(void){
	uart_init();
	hcsr04_init();
	float dist = -1;
	char str[10];

	while(1){
		dist = hcsr04_get_distance();
		sprintf(str,"%f",dist);
		uart_put_s(str);
		uart_put_c(' ');
		_delay_ms(500);
	}
	return 0;
}
