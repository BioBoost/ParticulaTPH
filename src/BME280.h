#ifndef MBED_BME280_H
#define MBED_BME280_H

#include "mbed.h"

<<<<<<< HEAD
namespace Particula{
    class BME280{
        public:
            BME280(I2C * i2cBus);
            double presure(void);                                   //Read the current Presure (in HPa) as double
            double temperature(void);                               //Read the current temperature (in celcuis) as float
            double humidity(void);                                  //Read the curretn relative humidity (in %) ass float
            bool sleep(void);                                       //Change the mode of the BME280 To Sleep
            bool awake(void);                                       //Change the mode of the BME280 To Normal
        private:
            I2C * i2cBus;
            void setMode(unsigned int mode);
            void loadSettings(void);
            void readCalibration(void);
            // As charpter 8 in the datasheet
            int adc_temperature(void);
            double compensate_temperature(int adc_T);     //WIP10
            int adc_presure(void);
            double compensate_presure(int adc_P);             //WIP11
            int adc_humidity(void);
            double compensate_humidity(int adc_H);           //WIP12
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


/*
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

        mbed::I2C         *i2c_p;
        mbed::I2C         &i2c;
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
*/
=======
#define DEFAULT_SLAVE_ADDRESS (0x76 << 1)

#ifdef _DEBUG
extern Serial pc;
#define DEBUG_PRINT(...) pc.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/**  Interface for controlling BME280 Combined humidity and pressure sensor
 *
 * @code
 * #include "mbed.h"
 * #include "BME280.h"
 * 
 * Serial pc(USBTX, USBRX);
 * 
 * #if defined(TARGET_LPC1768)
 * BME280 sensor(p28, p27);
 * #else
 * BME280 sensor(I2C_SDA, I2C_SCL);
 * #endif
 * 
 * int main() {
 *     
 *     while(1) {
 *         pc.printf("%2.2f degC, %04.2f hPa, %2.2f %%\n", sensor.getTemperature(), sensor.getPressure(), sensor.getHumidity());
 *         wait(1);
 *     }
 * }
 * 
 * @endcode
 */

/** BME280 class
 *
 *  BME280: A library to correct environmental data using Boshe BME280 environmental sensor device
 *
 */ 
class BME280
{
public:

    /** Create a BME280 instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param sda I2C-bus SDA pin
     * @param scl I2C-bus SCL pin
     * @param slave_adr (option) I2C-bus address (default: 0x76)
     */
    BME280(PinName sda, PinName sck, char slave_adr = DEFAULT_SLAVE_ADDRESS);

    /** Create a BME280 instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param i2c_obj I2C object (instance)
     * @param slave_adr (option) I2C-bus address (default: 0x76)
     */
    BME280(I2C &i2c_obj, char slave_adr = DEFAULT_SLAVE_ADDRESS);

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
    double FakePressure(void);

    double FakeTemperature(void);

    double FakeHumidity(void);

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

#endif // MBED_BME280_H
>>>>>>> 7ec08a817b7fc08274b9f7bfd9d2ced3f2f672b1
