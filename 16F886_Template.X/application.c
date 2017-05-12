#include "application.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "switch.h"
#include "varie.h"
#include "main.h"
#include "saveEEPROM.h"
#include "softPWM.h"

//Object
struct SWITCH P1_ROSSO;
struct SWITCH P2_BLU;
struct SWITCH P3_BLU;
struct SWITCH P4_GIALLO;
struct SWITCH *pulsanti[] = {&P1_ROSSO, &P2_BLU, &P3_BLU, &P4_GIALLO};

struct APP app;

struct OUTPUT POWER_ONLED;
struct OUTPUT output_P1;
struct OUTPUT output_P2;
struct OUTPUT output_P3;
struct OUTPUT output_P4;
struct OUTPUT *outputs[] = {&output_P1, &output_P2, &output_P3, &output_P4};

extern struct EEP_VAR eepvar;  //Qui carico le variabili che voglio salvare

struct SOFT_PWM soft_pwm;

void switch_init(void){
  WPUBbits.WPUB7 = 0;    
  WPUBbits.WPUB6 = 0;      
  P1_ROSSO.port = &PORTB;
  P1_ROSSO.pin = 0;
  TRISBbits.TRISB0 = 1;
  WPUBbits.WPUB0 = 1;      
  P2_BLU.port = &PORTB;
  P2_BLU.pin = 1;
  TRISBbits.TRISB1 = 1;  
  WPUBbits.WPUB1 = 1;      
  P3_BLU.port = &PORTB;
  P3_BLU.pin = 2;
  TRISBbits.TRISB2 = 1;    
  WPUBbits.WPUB2 = 1;      
  P4_GIALLO.port = &PORTB;
  P4_GIALLO.pin = 3;
  TRISBbits.TRISB3 = 1;    
  WPUBbits.WPUB3 = 1;      
  OPTION_REGbits.nRBPU = 0;           //For Debug
  __delay_ms(5);
  pulsante_INIT_debounce(&P1_ROSSO);
  pulsante_INIT_debounce(&P2_BLU);
  pulsante_INIT_debounce(&P3_BLU);
  pulsante_INIT_debounce(&P4_GIALLO);
  
  app.pwm = 100;
}

void buzzer_init(void){
 TRISAbits.TRISA6 = 0;  
 PORTAbits.RA6 = 0;
}

void buzzer_Start(uint16_t durata){
 PORTAbits.RA6 = 1;
 app.buzzer_state = 1;
 app.buzzer_starttime = millis();
 app.buzzer_duratabeep = durata;
}
void buzzer_Stop(void){
 PORTAbits.RA6 = 0;
 app.buzzer_state = 0; 
}

void out_init(void){
    TRISAbits.TRISA5 = 0;   
    TRISAbits.TRISA3 = 0;   
    TRISAbits.TRISA2 = 0;   
    TRISBbits.TRISB6 = 0;   
    TRISCbits.TRISC0 = 0;   
    TRISCbits.TRISC1 = 0;   
    TRISCbits.TRISC2 = 0;   
    TRISCbits.TRISC3 = 0;  
    TRISAbits.TRISA7 = 0;  
    PORTAbits.RA7 = 0;
    PORTAbits.RA5 = 0;
    PORTAbits.RA3 = 0;
    PORTAbits.RA2 = 0;
    PORTBbits.RB6 = 0;
    PORTCbits.RC0 = 0;
    PORTCbits.RC1 = 0;
    PORTCbits.RC2 = 0;
    PORTCbits.RC3 = 0;
            
    output_P1.portPOTENZA = &PORTA;
    output_P1.pinPOTENZA = 5;
    output_P1.portLED = &PORTC;
    output_P1.pinLED = 0;
    output_P1.state = false;
    
    output_P2.portPOTENZA = &PORTA;
    output_P2.pinPOTENZA = 3;
    output_P2.portLED = &PORTC;
    output_P2.pinLED = 1;
    output_P2.state = false;
    
    output_P3.portPOTENZA = &PORTA;
    output_P3.pinPOTENZA = 2;
    output_P3.portLED = &PORTC;
    output_P3.pinLED = 2;
    output_P3.state = false;

    output_P4.portPOTENZA = &PORTB;
    output_P4.pinPOTENZA = 6;
    output_P4.portLED = &PORTC;
    output_P4.pinLED = 3;    
    output_P4.state = false;
    
    POWER_ONLED.portLED = &PORTA;
    POWER_ONLED.pinLED = 7;

    soft_pwm.port = &PORTA;
    soft_pwm.pin = 7;
    softPWM_init(&soft_pwm,50);
    app.crepuscolare_changet = false;
    app.pwmstatemachine = 0;
    app.tpwm = 100;
    app.pwmlast = 100;
    app.pwm = 100;    
    
    //LED RGB
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB7 = 0;
    RGB_LED(RGB_OFF);
}

