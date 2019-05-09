/*###################################################################################################################
Created: 09/05/19
Modified: 09/05/19
Author: cwl36@bath.ac.uk
Description: SD Card read/write test code
####################################################################################################################*/

#include <Arduino.h>
#include <SD.h>
#include "C:\Repos\gait\SDcard\ReadWrite_modifited\USART_lib.h"

File data;

int main(){
	
	UART_INIT();
	
	//argument to SD.begin sets the CS pin i.e 10 is digital pin 10 i.e PB2
	if (!SD.begin(10)) {
		UART_TX_STR("Initialisation failed!");
		return 1;
	}
	UART_TX_STR("Initialisation done.\r\n");

	//Open in write mode
	data = SD.open("test.txt", FILE_WRITE);

	//The file opened
	if (data) {
		data.println("test");
		data.close();
	} 
	//File failed to open
	else {
			UART_TX_STR("Error opening file");
			return 1;
	}

	//Open in read mode
	data = SD.open("test.txt");
	
	//File opened successfully
	if (data) {
		UART_TX_STR("File contents:\r\n");

		//Read entirety of file
		uint8_t buff;
		while (data.available()){
			buff = data.read();
			UART_TX(buff);
		}
		
		data.close();
	} 
	
	else {
		//File failed to open
		UART_TX_STR("Error opening file");
		return 1;
	}
}

