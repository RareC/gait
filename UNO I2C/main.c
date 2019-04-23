/*###################################################################################################################
Created: 23/04/19
Author: cwl36@bath.ac.uk
Description: I2C test code
####################################################################################################################*/

#include <avr/io.h>

void I2Cinit();
void I2Cstart();
void I2Cstop();
void I2Cwrite(uint8_t);
uint8_t I2Cread();
uint8_t I2Cstatuscode();

//#define DEVICE_READ_ADDR
//#define DEVICE_WRITE_ADDR

int main(void){
    while (1) {
		//init
		//start
		//read
		//stop
    }
}

void I2Cinit(){	
	TWBR = 0x0C;		//SCL = F_CPU/(16 + 2(TWBR)*(Prescaler))
	TWCR = (1<<TWEN);   //Enable I2C interface
}

//TWCR states straight out of atmega328p data sheet
void I2Cstart(){
	//Clears interrupt flag, send start condition and enables I2C interface
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//wait for TWINT flag to be 0 (set)
	while ((TWCR & (1<<TWINT)) == 0);
}

void I2Cstop(void){
	//Clears interrupt flag, send stop condition and enables I2C interface
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2Cwrite(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	//wait for TWINT flag to be set to 0 (set)
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t I2Cread(){
	//Clears interrupt flag, enables I2C interface and sends ACK
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	//wait for TWINT flag to be 0 (set)
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t I2Cstatuscode(){
	uint8_t status;
	//mask pre-scaler bits we don't need and get status code
	status = TWSR & 0xF8;
	return status;
}