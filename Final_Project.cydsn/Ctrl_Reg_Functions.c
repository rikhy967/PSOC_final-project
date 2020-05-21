/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
# include "Ctrl_Reg_Functions.h"
# include "project.h"
# include "I2C_Interface.h"
# include "ErrorCodes.h"
# include "LIS3DH_Registers.h"

/*
void ReadRegister(uint8_t addr, uint8_t data){
    
    char message[50];
    
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        addr,
                                        &data);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", data);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    
}
*/
/* [] END OF FILE */