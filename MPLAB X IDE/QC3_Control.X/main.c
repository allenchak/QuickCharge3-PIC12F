/*
 * File:   main.c
 * Author: allenchak
 *
 * Created on January 9, 2021, 11:58 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"


//#define _XTAL_FREQ 16000000 //16Mhz
#define _XTAL_FREQ 4000000 //4MHz
//#define _XTAL_FREQ 1000000 //1MHz


#define PIN_USB_DP    0 // Physical Pin 7, Data+, Green //DpPin
#define PIN_USB_DM    1 // Physical Pin 6, Data-, White //DmPin
#define PIN_BUTTON    2 // Physical Pin 5, Toggle QC adaptor output voltage
#define PIN_LED_TEST  4 // Physical Pin 3, LED for indicator, Optional


#define CFG_BTN_COOLDOWN 200 // milliSecond, delay after the button press


//QC control variable
#define QC_T_GLITCH_BC_DONE_MS          1500
#define QC_T_GLICH_V_CHANGE_MS          60


//Arduino wrapper -- start
#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1


void pinMode(int pin, int mode){
	if (mode == INPUT) {
		TRISIO |= (1 << pin);
		GPIO &= ~(1 << pin); //mark as LOW, optional
	} else if (mode == INPUT_PULLUP) {
        //actually, it's not work on PIC12F675
		TRISIO |= (1 << pin);
		GPIO |= (1 << pin); //mark as HIGH, optional
	} else {
		TRISIO &= ~(1 << pin);
	}
}

void digitalWrite(int pin, int val){
	if (val == LOW) {
		GPIO &= ~(1 << pin);
	} else {
		GPIO |= (1 << pin);
	}
}

int digitalRead(int pin){
	if ( GPIO & (1 << pin) ) return HIGH;
	return LOW;
}
//Arduino wrapper -- end



//QC control core function -- start
void dm0V(){
  digitalWrite(PIN_USB_DM, LOW);
  pinMode(PIN_USB_DM, OUTPUT); // and pull D- down
}

void dm600mV(){
  pinMode(PIN_USB_DM, INPUT); // and pull D- down
}

void dm3300mV(){
  digitalWrite(PIN_USB_DM, HIGH); // Pull D- up (3.3V minimum)
  pinMode(PIN_USB_DM, OUTPUT); // and pull D- down
}

void dp600mV(){
  pinMode(PIN_USB_DP, INPUT); // Let the DP divider set D+ to about 0.6V
}

void dp3300mV(){
  digitalWrite(PIN_USB_DP, HIGH); // Pull D+ up (3.3V minimum)
  pinMode(PIN_USB_DP, OUTPUT);
}

void set5V(){
  dp600mV();
  dm0V();
  __delay_ms(QC_T_GLICH_V_CHANGE_MS);
}

void set9V(){
  dp3300mV();
  dm600mV();
  __delay_ms(QC_T_GLICH_V_CHANGE_MS);
}

void set12V(){
  dp600mV();
  dm600mV();
  __delay_ms(QC_T_GLICH_V_CHANGE_MS);
}

void set20V(){
  dp3300mV();
  dm3300mV();
  __delay_ms(QC_T_GLICH_V_CHANGE_MS);
}

void handshaking(){
  // The spec requires that D+ remains at 0.6V during _WaitTime.
  dp600mV(); // Setting D+ to 0.6V is done by default (Arduino pins are input on boot)
  
  // We're in "QC3 schema". There's no way to let D- "float", but setting it to 0.6V will prevent the D+/D- short from pulling D+/D- down
  dm600mV(); // Which is done by default
  
  //while(millis() < QC_T_GLITCH_BC_DONE_MS); //we don't have millis() function, replaced by __delay_ms()
  __delay_ms(QC_T_GLITCH_BC_DONE_MS);
  dm0V(); // "Acknowledge" by simulating that we "follow" the internal pull down
  __delay_ms(2);
}
//QC control core function -- end




// Core feature
int main(int argc, char** argv) {
	ANSEL  = 0x00;		// Set ports as digital I/O, not analog input
	ADCON0 = 0x00;		// Shut off the A/D Converter
	CMCON  = 0x07;		// Shut off the Comparator
	VRCON  = 0x00;		// Shut off the Voltage Reference
	TRISIO = 0x00;		// rest all output
	GPIO   = 0x00;		// Make all pins 0
	
	
	// Arduino setup() -- start
	//pinMode(PIN_LED_TEST, OUTPUT);
	pinMode(PIN_BUTTON, INPUT_PULLUP);
    
	//digitalWrite(PIN_LED_TEST, HIGH);
	__delay_ms(100);
	handshaking();
	__delay_ms(1000);
	//digitalWrite(PIN_LED_TEST, LOW);
	
	//char mode = eeprom_read(0);
    char mode = 0;
	// Arduino setup() -- end

	for(;;)
	{
        // Arduino loop() -- start
		//digitalWrite(PIN_LED_TEST, LOW);
		// put your main code here, to run repeatedly:
		if( digitalRead(PIN_BUTTON) == LOW ){
			//digitalWrite(PIN_LED_TEST, HIGH);
			mode++;
			if( mode > 3 ) //change the `2` to `3` in case you are using classB adaptor
				mode = 0;
            //eeprom_write(0, mode); //remove this line if no EEPROM access
		}

		if( mode == 0 ){
			set5V();
		}else if( mode == 1 ){
			set9V();
		}else if( mode == 2 ){
			set12V();
		}else if( mode == 3 ){
			set20V();
		}
		__delay_ms(200);
        // Arduino loop() -- end
	}
	//return (EXIT_SUCCESS);
}
