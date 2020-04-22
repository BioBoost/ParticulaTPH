#include "mbed.h"
#include "BME280.h"

namespace Particula{
    BME280::BME280(I2C * i2cBus){
        this->i2cBus = i2cBus;
        setMode(0);
        loadSettings();
        readCalibration();
    }

    double BME280::presure(void){
        //return adc_presure();
        return compensate_presure(adc_presure());
    }

    double BME280::temperature(void){
        //return adc_temperature();
        return compensate_temperature(adc_temperature());
    }

    double BME280::humidity(void){
        //return adc_humidity();
        return compensate_humidity(adc_humidity());
    }

    void BME280::sleep(void){
        setMode(0);
    }

    void BME280::awake(void){
        setMode(1);
    }

    void BME280::setMode(unsigned int mode){
        char setMode;
        switch (mode){
        case 1:                     //Normal Mode
            setMode = 0x03;
            break;
        case 2:                     //Froced Mode 1
            setMode = 0x01;
        case 3:                     //Forced Mode 2
            setMode = 0x02;        
        default:                    //Sleep Mode
            setMode = 0x00;
            break;
        }
        char data[] = {ctrl_meas, setMode};
        i2cBus->write(i2cAddress, data, 2);
    }

    void BME280::loadSettings(void){
        char data[] = {config, 0x00};               //Disable filter, standby time 0.5ms and spi is off
        i2cBus->write(i2cAddress, data, 2);
        data[0] = ctrl_meas;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 1);
        data[1] = 0x24 && data[0];                  //coppy mode, oversapming x1 for temp and presure
        data[0] = ctrl_meas;
        i2cBus->write(i2cAddress, data, 2);
        data[0] = ctrl_hum;
        data[1]  = 0x01;                  //oversampling x1 hum
        i2cBus->write(i2cAddress, data, 2);
    }

    void BME280::readCalibration(void){
        char data[25];
        data[0] = calib00;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 25);

        dig_T1 = (data[ 0] << 8) | data[ 1];
        dig_T2 = (data[ 2] << 8) | data[ 3];
        dig_T3 = (data[ 4] << 8) | data[ 5];

        dig_P1 = (data[ 6] << 8) | data[ 7];
        dig_P2 = (data[ 8] << 8) | data[ 9];
        dig_P3 = (data[10] << 8) | data[11];
        dig_P4 = (data[12] << 8) | data[13];
        dig_P5 = (data[14] << 8) | data[15];
        dig_P6 = (data[16] << 8) | data[17];
        dig_P7 = (data[18] << 8) | data[19];
        dig_P8 = (data[20] << 8) | data[21];
        dig_P9 = (data[22] << 8) | data[23];

        dig_H1 = data[24];

        data[0] = calib26;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 15);

        dig_H2 = (data[0] << 8) | data[1];
        dig_H3 = data[2];
        dig_H4 = (data[3] << 8) | data[4];
        dig_H5 = (data[5] << 8) | data[6];
        dig_H6 = data[7];
    }

    int BME280::adc_temperature(void){
        char data[4];
        data[0] = temp_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 3);
        return (int32_t)(((data[2] << 16) | (data[1] << 8) | data[0])<<4);
    }

    double BME280::compensate_temperature(int adc_T){
        int32_t var1, var2, T;
        var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
        var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) *
        ((int32_t)dig_T3)) >> 14;
        t_fine = var1 + var2;
        T = (t_fine * 5 + 128) >> 8;
        return T;
    }

    int BME280::adc_presure(void){
        char data[3];
        data[0] = pres_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 3);
        return (int32_t)(((data[0] << 16) | (data[1] << 8) | (data[2]))<<4); 
    }

    double BME280::compensate_presure(int adc_P){
        int32_t var1, var2;
        uint32_t p;
        var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
        var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
        var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
        var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
        var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) * var1)>>1))>>18;
        var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
        if (var1 == 0)
        {
        return 0; // avoid exception caused by division by zero
        }
        p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
        if (p < 0x80000000)
        {
        p = (p << 1) / ((uint32_t)var1);
        }
        else
        {
        p = (p / (uint32_t)var1) * 2;
        }
        var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
        var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
        p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
        return p;
    }

    int BME280::adc_humidity(void){
        char data[2];
        data[0] = hum_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 2);
        return(int32_t)(data[1] << 8) | data[0];
    }

    double BME280::compensate_humidity(int adc_H){
        int32_t v_x1_u32r;
        v_x1_u32r = (t_fine - ((int32_t)76800));
        v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) +
        ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r *
        ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
        ((int32_t)dig_H2) + 8192) >> 14));
        v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
        v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
        v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
        return (int32_t)(v_x1_u32r>>12);
    }
}

