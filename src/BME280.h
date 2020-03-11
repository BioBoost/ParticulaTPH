#pragma once
#include "mbed.h"

namespace BME280_TPH{
    /** BME280 class
     *
     *  BME280: A library to correct environmental data using Boshe BME280 environmental sensor device
     *
     */ 
    class BME280{
    public:

        /** Create a BME280 instance
         *  which is connected to specified I2C pins with specified address
         *
         * @param sda I2C-bus SDA pin
         * @param scl I2C-bus SCL pin
         * @param slave_adr (option) I2C-bus address (default: 0x76)
         */
        BME280(PinName sda, PinName sck, char slave_adr);

        /** Create a BME280 instance
         *  which is connected to specified I2C pins with specified address
         *
         * @param i2c_obj I2C object (instance)
         * @param slave_adr (option) I2C-bus address (default: 0x76)
         */
        BME280(I2C &i2c_obj, char slave_adr);

        /** Destructor of BME280
         */
        virtual ~BME280();

        /** Initializa BME280 sensor
         *
         *  Configure sensor setting and read parameters for calibration
         *
         */
        void initialize(void);

        /** Read the current temperature value (degree Celsius) from BME280 sensor
         *
         */
        float getTemperature(void);

        /** Read the current pressure value (hectopascal)from BME280 sensor
         *
         */
        float getPressure(void);

        /** Read the current humidity value (humidity %) from BME280 sensor
         *
         */
        float getHumidity(void);
    //edit
        double presure(void);

        double temperature(void);

        double humidity(void);

        int sleep();

        int awake();

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
