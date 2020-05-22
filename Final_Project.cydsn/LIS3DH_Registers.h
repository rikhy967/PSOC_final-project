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
//brief 7-bit I2C address of the slave device.
#define LIS3DH_DEVICE_ADDRESS 0x18

//brief Address of the WHO AM I register
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

//brief Address of the Status register
#define LIS3DH_STATUS_REG 0x27

//brief Address of the Control register 1
#define LIS3DH_CTRL_REG1 0x20

//brief Hex value to set high resolution mode 100Hz to the accelerator
#define LIS3DH_NORMAL_MODE_100_CTRL_REG1 0x57

//brief Address of the Control register 4
#define LIS3DH_CTRL_REG4 0x23

//output registers updated after MSB and LSB reading, FSR [-4g;+4g], high resolution mode
#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x90 

//brief Address of the out_x_l register
#define LIS3DH_OUT_X_L  0x28

//brief Address of the out_y_l register
#define LIS3DH_OUT_Y_L  0x2A

//brief Address of the out_z_l register
#define LIS3DH_OUT_Z_L  0x2C

#define LIS3DH_INT1_SRC 0x31
/* [] END OF FILE */
