/*###################################################################################################################
Created: 16/04/19
Last Modified: 09/05/19
Author: cwl36@bath.ac.uk
Description: Header for USART library
####################################################################################################################*/ 
#ifdef __cplusplus
extern "C"{
#endif
	#ifndef USART_lib
	#define USART_lib

	//definitions and inclusions
	#define BUFF_LEN 128

	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <stdlib.h>

	//Function Declarations
	void UART_INIT();

	uint8_t UART_RX();

	void UART_TX(uint8_t);

	void UART_TX_STR(char*);

	void UART_TX_NL();

	void UART_TX_UINT8(uint8_t, uint8_t);

	void UART_TX_UINT16(uint16_t, uint8_t);

	#endif
#ifdef __cplusplus
}
#endif