#include "mbed.h"
#include "BME280.h"


int main(){
    mbed::I2C i2cCom(D14, D15);
    BME280_TPH::BME280 tph_sensor = BME280_TPH::BME280(i2cCom, 0x76 << 1); // D4 en D5 voor kleine nucleo
    mbed::Serial pc(USBTX, USBRX);


    double temperature = tph_sensor.temperature();  // value in °C
    double humidity = tph_sensor.humidity();        // value in %
    double pressure = tph_sensor.presure();        // value in hPa

    // double temperature = (double) tph_sensor.getTemperature();  // value in °C
    // double humidity = (double) tph_sensor.getHumidity();        // value in %
    // double pressure = (double) tph_sensor.getPressure();        // value in hPa

    pc.printf("[Particula] Measered temperature:  %4.2f °C\r\n", temperature);
    pc.printf("[Particula] Measered humidity:     %4.2f %%\r\n", humidity);
    pc.printf("[Particula] Measered pressure:     %4.2f hPa\r\n", pressure);
}