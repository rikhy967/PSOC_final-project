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
#ifndef INITIALIZATION_H
    #define INITIALIZATION_H
    
    #include "project.h"
    #include "cytypes.h"
    #include "ErrorCodes.h"
    #include "I2C_Interface.h"
    #include "LIS3DH_Registers.h"
    #include "LIS3DH_Registers_Settings.h"
    #include "Interrupt_Routines.h"
    
    
    /************** INITIALIZATION FUNCTIONS **************/
    // Initialize peripherals for ON MODE 
    void INIT_ON(void);
    // Initialize peripherals for OFF MODE 
    void INIT_OFF(void);
    // Initialize peripherals for CONFIGURATION MODE 
    void INIT_CONF(void);
    
    
#endif

/* [] END OF FILE */
