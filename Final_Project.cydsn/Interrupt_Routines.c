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

/*********************   INTERRUPT ROUTINES FILE   *************************/

/* Inclusion of Header files */
#include "project.h"
#include "Interrupt_Routines.h"
#include "stdio.h"
#include "ErrorCodes.h"
#include "I2C_Interface.h"
#include "LIS3DH_Registers.h"
#include "LIS3DH_Registers_Settings.h"

/***************** VARIABLES' INITIALIZATION *************/

ErrorCode error;
uint8_t watermark;
uint8_t int1_src_reg;
uint8_t prev_state=0;
uint8_t short_press=0;
uint8_t long_press=0;

volatile uint16_t counter_button=0;

uint16_t timer_counter=0;


/* Variable that contains the message for UART debug communication*/
char mex[50];


/******************* Custom Interrupt on FIFO watermark ********************/

 CY_ISR(ISR_LIS3DH_FIFO_WATERMARK){
    
    // Read INT1 SRC Register
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT1_SRC,
                                         &int1_src_reg);
    if(int1_src_reg & 0x40) //read if 1 or more interrupts have been generated on INT1_SRC_REG
    {
        sprintf(mex, "Overthreshold event \r\n");
        UART_Debug_PutString(mex);
        flag_eeprom=1; // Write overthreshold data on EEPROM
    }

    OVR_FLAG=1; // Start reading data from LIS3DH
}

/****************** Custom Interrupt on Timer counter ************************/

// Period = 1ms

CY_ISR (ISR_TIMER)
{
    
    /* Read Timer Status Register to reset the counter */
    Timer_ReadStatusRegister();
    
    switch (status){
        /************      OFF MODE      *****************/
        case 0:
            // Check if button has been pressed
            if ((short_press!=0) | (long_press!=0)){
                timer_counter++;
                if (timer_counter>2000){
            
                    
            
                    
                    timer_counter=0;
                    
                    if (short_press==2){
                        sprintf(mex, "Switch ON device \r\n");
                        UART_Debug_PutString(mex);
                        //repetition=0;
                        status=1;
                        
                    }
                    if (long_press>=1){
                        prev_state=0;
                        sprintf(mex, "Switch to Configuration Mode \r\n");
                        UART_Debug_PutString(mex);
                        //repetition=0;
                        status=2;
                        
                    }
                    
                    counter_button=0;
                    short_press=0;
                    long_press=0;
                }
            }
            
        break;
        /************      ON MODE      *****************/
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
                
                /******************* INTERNAL BUTTON ********************/
                // When the button is pressed, increment the timer counter
                if ((short_press!=0) | (long_press!=0)){
                    timer_counter++;
                    if (timer_counter>2000){
                        
                        // If button was pressed 2 times, go to OFF MODE
                        if (short_press==2){
                            sprintf(mex, "Switch OFF \r\n");
                            UART_Debug_PutString(mex);
                            //repetition=0;
                            status=0;
                            
                            
                        }
                        // If button was pressed more than 3 times (long press), go to CONFIGURATION MODE
                        else if (long_press>=1){
                            
                            prev_state=1;
                            sprintf(mex, "Switch to Configuration Mode \r\n");
                            UART_Debug_PutString(mex);
                            //repetition=0;
                            status=2;
                            
                        
                        }
                         
                        // Reset counters
                        counter_button=0;
                        timer_counter=0;
                        short_press=0;
                        long_press=0;
                        
                        
                    }
                    
                    
                }
                        
                        
        break;
                
        /************      CONFIGURATION MODE      *****************/        
        case 2:
                
                
                //counter_int_led--;
                /* Decrement the counter variable when Interrupt occurs */
                counter_blue--;
                
                
                
                 /******************* INTERNAL BUTTON ********************/
                // When the button is pressed, increment the timer counter
                if ((short_press!=0) | (long_press!=0)){
                    timer_counter++;
                    if (timer_counter>2000){
                        
                        
                        // If button was pressed more than 3 times (long press), go to the previous mode
                        if (long_press>=1){
                            sprintf(mex, "Switch to previous Mode \r\n");
                            UART_Debug_PutString(mex);
                            //repetition=0;
                            status= prev_state;

                        
                        }
                        
                        // Reset counters
                        counter_button=0;
                        timer_counter=0;
                        short_press=0;
                        long_press=0;
                    }
                }
                
   
                /********************* BLUE LED ******************/
                /* If the counter variable goes to 0, the Blue channel must be toggled*/
                if (counter_blue==0)
                    {
                        BLUE_Led_Write(~BLUE_Led_Read());
                        /* Reset the counter to the period value */
                        counter_blue = period_blue;
                    }
    

        break;
    
    }
}

/****************** Custom Interrupt on Timer Button counter ************************/

// When the Internal Button is pressed, the 'q' output of the Debouncer goes to 0.
// Using a NOT port, this pression 'enable' the Timer Button coumponents, which starts
// to increment. An interrupt is generated on its Timer Counter with a period of 100ms

 CY_ISR(ISR_COUNTER_BUTTON)
{
    // Read Status Register in order to reset it
    Timer_Button_ReadStatusRegister();
    // Increment by 1 the counter button variable
    long_press++;
    
    sprintf(mex, "Long Button Pressed \r\n");
    UART_Debug_PutString(mex);
    
    
}

CY_ISR(ISR_DEBOUNCER)
{
    short_press++;
    // Read Status Register in order to reset it
    Timer_Button_ReadStatusRegister();
    //sprintf(mex, "Short Pressed \r\n");
    //UART_Debug_PutString(mex);
    sprintf(mex, "N press %d \r\n",short_press);
    UART_Debug_PutString(mex);
}