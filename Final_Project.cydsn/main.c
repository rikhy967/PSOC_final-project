
/********************  FINAL PROJECT  ************************/

// Title: 

// Students: Riccardo Levi & Simone Toffoli

// Course: Laboratorio di Tecnologie Elettroniche e Biosensori

// Professor: Pietro Cerveri

/*************************************************************/


/*********************   MAIN FILE   *************************/


/* Inclusion of Header files */
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "LIS3DH_Registers.h"
#include "LIS3DH_Registers_Settings.h"
#include "Interrupt_Routines.h"
#include "SPI_Interface.h"
#include "EEPROM_Interface.h"
#include "Initialization.h"

/* Definition of EEPROM Register where data will be saved */
#define EEPROM_REG_THR 0x0002   // Save over threshold data from LIS3DH 
#define EEPROM_SS_PARAM 0x0000  // Save current status (ON,OFF) and the data rate of LIS3DH


/* MAIN */
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
    
    /***************** VARIABLES' INITIALIZATION *************/
    
    /* Initialization of blinking period of the RGB led channels in order to be set OFF */
    period_red = THR_OFF+1; 
    period_green = THR_OFF+1;
    period_blue = THR_OFF+1;
    
    /* Initialization of blinking period of the Internal LED (fixed value)*/
    period_int_led=100;
    
    /* Initialization of the counter needed for toggle of RGB Led and Internal Led */
    counter_red = period_red;
    counter_green = period_green;
    counter_blue = period_blue;
    counter_int_led = period_int_led;
    
    /* String to print out messages on the UART */
    char message[50];
    
    /* Set 'timestamp' variable to 0 */
    timestamp=0;
    
    /* Set 'status' variable to 0 (OFF MODE) */
    status=0;
    
    /**************** COMPONENTS' INITIALIZATION *******************/
    
    /* Timer */
    Timer_Start(); 
    Timer_Button_Start();
    
    /* I2C communication */
    CS_LIS3DH_Write(1); 
    SDO_LIS3DH_Write(0);
    I2C_Peripheral_Start();
    
    /*SPI communication*/
    SPIM_1_Start();
    CyDelay(10);
    
    /* UART Debug communication*/
    UART_Debug_Start();
    
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    

    /* Check which devices are present on the I2C bus*/
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    
    /**********  Read LIS3DH Register using I2C communication *********/
    
    
    /* Read  LIS3DH - WHO AM I Register */
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
    
    /* Read LIS3DH - STATUS Register */
    
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
    
    
    /* Read LIS3DH - CONTROL Register 1 */
    
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
    
    
    /*  Write 100 Hz data rate (Normal Mode) on LIS3DH - CONTROL Register 1  */
    
    
        
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
    
    
    /* Read LIS3DH - CONTROL Register 4 */


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
    
    
    /*  Write BDU active on LIS3DH - CONTROL Register 4  */
    
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
    /*                 FIFO CONFIGURATION                 */
    /******************************************************/
    
    
    /*  Write FIFO enable and LATCH enable on LIS3DH - CONTROL Register 5  */
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
    
    
    /*  Write BYPASS MODE on LIS3DH - FIFO CONTROL Register  */

   
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
    
    
    
    /*  Write FIFO WATERMARK INTERRUPT on LIS3DH -  CONTROL Register 3 */
        
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
    
    
    /*********************************************************/
    /* CONFIGURATION OF REGISTERS FOR OVER THRESHOLD EVENTS  */ 
    /*********************************************************/
        
    /*  Write OR on INT1 on LIS3DH -  INT1 CFG Register */
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
    
    /*  Write THRESHOLD on LIS3DH -  INT1 THS Register */  
        
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
    
    /*  Write DURATION on LIS3DH -  INT1 DURATION Register */  
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
        
        
    
    
    /********************************************************/    
    /*                 VARIABLES' DEFINITION                */
    /********************************************************/
    
    //variables to save the registers output value in digit
    int16_t Out_X; 
    int16_t Out_Y;
    int16_t Out_Z;
    
    //variables to save the output value in mg
    int16_t Out_X_mg; 
    int16_t Out_Y_mg;
    int16_t Out_Z_mg;
    
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[8]; 
    OutArray[0] = header;
    OutArray[7] = footer;
    
    uint8_t X_Data[2];
    uint8_t Y_Data[2];
    uint8_t Z_Data[2];
    
    // Variables that contains the mean of the acceleration in a single FIFO burst
    
    int16_t mean_X_mg=0; 
    int16_t mean_Y_mg=0;
    int16_t mean_Z_mg=0;
    

    uint8_t  fss;                             // Number of unread samples
    uint8_t counter=0;                        // Variable used for counting the number of while cycles when INT1 occurs
    uint8_t  int1_src_reg;                    // Value to be saved on INT1 SRC REG
    uint8_t data_EEPROM_THR[46];              // Array for saving overthreshold data on EEPROM
    uint8_t read_data_EEPROM_THR[46];         // Array used for cheacking saved data on EEPROM 
    int16 data_write;                         //
    int16 data_read;                          //
    int32 timestamp_read;                     // Store timestamp value
    int32 pot_digit;                          // Store Potentiometer value in digit
    int32 pot_mv;                             // Store Potentiometer value in mV
    uint16_t data_rate=5;                     // Initialization of data rate value (5 --> 100Hz)
    
    uint8_t repetition_stop=0;
    uint8_t repetition_start=0;
    uint8_t repetition_menu=0;
    uint8_t data_byte;
    uint8_t byte_read;
    
    /*****************************************************/
    /*             INITIALIZATION OF CUSTOM ISR          */
    /*****************************************************/
    isr_TIMER_StartEx(ISR_TIMER);
    isr_Button_StartEx(ISR_COUNTER_BUTTON);
    isr_Debouncer_StartEx(ISR_DEBOUNCER);
    
    
    
    /****************************************************/
    /*             START INFINITE LOOP                  */
    /****************************************************/
    for(;;)
    {   
        
        /**************************************************/
        /*           SWITCH BETWEEN STATUS                */
        /* The device implements 3 diffentent modalities  */
        /* that are handled by switch-case statement      */
        /*                                                */
        /* status = 0  OFF MODE                           */
        /* status = 1  ON MODE                            */
        /* status = 2  CONFIGURATION MODE                 */
        /**************************************************/
        
        
        switch (status){
            
            /************      OFF MODE      *****************/
            case 0:
                if (repetition_stop==0){
                    
                    sprintf(message, "CASE 0 Repetition flag: %d \r\n",repetition_stop);
                    UART_Debug_PutString(message);
                    
                    // Start SPI communication with EEPROM
                    SPIM_1_Start();
                    CyDelay(10);
                    data_byte=(0x00 | data_rate);
                    // Write the data rate on EEPROM register
                    EEPROM_writeByte(EEPROM_SS_PARAM, data_byte);
                    EEPROM_waitForWriteComplete();
                    // Check if the byte has been written correctly
                    byte_read= EEPROM_readByte(EEPROM_SS_PARAM);
                    sprintf(message, "Byte read: %d \r\n",byte_read);
                    UART_Debug_PutString(message);
                      
                    // Init pheripheral that are needed in this mode
                    INIT_OFF();
                    
                    repetition_stop=1;
                    repetition_start=0;
                    repetition_menu=0;
                }
                /*ADC_DelSig_Stop();         
                RED_Led_Write(1);
                GREEN_Led_Write(1);
                BLUE_Led_Write(1);
                INT_Led_Write(0);*/
                          
                // Reset timestamp counter          
                timestamp=0;
                          
                         
                        
                break;
            /************      ON  MODE      *****************/        
            case 1:
                      
                if(repetition_start==0){
                    
                    sprintf(message, "CASE 1 Repetition flag: %d \r\n",repetition_start);
                    UART_Debug_PutString(message);
                    
                    // Start SPI communication with EEPROM
                    SPIM_1_Start();
                    CyDelay(10);
                    data_byte=(0x80 | data_rate);
                    // Write the data rate on EEPROM register
                    EEPROM_writeByte(EEPROM_SS_PARAM, data_byte);
                    EEPROM_waitForWriteComplete();
                    // Check if the byte has been written correctly
                    byte_read= EEPROM_readByte(EEPROM_SS_PARAM);
                    sprintf(message, "Byte read: %d \r\n",byte_read);
                    UART_Debug_PutString(message);
                      
                    // Init pheripheral that are needed in this mode
                    INIT_ON();
                    
                    repetition_stop=0;
                    repetition_start=1;
                    repetition_menu=0;
                    
                      
                    }
                      
                // If OVR_FLAG == 1, a WATERMARK Interrupt has been occured, therefore start reading data    
                
                while (OVR_FLAG){
            
                    /**************************************************************/
                    /*                   READING DATA FROM LIS3DH                 */
                    /**************************************************************/
                    
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
            
                    if(error == NO_ERROR){
                        Out_X = (int16)((X_Data[0] | (X_Data[1]<<8)))>>6;//out_x in digit (10bit)
                        Out_X_mg = Out_X*4;//out_x in mg, the sensitivity is 4mg/digit in this operation mode (13 bits needed [-4096;+4095])
                        //Out_X_ms2 = Out_X_mg*9.81/1000;//out_x in m/s^2 (float variable)

         
                        //Out_X_mms2 = (int32)((Out_X_ms2*1000));//int32 needed since this value can exceed the ones covered by an int16
                        OutArray[1] = (uint8_t)(Out_X_mg & 0xFF);
                        OutArray[2] = (uint8_t)(Out_X_mg >> 8);
                   
                
                        mean_X_mg = mean_X_mg+Out_X_mg;
                 
                   
                        Out_Y = (int16)((Y_Data[0] | (Y_Data[1]<<8)))>>6;//out_y in digit (10bit)
                        Out_Y_mg = Out_Y*4;//out_y in mg (13 bits needed)
                        //Out_Y_ms2 = Out_Y_mg*9.81/1000;
                        //Out_Y_mms2 = (int32)((Out_Y_ms2*1000));
                        OutArray[3] = (uint8_t)(Out_Y_mg & 0xFF);
                        OutArray[4] = (uint8_t)(Out_Y_mg >> 8);
                   
                
                        mean_Y_mg = mean_Y_mg+Out_Y_mg;
                
                        Out_Z = (int16)((Z_Data[0] | (Z_Data[1]<<8)))>>6;//out_z in digit (10bit)
                        Out_Z_mg= Out_Z*4;//out_z in mg (13 bits needed)
                        //Out_Z_ms2=Out_Z_mg*9.81/1000;
                        //Out_Z_mms2= (int32)((Out_Z_ms2*1000));
                        OutArray[5] = (uint8_t)(Out_Z_mg & 0xFF);
                        OutArray[6] = (uint8_t)(Out_Z_mg>> 8);
                   
                
                        mean_Z_mg = mean_Z_mg+Out_Z_mg;
                
                
                    
                    
                        if (flag_eeprom==1){
                            
                            // Stop ISRs in order to don't generate interrupt during writing operations.
                            isr_TIMER_Stop(); 
                            isr_LIS3DH_Stop();
                        
                            
                            int i;
                            // Write data on data_EEPROM_THR array using counter to keep into account for different loop
                            for (i=0;i<6;i=i+2){
                        
                                data_EEPROM_THR[i+counter*6]=OutArray[i+1];
                                data_EEPROM_THR[i+counter*6+1]=OutArray[i+1+1];
                                data_write = (data_EEPROM_THR[i+counter*6] | (data_EEPROM_THR[i+counter*6+1]<<8));
                            
                                sprintf(message, "Data write: %d \r\n",data_write);
                                UART_Debug_PutString(message);
                    
                                }
                            // When all values are stored, save the timestamp variable on the last byte of the array 
                            if (counter==6){
                                data_EEPROM_THR[42]=(uint8_t)(timestamp & 0xFF);
                                data_EEPROM_THR[43]=(uint8_t)(timestamp >> 8);
                                data_EEPROM_THR[44]=(uint8_t)(timestamp >> 16);
                                data_EEPROM_THR[45]=(uint8_t)(timestamp >> 24);
                            
                                sprintf(message, "Timestamp: %d \r\n",timestamp);
                                UART_Debug_PutString(message);
                                
                                // Write the array on EEPROM_REG_THR
                                EEPROM_writePage( EEPROM_REG_THR,data_EEPROM_THR,46); 
                                EEPROM_waitForWriteComplete();
                            
                                sprintf(message, "End Write on EEPROM\r\n");
                                UART_Debug_PutString(message);
                                flag_eeprom=0;
                                // Check the array written on EEPROM_REG_THR
                                EEPROM_readPage( EEPROM_REG_THR,read_data_EEPROM_THR,46);
                                sprintf(message,"End Read on EEPROM\r\n");
                                UART_Debug_PutString(message);
                            
                                for (i=0;i<42;i=i+2){
                                    data_read = (read_data_EEPROM_THR[i] | (read_data_EEPROM_THR[i+1]<<8));
                                    sprintf(message, "Data read: %d\r\n",data_read);
                                    UART_Debug_PutString(message);
                                    }
                                timestamp_read = (read_data_EEPROM_THR[42] | (read_data_EEPROM_THR[43]<<8) | (read_data_EEPROM_THR[44]<<16)|(read_data_EEPROM_THR[45]<<24));
                                sprintf(message, "Data read: %ld \r\n",timestamp_read);
                                UART_Debug_PutString(message);
                            
                                }
                            
                            // Restart Interrupts 
                            isr_TIMER_StartEx(ISR_TIMER);
                            isr_LIS3DH_StartEx(ISR_LIS3DH_FIFO_WATERMARK);
                            error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT1_SRC,
                                         &int1_src_reg);
                            }
                   
                
                
                   //UART_Debug_PutArray(OutArray, 14);
                
                
                            counter++;
                
                
                            // When all the WMK_THRESHOLD samples has been read change the period of 
                            // the blinking frequency for RGB led
                            if (counter>=WMK_THRESHOLD) {
                                
                                
                                // Compute the mean for each channel
                                mean_X_mg = mean_X_mg/counter;
                                mean_Y_mg = mean_Y_mg/counter;
                                mean_Z_mg = mean_Z_mg/counter;
                                // Compute the absolute value
                                mean_X_mg = abs(mean_X_mg);
                                mean_Y_mg = abs(mean_Y_mg);
                                mean_Z_mg = abs(mean_Z_mg);
                    
                    
                                // Change blinking period by using a quadratic function
                                // if mean_X_mg == 0      period green = 402 (OFF)
                                // if mean_X_mg == 2000   period green = 2 (Max frequency possible)
                                period_green = 0.0001*mean_X_mg*mean_X_mg-0.4*mean_X_mg+402;
                                period_blue = 0.0001*mean_Y_mg*mean_Y_mg-0.4*mean_Y_mg+402;
                                period_red = 0.0001*mean_Z_mg*mean_Z_mg-0.4*mean_Z_mg+402;
                                
                                
                                // Check if all samples as been read
                                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                        LIS3DH_FIFO_SRC_REG_3,
                                                        &fss);
                                fss= fss & (0b00011111);
               
                                // Restart mean variables
                                mean_X_mg=0;
                                mean_Y_mg=0;
                                mean_Z_mg=0;
                                
                                // Restart counter
                                counter = 0;
                              
                                // If not all values are read, remain in while loop
                                // Otherwise exit
                                if(fss > 1) OVR_FLAG = 1;
                                else OVR_FLAG =0 ;
                            }
                        }
           
                    }   //end while (OVR_FLAG)
            break;
            
                
                /************      CONFIGURATION  MODE      *****************/  
            case 2:
                
                if (repetition_menu==0){
                   
                    sprintf(message, "CASE 2 Repetition flag: %d \r\n",repetition_menu);
                    UART_Debug_PutString(message);
                    
                    // Init pheripheral that are needed in this mode
                    INIT_CONF();
                    
                    repetition_stop=0;
                    repetition_start=0;
                    repetition_menu=1;
                    sprintf(message, "Repetition flag after settings: %d \r\n",repetition_menu);
                    UART_Debug_PutString(message);
                }
                // Toggle INT led
                INT_Led_Write(~INT_Led_Read());
                
                // Reset timestamp
                timestamp=0;
                //ADC_DelSig_Start();
                //ADC_DelSig_StartConvert();
                //I2C_Peripheral_Start();
                
                // Read value of the potentiometer using ADC component
                pot_digit = ADC_DelSig_Read32();
                if (pot_digit < 0) pot_digit = 0;
                if (pot_digit > 65535) pot_digit = 65535;
                pot_mv = ADC_DelSig_CountsTo_mVolts(pot_digit);

                sprintf(message, "Potentiometer: %ld \r\n",pot_mv);
                UART_Debug_PutString(message);
                   
                // Evaluate data rate by dividing the potentiometer value by 700
                data_rate = pot_mv/700;
                
                data_rate = data_rate+1;                  // Required in order to avoid data_rate=0 (Switch off LIS3DH)
                if ((uint8_t)data_rate>=7) data_rate=6;   // Avoid getting value too high than available mode
                period_blue= (8-data_rate)*100;           // Set blinking period of BLUE channel as the inverse of data_rate (Higher data rate, higher blinking frequency)
                
                // Write data_rate on CONTROL Register 1
                ctrl_reg1=((uint8_t)data_rate<<4|0b00000111);
                sprintf(message, "Ctrl Reg1: 0x%02X \r\n",ctrl_reg1);
                UART_Debug_PutString(message);
                error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            ctrl_reg1);
        
                if (error == NO_ERROR)
                    {
                        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
                        UART_Debug_PutString(message); 
                    }
                else
                    {
                        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
                    }
                
                CyDelay(1000);
                
                    
            break;
     
     }//end switch
        
        
    

  }//end infinite loop
}//end main