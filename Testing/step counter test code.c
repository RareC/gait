/*###################################################################################################################
Created: 25/04/19
Author: cwl36@bath.ac.uk
Description: LSM6DS3 code
####################################################################################################################*/

#define LSM_READ 0xD7
#define LSM_WRITE 0xD6

//Register addresses
#define FIFO_CTRL2 0x07
#define CTRL1_XL 0x10
#define CTRL6_C 0x15
#define CTRL10_C 0x19
#define TAP_CFG 0x58

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "USART_lib.h"
#include "I2C_lib.h"

volatile uint8_t rx_counter = 0;
volatile uint8_t rx_buff[BUFF_LEN];

uint8_t LSMreadbyte(uint8_t);
void LSMwritebyte(uint8_t, uint8_t);

void LSMinit();

int main(void){
	
	I2Cinit();
	UART_INIT();
    LSMinit();
	
    while (1){
		uint8_t stepL = LSMreadbyte(0x4B);
		uint8_t stepH = LSMreadbyte(0x4C);
		uint16_t step = (stepH<<8) + stepL;
		uint8_t buff [4];
		itoa(step, buff, 10);
		UART_TX_STR("Steps: ");
		UART_TX_STR(buff);
		UART_TX_STR("\n\r");
		_delay_ms(4000);
    }
}

uint8_t LSMreadbyte(uint8_t reg){
/*  LSM6DS3 read process:
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
	uint8_t buffer [2];
	uint8_t status = I2Cstatuscode();
	if (status != 0x18){
		itoa(status, buffer ,16);
		UART_TX_STR(buffer);
		UART_TX_STR("NO ACK");
		while (1);
	}
	//choose register to read. WHO_AM_I_G (0x0F) should return 0x69 / 105d
	I2Cwrite(reg);
		
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

void LSMwritebyte(uint8_t reg, uint8_t data){
	/*  LSM6DS3 read process:
	Master generates Start
	Master sends slave address , slave returns ACK, check for code 0x18
	Master sends register (MW), slave returns ACK, check for code 0x28
	Master sends data (MW) , check for code 0x28
	Master generates Stop   
*/
	I2Cstart();
	
	//SLA+W
	I2Cwrite(LSM_WRITE);
	
	//Check for slave ACK
	uint8_t status = I2Cstatuscode();
	uint8_t buffer [2];
	if (status != 0x18){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
		UART_TX_STR("NO ACK");
		while (1);
	}
	
	//Select register to write to
	I2Cwrite(reg);
	
	//Check for slave ACK
	status = I2Cstatuscode();
	if (status != 0x28){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
	}
	
	//Send data
	I2Cwrite(data);
	
	//Check for slave ACK
	status = I2Cstatuscode();
	if (status != 0x28){
		itoa(status,buffer,16);
		UART_TX_STR(buffer);
	}
	
	//send stop
	I2Cstop();
}

void LSMinit(){
	//Enable step counter
	LSMwritebyte(FIFO_CTRL2, 0xC0);
	
	//Set accelerometer data rate and accelerometer scale
	LSMwritebyte(CTRL1_XL, 0x20);
	
	//Enable accelerometer low power mode
	LSMwritebyte(CTRL6_C, 0x10);
	
	//Clear step counter and enable embedded functionalities
	LSMwritebyte(CTRL10_C, 0x3E);
	
	//Enable pedometer algorithm
	LSMwritebyte(TAP_CFG, 0x40);
}