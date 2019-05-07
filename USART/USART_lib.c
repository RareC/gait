/*###################################################################################################################
Created: 16/04/19
Author: cwl36@bath.ac.uk
Description: USART library
####################################################################################################################*/ 

#include "USART_lib.h"

//THESE REQUIRE DECLARING IN THE PROGRAM THEY ARE USED IN, EVEN IF NOT USED
volatile extern uint8_t rx_buff[BUFF_LEN];			//volatile for use in ISR and extern so can be set by another file
volatile extern uint8_t rx_counter;					//counter for how many bytes have been received

ISR(USART_RX_vect){
	rx_buff[rx_counter]=UDR0;
	rx_counter++;
	for (uint8_t i = rx_counter; i < BUFF_LEN; i++){
		rx_buff[i] = '\0';
	}
} 

//Initialize UART
void UART_INIT (){
	UBRR0H = 0x00;									//For 16 MHz clock & 9600 baud, set:
	UBRR0L = 0x67;									//UBBR = 103d = 0000 0110 0111
	UCSR0B|= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);	//Receive interrupt + RX/TX enable
	UCSR0C|= (1<<UCSZ01) | (1<<UCSZ00);				//8 bit data, 1 stop bit and no parity
}

//Send 1 byte of data
void UART_TX (uint8_t data){
	while (!( UCSR0A & (1<<UDRE0)));			//wait until register is free
	UDR0 = data;								//load data in the register
	while (!(UCSR0A & (1<<TXC0)));				//wait until data has been transmitted
}

//Send multiple bytes
void UART_TX_STR (char *str){
	uint8_t i = 0;
	while (*(str+i)){							//iterate over all data in the string
		UART_TX(*(str+i));
		i++;
	}
}

//Newline and carriage return 
void UART_TX_NL(){
	UART_TX_STR("\n\r");
}

//Send 8-bit integer value
void UART_TX_UINT8 (uint8_t val, uint8_t base){
	char buff [8];
	itoa (val, buff, base);
	UART_TX_STR (buff);
}

//Send 16-bit integer value
void UART_TX_UINT16 (uint16_t val, uint8_t base){
	char buff [16];
	itoa (val, buff, base);
	UART_TX_STR (buff);
}
