#pragma once
#include "mbed.h"

namespace BME280_TPH{
    class BME280{
    public:

        BME280(PinName sda, PinName sck, char slave_adr);       //Creating a instanstance of the BME280 object without passing a I2C object
        BME280(I2C &i2c_obj, char slave_adr);                   //Creating a instanstance of the BME280 object with passing a I2C object
        virtual ~BME280();                                      //Destructor of a instance of the BME280 object
        void initialize(void);                                  
        float getTemperature(void);                             //Read the current temperature (in celcuis) as float
        float getPressure(void);                                //Read the current Presure (in HPa) as float
        float getHumidity(void);                                //Read the curretn relative humidity (in %) ass float
    //edit
        double presure(void);                                   //Read the current Presure (in HPa) as double
        double temperature(void);                               //Read the current temperature (in celcuis) as float
        double humidity(void);                                  //Read the curretn relative humidity (in %) ass float
        bool sleep(void);                                       //Change the mode of the BME280 To Sleep
        bool awake(void);                                       //Change the mode of the BME280 To Normal

    private:

        I2C         *i2c_p;
        I2C         &i2c;
        char        address;
        uint16_t    dig_T1;
        int16_t     dig_T2, dig_T3;
        uint16_t    dig_P1;
        int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
        uint16_t    dig_H1, dig_H3;
        int16_t     dig_H2, dig_H4, dig_H5, dig_H6;
        int32_t     t_fine;

    };
}
