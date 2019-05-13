#include "mbed.h"

#include "Interface_Analog.cpp"
#include "Interface_I2C.cpp"
#include "Interface_PWM.cpp"
#include "Interface_SPI.cpp"

#include "Click_BarGraph2.cpp"

enum clicks : short int { 
              Temp_Hum     = 1, 
              USB_UART     = 2, 
              Color5       = 3, 
              BarGraph2    = 4,
              Accel5       = 5,
              Gaussmeter   = 6,
              LightRanger3 = 7,
              Alcohol      = 8,
              AirQuality   = 9,
              MicroSD      = 10
            };

int main() {
    Interface_SPI(short int socket, short int click, short int channel)
    Click_BarGraph2(SPI spi, DigitalOut cs, PwmOut pwm)
    
    Interface_SPI socket_1_SPI(1,BarGraph2,1);
    
    SPI S1_spi(S1_SPI.getPinMOSI(), S1_SPI.getPinMISO(), S1_SPI.getPinSCK());
    DigitalOut S1_cs(PTC10);
    
    Click_BarGraph2  click_BarGraph2(
}