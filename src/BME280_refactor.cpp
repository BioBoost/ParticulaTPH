#include "mbed.h"
#include "BME280_refactor.h"

namespace Particula{
    BME280::BME280(I2C * i2c_bus):
    BME280 (i2c_bus, 0x76){;}
    
    BME280::BME280(I2C * i2c_bus, char i2c_address){
        this->i2c_bus = i2c_bus;                //Passing a refference to the i2c-object
        this->i2c_address = i2c_address << 1;   //We need to shift the adress 1 bit to make it an 8 bit address
        load_settings();                        //Loading all the settings
        read_calibration();                     //Reading the calibration data form the sensor to local variables
        set_mode(0);                            //Set the mode of the BME280 to Sleep
    }

    /*
    Returning the value of the presure that the sensor has measured
    */
    double BME280::presure(void){
        return pres_prev_log = compensate_presure(adc(pres_msb, 3));
    }

    /*
    Returning the value of the presure that the sensor has measured
    Changing a value of a pointer so that we can track if the previous measurement was correct
    */
    double BME280::presure(bool * success){
        double presure_current = compensate_presure(adc(pres_msb, 3));
        success[0] = (presure_current != pres_prev_log) && (compensate_temperature(0) != presure_current);
        return pres_prev_log = presure_current;
    }

    /*
    Returning the value of the temperature that the sensor has measured
    */
    double BME280::temperature(void){
        return temp_prev_log = compensate_temperature(adc(temp_msb,3));
    }

    /*
    Returning the value of the temperature that the sensor has measured
    Changing a value of a pointer so that we can track if the previous measurement was correct
    */
    double BME280::temperature(bool * success){
        double temperature_current = compensate_temperature(adc(temp_msb,3));
        success[0] = (temperature_current != temp_prev_log) && (compensate_temperature(0) != temperature_current);
        return temp_prev_log = temperature_current;
    }

    /*
    Returning the value of the humidity that the sensor has measured
    */
    double BME280::humidity(void){
        return humi_prev_log = compensate_humidity(adc(hum_msb, 2));
    }

    /*
    Returning the value of the humidity that the sensor has measured
    Changing a value of a pointer so that we can track if the previous measurement was correct
    */
    double BME280::humidity(bool * success){
        double humidity_current = compensate_humidity(adc(hum_msb, 2));
        success[0] = (humidity_current != humi_prev_log) && (compensate_humidity(0) != humidity_current);
        return humi_prev_log = humidity_current;
    }

