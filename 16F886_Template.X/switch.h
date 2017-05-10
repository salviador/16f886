/* 
 * File:   switch.h
 * Author: michele
 * Version : 1.00
 * Created on 7 maggio 2017, 16.05
 */

#ifndef SWITCH_H
#define	SWITCH_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>
#include "varie.h"
    
#define DELAY_DEBOUNCE 30
    
    struct SWITCH{
        //Hardware Pin assigned
        unsigned char* port;
        unsigned char pin;
        //Debounce variable     
        uint8_t state_debounce;
        uint32_t timewaitdebounce;
        uint8_t laststato;
    
        //Signal
        bool changeState;       //true a ogni cambio di stato, AZZERARE DA SOFTWARE !!!
        bool State;             //Stato dello switch, ON o OFF;
        uint32_t time_ONstate;  //start time state on
        uint32_t time_OFFstate; //start time state off       
    };
    
    void pulsante_INIT_debounce(struct SWITCH* p);
    void pulsanti_alldebounce_task(struct SWITCH* switchs[], uint8_t size);
    void pulsante_debounce(struct SWITCH* p);



#ifdef	__cplusplus
}
#endif

#endif	/* SWITCH_H */

