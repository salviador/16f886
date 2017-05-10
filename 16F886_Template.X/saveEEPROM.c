#include "saveEEPROM.h"
#include <xc.h>
#include "main.h"


struct EEP_VAR eepvar;  //Qui carico le variabili che voglio salvare

//Private
struct _conv16to8 {
  union {
    uint16_t a;
    uint8_t bytes[2];
  };  
}conv16to8;
 

//Public
bool eeprom_Load(void){
  //Prendi i settaggi dall'eeprom
  uint16_t i, checksum;
  uint8_t data[sizeof(eepvar)];
  checksum = 0;

  for(i = 0; i<sizeof(eepvar); i++){
      *((uint8_t*)(&eepvar) + i) = 0; 
      data[i] = eeprom_read(i);
      checksum = checksum + data[i];
  }
  conv16to8.bytes[1] = eeprom_read(sizeof(eepvar));
  conv16to8.bytes[0] = eeprom_read(sizeof(eepvar) + 1);
  
  if(conv16to8.a == checksum){
    for(i = 0; i<sizeof(eepvar); i++){
        *((uint8_t*)(&eepvar) + i) = data[i]; 
    }    
    return true;
  }
  return false;
}

void eeprom_Save(void){
  uint16_t i, checksum;
  uint8_t data[sizeof(eepvar)];
  checksum = 0;
  
  for(i = 0; i<sizeof(eepvar); i++){
      data[i] = *((uint8_t*)(&eepvar) + i);     
      checksum = checksum + data[i];
      eeprom_write(i, data[i]);
  }
  //Scrivi checksum
  conv16to8.a = checksum;
  eeprom_write(sizeof(eepvar), conv16to8.bytes[1]);
  eeprom_write(sizeof(eepvar) + 1, conv16to8.bytes[0]);
}