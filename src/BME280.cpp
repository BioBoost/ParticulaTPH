#include "mbed.h"
#include "BME280.h"

namespace Particula{
    BME280::BME280(I2C * i2cBus){
        this->i2cBus = i2cBus;
        setMode(0);
        readCalibration();
    }

    double BME280::presure(void){
        return compensate_presure(adc_presure());
    }

    double BME280::temperature(void){
        return compensate_temperature(adc_temperature());
    }

    double BME280::humidity(void){
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

        dig_T1 = (data[ 1] << 8) | data[ 0];
        dig_T2 = (data[ 3] << 8) | data[ 2];
        dig_T3 = (data[ 5] << 8) | data[ 4];

        dig_P1 = (data[ 7] << 8) | data[ 6];
        dig_P2 = (data[ 9] << 8) | data[ 8];
        dig_P3 = (data[11] << 8) | data[10];
        dig_P4 = (data[13] << 8) | data[12];
        dig_P5 = (data[15] << 8) | data[14];
        dig_P6 = (data[17] << 8) | data[16];
        dig_P7 = (data[19] << 8) | data[18];
        dig_P8 = (data[21] << 8) | data[20];
        dig_P9 = (data[23] << 8) | data[22];

        dig_H1 = data[24];

        data[0] = calib26;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 15);

        dig_H2 = (data[1] << 8) | data[0];
        dig_H3 = data[2];
        dig_H4 = (data[4] << 8) | data[3];
        dig_H5 = (data[6] << 8) | data[5];
        dig_H6 = data[7];
    }

    int BME280::adc_temperature(void){
        char data[4];
        data[0] = temp_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 3);
        return (int32_t)((data[1] << 12) | (data[2] << 4) | (data[3] >> 4));
    }

    double BME280::compensate_temperature(int adc_T){
        double var1, var2, T;
        var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
        var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) *
        (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
        t_fine = (int32_t)(var1 + var2);
        T = (var1 + var2) / 5120.0;
        return T;
    }

    int BME280::adc_presure(void){
        char data[4];
        data[0] = pres_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 3);
        return (int32_t)((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)); 
    }

    double BME280::compensate_presure(int adc_P){
        double var1, var2, p;
        var1 = ((double)t_fine/2.0) - 64000.0;
        var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
        var2 = var2 + var1 * ((double)dig_P5) * 2.0;
        var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
        var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
        var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
        if (var1 == 0.0)
        {
        return 0; // avoid exception caused by division by zero
        }
        p = 1048576.0 - (double)adc_P;
        p = (p - (var2 / 4096.0)) * 6250.0 / var1;
        var1 = ((double)dig_P9) * p * p / 2147483648.0;
        var2 = p * ((double)dig_P8) / 32768.0;
        p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
        return p;
    }

    int BME280::adc_humidity(void){
        char data[3];
        data[0] = hum_msb;
        i2cBus->write(i2cAddress, data, 1);
        i2cBus->read(i2cAddress, data, 2);
        return(int32_t)(data[1] << 8) | data[2];
    }

    double BME280::compensate_humidity(int adc_H){
        double var_H;
        var_H = (((double)t_fine) - 76800.0);
        var_H = (adc_H - (((double)dig_H4) * 64.0 + ((double)dig_H5) / 16384.0 * var_H)) *
        (((double)dig_H2) / 65536.0 * (1.0 + ((double)dig_H6) / 67108864.0 * var_H *
        (1.0 + ((double)dig_H3) / 67108864.0 * var_H)));
        var_H = var_H * (1.0 - ((double)dig_H1) * var_H / 524288.0);
        if (var_H > 100.0)
        var_H = 100.0;
        else if (var_H < 0.0)
        var_H = 0.0;
        return var_H;
    }
}

