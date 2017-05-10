#include "switch.h"


void pulsante_INIT_debounce(struct SWITCH* p) {
    //Read Pin
    uint8_t value, pinmask;
    pinmask = (1<<p->pin);
    value = *p->port & pinmask;
    
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
    uint8_t value, pinmask;
    pinmask = (1<<p->pin);
    value = *p->port & pinmask;

    switch(p->state_debounce){
        case 0:
            if(value != p->laststato){
                //Stato Cambiato stato
                p->timewaitdebounce = millis();
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
                        p->time_ONstate = millis();
                    }else{
                        p->State = false;
                        p->time_OFFstate = millis();
                    }
                }
                p->state_debounce = 0;
            }
        break;
    }
}