    /*
    Changing the mode that the sensor is in to Sleep
    Returing if the mode was set succesfully
    */
    bool BME280::sleep(void){
        set_mode(0);
        char data[] = {ctrl_meas};
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, 1);
        return data[0] == ctrl_meas_sleep;
    }

    /*
    Changing the mode that the sensor is in to Normal
    Returing if the mode was set succesfully
    */
    bool BME280::awake(void){
        set_mode(1);
        char data[] = {ctrl_meas};
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, 1);
        return data[0] == ctrl_meas_normal;
    }

    /*
    Returning if the sensor is accesable from the microcontroler, in other words is the sensor present?
    */
    bool BME280::present(){
        load_settings();
        char data[4];
        data[0] = ctrl_hum;
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, 4);
        return (data[0] == ctrl_hum_data) && (data[2] == ctrl_meas_sleep) && (data[3] == config_data);
    }

    /*
    Setting 1 out of 4 difrent modes
    The default mode is Sleep
    */
    void BME280::set_mode(unsigned int mode){
        char set_mode;
        switch (mode){
        case 1:     //Normal Mode
            set_mode = ctrl_meas_normal;
            break;
        case 2:     //Froced Mode 1
            set_mode = ctrl_meas_forced_1;
            break;
        case 3:     //Forced Mode 2
            set_mode = ctrl_meas_forced_2;
            break;        
        default:    //Sleep Mode
            set_mode = ctrl_meas_sleep;
            break;
        }
        char data[] = {ctrl_meas, set_mode};
        i2c_bus->write(i2c_address, data, 2);
    }

    /*
    Loading the settings that we definend in the BME280.h
    */
    void BME280::load_settings(void){
        char data[] = {config, config_data};
        i2c_bus->write(i2c_address, data, 2);
        data[1] = ctrl_meas_sleep;
        data[0] = ctrl_meas;
        i2c_bus->write(i2c_address, data, 2);
        data[0] = ctrl_hum;
        data[1]  = ctrl_hum_data;
        i2c_bus->write(i2c_address, data, 2);
    }

    /*
    Reading the calibration data that the manufacturer put in the chip
    */
    void BME280::read_calibration(void){
        char data[25];
        data[0] = calib00;
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, 25);

        dig_T1 = (uint16_t)((data[ 1] << 8) | data[ 0]);
        dig_T2 = (int16_t)((data[ 3] << 8) | data[ 2]);
        dig_T3 = (int16_t)((data[ 5] << 8) | data[ 4]);

        dig_P1 = (uint16_t)((data[ 7] << 8) | data[ 6]);
        dig_P2 = (int16_t)((data[ 9] << 8) | data[ 8]);
        dig_P3 = (int16_t)((data[11] << 8) | data[10]);
        dig_P4 = (int16_t)((data[13] << 8) | data[12]);
        dig_P5 = (int16_t)((data[15] << 8) | data[14]);
        dig_P6 = (int16_t)((data[17] << 8) | data[16]);
        dig_P7 = (int16_t)((data[19] << 8) | data[18]);
        dig_P8 = (int16_t)((data[21] << 8) | data[20]);
        dig_P9 = (int16_t)((data[23] << 8) | data[22]);

        dig_H1 = (uint16_t)data[24];

        data[0] = calib26;
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, 7);

        dig_H2 = (int16_t)((data[1] << 8) | data[0]);
        dig_H3 = (uint16_t)data[2];
        dig_H4 = (int16_t)((data[3] << 4) | (data[3] & 0x0f));
        dig_H5 = (int16_t)((data[5] << 4) | ((data[3] >> 4) & 0x0f));
        dig_H6 = (int16_t)data[6];
    }

    /*
    Source is datasheet for BME280
    Calculating the Tempurature that the sensor has measured and returning it as a double
    */
    double BME280::compensate_temperature(int adc_T){
        t_fine = ((adc_T-pow(2,4)*dig_T1)*(dig_T3*adc_T-pow(2,4)*(adc_T*dig_T3-pow(2,16)*dig_T2)))/pow(2,24);
        return t_fine / (pow(2,10)+pow(2,12));
    }

    /*
    Source is datasheet for BME280
    Calculating the Presure that the sensor has measured and returning it as a double
    */
    double BME280::compensate_presure(int adc_P){
        double var1, var2, p;
        var1 = (t_fine/2.0) - 64000.0;
        var2 = var1 * var1 * (dig_P6) / 32768.0;
        var2 = var2 + var1 * (dig_P5) * 2.0;
        var2 = (var2/4.0)+((dig_P4) * 65536.0);
        var1 = ((dig_P3) * var1 * var1 / 524288.0 + (dig_P2) * var1) / 524288.0;
        var1 = (1.0 + var1 / 32768.0)*(dig_P1);
        if (var1 == 0.0)
        {
        return 0; // avoid exception caused by division by zero
        }
        p = 1048576.0 - (double)adc_P;
        p = (p - (var2 / 4096.0)) * 6250.0 / var1;
        var1 = (dig_P9) * p * p / 2147483648.0;
        var2 = p * (dig_P8) / 32768.0;
        p = p + (var1 + var2 + (dig_P7)) / 16.0;
        p = p / 100;                                                    // measurment was off by 100 #CONTROLE
        return p;
    }

    /*
    Source is datasheet for BME280
    Calculating the Humidity that the sensor has measured and returning it as a double
    */
    double BME280::compensate_humidity(int adc_H){
        double var_H;
        var_H = ((t_fine) - 76800.0);
        var_H = (adc_H - ((dig_H4) * 64.0 + (dig_H5) / 16384.0 * var_H)) *
        ((dig_H2) / 65536.0 * (1.0 + (dig_H6) / 67108864.0 * var_H *
        (1.0 + (dig_H3) / 67108864.0 * var_H)));
        var_H = var_H * (1.0 - (dig_H1) * var_H / 524288.0);
        if (var_H > 100.0)
        var_H = 100.0;
        else if (var_H < 0.0)
        var_H = 0.0;
        return var_H;
    }

    /*
    Translating the data in the registers to a raw calculatble value
    */
    int BME280::adc(char reg_address, int length){
        char data[length];
        data[0] = reg_address;
        i2c_bus->write(i2c_address, data, 1);
        i2c_bus->read(i2c_address, data, length);
        return (int32_t)(((data[0] << 16) | (data[1] << 8) | (data[2]))>>4); 
    }
}

