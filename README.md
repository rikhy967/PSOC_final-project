# *Final Project - PSOC Creator*

## Course :

Electronic Technologies and Biosensor Laboratories

## Students : 
* Riccardo Levi 
* Simone Toffoli

## Materials:
* [PSoC 5LP CY8CKIT-059](https://www.cypress.com/file/45906/download) - Microcontroller
* [LIS3DH](https://www.st.com/resource/en/datasheet/cd00274221.pdf) - 3-axis accelerometer
* [25LC256](http://ww1.microchip.com/downloads/en/DeviceDoc/20005715A.pdf) - 256K EEPROM
* RGB Led
* Potentiometer
* [PSoC Creator v. 4.3](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide) - Software for programming the microcontroller

## Description:
The device is able to change the blinking frequency of the 3 channels of the RGB Led according to the sensed values acquired by the 3-axis accelerometer.
In addition, it is possible to switch between 3 modality:
1. **OFF MODE**: the device is turned off;
2. **ON MODE**: the device is turned on and stream data from the accelerometer. RGB Led blinking frequency changes accordingly.
3. **CONFIGURATION MODE**: through the potentiometer, it is possible to change the data rate of the accelerometer
