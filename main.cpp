#include "mbed.h"
#include "BME280.h"

Serial pc(USBTX,USBRX); 
char addr = 0x76 << 1;

BME280 TphI2c = BME280(D0, D1, addr);

int main() {
    while(1) {
        pc.printf("\n Humidity = %lf", TphI2c.getHumidity());
        pc.printf("\n Temperature = %lf", TphI2c.getTemperature());
        pc.printf("\n Presure = %lf", TphI2c.getPressure());
        wait(2);
    }

}