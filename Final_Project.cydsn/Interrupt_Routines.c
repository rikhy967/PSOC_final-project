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
#include "project.h"
#include "Interrupt_Routines.h"
#include "stdio.h"
#include "ErrorCodes.h"
#include "I2C_Interface.h"
#include "LIS3DH_Registers.h"
#include "LIS3DH_Registers_Settings.h"
uint8_t error;
uint8_t watermark;
uint8_t int1_src_reg;
volatile uint16_t counter_button=0;
uint16_t timer_counter=0;


/* Variable that contains the message for UART debug communication*/
char mex[50];


/* Custom Interrupt on FIFO watermark */
 CY_ISR(ISR_LIS3DH_FIFO_WATERMARK)
{/*error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &watermark);
if((watermark & 0b10000000)==0b10000000)  OVR_FLAG=1;*/
error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                     LIS3DH_INT1_SRC,
                                     &int1_src_reg);
if(int1_src_reg & 0x40) //read if 1 or more interrupts have been generated on INT1_SRC_REG
{
    sprintf(mex, "Overthreshold event \r\n");
    UART_Debug_PutString(mex);
    flag_eeprom=1;
}

OVR_FLAG=1;
}

/* Custom Interrupt on Timer counter */
CY_ISR (ISR_TIMER)
{
    
    /* Read Timer Status Register to reset the counter */
    Timer_ReadStatusRegister();
    
    switch (status){
        case 0:
            if (counter_button!=0){
                timer_counter++;
                if (timer_counter>2000){
            
                    sprintf(mex, "Counter Button: %d \r\n",counter_button);
                    UART_Debug_PutString(mex);
            
                    
                    timer_counter=0;
                    
                    if (counter_button==2){
                        status=1;
                        
                    }
                    counter_button=0;
                }
            }
            break;
        
        case 1:
            
            /* Decrement the counter variable when Interrupt occurs */
                counter_blue--;
                counter_green--;
                counter_red--;
    
                /********************* BLUE LED ******************/
                /* If the counter variable goes to 0, the Blue channel must be toggled*/
                if (counter_blue==0)
                    {
                        /* Toggle the Led only if the period set is lower than the THR_OFF */
                        if (period_blue<=THR_OFF) BLUE_Led_Write(~BLUE_Led_Read());
                        /* Reset the counter to the period value */
                        counter_blue = period_blue;
                    }
    
                /* In case the period is set more than THR_OFF, turn off the Led */
                if (period_blue>THR_OFF)
                    {
                        BLUE_Led_Write(1);
                    }
     
    
                /******************** GREEN LED *******************/
                /* If the counter variable goes to 0, the Green channel must be toggled*/
                if (counter_green==0)
                {
                    /* Toggle the Led only if the period set is lower than the THR_OFF */
                    if (period_green<=THR_OFF) GREEN_Led_Write(~GREEN_Led_Read());
                    /* Reset the counter to the period value */
                    counter_green = period_green;
                }
                /* In case the period is set more than THR_OFF, turn off the Led */
                if (period_green>THR_OFF)
                {
                    GREEN_Led_Write(1);
                }
                
                
                /******************* RED LED **********************/
                /* If the counter variable goes to 0, the Red channel must be toggled*/
                if (counter_red==0 )
                {
                    /* Toggle the Led only if the period set is lower than the THR_OFF */
                    if (period_red<=THR_OFF) RED_Led_Write(~RED_Led_Read());
                    /* Reset the counter to the period value */
                    counter_red = period_red;
                }
                /* In case the period is set more than THR_OFF, turn off the Led */
                if (period_red>THR_OFF)
                {
                    RED_Led_Write(1);
                }
                
                /******************* TIMESTAMP ********************/
                
                timestamp++;
                if (counter_button!=0){
                    timer_counter++;
                    if (timer_counter>2000){
                        
                        sprintf(mex, "Counter Button: %d \r\n",counter_button);
                        UART_Debug_PutString(mex);
                        
                        counter_button=0;
                        timer_counter=0;
                        
                        
                    }
                    
                    
                }
                        
                        
                break;
    
    }
}



 CY_ISR(ISR_COUNTER_BUTTON)
{
    Timer_Button_ReadStatusRegister();
    counter_button++;
    
    sprintf(mex, "Button Pressed \r\n");
    UART_Debug_PutString(mex);
}