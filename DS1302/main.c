/*
 * DS1302.c
 *
 * Created: 28/04/2019 13:45:57
 * Author : Chris
 */ 

//Drive CE high
//Output command byte - LSB sent first on rising edge 1 A0 A1 A2 A3 A4 0 1 to read from clock registers
// D0 D1... read in on falling edge
#define CE PORTD2
#define DAT PORTD4
#define SCLK PORTD5


#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "USART_lib.h"

volatile uint8_t rx_buff[BUFF_LEN];			//volatile for use in ISR and extern so can be set by another file
volatile uint8_t rx_counter = 0;			//counter for how many bytes have been received

void shiftout(uint8_t);
uint8_t shiftin();

void DS1302_init();
uint8_t readbyte(uint8_t);
void writebyte(uint8_t, uint8_t);

int main(void)
{
	UART_INIT();
	DS1302_init();
	
	UART_TX_STR("\r\n");
	uint8_t rd1 = readbyte(0x8F);
	uint8_t buff [2];
	itoa(rd1,buff,16);
	UART_TX_STR(buff);
	UART_TX_STR("\r\n");

	
	while (1){
		
		/*uint8_t rd2 = readbyte(0x8F);
		itoa(rd2,buff,16);
		UART_TX_STR(buff);
		_delay_ms(1000);*/
	}
}

void shiftout(uint8_t byte){
	//set data line to output
	DDRD |= (1<<DAT);
	
	for (uint8_t i = 0; i < 8; i++){
		//LSB is sent first so mask all except LSB and shift right afterwards
		if (byte & 0x01)
			PORTD |= (1<<DAT);
		else
			PORTD &= ~(1<<DAT);
		
		_delay_us(1); //setup time
		//clock high
		PORTD |= (1<<SCLK);
		_delay_us(10);
		//clock low
		PORTD &= ~(1<<SCLK);
		_delay_us(10);
		
		byte >>= 1;
	}
}

uint8_t shiftin(){
	//set data line to input
	DDRD &= ~(1<<DAT);
	uint8_t data = 0;
	
	for (uint8_t i = 0; i < 8; i++)	{
		
		if (PIND & (1<<DAT))
			data |= (1 << i);

		PORTD |= (1<<SCLK);
		_delay_us(10);
		PORTD  &= ~(1<<SCLK);
		_delay_us(10);
	}
	
	return data;
}

void DS1302_init(){
	_delay_us(4);
	DDRD &= ~((1<<SCLK)|(1<<CE));
	PORTD &= ~((1<<CE)|(1<<SCLK));
	
	//disable write protection
	writebyte(0x8e,0x00);
	
	uint8_t test = readbyte(0x8f);
	if (test == 0x00)
		UART_TX_STR("PASS NWP");
	else if (test == 0x80)
		UART_TX_STR("PASS WP");
	else {
		UART_TX_STR("FAIL1");
		uint8_t buff [2];
		itoa(test,buff,16);
		UART_TX_STR(buff);
	}
		
	writebyte(0x8e,(test^0x80));
	uint8_t test2 = readbyte(0x8f);
	if (test2 == (test^0x80))
		UART_TX_STR("PASS2");
	else
		UART_TX_STR("FAIL2");
}

uint8_t readbyte(uint8_t address){
	PORTD |= (1<<CE); //set CE high to begin transfer
	_delay_us(4);
	
	shiftout(address);
	_delay_us(10);
	uint8_t byte = shiftin();
	//end
	PORTD &= ~(1<<CE);
	_delay_us(4);
	return byte;
}

void writebyte(uint8_t reg, uint8_t byte){
	PORTD |= (1<<CE); //set CE high to begin transfer
	_delay_us(4);
	shiftout(reg);
	_delay_us(10);	
	shiftout(byte);
	//end
	PORTD &= ~(1<<CE);
	_delay_us(4);
}