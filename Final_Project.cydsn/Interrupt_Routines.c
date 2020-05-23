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
char mex[50];

CY_ISR(ISR_LIS3DH_FIFO_OVERRUN)
{
OVR_FLAG=1;
//counter= 0;
//sprintf(mex, "INTERRUPT OCCURED\r\n");
//UART_Debug_PutString(mex);

}

 CY_ISR(ISR_LIS3DH_FIFO_WATERMARK)
{
OVR_FLAG=1;
//counter= 0;
//sprintf(mex, "INTERRUPT OCCURED\r\n");
//UART_Debug_PutString(mex);
}

CY_ISR (ISR_TIMER)
{
    
    Timer_ReadStatusRegister();   
    counter_blue--;
    counter_green--;
    counter_red--;
    //////// BLUE LED ////////////////
    if (counter_blue==0)
    {
        if (fq_blue<=100) BLUE_Led_Write(~BLUE_Led_Read());
        counter_blue = fq_blue;
    }
    if (fq_blue>100)
    {
        BLUE_Led_Write(1);
    }
     
    
    //////////GREEN LED //////////////
    if (counter_green==0)
    {
        if (fq_green<=100) GREEN_Led_Write(~GREEN_Led_Read());
        counter_green = fq_green;
    }
    if (fq_green>100)
    {
        GREEN_Led_Write(1);
    }
    
    
    //////////////RED LED //////////////////
    if (counter_red==0 )
    {
        if (fq_red<=100) RED_Led_Write(~RED_Led_Read());
        counter_red = fq_red;
    }
    if (fq_red>100)
    {
        RED_Led_Write(1);
    }
    
    
    
}