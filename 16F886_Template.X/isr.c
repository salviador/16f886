#include "isr.h"
#include "varie.h"
#include "application.h"

extern uint32_t timer0_millis;
extern struct APP app;

void interrupt isr(void)
{   //1mS
    if(INTCONbits.T0IF){
        INTCONbits.T0IF = 0;
        timer0_millis++;
        TMR0 = 9;
    }
    if(PIR1bits.ADIF){
        PIR1bits.ADIF = 0;
        app.adc_flag = true;
    }
}