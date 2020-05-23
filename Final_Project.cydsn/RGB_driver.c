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
#include "RGB_driver.h"

void RGB_Start(){
 
    PWM_RG_Start();
    PWM_B_INT_Start();
   
}

void RGB_Stop(){
    
    PWM_RG_Stop();
    PWM_B_INT_Stop();
    
}

/*void RGBLed_WriteFrequency(uint8_t color, uint8_t fq_value){
    
    switch (color)
    {
        case 0:
            PWM_RG_WriteCompare1();
            PWM_RG_WritePeriod
        
        
        
    }
    
    
    
    
    
}*/





/* [] END OF FILE */
