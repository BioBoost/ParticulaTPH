#pragma once
#include "mbed.h"

namespace Patricula{
    class BME280{
        private:
            /*
            We need the shift the address 1 bit, the datasheet address is only 7 bits
            The address is either 0x77 or 0x76, this depends on the fysical configuration of you chip
            */
            const char i2c_address =   0x77 << 1;
            /*---------------------------------Data Points------------------------------------*/
            //Normal mode, oversapming x1 for temp and presure
            const char ctrl_meas_normal = 0x27;
            //Sleep mode, oversapming x1 for temp and presure
            const char ctrl_meas_sleep = 0x24;
            //Forced mode 1, oversapming x1 for temp and presure
            const char ctrl_meas_forced_1 = 0x25;
            //Forced mode 2, oversapming x1 for temp and presure
            const char ctrl_meas_forced_2 = 0x26;
            //Oversampling x1 hum
            const char ctrl_hum_data = 0x01;
            //Disable filter, standby time 0.5ms and spi is off
            const char config_data = 0x00;
            
    }
}