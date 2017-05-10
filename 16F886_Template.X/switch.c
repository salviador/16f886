#include "switch.h"


void pulsante_INIT_debounce(struct SWITCH* p) {
    //Read Pin   
#ifdef __ICCARM__
    uint8_t value;
    value = HAL_GPIO_ReadPin(p->port, p->pin);
#endif        
#ifdef __XC
    uint8_t value, pinmask;
    pinmask = (1<<p->pin);
    value = *p->port & pinmask;
#endif       
    
    p->state_debounce = 0;
    p->laststato = value;
    p->timewaitdebounce = 0;
    p->changeState = false;
    if(value){
        p->State = true;
    }else{
        p->State = false;
    }
}


void pulsanti_alldebounce_task(struct SWITCH* switchs[], uint8_t size) {
    uint8_t i;
   // struct SWITCH *v;
    for(i=0;i<size; i++){
         //v = switchs[i];
        pulsante_debounce(switchs[i]);//*v);
    }
}

//Debounce
void pulsante_debounce(struct SWITCH* p){
    //Read Pin
#ifdef __ICCARM__
    uint8_t value;
    value = HAL_GPIO_ReadPin(p->port, p->pin);
#endif        
#ifdef __XC
    uint8_t value, pinmask;
    pinmask = (1<<p->pin);
    value = *p->port & pinmask;
#endif       
    switch(p->state_debounce){
        case 0:
            if(value != p->laststato){
                //Stato Cambiato stato
                #ifdef __ICCARM__
                    p->timewaitdebounce = HAL_GetTick();
                #endif        
                #ifdef __XC
                    p->timewaitdebounce = millis();
                #endif                                 
                p->state_debounce = 1;
                //p.laststato = value;
            }
        break;
        case 1:
            if((millis() - p->timewaitdebounce)>=DELAY_DEBOUNCE){
                if(value != p->laststato){
                    p->laststato = value;
                    //Notifica Cambio Stato
                    p->changeState = true;
                    if(value){
                        p->State = true;
                        #ifdef __ICCARM__
                            p->time_ONstate = HAL_GetTick();
                        #endif        
                        #ifdef __XC
                            p->time_ONstate = millis();
                        #endif                                 
                        
                    }else{
                        p->State = false;
                        #ifdef __ICCARM__
                            p->time_OFFstate = HAL_GetTick();
                        #endif        
                        #ifdef __XC
                            p->time_OFFstate = millis();
                        #endif                                                         
                    }
                }
                p->state_debounce = 0;
            }
        break;
    }
}