#pragma once
#include "mbed.h"

namespace Particula{
    class BME280{
        public:
            BME280(I2C * i2cBus);
            double presure(void);                                   //Read the current Presure (in HPa) as double
            double temperature(void);                               //Read the current temperature (in celcuis) as float
            double humidity(void);                                  //Read the curretn relative humidity (in %) ass float
            void sleep(void);                                       //Change the mode of the BME280 To Sleep
            void awake(void);                                       //Change the mode of the BME280 To Normal
        private:
            I2C * i2cBus;
            void setMode(unsigned int mode);
            void loadSettings(void);
            void readCalibration(void);
            // As charpter 8 in the datasheet
            int adc_temperature(void);
            double compensate_temperature(int adc_T);     
            int adc_presure(void);
            double compensate_presure(int adc_P);             
            int adc_humidity(void);
            double compensate_humidity(int adc_H);           
            uint16_t    dig_T1;
            int16_t     dig_T2, dig_T3;
            uint16_t    dig_P1;
            int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
            uint16_t    dig_H1, dig_H3;
            int16_t     dig_H2, dig_H4, dig_H5, dig_H6;
            int32_t     t_fine;
            //Adress table
            const char 
            i2cAddress =    0x76 << 1, 
            hum_lsb =       0xFE,
            hum_msb =       0xFD,
            temp_xlsb =     0xF9,
            temp_lsb =      0xF8,
            temp_msb =      0xFA,
            pres_lsb =      0xF8,
            pres_msb =      0xF7, 
            config =        0xF5,
            ctrl_meas =     0xF4,
            status =        0xF3,
            ctrl_hum =      0xF2,
            reset =         0xE0,
            id =            0xD0,
            calib00 =       0x88,
            calib25 =       0xA1,
            calib26 =       0xE1,
            calib41 =       0xF0;
    };
}