void out_toggle(struct OUTPUT* p){
    //Write Pin
    uint8_t value, pinmask;
    //Potenza
    if(p->state){
        //Set 0
        pinmask = ~(1<<p->pinPOTENZA);
        *p->portPOTENZA = *p->portPOTENZA & pinmask;
    }else{
        //Set 1
        pinmask = (1<<p->pinPOTENZA);
        *p->portPOTENZA = *p->portPOTENZA | pinmask;
    }
    //Led
    if(p->state){
        //Set 0
        pinmask = ~(1<<p->pinLED);
        *p->portLED = *p->portLED & pinmask;
    }else{
        //Set 1
        pinmask = (1<<p->pinLED);
        *p->portLED = *p->portLED | pinmask;
    }
    if(p->state){
        p->state = false;
    }else{
        p->state = true;
    }
    
}
void out_SW1_SW2__ON(struct OUTPUT* o1, struct OUTPUT* o2){
    //Write Pin
    uint8_t value, pinmask;
    //Potenza
    //Set 1
    pinmask = (1<<o1->pinPOTENZA);
    *o1->portPOTENZA = *o1->portPOTENZA | pinmask;
    //Led
    //Set 1
    pinmask = (1<<o1->pinLED);
    *o1->portLED = *o1->portLED | pinmask;
    //SW2
    //Set 1
    pinmask = (1<<o2->pinPOTENZA);
    *o2->portPOTENZA = *o2->portPOTENZA | pinmask;
    //Led
    //Set 1
    pinmask = (1<<o2->pinLED);
    *o2->portLED = *o2->portLED | pinmask;
    
    o1->state = true;
    o2->state = true;
    
}

void out_mod1_SW4__ON(struct OUTPUT* v){
    //Write Pin
    uint8_t pinmask;
    //Potenza
    //Set 1
    pinmask = (1<<v->pinPOTENZA);
    *v->portPOTENZA = *v->portPOTENZA | pinmask;
    //Led
    //Set 1
    pinmask = (1<<v->pinLED);
    *v->portLED = *v->portLED | pinmask;    
    v->state = true;
    
    app.SW4_mod1_starttime = millis();
}

void out_mod1_SW4__OFF(struct OUTPUT* v){
    //Write Pin
    uint8_t pinmask;
    //Potenza
    pinmask = ~(1<<v->pinPOTENZA);
    *v->portPOTENZA = *v->portPOTENZA & pinmask;
    pinmask = ~(1<<v->pinLED);
    *v->portLED = *v->portLED & pinmask;       
    v->state = false;
}

void ON_LED(bool state){
    if(state){
        //PORTAbits.RA7 = 1;
        softPWM_Set(&soft_pwm, app.pwm);
        app.onLed_starttime = millis();
        app.onLed_state = true;
    }else{
        //PORTAbits.RA7 = 0;
        softPWM_Set(&soft_pwm, 0);
        app.onLed_state = false;        
    }
}

void read_modalita_SW4(void){
    if(eeprom_Load()==false){
        //Se Non trovi salvato nulla o è corrotta, imposta a 0
        eepvar.P4_state = 0;
    }
}
void save_modalita_SW4(uint8_t modalita){
    if(modalita){
        eepvar.P4_state = 1;
        eeprom_Save();
    }else{
        eepvar.P4_state = 0;
        eeprom_Save();
    }
}

