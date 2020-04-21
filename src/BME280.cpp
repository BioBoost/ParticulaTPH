#include "mbed.h"
#include "BME280.h"
#include <ctime>

<<<<<<< HEAD
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

    bool BME280::sleep(void){
        setMode(0);
    }

    bool BME280::awake(void){
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
            setMode = 0x02        
        default:                    //Sleep Mode
            setMode = 0x00;
            break;
        }
        char data[] = {ctrl_meas, setMode};
        i2cBus.write(i2cAddress, data, 2);
    }

    void BME280::loadSettings(void){
        char data[] = {config, 0x00};               //Disable filter, standby time 0.5ms and spi is off
        i2cBus.write(i2cAddress, data, 2);
        i2cBus.write(i2cAddress, ctrl_meas, 1);
        i2cBus.read(i2cAddress, data, 1);
        data[1] = 0x24 && data[0];                  //coppy mode, oversapming x1 for temp and presure
        data[0] = ctrl_meas;
        i2cBus.write(i2cAddress, data, 2);
        data[] = {ctrl_hum, 0x01};                  //oversampling x1 hum
        i2cBus.write(i2cAddress, data, 2);
    }

    void BME280::readCalibration(void){
        char data[25];
        i2c.write(i2cAddress, calib00, 1);
        i2c.read(i2cAddress, data, 25);

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

        i2c.write(i2cAddress, calib26, 1);
        i2c.read(i2cAddress, data, 15);

        dig_H2 = (data[1] << 8) | data[0];
        dig_H3 = data[2];
        dig_H4 = (data[4] << 8) | data[3];
        dig_H5 = (data[6] << 8) | data[5];
        dig_H6 = data[7];
    }

    int BME280::adc_temperature(void){
        char data[3];
        i2c.write(i2cAddress, temp_msb, 1);
        i2c.read(i2cAddress, data, 3);
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
       char data[3];
        i2c.write(i2cAddress, pres_msb, 1);
        i2c.read(i2cAddress, data, 3);
        return (int32_t)((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)); 
    }

    double BME280::compensate_presure(int adc_p){
        double var1, var2, p;
        var1 = ((double)t_fine/2.0) – 64000.0;
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
        char data[2];
        i2c.write(i2cAddress, hum_msb, 1);
        i2c.read(i2cAddress, data, 2);
        return(int32_t)(cmd[1] << 8) | cmd[2];
    }

    double BME280::compensate_humidity(int humidity_adc){
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


/*

namespace BME280_TPH{

    BME280::BME280(PinName sda, PinName scl, char slave_adr)
        :i2c_p(new I2C(sda, scl)), i2c(*i2c_p), address(slave_adr), t_fine(0){
        initialize();
    }

    BME280::BME280(I2C &i2c_obj, char slave_adr)
        : i2c_p(NULL), i2c(i2c_obj), address(slave_adr), t_fine(0){
        initialize();
    }

    BME280::~BME280(){
        if (NULL != i2c_p)
            delete  i2c_p;
    }
        
    void BME280::initialize()
    {
        char cmd[18];
    
        cmd[0] = 0xf2; // ctrl_hum
        cmd[1] = 0x01; // Humidity oversampling x1
        i2c.write(address, cmd, 2);
    
        cmd[0] = 0xf4; // ctrl_meas
        cmd[1] = 0x27; // Temparature oversampling x1, Pressure oversampling x1, Normal mode
        i2c.write(address, cmd, 2);
    
        cmd[0] = 0xf5; // config
        cmd[1] = 0xa0; // Standby 1000ms, Filter off
        i2c.write(address, cmd, 2);
    
        cmd[0] = 0x88; // read dig_T regs
        i2c.write(address, cmd, 1);
        i2c.read(address, cmd, 6);
    
        dig_T1 = (cmd[1] << 8) | cmd[0];
        dig_T2 = (cmd[3] << 8) | cmd[2];
        dig_T3 = (cmd[5] << 8) | cmd[4];
    
        //DEBUG_PRINT("dig_T = 0x%x, 0x%x, 0x%x\n", dig_T1, dig_T2, dig_T3);
    
        cmd[0] = 0x8E; // read dig_P regs
        i2c.write(address, cmd, 1);
        i2c.read(address, cmd, 18);
    
        dig_P1 = (cmd[ 1] << 8) | cmd[ 0];
        dig_P2 = (cmd[ 3] << 8) | cmd[ 2];
        dig_P3 = (cmd[ 5] << 8) | cmd[ 4];
        dig_P4 = (cmd[ 7] << 8) | cmd[ 6];
        dig_P5 = (cmd[ 9] << 8) | cmd[ 8];
        dig_P6 = (cmd[11] << 8) | cmd[10];
        dig_P7 = (cmd[13] << 8) | cmd[12];
        dig_P8 = (cmd[15] << 8) | cmd[14];
        dig_P9 = (cmd[17] << 8) | cmd[16];
    
        //DEBUG_PRINT("dig_P = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);
    
        cmd[0] = 0xA1; // read dig_H regs
        i2c.write(address, cmd, 1);
        i2c.read(address, cmd, 1);
        cmd[1] = 0xE1; // read dig_H regs
        i2c.write(address, &cmd[1], 1);
        i2c.read(address, &cmd[1], 7);
=======
BME280::BME280(PinName sda, PinName scl, char slave_adr)
    :
    i2c_p(new I2C(sda, scl)), 
    i2c(*i2c_p),
    address(slave_adr),
    t_fine(0)
{
    initialize();
}

BME280::BME280(I2C &i2c_obj, char slave_adr)
    :
    i2c_p(NULL), 
    i2c(i2c_obj),
    address(slave_adr),
    t_fine(0)
{
    initialize();
}
>>>>>>> 7ec08a817b7fc08274b9f7bfd9d2ced3f2f672b1

BME280::~BME280()
{
    if (NULL != i2c_p)
        delete  i2c_p;
}
    
void BME280::initialize()
{
    char cmd[18];
 
    cmd[0] = 0xf2; // ctrl_hum
    cmd[1] = 0x01; // Humidity oversampling x1
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0xf4; // ctrl_meas
    cmd[1] = 0x27; // Temparature oversampling x1, Pressure oversampling x1, Normal mode
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0xf5; // config
    cmd[1] = 0xa0; // Standby 1000ms, Filter off
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0x88; // read dig_T regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 6);
 
    dig_T1 = (cmd[1] << 8) | cmd[0];
    dig_T2 = (cmd[3] << 8) | cmd[2];
    dig_T3 = (cmd[5] << 8) | cmd[4];
 
    DEBUG_PRINT("dig_T = 0x%x, 0x%x, 0x%x\n", dig_T1, dig_T2, dig_T3);
 
    cmd[0] = 0x8E; // read dig_P regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 18);
 
    dig_P1 = (cmd[ 1] << 8) | cmd[ 0];
    dig_P2 = (cmd[ 3] << 8) | cmd[ 2];
    dig_P3 = (cmd[ 5] << 8) | cmd[ 4];
    dig_P4 = (cmd[ 7] << 8) | cmd[ 6];
    dig_P5 = (cmd[ 9] << 8) | cmd[ 8];
    dig_P6 = (cmd[11] << 8) | cmd[10];
    dig_P7 = (cmd[13] << 8) | cmd[12];
    dig_P8 = (cmd[15] << 8) | cmd[14];
    dig_P9 = (cmd[17] << 8) | cmd[16];
 
    DEBUG_PRINT("dig_P = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);
 
    cmd[0] = 0xA1; // read dig_H regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 1);
     cmd[1] = 0xE1; // read dig_H regs
    i2c.write(address, &cmd[1], 1);
    i2c.read(address, &cmd[1], 7);

<<<<<<< HEAD
    double BME280::temperature(){                           //returns Termperature as a double in °C    
        return (double)(getTemperature());
    }
    
    float BME280::getPressure()
    {
        uint32_t press_raw;
        float pressf;
        char cmd[4];
    
        cmd[0] = 0xf7; // press_msb
        i2c.write(address, cmd, 1);
        i2c.read(address, &cmd[1], 3);
    
        press_raw = (cmd[1] << 12) | (cmd[2] << 4) | (cmd[3] >> 4);
    
        int32_t var1, var2;
        uint32_t press;
    
        var1 = (t_fine >> 1) - 64000;
        var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
        var2 = var2 + ((var1 * dig_P5) << 1);
        var2 = (var2 >> 2) + (dig_P4 << 16);
        var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
        var1 = ((32768 + var1) * dig_P1) >> 15;
        if (var1 == 0) {
            return 0;
        }
        press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
        if(press < 0x80000000) {
            press = (press << 1) / var1;
        } else {
            press = (press / var1) * 2;
        }
        var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
        var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
        press = (press + ((var1 + var2 + dig_P7) >> 4));
    
        pressf = (float)press;
        return (pressf/100.0f);
    }

    double BME280::presure(){                               //returns Presure as a double in hPa
        return (double)(getPressure());
    }
    
    float BME280::getHumidity()
    {
        uint32_t hum_raw;
        float humf;
        char cmd[4];
    
        cmd[0] = 0xfd; // hum_msb
        i2c.write(address, cmd, 1);
        i2c.read(address, &cmd[1], 2);
    
        hum_raw = (cmd[1] << 8) | cmd[2];
    
        int32_t v_x1;
    
        v_x1 = t_fine - 76800;
        v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
                ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                                (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                                (int32_t)dig_H2 + 8192) >> 14));
        v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
        v_x1 = (v_x1 < 0 ? 0 : v_x1);
        v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
    
        humf = (float)(v_x1 >> 12);
    
        return (humf/1024.0f);
    }

    double BME280::humidity(){                              //returns Humidity as a doubel in %
        return (double)(this -> getHumidity());
=======
    dig_H1 = cmd[0];
    dig_H2 = (cmd[2] << 8) | cmd[1];
    dig_H3 = cmd[3];
    dig_H4 = (cmd[4] << 4) | (cmd[5] & 0x0f);
    dig_H5 = (cmd[6] << 4) | ((cmd[5]>>4) & 0x0f);
    dig_H6 = cmd[7];
 
    DEBUG_PRINT("dig_H = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6);
}
 
float BME280::getTemperature()
{
    uint32_t temp_raw;
    float tempf;
    char cmd[4];
 
    cmd[0] = 0xfa; // temp_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 3);
 
    temp_raw = (cmd[1] << 12) | (cmd[2] << 4) | (cmd[3] >> 4);
 
    int32_t temp;
 
    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14);
 
    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;
  
    return (tempf/100.0f);
}

double BME280::FakeTemperature(){                           //returns Termperature as a double in °C    
      
    srand((unsigned)time(NULL));
    double tempf = rand() % 25 + 1;
   
    return tempf;
}
 
float BME280::getPressure()
{
    uint32_t press_raw;
    float pressf;
    char cmd[4];
 
    cmd[0] = 0xf7; // press_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 3);
 
    press_raw = (cmd[1] << 12) | (cmd[2] << 4) | (cmd[3] >> 4);
 
    int32_t var1, var2;
    uint32_t press;
 
    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 1);
    var2 = (var2 >> 2) + (dig_P4 << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * dig_P1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
>>>>>>> 7ec08a817b7fc08274b9f7bfd9d2ced3f2f672b1
    }
    var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
    press = (press + ((var1 + var2 + dig_P7) >> 4));
 
    pressf = (float)press;


    return (pressf/100.0f);
}

double BME280::FakePressure(){                               //returns Presure as a double in hPa
    
    srand((unsigned)time(NULL));
    double pressure = rand() % 120 + 1;
    return pressure;
}
 
float BME280::getHumidity()
{
    uint32_t hum_raw;
    float humf;
    char cmd[4];
 
    cmd[0] = 0xfd; // hum_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 2);
 
    hum_raw = (cmd[1] << 8) | cmd[2];
 
    int32_t v_x1;
 
    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
 
    humf = (float)(v_x1 >> 12);
 
    return (humf/1024.0f);
}

<<<<<<< HEAD
*/
=======
double BME280::FakeHumidity(){                              //returns Humidity as a doubel in %
    srand((unsigned)time(NULL));
    double humidity = rand() % 100 + 1; 
    return humidity;
}
>>>>>>> 7ec08a817b7fc08274b9f7bfd9d2ced3f2f672b1
