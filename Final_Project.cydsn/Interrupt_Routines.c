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

/* Variable that contains the message for UART debug communication*/
char mex[50];

/* Custom Interrupt on FIFO overrun */
CY_ISR(ISR_LIS3DH_FIFO_OVERRUN)
{
    OVR_FLAG=1;
//counter= 0;
//sprintf(mex, "INTERRUPT OCCURED\r\n");
//UART_Debug_PutString(mex);

}

/* Custom Interrupt on FIFO watermark */
 CY_ISR(ISR_LIS3DH_FIFO_WATERMARK)
{
    OVR_FLAG=1;
//counter= 0;
//sprintf(mex, "INTERRUPT OCCURED\r\n");
//UART_Debug_PutString(mex);
}

/* Custom Interrupt on Timer counter */
CY_ISR (ISR_TIMER)
{
    
    /* Read Timer Status Register to reset the counter */
    Timer_ReadStatusRegister();
    
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
    
}