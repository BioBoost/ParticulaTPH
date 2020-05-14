#pragma once
#include "mbed.h"

namespace Particula{
    class BME280{
        public:
            BME280(I2C * i2c_bus);
            BME280(I2C * i2c_bus, char i2c_address);
            double presure(void);                  //Read the current Presure (in HPa) as double
            double presure(bool * success);
            double temperature(void);              //Read the current temperature (in Celcuis) as double
            double temperature(bool * success);
            double humidity(void);                 //Read the curretn relative humidity (in %) as double
            double humidity(bool * success);
            bool sleep(void);                    //Change the mode of the BME280 To Sleep
            bool awake(void);                    //Change the mode of the BME280 To Normal
            bool present(void);                  //Check if the sensor is present
        private:
            //A pointer to the I2C object for cumunicating with the sensor
            I2C * i2c_bus;
            //For changing the mode of the sensor
            void set_mode(unsigned int mode);
            //For writing the settings to the sensor
            void load_settings(void);
            //For reading the calibration data from the sensor to variables
            void read_calibration(void);
            /*------------------For converting raw data to a calculatble value---------------*/
            int adc(char reg_address, int length);
            /*======================As charpter 8 in the datasheet===========================*/
            double compensate_temperature(int adc_T);
            double compensate_presure(int adc_P);         
            double compensate_humidity(int adc_H);
            /*----------------------values for calculating the output------------------------*/
            //Temperature
            double dig_T1;
            double dig_T2;
            double dig_T3;
            //Presure
            double dig_P1;
            double dig_P2;
            double dig_P3;
            double dig_P4;
            double dig_P5;
            double dig_P6;
            double dig_P7;
            double dig_P8;
            double dig_P9;
            //Humidity
            double dig_H1;
            double dig_H2;
            double dig_H3;
            double dig_H4;
            double dig_H5;
            double dig_H6;
            /*---Signed 32 bit integer for comensation calculations in Humidity and presure---*/
            double t_fine;
            /*------------For logging if the previous measurement is correct-----------------*/
            double pres_prev_log;
            double temp_prev_log;
            double humi_prev_log;
            /*---------------------------------Data Points------------------------------------*/
            //Normal mode, oversapming x1 for temp and presure
            const char ctrl_meas_normal = 0x27;
            //Sleep mode, oversapming x1 for temp and presure
            const char ctrl_meas_sleep = 0x24;
            //Forced mode 1, oversapming x1 for temp and presure
            const char ctrl_meas_forced_1 = 0x25;
            //Forced mode 2, oversapming x1 for temp and presure
            const char ctrl_meas_forced_2 = 0x26;
            //Oversampling x1 hum
            const char ctrl_hum_data = 0x01;
            //Disable filter, standby time 0.5ms and spi is off
            const char config_data = 0x00;
            /*---------------------------Adress table from datasheet--------------------------*/
            char i2c_address;
            const char hum_lsb =       0xFE;
            const char hum_msb =       0xFD;
            const char temp_xlsb =     0xFC;
            const char temp_lsb =      0xFB;
            const char temp_msb =      0xFA;
            const char pres_xlsb =     0xF9;
            const char pres_lsb =      0xF8;
            const char pres_msb =      0xF7;
            const char config =        0xF5;
            const char ctrl_meas =     0xF4;
            const char status =        0xF3;
            const char ctrl_hum =      0xF2;
            const char reset =         0xE0;
            const char id =            0xD0;
            const char calib00 =       0x88;
            const char calib25 =       0xA1;
            const char calib26 =       0xE1;
            const char calib41 =       0xF0;
    };
}