#include "softPWM.h"
#include "application.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "varie.h"
#include "main.h"


void softPWM_init(struct SOFT_PWM* spwm, uint16_t freq){
    spwm->ms_time_freq = 1000/freq;
    spwm->percent_Pwm = 0;
    softPWM_Set(spwm,0);
    spwm->mstime = millis();
}

void softPWM_Set(struct SOFT_PWM* spwm, uint8_t perc){
    float p,t;
    spwm->percent_Pwm = perc;
    p=perc/100;
    t=(float)spwm->ms_time_freq * p;
    spwm->ms_time_ON = (uint16_t)t;
    spwm->ms_time_OFF = spwm->ms_time_freq - spwm->ms_time_ON;
    
    spwm->needchange = true;
}

void softPWM_task(struct SOFT_PWM* spwm){
    //Write Pin
    uint8_t pinmask;

    if((spwm->needchange)&&(spwm->percent_Pwm==0)){
        //Set 0
        pinmask = ~(1<<spwm->pin);
        *spwm->port = *spwm->port & pinmask;
        spwm->needchange=false;
    }else if((spwm->needchange)&&(spwm->percent_Pwm==100)){
        //Set 1
        pinmask = (1<<spwm->pin);
        *spwm->port = *spwm->port | pinmask;
        spwm->needchange=false;
    }else{
        //modula
        if(spwm->statepwm){
            //se stai on
            if((millis()-spwm->mstime) >= spwm->ms_time_ON){
                spwm->mstime = millis();
                //go off
                //Set 0
                pinmask = ~(1<<spwm->pin);
                *spwm->port = *spwm->port & pinmask;
                spwm->statepwm = false;
            }
        }else{
            //se stai off
            if((millis()-spwm->mstime) >= spwm->ms_time_OFF){
                spwm->mstime = millis();
                //go on
                //Set 1
                pinmask = (1<<spwm->pin);
                *spwm->port = *spwm->port | pinmask;                
                spwm->statepwm = true;
            }
        }
    }    
}