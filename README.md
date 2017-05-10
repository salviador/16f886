# Library abstract objective
## varie.c
	(required ->isr.c)
	millis();
	Use: uint32_t time_mS = millis();
## switch.c
	(required ->varie.c)
	switch debounce
	Use:
		Define object
			struct SWITCH P1_ROSSO;
		Assign Port and Pin of the uC
			P1_ROSSO.port = &PORTB;
			P1_ROSSO.pin = 0;
		Setting HW of the uC as INPUT
			TRISBbits.TRISB0 = 1;
		Init Object
			pulsante_INIT_debounce(&P1_ROSSO);
		Main:
			Run Task Without delay[for 1 switch]
					pulsante_debounce(&P1_ROSSO);
				or [for multiple switch]
					pulsanti_alldebounce_task(&pulsanti, NUM_PULSANTI);
			Check PRESS
				[for 1 switch]			
					if(P1_ROSSO.changeState){
						if(P1_ROSSO.State == false){	//Switch go to ground
							//........................
							//........................
							//........................
						}
						P1_ROSSO.changeState = false;	//Need Reset FLAG!
					}
				or [for multiple switch]
					for(i=0;i<NUM_PULSANTI;i++){
						if(pulsanti[i]->changeState){
							if(pulsanti[i]->State == false){
								//Pulsante premuto
								//........................
								//........................
								//........................
							}
							pulsanti[i]->changeState = false;
						}
					}
					
					
	