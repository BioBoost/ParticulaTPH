#include "mbed.h"
#include "BME280.h"

I2C i2c(D0, D1);
char addr = 0x76 << 1;
char out[18];
char in[18];
BME280 testI2C = BME280(i2c, addr);
Serial pc(USBTX, USBRX);

int main(){
    
    out[0] = (char)0x00;
    in[0] = (char)0xf8;

    //So compiler stops complaining
    pc.printf("test");
    while(1){
//        i2c.write((char) 0xCE, *in , 1);
        i2c.read((char) 0xED, out, 3);
        pc.printf("  '%x'  ", out);
        wait(1);
    }
}