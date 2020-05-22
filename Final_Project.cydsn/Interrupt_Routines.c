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
sprintf(mex, "INTERRUPT OCCURED\r\n");
UART_Debug_PutString(mex);

}

 CY_ISR(ISR_LIS3DH_FIFO_WATERMARK)
{
OVR_FLAG=1;
//counter= 0;
sprintf(mex, "INTERRUPT OCCURED\r\n");
UART_Debug_PutString(mex);
}