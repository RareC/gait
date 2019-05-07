/*###################################################################################################################
Created: 16/04/19
Author: cwl36@bath.ac.uk
Description: USART library test code
####################################################################################################################*/

#define F_CPU 16000000

#include "USART_lib.h"
#include <util/delay.h>

volatile char rx_buff[BUFF_LEN] = "default";		//volatile for use in ISR, preloaded with string so something is sent by default
volatile uint8_t rx_counter = 0;					//counter for how many bytes have been received

int main(void){
	UART_INIT();
	
	while (1) {
		UART_TX_STR(rx_buff);
		UART_TX_NL();
		uint16_t test1 = 6000;
		uint8_t test2 = 169;
		UART_TX_UINT16(test1,10);
		UART_TX_UINT8(test2,10);
		_delay_ms(2000);							//info can be received during this period to update what is sent
		rx_counter = 0;								//needs to be reset each cycle
	}
}