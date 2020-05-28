#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "math.h"
#include "LIS3DH_Registers.h"
#include "LIS3DH_Registers_Settings.h"
#include "Interrupt_Routines.h"
#include "SPI_Interface.h"
#include "EEPROM_Interface.h"

#define EEPROM_REG_THR 0x0002



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
    
    /***************** VARIABLES' INITIALIZATION *************/
    
    /* Initialization of blinking period of the RGB led channels in order to be set OFF */
    period_red = THR_OFF; 
    period_green = THR_OFF;
    period_blue = THR_OFF;
    
    /* Initialization of the counter needed for toggle of RGB Led */
    counter_red = period_red;
    counter_green = period_green;
    counter_blue = period_blue;
    
    /* String to print out messages on the UART */
    char message[50];
    
    /* Status variable for Hardware menu*/
    
    status=0;
    /**************** COMPONENTS' INITIALIZATION *******************/
    
    /* Timer */
    Timer_Start(); 
    
    /* I2C communication */
    CS_LIS3DH_Write(1); 
    SDO_LIS3DH_Write(0);
    I2C_Peripheral_Start();
    SPIM_1_Start();
    CyDelay(10);
    Timer_Button_Start();
    /* UART Debug communication*/
    UART_Debug_Start();
    
    
    
    /***************** CUSTOM ISR INITIALIZATION ******************/
    
    //isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_OVERRUN); //LIS3DH interrupt enable
    
    /* Timer */
    isr_TIMER_StartEx(ISR_TIMER);
    
    /* LIS3DH Watermark*/
    isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_WATERMARK);
    isr_Button_StartEx(ISR_COUNTER_BUTTON);
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    timestamp=0;

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_NORMAL_MODE_100_CTRL_REG1)
    {
        ctrl_reg1 = LIS3DH_NORMAL_MODE_100_CTRL_REG1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
     /******************************************/
     /* Read Control Register 4 */
     /******************************************/

    
    
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_REG4_BDU_ACTIVE; 
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    /******************************************************/
    // FIFO CONFIGURATION 
    /******************************************************/
    ////////// CONTROL REGISTER 5 ///////////////////
     
    
   
    uint8_t ctrl_reg_5 = FIFO_ENABLE_LATCH;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG_5,
                                             ctrl_reg_5);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 5 FIFO ENABLE successfully written as: 0x%02X\r\n", ctrl_reg_5);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
    
    //////////////// FIFO CONTROL REGISTER /////////////////
    
    
    
   
    uint8_t    fifo_ctrl_reg = BYPASS_MODE;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_FIFO_CTRL_REG,
                                             fifo_ctrl_reg);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 5 BYPASS MODE successfully written as: 0x%02X\r\n", fifo_ctrl_reg);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
    
    
    //ENABLING FIFO INTERRUPT ON OVERRUN
         uint8_t ctrl_reg_3 = INT_FIFO_WMK_IA1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG_3,
                                             ctrl_reg_3);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 3 OVERRUN INT ENABLE successfully written as: 0x%02X\r\n", ctrl_reg_3);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
    
    /******************************************************/
    // REGISTERS FOR OVER THRESHOLD EVENTS CONFIGURATION 
    /******************************************************/
    //INT1 CONFIGURATION REGISTER
    uint8_t int1_cfg_reg = INT1_0R_6D;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT1_CFG,
                                             int1_cfg_reg);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "INT1 CONF REGISTER 0R 6D successfully written as: 0x%02X\r\n", int1_cfg_reg);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
     //INT1 THRESHOLD REGISTER   
        
     uint8_t int1_ths_reg = INT1_THS;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT1_THS,
                                             int1_ths_reg);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "INT1 THS REGISTER successfully written as: 0x%02X\r\n", int1_ths_reg);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
    //INT1 DURATION REGISTER
         uint8_t int1_duration_reg = INT1_DURATION;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT1_DURATION,
                                             int1_duration_reg);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "INT1 DURATION REGISTER successfully written as: 0x%02X\r\n", int1_duration_reg);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    /*******************************************************/
    
    
    
    //variables to save the registers output value in digit
    int16_t Out_X; 
    int16_t Out_Y;
    int16_t Out_Z;
    
    //variables to save the output value in mg
    int16_t Out_X_mg; 
    int16_t Out_Y_mg;
    int16_t Out_Z_mg;
    
    //float variables to save the output value in m/s^2
    float32 Out_X_ms2;
    float32 Out_Y_ms2;
    float32 Out_Z_ms2;
    
    //int  variables for the casting of the float values, the obtained results are acceleration
    //values in mm/s^2
    int32_t Out_X_mms2;
    int32_t Out_Y_mms2;
    int32_t Out_Z_mms2;
    
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[8]; 
    uint8_t X_Data[2];
    uint8_t Y_Data[2];
    uint8_t Z_Data[2];
    
    // Variables that contains the mean of the acceleration in a single FIFO burst
    
    int16_t mean_X_mg=0; 
    int16_t mean_Y_mg=0;
    int16_t mean_Z_mg=0;
    
    
    OutArray[0] = header;
    OutArray[7] = footer;
    
    
    uint8_t  fss; // Number of unread samples
    
    uint8_t flag_mode = 1; // 1= Stream Mode, 0= Read data
    //uint8_t overrun_reg;
    //uint8_t empty_reg;
    //uint8_t wmk_reg;
    //uint8_t flag_overrun = 0;
    uint8_t counter=0;
    uint8_t  int1_src_reg;
    uint8_t data_EEPROM_THR[46];
    uint8_t read_data_EEPROM_THR[46];
    int16 data_write;
    int16 data_read;
    int32 timestamp_read;
    uint8_t init=0;
    
    fifo_ctrl_reg = FIFO_MODE_THR_WMK;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_FIFO_CTRL_REG,
                                             fifo_ctrl_reg);
            
    
    for(;;)
    {   
        
        
        switch (status){
            case 0:
            
                RED_Led_Write(1);
                GREEN_Led_Write(1);
                BLUE_Led_Write(1);
                INT_Led_Write(0);
                
                isr_LIS3DH_Stop();
         
                timestamp=0;
                I2C_Peripheral_Stop();
                SPIM_1_Stop();
                //UART_Debug_Stop();
                fifo_ctrl_reg = BYPASS_MODE;
                error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_FIFO_CTRL_REG,
                                             fifo_ctrl_reg);
                
                init=1;
            
            break;
                
            
        
            case 1:
                if (init==1){
                    fifo_ctrl_reg = STREAM_MODE_THR_WMK;
                    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_FIFO_CTRL_REG,
                                                 fifo_ctrl_reg);
                    INT_Led_Write(1);
                    isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_WATERMARK);
                    
                    I2C_Peripheral_Start();
                    SPIM_1_Start();
                    CyDelay(10);
                    init=0;
                }
                
                
        
        
                while (OVR_FLAG){
            
             
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_X_L,
                                            2,
                                            &X_Data[0]);
            
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_Y_L,
                                            2,
                                            &Y_Data[0]);
            
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_Z_L,
                                            2,
                                            &Z_Data[0]);
            
            if(error == NO_ERROR)
              {
               Out_X = (int16)((X_Data[0] | (X_Data[1]<<8)))>>6;//out_x in digit (10bit)
               Out_X_mg = Out_X*4;//out_x in mg, the sensitivity is 4mg/digit in this operation mode (13 bits needed [-4096;+4095])
               Out_X_ms2 = Out_X_mg*9.81/1000;//out_x in m/s^2 (float variable)

     //cast of the float variable to int that mantains the first 3 decimals. now the value is in mm/s^2
               Out_X_mms2 = (int32)((Out_X_ms2*1000));//int32 needed since this value can exceed the ones covered by an int16
               OutArray[1] = (uint8_t)(Out_X_mg & 0xFF);
               OutArray[2] = (uint8_t)(Out_X_mg >> 8);
               
            
               mean_X_mg = mean_X_mg+Out_X_mg;
             
               
               Out_Y = (int16)((Y_Data[0] | (Y_Data[1]<<8)))>>6;//out_y in digit (10bit)
               Out_Y_mg = Out_Y*4;//out_y in mg (13 bits needed)
               Out_Y_ms2 = Out_Y_mg*9.81/1000;
               Out_Y_mms2 = (int32)((Out_Y_ms2*1000));
               OutArray[3] = (uint8_t)(Out_Y_mg & 0xFF);
               OutArray[4] = (uint8_t)(Out_Y_mg >> 8);
               
            
               mean_Y_mg = mean_Y_mg+Out_Y_mg;
            
               Out_Z = (int16)((Z_Data[0] | (Z_Data[1]<<8)))>>6;//out_z in digit (10bit)
               Out_Z_mg= Out_Z*4;//out_z in mg (13 bits needed)
               Out_Z_ms2=Out_Z_mg*9.81/1000;
               Out_Z_mms2= (int32)((Out_Z_ms2*1000));
               OutArray[5] = (uint8_t)(Out_Z_mg & 0xFF);
               OutArray[6] = (uint8_t)(Out_Z_mg>> 8);
               
            
               mean_Z_mg = mean_Z_mg+Out_Z_mg;
            
            
                //sprintf(message, "X: %d    Y: %d     Z:%d\r\n",Out_X_mg,Out_Y_mg,Out_Z_mg);
                //UART_Debug_PutString(message);
                
                
                if (flag_eeprom==1){
                    isr_TIMER_Stop(); 
                    isr_LIS3DH_Stop();
                    
                        
                    int i;
                    for (i=0;i<6;i=i+2){
                    
                        data_EEPROM_THR[i+counter*6]=OutArray[i+1];
                        data_EEPROM_THR[i+counter*6+1]=OutArray[i+1+1];
                        data_write = (data_EEPROM_THR[i+counter*6] | (data_EEPROM_THR[i+counter*6+1]<<8));
                        
                        sprintf(message, "Data write: %d \r\n",data_write);
                        UART_Debug_PutString(message);
                        
                        
                        //sprintf(message, "N cycle: %d\r\n\n", counter);
                        //UART_Debug_PutString(message);
                
                    }
                    if (counter==6)
                    {
                        data_EEPROM_THR[42]=(uint8_t)(timestamp & 0xFF);
                        data_EEPROM_THR[43]=(uint8_t)(timestamp >> 8);
                        data_EEPROM_THR[44]=(uint8_t)(timestamp >> 16);
                        data_EEPROM_THR[45]=(uint8_t)(timestamp >> 24);
                        
                        sprintf(message, "Timestamp : %d \r\n",timestamp);
                        UART_Debug_PutString(message);
                        
                        
                        EEPROM_writePage( EEPROM_REG_THR,data_EEPROM_THR,46); 
                        EEPROM_waitForWriteComplete();
                        
                        sprintf(message, "End Write on EEPROM\r\n");
                        UART_Debug_PutString(message);
                        flag_eeprom=0;
                        
                        EEPROM_readPage( EEPROM_REG_THR,read_data_EEPROM_THR,46);
                        sprintf(message,"End Read on EEPROM\r\n");
                        UART_Debug_PutString(message);
                        
                        for (i=0;i<42;i=i+2)
                        {
                            data_read = (read_data_EEPROM_THR[i] | (read_data_EEPROM_THR[i+1]<<8));
                            sprintf(message, "Data read: %d\r\n",data_read);
                            UART_Debug_PutString(message);
                            
                            
                        }
                        timestamp_read = (read_data_EEPROM_THR[42] | (read_data_EEPROM_THR[43]<<8) | (read_data_EEPROM_THR[44]<<16)|(read_data_EEPROM_THR[45]<<24));
                        sprintf(message, "Timestamp : %ld \r\n",timestamp_read);
                        UART_Debug_PutString(message);
                        
                        
                    }    
                    isr_TIMER_StartEx(ISR_TIMER);
                    isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_WATERMARK);
                    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                     LIS3DH_INT1_SRC,
                                     &int1_src_reg);
                }
               /*error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_FIFO_SRC_REG_3,
                                        &fss);
               fss= fss & (0b00011111);*/
            
            
               //UART_Debug_PutArray(OutArray, 14);
            
            
            counter++;
            
            /*error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_FIFO_SRC_REG_3,
                                        &wmk_reg);//&empty_reg);*/
            
            if (counter>=WMK_THRESHOLD) {
                
                mean_X_mg = mean_X_mg/counter;
                mean_Y_mg = mean_Y_mg/counter;
                mean_Z_mg = mean_Z_mg/counter;
                
                //sprintf(message, "meanX: %d    meanY: %d     meanZ:%d\r\n",mean_X_mg,mean_Y_mg,mean_Z_mg);
                //UART_Debug_PutString(message);
                
                mean_X_mg = abs(mean_X_mg);
                mean_Y_mg = abs(mean_Y_mg);
                mean_Z_mg = abs(mean_Z_mg);
                
                //sprintf(message, "abs meanX: %d    abs meanY: %d     abs meanZ:%d\r\n",mean_X_mg,mean_Y_mg,mean_Z_mg);
                //UART_Debug_PutString(message);
                
                //period_green = -49*mean_X_mg/960+1249/12;
                //period_blue = -49*mean_Y_mg/960+1249/12;
                //period_red =-49*mean_Z_mg/960+1249/12;
                
                period_green = 0.0001*mean_X_mg*mean_X_mg-0.4*mean_X_mg+402;
                period_blue = 0.0001*mean_Y_mg*mean_Y_mg-0.4*mean_Y_mg+402;
                period_red = 0.0001*mean_Z_mg*mean_Z_mg-0.4*mean_Z_mg+402;
                
                //sprintf(message, "counter: %d\r\n\n", counter);
                //UART_Debug_PutString(message);
                
                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_FIFO_SRC_REG_3,
                                        &fss);
                fss= fss & (0b00011111);
                //sprintf(message, "unread samples: %d\r\n\n", fss);
                //UART_Debug_PutString(message);
                
                
                
                
                //sprintf(message, "fq_green: %d    fq_blue: %d     fq_red:%d\r\n",period_green,period_blue,period_red);
                //UART_Debug_PutString(message);
                
                mean_X_mg=0;
                mean_Y_mg=0;
                mean_Z_mg=0;
                
                counter = 0;
              
                
                if(fss > 1) OVR_FLAG = 1;
                else OVR_FLAG =0 ;
                //flag_mode=1;
            
            }
                
        }
        
        
    }
                
                break;
        }
        
    
//}
    }
}