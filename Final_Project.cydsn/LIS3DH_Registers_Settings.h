
#ifndef LIS3DH_FIFO_REGISTER_H
    #define LIS3DH_FIFO_REGISTER_H
    
    
    /*********** LIS3DH CONTROL REGISTER 5 **********/
    /* Address of control register 5 */
    #define LIS3DH_CTRL_REG_5 0x24
    /* Enable FIFO mode */
    #define FIFO_ENABLE 0x40
     /* Enable FIFO mode and latch request for INT1_SRC_REG */
    #define FIFO_ENABLE_LATCH 0x48


    /*********** LIS3DH CONTROL REGISTER 3 **********/
    /* Address of control register 3 */
    #define LIS3DH_CTRL_REG_3 0x22
    /* Enable interrupt on FIFO watermark */
    #define INT_FIFO_WMK 0x04
     /* Enable interrupt on FIFO watermark + IA1 interrupt on INT1 */
    #define INT_FIFO_WMK_IA1 0x44
    /* Enable interrupt on FIFO overrun */
    #define INT_FIFO_OVERRUN 0x02

    
    /*********** LIS3DH FIFO CONTROL REGISTER **********/
    /* Address of FIFO control register */
    #define LIS3DH_FIFO_CTRL_REG 0x2E
    /* FIFO mode + selection of number of samples for watermark */
    #define FIFO_MODE_THR_WMK 0x47
    /* Definition of FIFO modes */
    #define BYPASS_MODE 0x00
    #define FIFO_MODE 0x40
    #define STREAM_MODE 0x80
    #define STREAM_MODE_THR_WMK 0x86 //stream mode and 7 samples for watermark threshold
    #define WMK_THRESHOLD 7
    #define STREAM_TO_FIFO_MODE 0xC0

    
    /*********** LIS3DH FIFO SOURCE REGISTER **********/
    /* Address of FIFO source register */
    #define LIS3DH_FIFO_SRC_REG_3 0x2F
    /* Mask for FIFO overrun*/
    #define FIFO_OVR 0x40
    /* Mask for FIFO empty*/
    #define FIFO_EMPTY 0x20
    /* Mask for FIFO watermark*/
    #define FIFO_WMK 0x80

    /*********** LIS3DH INT1 CONFIGURATION REGISTER **********/
    /* */
    #define INT1_0R_6D 0x2A //interrupt OR mode and enabling of interrupts on high events
    
    /*********** LIS3DH INT1 THS REGISTER **********/
    /* */
    #define INT1_THS 0x46 
    
    /*********** LIS3DH INT1 DURATION REGISTER **********/
    /* */
    #define INT1_DURATION 0x03 //30 ms over threshold 
    
    
    
#endif
