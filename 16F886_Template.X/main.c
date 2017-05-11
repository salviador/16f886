// PIC16F886 Configuration Bit Settings
#define BUZZET_TIME_SHORT 50
#define PRESS_LONG_TIME 5000
#define ONLED_TIME 5000
#define SW4_MOD1_TIME 1000
#define CREPUSCOLARE 35
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
//__MPLAB_REALICE__

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "switch.h"
#include "isr.h"
#include "varie.h"
#include "application.h"
#include "main.h"
#include "saveEEPROM.h"
#include "softPWM.h"

__EEPROM_DATA(0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07);

//Object
extern struct SWITCH P1_ROSSO;
extern struct SWITCH P2_BLU;
extern struct SWITCH P3_BLU;
extern struct SWITCH P4_GIALLO;
extern struct SWITCH *pulsanti[] = {&P1_ROSSO, &P2_BLU, &P3_BLU, &P4_GIALLO};

extern struct APP app;

extern struct OUTPUT POWER_ONLED;
extern struct OUTPUT output_P1;
extern struct OUTPUT output_P2;
extern struct OUTPUT output_P3;
extern struct OUTPUT output_P4;
extern struct OUTPUT *outputs[] = {&output_P1, &output_P2, &output_P3, &output_P4};

extern struct EEP_VAR eepvar;  //Qui carico le variabili che voglio salvare

extern struct SOFT_PWM soft_pwm;

void main(void) {
  uint8_t i, temp;
  //Osc 8MHz internal
  OSCCONbits.IRCF = 7;
  while(OSCCONbits.HTS==0);  
  //Timer0
  OPTION_REGbits.T0CS = 0;          // Fosc / 4
  OPTION_REGbits.PSA = 0;           
  OPTION_REGbits.PS =  2;            //Presclar 1:8
  TMR0 = 0;
  //Attiva ISR
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  INTCONbits.T0IE = 1;
  INTCONbits.T0IF = 0;

  ANSEL = 0x00; // DIGITAL IO
  ANSELH = 0x00; // DIGITAL IO
  ANSELbits.ANS1 = 1;   //RA1 ANALOG
  
  //Setting SWITCH BOARD
  switch_init();  
  buzzer_init();
  out_init();  
  read_modalita_SW4();
  ADC_notte_init();

  NOP();
 
  while(1){
    pulsanti_alldebounce_task(&pulsanti, NUM_PULSANTI);
    NOP();
    //Check Switch
    for(i=0;i<NUM_PULSANTI;i++){
        if(pulsanti[i]->changeState){
            if(pulsanti[i]->State == false){
                //Pulsante premuto
                ON_LED(true);
                if((i==0)&&(outputs[i]->state == false)){
                    //Accendi SW1 e SW2
                    out_SW1_SW2__ON(outputs[0],outputs[1]);
                }else if((i==3)&&(eepvar.P4_state == 1)){
                    //Modalita 1 di SW4
                    out_mod1_SW4__ON(outputs[i]);                
                }else{
                    out_toggle(outputs[i]);
                }
                buzzer_Start(BUZZET_TIME_SHORT);
            }
            pulsanti[i]->changeState = false;
        }
    }
    NOP();
    //Check if Switch press > 5 second  
    if((P4_GIALLO.State == false) && ((millis()- P4_GIALLO.time_OFFstate)>=PRESS_LONG_TIME) ){
        //Cambia modalita SW4
        buzzer_Start(2000);
        if(eepvar.P4_state == 0){
            save_modalita_SW4(1);
        }else{
            save_modalita_SW4(0);        
        }
        P4_GIALLO.State = true;
    }
    //Check if Switch press > xxxx second  beep beep beep
    if((P4_GIALLO.State == false) && ((millis()- P4_GIALLO.time_OFFstate)>=1000) ){
        //beep beep beep
        switch(app.state_P4_SWITCH_INTERMEDIO_BEEPBEEP){
            case 0:
                buzzer_Start(BUZZET_TIME_SHORT);
                app.state_P4_SWITCH_INTERMEDIO_BEEPBEEP = 1;
                app.time_P4_SWITCH_INTERMEDIO_BEEPBEEP = millis();
            break;            
            case 1:
                if((millis()-app.time_P4_SWITCH_INTERMEDIO_BEEPBEEP)>200){
                    app.state_P4_SWITCH_INTERMEDIO_BEEPBEEP = 0;
                    app.time_P4_SWITCH_INTERMEDIO_BEEPBEEP = millis();
                }
            break;
        }
    }    
    NOP();
    //Buzzer task stop
    if(app.buzzer_state){
        if((millis()-app.buzzer_starttime)>=app.buzzer_duratabeep){
            buzzer_Stop();
        }
    }
    NOP();
    //OnLed task stop se inattivi
    if((app.onLed_state)&&(output_P1.state == 0)&&(output_P2.state == 0)&&(output_P3.state == 0)&&(output_P4.state == 0)){
        if((millis()-app.onLed_starttime)>=ONLED_TIME){
            ON_LED(false);
        }
    }
    NOP();
    //SW4 Modalita 1
    if((outputs[3]->state)&&(eepvar.P4_state == 1)&&((millis()-app.SW4_mod1_starttime)>=SW4_MOD1_TIME)){
        out_mod1_SW4__OFF(outputs[3]);
    }
    NOP();
    //adc crepuscolare
    if(ADC_IS_DONE()){
        if(ADC_VALUE()<= CREPUSCOLARE){
            app.tpwm=10;
        }else{
            app.tpwm=100;
        }
        if(app.tpwm != app.pwmlast){
            app.crepuscolare_changet = true;
        }
        if(app.crepuscolare_changet){
            switch(app.pwmstatemachine){
                case 0:
                    app.time_isteresi_crepuscolare = millis();
                    app.pwmstatemachine = 1;
                break;
                case 1:
                    if((millis()-app.time_isteresi_crepuscolare)>=3000){
                        if(app.tpwm != app.pwmlast){
                            app.pwmlast = app.tpwm;
                            app.pwmstatemachine = 0;
                            app.crepuscolare_changet = false;
                            //cambia pwm
                            app.pwm = app.tpwm;
                            if(app.onLed_state){
                                softPWM_Set(&soft_pwm, app.pwm);
                            }
                        }
                    }
                break;                
            }
        }
        ADC_START();
    }

    NOP();
    //Soft PWM TASK
    softPWM_task(&soft_pwm);
  }
   // __delay_ms(1000); // 1 Second Delay
}
