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
#include "Initialization.h"
ErrorCode error;
uint8_t fifo_ctrl_reg;


 void INIT_START(void)
{   I2C_Peripheral_Start();
                    

    fifo_ctrl_reg = STREAM_MODE_THR_WMK;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_FIFO_CTRL_REG,
                                             fifo_ctrl_reg);
    ADC_DelSig_Stop();
    INT_Led_Write(1);  

    isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_WATERMARK);
    SPIM_1_Start();
    CyDelay(10);
}



void INIT_STOP(void)
{   I2C_Peripheral_Start();
    fifo_ctrl_reg = BYPASS_MODE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_FIFO_CTRL_REG,
                                                 fifo_ctrl_reg);

    isr_LIS3DH_Stop();
    I2C_Peripheral_Stop();
    SPIM_1_Stop();
    ADC_DelSig_Stop();         
    RED_Led_Write(1);
    GREEN_Led_Write(1);
    BLUE_Led_Write(1);
    INT_Led_Write(0);
                   

}
    
    
void INIT_MENU(void)
{   RED_Led_Write(1);
    GREEN_Led_Write(1);
    BLUE_Led_Write(1);


    I2C_Peripheral_Start();
    fifo_ctrl_reg = BYPASS_MODE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_FIFO_CTRL_REG,
                                                 fifo_ctrl_reg);
    
    isr_LIS3DH_Stop();
    SPIM_1_Stop();
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();

}
/* [] END OF FILE */
