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
    
    CY_ISR_PROTO(ISR_LIS3DH_FIFO_OVERRUN);
    CY_ISR_PROTO(ISR_LIS3DH_FIFO_WATERMARK);
    CY_ISR_PROTO (ISR_TIMER);
    volatile uint8_t fq_red;
    volatile uint8_t fq_green;
    volatile uint8_t fq_blue;
    volatile uint8_t counter_red;
    volatile uint8_t counter_green;
    volatile uint8_t counter_blue;
    
    volatile uint8_t OVR_FLAG;
    //volatile uint8_t counter = 0;
    
#endif
/* [] END OF FILE */
