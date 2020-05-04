 
#include "mbed.h"
#include "BME280.h"

using namespace Particula;

int main(){
    mbed::I2C i2cCom(D14, D15);
    BME280 tph_sensor = BME280(&i2cCom); // D4 en D5 voor kleine nucleo
    mbed::Serial pc(USBTX, USBRX);

    tph_sensor.awake();

    double temperature;             // value in °C
    double humidity;                // value in %
    double pressure;                // value in hPa
 
    // double temperature = (double) tph_sensor.getTemperature();  // value in °C
    // double humidity = (double) tph_sensor.getHumidity();        // value in %
    // double pressure = (double) tph_sensor.getPressure();        // value in hPa

    while(true){
        wait_us(1000000);
        temperature = tph_sensor.temperature();  // value in °C
        humidity = tph_sensor.humidity();        // value in %
        pressure = tph_sensor.presure();        // value in hPa
        pc.printf("[Particula] Measered temperature:  %4.2f °C\r\n", temperature);
        pc.printf("[Particula] Measered humidity:     %4.2f %%\r\n", humidity);
        pc.printf("[Particula] Measered pressure:     %4.2f hPa\r\n", pressure);
        tph_sensor.sleep();
        wait_us(10);
        tph_sensor.awake();
        wait_us(1000000);
    }
}