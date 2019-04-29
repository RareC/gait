/*
 * MQ-135.c
 *
 * Created: 29/04/2019 13:52:43
 * Author : Chris
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "USART_lib.h"
#include "ADC_lib.h"

volatile uint8_t rx_buff[BUFF_LEN];			//volatile for use in ISR and extern so can be set by another file
volatile uint8_t rx_counter;

int main(void){
    UART_INIT();
	ADCinit();
	
	uint8_t dat;
	uint8_t buff [2];
	
    for (uint8_t i = 0; i < 210; i++){
		ADCwake();
		dat = ADCread();
		itoa(dat,buff,16);
		UART_TX_STR(buff);
		UART_TX_STR("\r\n");
		ADCsleep();
		_delay_ms(1000);
    }
	while(1);
}