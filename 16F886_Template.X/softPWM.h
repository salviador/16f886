/* 
 * File:   softPWM.h
 * Author: michele
 *
 * Created on 9 maggio 2017, 16.26
 */

#ifndef SOFTPWM_H
#define	SOFTPWM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <stdint.h>
    #include <stdbool.h>
    #include "varie.h"
    
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    ////Qui carico le variabili che voglio salvare
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    struct SOFT_PWM{
        //Hardware Pin assigned
        unsigned char* port;
        unsigned char pin;
        //Public-Protect
        uint16_t ms_time_freq;
        uint16_t ms_time_ON;        
        uint16_t ms_time_OFF;        
        uint8_t percent_Pwm;
        //private
        uint32_t mstime;           
        bool needchange;
        bool statepwm;
    };
    
    void softPWM_init(struct SOFT_PWM* spwm, uint16_t freq);
    void softPWM_Set(struct SOFT_PWM* spwm, uint8_t perc);
    void softPWM_task(struct SOFT_PWM* spwm);

#ifdef	__cplusplus
}
#endif

#endif	/* SOFTPWM_H */

