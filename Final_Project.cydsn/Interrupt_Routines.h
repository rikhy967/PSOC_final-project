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
#ifndef INTERRUPT_ROUTINES_H
    #define INTERRUPT_ROUTINES_H
    
    #include "project.h"
    #include "cytypes.h"
    #include "ErrorCodes.h"
    
  
    /* Interrupt on FIFO watermark */
    CY_ISR_PROTO(ISR_LIS3DH_FIFO_WATERMARK);
    /* Interrupt on Timer counter */
    CY_ISR_PROTO (ISR_TIMER);
    
    /* Flag that is set to 1 when an overrun occurs on LIS3DH FIFO*/
    volatile uint8_t OVR_FLAG;
    
    
    /* Threshold that defines when the RGB channel is set OFF */
    #define THR_OFF 363 // --> 100 mg
    
    /* Global variables that contains each period of RGB Led channels */
    volatile uint16_t period_red;
    volatile uint16_t period_green;
    volatile uint16_t period_blue;
    /* Global variables that contains the counters for toggle RGB Led channels */
    volatile uint16_t counter_red;
    volatile uint16_t counter_green;
    volatile uint16_t counter_blue;
    
    
#endif

