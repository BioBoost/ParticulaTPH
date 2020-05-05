#pragma once
#include "mbed.h"

namespace Particula{
    class BME280{
        public:
            BME280(I2C * i2cBus);
            double
            presure(void),                  //Read the current Presure (in HPa) as double
            presure(bool * success),
            temperature(void),              //Read the current temperature (in Celcuis) as double
            temperature(bool * success),
            humidity(void),                 //Read the curretn relative humidity (in %) as double
            humidity(bool * success);
            bool
            sleep(void),                    //Change the mode of the BME280 To Sleep
            awake(void),                    //Change the mode of the BME280 To Normal
            present(void);                  //Check if the sensor is present
        private:
            //A pointer to the I2C object for cumunicating with the sensor
            I2C * i2c_bus;
            void 
            //For changing the mode of the sensor
            set_mode(unsigned int mode),
            //For writing the settings to the sensor
            load_settings(void),
            //For reading the calibration data from the sensor to variables
            read_calibration(void);
            /*------------------For converting raw data to a calculatble value---------------*/
            int
            adc_temperature(void),
            adc_presure(void),
            adc_humidity(void);
            /*------------For logging if the previous measurement is correct-----------------*/
            double 
            pres_prev_log,
            temp_prev_log,
            humi_prev_log;
            /*======================As charpter 8 in the datasheet===========================*/
            double
            compensate_temperature(int adc_T),
            compensate_presure(int adc_P),         
            compensate_humidity(int adc_H);
            /*--------------Unsigned 16bit integers for calculating the output---------------*/
            uint16_t
            //Temperature
            dig_T1,
            //Presure
            dig_P1,
            //Humidity
            dig_H1,
            dig_H3;
            /*--------------Signed 16 bit intergers for calculating the output---------------*/
            int16_t
            //Temperature
            dig_T2,
            dig_T3,
            //Presure
            dig_P2,
            dig_P3,
            dig_P4,
            dig_P5,
            dig_P6,
            dig_P7,
            dig_P8,
            dig_P9,
            //Humidity
            dig_H2,
            dig_H4,
            dig_H5,
            dig_H6;
            /*---Signed 32 bit integer for comensation calculations in Humidity and presure---*/
            int32_t     t_fine;
            /*---------------------------------Data Points------------------------------------*/
            const char 
            //Normal mode, oversapming x1 for temp and presure
            ctrl_meas_normal = 0x27,
            //Sleep mode, oversapming x1 for temp and presure
            ctrl_meas_sleep = 0x24,
            //Forced mode 1, oversapming x1 for temp and presure
            ctrl_meas_forced_1 = 0x25,
            //Forced mode 2, oversapming x1 for temp and presure
            ctrl_meas_forced_2 = 0x26,
            //Oversampling x1 hum
            ctrl_hum_data = 0x01,
            //Disable filter, standby time 0.5ms and spi is off
            config_data = 0x00;
            /*---------------------------Adress table from datasheet--------------------------*/
            const char 
            //We need the shift the address 1 bit, the datasheet address is only 7 bits
            i2c_address =   0x77 << 1,
            hum_lsb =       0xFE,
            hum_msb =       0xFD,
            temp_xlsb =     0xFC,
            temp_lsb =      0xFB,
            temp_msb =      0xFA,
            pres_xlsb =     0xF9,
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