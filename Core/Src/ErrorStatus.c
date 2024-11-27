/*
 * ErrorStatus.c
 *
 *  Created on: Mar 21, 2022
 *      Author: karl.yamashita
 */

#include "main.h"
#include "ErrorStatus.h"



/*
 * Description: Returns string based off error code.
 *
 *
 */
void GetErrorString(int errorCode, char *str)
{
    switch(errorCode){
    case NO_ERROR:
        strcpy(str, "OK");
        break;

    case TIMEOUT:
        strcpy(str, "ERROR-Timeout");
        break;
    case COMMAND_UNKNOWN:
        strcpy(str, "ERROR-Command Unknown");
        break;
    case VALUE_OUT_OF_RANGE:
        strcpy(str, "ERROR-Value out of range");
        break;
    case VALUE_NOT_VALID:
        strcpy(str, "ERROR-Value not valid");
        break;
    case NO_SERIAL_PART:
    	strcpy(str, "ERROR-No Serial or Part");
    	break;
    case NO_INFO_STRING:
    	strcpy(str, "ERROR-No Info String");
    	break;
    case LTM46xx_SLAVE_ADDRESS_NOT_SET:
    	strcpy(str, "ERROR-not set");
    	break;

    case LTM46xx_BUSY:
        strcpy(str, "ERROR-LTM4675 busy");
        break;
    case LTM46xx_NVM:
        strcpy(str, "ERROR-LTM4675 NVM unlock not successful");
        break;

    case I2C_ERROR_WRITE:
        strcpy(str, "ERROR-I2C no data sent");
        break;
    case I2C_ERROR_READ:
        strcpy(str, "ERROR-I2C no data read");
        break;
    case LTM46xx_RAIL_ADDR_NOT_ALLOWED:
    	strcpy(str, "ERROR-Rail address not allowed");
    	break;
    case INTERPOLATION_ERROR_VALUE_INVALID:
    	strcpy(str, "ERROR-Interpolation value(s) not valid");
    	break;

        // for debug
    case ERR_1:
        strcpy(str, "ERROR-DEBUG 1");
        break;
    case ERR_2:
        strcpy(str, "ERROR-DEBUG 2");
        break;
    case ERR_3:
        strcpy(str, "ERROR-DEBUG 3");
        break;
    case ERR_4:
        strcpy(str, "ERROR-DEBUG 4");
        break;
    default:
        sprintf(str, "ERROR-returned [%d], is not in error list", errorCode);
        break;
    }
}
