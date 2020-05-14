private:
    /*---------------------------Adress table from datasheet--------------------------*/
    const char hum_lsb =       0xFE;
    const char hum_msb =       0xFD;
    const char temp_xlsb =     0xFC;
    const char temp_lsb =      0xFB;
    const char temp_msb =      0xFA;
    const char pres_xlsb =     0xF9;
    const char pres_lsb =      0xF8;
    const char pres_msb =      0xF7;
    const char config =        0xF5;
    const char ctrl_meas =     0xF4;
    const char status =        0xF3;
    const char ctrl_hum =      0xF2;
    const char reset =         0xE0;
    const char id =            0xD0;
    const char calib00 =       0x88;
    const char calib25 =       0xA1;
    const char calib26 =       0xE1;
    const char calib41 =       0xF0;