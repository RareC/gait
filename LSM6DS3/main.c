/*###################################################################################################################
Created: 25/04/19
Author: cwl36@bath.ac.uk
Description: LSM6DS3 code
####################################################################################################################*/

#define LSM_READ 0xD7
#define LSM_WRITE 0xD6

//Control register addresses
#define FIFO_CTRL2 0x07
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL6_C 0x15
#define CTRL7_G 0x16
#define CTRL10_C 0x19
#define FUNC_SRC 0x53
#define TAP_CFG 0x58
#define MD1_CFG 0x5E

//Output data register addresses
#define GYRO_X_LOW 0x22
#define GYRO_X_HIGH 0x23
#define GYRO_Y_LOW 0x24
#define GYRO_Y_HIGH 0x25
#define GYRO_Z_LOW 0x26
#define GYRO_Z_HIGH 0x27
#define ACCEL_X_LOW 0x28
#define ACCEL_X_HIGH 0x29
#define ACCEL_Y_LOW 0x2A
#define ACCEL_Y_HIGH 0x2B
#define ACCEL_Z_LOW 0x2C
#define ACCEL_Z_HIGH 0x2D

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART_lib.h"
#include "I2C_lib.h"

volatile uint8_t rx_counter = 0;
volatile uint8_t rx_buff[BUFF_LEN];

volatile uint8_t tilt;

uint8_t LSMreadbyte(uint8_t);
void LSMwritebyte(uint8_t, uint8_t);

void LSMinit();

//ISR
ISR(PCINT0_vect){
	tilt = 1;
}

int main(void){
	
	I2Cinit();
	UART_INIT();
    LSMinit();
	
	uint8_t *newline = "\r\n";
	
	sei();
	PCMSK0 |= (1<<PCINT0);
	PCICR |= (1<<PCIE0);
	
    while (1){
		/*//step detection
		uint8_t stepL = LSMreadbyte(0x4B);
		uint8_t stepH = LSMreadbyte(0x4C);
		uint16_t step = (stepH<<8) + stepL;
		uint8_t buff [4];
		itoa(step, buff, 10);
		UART_TX_STR("Steps: ");
		UART_TX_STR(buff);
		UART_TX_STR("\n\r");
		
		//tilt detection
		if (tilt)
			UART_TX_STR("Tilt Detected\n\r");
		else
			UART_TX_STR("No Tilt Detected\n\r");
		
		tilt = 0;
		_delay_ms(4000); */
		
		
		_delay_ms(100);
		
		uint8_t GyroX = LSMreadbyte(GYRO_X_HIGH);
		uint8_t GyroY = LSMreadbyte(GYRO_Y_HIGH);
		uint8_t GyroZ = LSMreadbyte(GYRO_Z_HIGH);
		
		uint8_t XlX = LSMreadbyte(ACCEL_X_HIGH);
		uint8_t XlY = LSMreadbyte(ACCEL_Y_HIGH);
		uint8_t XlZ = LSMreadbyte(ACCEL_Z_HIGH);
		
		uint8_t buff [16];
		itoa(GyroX,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);
		
		itoa(GyroY,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);

		itoa(GyroZ,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);
		
		itoa(XlX,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);
		
		itoa(XlY,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);

		itoa(XlZ,buff,2);
		UART_TX_STR(buff);
		UART_TX_STR(newline);
		
		UART_TX_STR(newline);
		
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
	
	//Set accelerometer data rate @208Hz
	LSMwritebyte(CTRL1_XL, 0x50);
	
	//Set gyro data rate @208Hz
	LSMwritebyte(CTRL2_G, 0x50);
	
	//Enable gyro low power mode
	LSMwritebyte(CTRL7_G, 0x80);	
	
	//Enable accelerometer low power mode
	LSMwritebyte(CTRL6_C, 0x10);
	
	//Clear step counter and enable embedded functionalities
	LSMwritebyte(CTRL10_C, 0x3E);
	
	//Enable pedometer algorithm and tilt detection
	LSMwritebyte(TAP_CFG, 0x60);
	
	//send tilt event on INT1 pin
	LSMwritebyte(MD1_CFG , 0x02);
}