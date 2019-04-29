/*###################################################################################################################
Created: 16/04/19
Author: cwl36@bath.ac.uk
Description: ADC library
####################################################################################################################*/

#include <avr/io.h>
#include "ADC_lib.h"

void ADCinit(){
	ADCSRA |= ((1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)); //Enable ADC and set pre-scaler to 16M/64 = 250kHz (> than recomended but works fine for our accuracy requirements)
	ADMUX |= ((1<<REFS0)|(1<<ADLAR)); //Use external capacitor on AREF (0.1uF) and left adjust data, use ADC0.
}

uint8_t ADCread(){
	ADCSRA |= (1<<ADSC); //begin conversion
	
	while(ADCSRA & (1<<ADSC)); //wait for conversion to finish
	
	return ADCH;
}

void ADCwake(){
	PRR &= ~(1<<PRADC);
	ADCSRA |= (1<<ADEN);
}

void ADCsleep(){
	ADCSRA &= ~(1<<ADEN);
	PRR |= (1<<PRADC);
}