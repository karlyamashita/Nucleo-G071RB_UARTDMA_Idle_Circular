/*
 * ErrorStatus.h
 *
 *  Created on: Nov 24, 2021
 *      Author: karl.yamashita
 */

#ifndef ERRORSTATUS_H_
#define ERRORSTATUS_H_


// error status
enum {
    RETURN_DATA = -2,
    NO_ACK = -1,
    NO_ERROR = 0,
    BUSY_UART,
    TIMEOUT,
    COMMAND_UNKNOWN,
    VALUE_OUT_OF_RANGE,
    VALUE_NOT_VALID, // 5
    LTM46xx_BUSY,
    LTM46xx_NVM,
    I2C_ERROR_WRITE,
    I2C_ERROR_READ, // 10
    ERROR_LTM46xx_READ_ONLY,
    ERROR_DUTY_CYCLE_OUT_OF_RANGE,
	INTERPOLATION_ERROR_VALUE_INVALID,
	LTM46xx_SLAVE_ADDRESS_NOT_SET,
	LTM46xx_RAIL_ADDR_NOT_ALLOWED, // 15
	LTM46xx_READ_ONLY,
	FLASH_WRITE_FAILED,
	FLASH_ERASED_FAILED,
	NO_SERIAL_PART,
	NO_INFO_STRING,
	BOOTLOADER_ENABLE_FAILED, // 20

    // for debug
    ERR_1 = 200,
    ERR_2,
    ERR_3,
    ERR_4
};



void GetErrorString(int errorCode, char *str);

#endif /* ERRORSTATUS_H_ */
