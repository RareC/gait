/*###################################################################################################################
Created: 24/04/19
Author: cwl36@bath.ac.uk
Description: I2C library
####################################################################################################################*/

#include "I2C_lib.h"

void I2Cinit(){
	TWBR = 0x0C;		//SCL = F_CPU/(16 + 2(TWBR)*(Prescaler))
	TWCR = (1<<TWEN);   //Enable I2C interface
}

//TWCR states straight out of atmega328p data sheet
void I2Cstart(){
	//Clears interrupt flag, send start condition and enables I2C interface
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));
}

void I2Cstop(void){
	//Clears interrupt flag, send stop condition and enables I2C interface
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2Cwrite(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	//wait for TWINT flag to be set
	while(!(TWCR & (1<<TWINT)));
}

uint8_t I2CreadMAK(){
	//Clears interrupt flag, enables I2C interface and sends ACK once data received
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	
	//wait for TWINT flag to be set. Happens once data byte is received.
	while(!(TWCR & (1<<TWINT)));
	
	return TWDR;
}

uint8_t I2CreadNMAK(){
	//Clears interrupt flag, enables I2C interface. No ACK is sent
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	//wait for TWINT flag to be set. Happens once data byte is received.
	while(!(TWCR & (1<<TWINT)));
	
	return TWDR;
}

uint8_t I2Cstatuscode(){
	uint8_t status;
	//mask pre-scaler bits we don't need and get status code
	status = TWSR & 0xF8;
	return status;
}