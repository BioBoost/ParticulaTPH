# ParticulaTPH
This is a repository where we put code for the BME280 sensor (TPH)

## BME 280 mbed library
Here we convert the raw register data from the sensor (BME280) to usable variables. We do this by using the I2C bus to transport the data from registery 0XF7 to 0XFE. That we then in turn can do fancy stuff with. 

### Proof of concept
The only thing that is currently added is the posiblility to return the values as doubles instead of floats.

#### Example code
```
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
```

#### Pinout
Connect the folowing leads From the BME 280 sensor:  
| FROM | TO | Desription |
| :----: | :----: | :--- |
| VDD | 3V3 | 3V3 powersuply |  
| GND | GND | comon ground |
| SDO | GND | This selects the Address on the I2C buss To 0X76 |
| CSB | VDD | This is to set the comunication to I2C |
| SCK | SCL | SCL is the chosen I2C pins on the chosen microconrtoller |
| SDI | SDA | SDA is the chosen I2C pins on the chosen microconrtoller |

In the folowing image we are using a Nucleo_F303K8 and a ADAFRUIT_BME280 board.
![](img/schematic.png)

#### UML
Here you can find all the data and methodes neded for use in the Particula project.
![](img/UMLBME280.png)

### Using the Library

#### List of ingredients
-> Make sure you can compile the code with an mbed compiler (we used mbe-cli)  
-> Get a compapatible editor  
-> Get a BME280  
-> Get a complaibel Mbed microcontroler  
Connect the sensor and the microcontroller like in the pinout above.

#### using mbed cli to add this library
Go to your mbed project root folder and type the folowing line of code to import the code into your mbed project.  
```
mbed add https://github.com/vives-projectwerk-2-2020/ParticulaTPH.git
```
Now you have imported the library, now you still need to add it to you code by including the folowing line at the begining of your code:  
```
#include "BME280.h"
```

There still is the problem left of defining your pinout so the microcontroller can comunicate with the sesor:  
```
mbed::I2C i2c_com(SDA, SCL);
BME280 tph_sensor = BME280(&i2c_com);
```