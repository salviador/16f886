#include "varie.h"

uint32_t timer0_millis; // In the ISR

uint32_t millis(void){
	uint32_t m;
    INTCONbits.GIE = 0;     //Disable Global Intterrupt
    m = timer0_millis;
    INTCONbits.GIE = 1;     //Enable Global Intterrupt
    return m;
}
