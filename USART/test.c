/*###################################################################################################################
Created: 16/04/19
Author: cwl36@bath.ac.uk
Description: USART library test code
####################################################################################################################*/

#define F_CPU 16000000

#include "USART_lib.h"
#include <util/delay.h>

volatile uint8_t rx_buff[BUFF_LEN] = "default";		//volatile for use in ISR, preloaded with string so something is sent by default
volatile uint8_t rx_counter = 0;					//counter for how many bytes have been received

int main(void){
	UART_INIT();
	
	while (1) {
		UART_TX_STR(rx_buff);
		sei();										//enable interrupts during delay so new data can be passed
		_delay_ms(2000);							//info can be received during this period to update what is sent
		cli();										//disable interrupts so data isn't modified during tx
		rx_counter = 0;								//needs to be reset each cycle
	}
}