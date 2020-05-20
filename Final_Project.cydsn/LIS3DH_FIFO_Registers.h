//info for ctrl_reg 5
// brief address of control register 5
#define LIS3DH_CTRL_REG_5 0x24
//fifo enable
#define FIFO_ENABLE 0x40


//info for ctrl_reg 3
// brief address of control register 3
#define LIS3DH_CTRL_REG_3 0x22
//interrupt on fifo watermark
#define INT_FIFO_WMK 0x04

//interrupt on fifo overrun
#define INT_FIFO_OVERRUN 0x02

//info for fifo_ctrl_reg
//brief address of fifo control register
#define LIS3DH_FIFO_CTRL_REG 0x2E
//mask for fifo overrun
#define FIFO_OVR 0x40
//mask for fifo empty
#define FIFO_EMPTY 0x20

//info for fifo_src_reg
// brief address of fifo src reg
#define LIS3DH_FIFO_SRC_REG_3 0x2F


//definition of fifo modes
#define BYPASS_MODE 0x00
#define FIFO_MODE 0X40
#define STREAM_MODE 0x80
#define STREAM_TO_FIFO_MODE 0xC0
