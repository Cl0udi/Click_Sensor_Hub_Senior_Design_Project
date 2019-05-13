#include "Click_BarGraph2.h"

Click_BarGraph2(SPI spi, DigitalOut cs, PwmOut){
    SPI_Selections = spi;
    ChipSelectForTesting = cs;
    PWM_Selections pwm;
}

void Click_BarGraph2::setColors(int greenLEDs, int redLEDs){
    
    long control_bargraph = (greenLEDs << 10) + redLEDs;
    uint8_t byte0 = control_bargraph & 0xFF;
    uint8_t byte1 = (control_bargraph >> 8) & 0xFF;
    uint8_t byte2 = (control_bargraph >> 16) & 0xF;
    
    pwm.write(1);
    
    spi.write(byte0);
    spi.write(byte1);
    spi.write(byte2);
    cs = 1;
    wait(.5);
    cs = 0;
}