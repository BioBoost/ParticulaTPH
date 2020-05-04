#pragma once
#include "mbed.h"

namespace Particula{
    class BME280{
        public:
            BME280(I2C * i2cBus);
            double
            presure(void),                                   //Read the current Presure (in HPa) as double
            presure(bool * success),
            temperature(void),                               //Read the current temperature (in celcuis) as float
            temperature(bool * success),
            humidity(void),                                  //Read the curretn relative humidity (in %) ass float
            humidity(bool * success);
            bool
            sleep(void),                                       //Change the mode of the BME280 To Sleep
            awake(void),                                       //Change the mode of the BME280 To Normal
            present(void);
        private:
            I2C * i2c_bus;
            void 
            set_mode(unsigned int mode),
            load_settings(void),
            read_calibration(void);
            // As charpter 8 in the datasheet
            int
            adc_temperature(void),
            adc_presure(void),
            adc_humidity(void);
            double 
            compensate_temperature(int adc_T),
            compensate_presure(int adc_P),         
            compensate_humidity(int adc_H),
            pres_prev_log,
            temp_prev_log,
            humi_prev_log;
            uint16_t    dig_T1;
            int16_t     dig_T2, dig_T3;
            uint16_t    dig_P1;
            int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
            uint16_t    dig_H1, dig_H3;
            int16_t     dig_H2, dig_H4, dig_H5, dig_H6;
            int32_t     t_fine;
            //Data Points
            const char 
            ctrl_meas_normal = 0x27,
            ctrl_meas_sleep = 0x24,
            ctrl_meas_forced_1 = 0x25,
            ctrl_meas_forced_2 = 0x26,
            ctrl_hum_data = 0x01,
            config_data = 0x00;
            //Adress table (pulled from datasheet)
            const char 
            i2c_address =    0x77 << 1, 
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