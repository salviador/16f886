/* 
 * File:   application.h
 * Author: michele
 *
 * Created on 8 maggio 2017, 12.12
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include <stdint.h>
    #include <stdbool.h>
    #include "varie.h"
    
    #define NUM_PULSANTI 4

    struct APP{
        //BUZZER
        bool buzzer_state;
        uint32_t buzzer_starttime;
        uint16_t buzzer_duratabeep;        
        //TIME Inactive ON_LED
        bool onLed_state;
        uint32_t onLed_starttime;        
        //SW4 modalita 1
        uint32_t SW4_mod1_starttime;  
        //---
        uint8_t state_P4_SWITCH_INTERMEDIO_BEEPBEEP;
        uint32_t time_P4_SWITCH_INTERMEDIO_BEEPBEEP;
        //soglia crepuscolare pwm led
        uint8_t pwm;         //variabile pwm di riferimento
                            //temp pwm
        uint8_t tpwm,pwmlast,pwmstatemachine;
        uint32_t time_isteresi_crepuscolare;
        bool crepuscolare_changet;
        //adc
            //public
        uint8_t ADC_NOTTE_value;
        uint8_t ADC_BATTERIA_value;        
            //private
        uint8_t state_ADC_sequence;
        bool adc_flag;
        //Batteria
        bool BATTERIA_IDENTIFY; //0= 12V ;;;; 1=24V
            //private
        uint8_t state_BATTERY_ANOMALIA;
        uint32_t beep_ms_BATTERY_ANOMALIA;
    };
    
    struct OUTPUT{
        //Hardware Pin assigned
        unsigned char* portPOTENZA;
        unsigned char pinPOTENZA;        
        unsigned char* portLED;
        unsigned char pinLED;   
        //state
        bool state;
    };
    
    void switch_init(void);
    void buzzer_init(void);
    void buzzer_Start(uint16_t durata);
    void buzzer_Stop(void);
    void out_init(void);
    void out_toggle(struct OUTPUT* p);
    void out_SW1_SW2__ON(struct OUTPUT* o1, struct OUTPUT* o2);
    void out_mod1_SW4__ON(struct OUTPUT* v);
    void out_mod1_SW4__OFF(struct OUTPUT* v);
    void ON_LED(bool state);
    void read_modalita_SW4(void);
    void save_modalita_SW4(uint8_t modalita);
    void ADC_sequence_init(uint8_t sogliaBatteria);
    bool ADC_IS_DONE(void);
    void ADC_START(void);
    uint8_t ADC_VALUE(void);
    void ADC_Task_sequence_conversion(void);
    void RGB_LED(uint8_t value);
    void battery_anomalia_MID(void);
    void battery_anomalia_LOW(void);
    
    
    enum RGB_LED_COLOR {RGB_OFF, RGB_VERDE, RGB_GIALLO, RGB_ROSSO};


#ifdef	__cplusplus
}
#endif

#endif	/* APPLICATION_H */

