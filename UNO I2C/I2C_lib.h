/*###################################################################################################################
Created: 24/04/19
Modified: 09/05/19
Author: cwl36@bath.ac.uk
Description: Header for I2C library
####################################################################################################################*/

#ifdef __cplusplus
extern "C"{
#endif
	#ifndef I2C_lib
	#define I2C_lib

	#include <avr/io.h>

	//I2C functions
	void I2Cinit();
	void I2Cstart();
	void I2Cstop();
	void I2Cwrite(uint8_t);
	uint8_t I2CreadMAK();
	uint8_t I2CreadNMAK();
	uint8_t I2Cstatuscode();

	#endif
#ifdef __cplusplus
}
#endif