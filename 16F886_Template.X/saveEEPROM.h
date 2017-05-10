/* 
 * File:   saveEEPROM.h
 * Author: michele
 * Ver: 1.00
 * Created on 9 maggio 2017, 13.46
 */

#ifndef SAVEEEPROM_H
#define	SAVEEEPROM_H

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
    struct EEP_VAR{
        uint8_t P4_state;
       // uint32_t test1;        
        //uint16_t test2;        
        //float test3;        
        //uint8_t test4;                
    };
    
    bool eeprom_Load(void); //return : true is data load in structure
    void eeprom_Save(void);

    
#ifdef	__cplusplus
}
#endif

#endif	/* SAVEEEPROM_H */

