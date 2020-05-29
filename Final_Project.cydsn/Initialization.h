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
    
    void INIT_START(void);
    void INIT_STOP(void);
    void INIT_MENU(void);
    
    
#endif

/* [] END OF FILE */