void RGB_LED(uint8_t value){
    switch(value){
        case RGB_OFF:
            PORTBbits.RB5 = 0;
            PORTBbits.RB7 = 0;            
        break;
        case RGB_VERDE:
            PORTBbits.RB5 = 1;
            PORTBbits.RB7 = 0;            
        break;
        case RGB_GIALLO:
            PORTBbits.RB5 = 1;
            PORTBbits.RB7 = 1;            
        break;
        case RGB_ROSSO:
            PORTBbits.RB5 = 0;
            PORTBbits.RB7 = 1;            
        break;
    }
}

void battery_anomalia_MID(void){
    switch(app.state_BATTERY_ANOMALIA){
        case 0:
            buzzer_Start(1000);
            app.state_BATTERY_ANOMALIA = 1;
            app.beep_ms_BATTERY_ANOMALIA = millis();
        break;
        case 1:
            if((millis()-app.beep_ms_BATTERY_ANOMALIA)>2000){
                app.state_BATTERY_ANOMALIA = 2;
            }
        break;
        case 2:
            buzzer_Start(1000);
            app.state_BATTERY_ANOMALIA = 3;
            app.beep_ms_BATTERY_ANOMALIA = millis();                    
        break;
        case 3:
            if((millis()-app.beep_ms_BATTERY_ANOMALIA)>2000){
                app.state_BATTERY_ANOMALIA = 4;
            }
        break;                
        case 4:
            buzzer_Start(1000);
            app.state_BATTERY_ANOMALIA = 5;
            app.beep_ms_BATTERY_ANOMALIA = millis();
        break;
        default:
            if(outputs[3]->state == true){
                out_mod1_SW4__OFF(outputs[3]);
            }
        break;                
    } 
}
void battery_anomalia_LOW(void){
    if(outputs[3]->state == true){
        out_mod1_SW4__OFF(outputs[3]);
    }
    if(outputs[0]->state == true){
        out_toggle(outputs[0]);
    }   
}

//*****************ADC*****************
//*****************ADC*****************
//*****************ADC*****************
//*****************ADC*****************
//*****************ADC*****************

enum ADC_CHANNEL {CH_BATTERIA=0u, CH_NOTTE=1u};

void ADC_Task_sequence_conversion(void){
    switch(app.state_ADC_sequence){
        case 0:
            if(!ADC_IS_DONE()){
                ADCON0bits.CHS = CH_BATTERIA;
                ADC_START();
                app.state_ADC_sequence = 1;
            }
        break;
        case 1:
            if(ADC_IS_DONE()){
                app.ADC_BATTERIA_value = ADC_VALUE();
                app.state_ADC_sequence = 2;
                app.adc_flag = false;
           }            
        break;
        case 2:
            if(!ADC_IS_DONE()){
                ADCON0bits.CHS = CH_NOTTE;
                ADC_START();
                app.state_ADC_sequence = 3;
            }
        break;
        case 3:
            if(ADC_IS_DONE()){
                app.ADC_NOTTE_value = ADC_VALUE();
                app.state_ADC_sequence = 0;
                app.adc_flag = false;
           }            
        break;        
    }
}

void ADC_sequence_init(uint8_t sogliaBatteria){
    TRISAbits.TRISA0 = 1;    
    TRISAbits.TRISA1 = 1;    
    ADCON0bits.CHS = CH_BATTERIA; //CH1
    ADCON0bits.ADON = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    
    app.adc_flag = false;
  //  ADCON0bits.GO_DONE = 1; //Start Conversion ADC
    //ADRESH
    app.state_ADC_sequence = 0;
    app.state_BATTERY_ANOMALIA = 0;
    //Identifica Batteria
    __delay_ms(500);
    //while(1){
        ADC_START();
        while(ADC_IS_DONE()==0){}
        app.ADC_BATTERIA_value = ADC_VALUE();
        if(app.ADC_BATTERIA_value < sogliaBatteria){
            app.BATTERIA_IDENTIFY = false;   // BATT 12V
        }else{
            app.BATTERIA_IDENTIFY = true;   // BATT 24V
        }
        app.adc_flag = false; 
        NOP();
    //}
    
}
bool ADC_IS_DONE(void){
    return app.adc_flag;
}
void ADC_START(void){
    app.adc_flag = false;    
    ADCON0bits.GO_DONE = 1; //Start Conversion ADC
}
uint8_t ADC_VALUE(void){
    uint8_t adcva;
    adcva = ADRESH;
    return adcva;
}