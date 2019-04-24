/*###################################################################################################################
Created: 23/04/19
Author: cwl36@bath.ac.uk
Description: I2C test code
####################################################################################################################*/

//I2C device addresses
#define LSM_READ 0xD7
#define LSM_WRITE 0xD6

#define F_CPU 16000000

#include "USART_lib.h"
#include "I2C_lib.h"
#include <avr/io.h>
#include <util/delay.h>

//LSM functions
uint8_t LSMreadbyte();

volatile uint8_t rx_counter = 0;
volatile uint8_t rx_buff[BUFF_LEN];
uint8_t buffer [2];

int main(void){
	UART_INIT();
	I2Cinit();
    while (1) {
		uint8_t data = LSMreadbyte();
		UART_TX_STR("DATA: ");
		itoa(data,buffer,16);
		UART_TX_STR(buffer);
		//UART_TX(data);
		UART_TX_STR("\n \r");
		_delay_ms(1000);
    }
}

uint8_t LSMreadbyte(){
/*  I2C read process:
	Master generates Start
	Master sends slave address , slave returns ACK, check for code 0x18
	Master sends register (MW), slave returns ACK, check for code 0x28
	Master generates start repeat (MW)
	Master sends slave address (MR), device returns ACK, check for code 0x40
	Master reads data (MR) from device, returns NACK, check for code 0x58
	Master generates Stop   
*/

	I2Cstart();

	//SLA+W
	I2Cwrite(LSM_WRITE);
		
	//check for Slave ACK
	uint8_t status = I2Cstatuscode();
	if (status != 0x18){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
		UART_TX_STR("NO ACK");
		while (1);
	}
	//choose register to read. WHO_AM_I_G (0x0F) should return 0x69 / 105d
	I2Cwrite(0x0F);
		
	//check for Slave ACK
	status = I2Cstatuscode();
	if (status != 0x28){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
	}
			
	//repeated start
	I2Cstart();
		
	//SLA+R
	I2Cwrite(LSM_READ);
		
	//check for Slave ACK
	status = I2Cstatuscode();
	if (status != 0x40){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
	}
		
	//read data
	uint8_t byte = I2CreadNMAK();
		
	//send stop code
	I2Cstop();
	
	return byte;
}