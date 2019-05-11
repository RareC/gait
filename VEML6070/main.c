/*###################################################################################################################
Created: 16/04/19
Last Modified: 09/05/19
Author: cwl36@bath.ac.uk
Description: VEML6070 test code
####################################################################################################################*/

#define F_CPU 16000000

//I2C addresses for the VEML6070
#define VEML_ARA 0x31
#define VEML_WRITE 0x70
#define VEML_READ_L 0x71
#define VEML_READ_H 0x73

#include <avr/io.h>
#include <util/delay.h>
#include "USART_lib.h"
#include "I2C_lib.h"


volatile uint8_t rx_buff[BUFF_LEN];			//volatile for use in ISR and extern so can be set by another file
volatile uint8_t rx_counter;					//counter for how many bytes have been received

uint8_t VemlRead(uint8_t);
void VemlWrite(uint8_t);
void VemlInit();

int main(void){
	UART_INIT();
	I2Cinit();
	
	VemlInit();
	uint16_t reading;
    while (1) {
		reading = (VemlRead(VEML_READ_H)<<8);
		reading += VemlRead(VEML_READ_L);
		UART_TX_UINT16(reading,16);
		_delay_ms(500);
    }
}

uint8_t VemlRead(uint8_t addr){
/*  VEML6070 read process:
	Master generates Start
	Master sends slave address , slave returns ACK, check for code 0x18
	Master reads data (MR) from device, returns NACK, check for code 0x58
	Master generates Stop   
*/

	I2Cstart();

	//SLA+R
	I2Cwrite(addr);
		
	//check for Slave ACK
	uint8_t status = I2Cstatuscode();
	if (status != 0x40){
		UART_TX_UINT8(status,16);
		UART_TX_STR("NO ADDR ACK");
		while (1);
	}
		
	//read data
	uint8_t byte = I2CreadMAK();
	
	//Check data has been received
	status = I2Cstatuscode();
	if (status != 0x50){
		UART_TX_UINT8(status,16);
		UART_TX_STR("NO DATA");
		while (1);
	}
	
	//send stop code
	I2Cstop();
	
	return byte;
}

void VemlWrite(uint8_t data){
/*  VEML6070 write process:
	Master generates Start
	Master sends slave address , slave returns ACK, check for code 0x18
	Master sends data, slave returns ACK, check for code 0x28
	Master generates Stop   
*/
	I2Cstart();
	
	//SLA+W
	I2Cwrite(VEML_WRITE);
	
	//Check for slave ACK
	uint8_t status = I2Cstatuscode();
	if (status != 0x18){
		UART_TX_UINT8(status, 16);
		UART_TX_STR("NO ADDR ACK");
		while (1);
	}
	
	//Send data
	I2Cwrite(data);
	
	//Check for slave ACK
	status = I2Cstatuscode();
	if (status != 0x28){
		UART_TX_UINT8(status, 16);
		UART_TX_STR("NO DATA ACK");
	}
	
	//send stop
	I2Cstop();
}

//Initialise Hardware by sending 0x06 as specified in data sheet
void VemlInit(){
	//VemlRead(VEML_ARA);
	VemlRead(0x31);
	UART_TX_STR("cleared ack");
	VemlWrite(0x06);
}