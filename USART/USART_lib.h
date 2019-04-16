/*###################################################################################################################
Created: 16/04/19
Author: cwl36@bath.ac.uk
Description: Header for USART library
####################################################################################################################*/ 

#ifndef USART_lib
#define USART_lib

#define F_CPU 16000000
#define BUFF_LEN 100

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Function Declarations
void UART_INIT();

uint8_t UART_RX();

void UART_TX(uint8_t);

void UART_TX_STR(uint8_t*);

#endif