#ifndef Click_BarGraph2_H
#define Click_BarGraph2

class Click_BarGraph2 {
 public:
    Click_BarGraph2(SPI spi, DigitalOut cs, PwmOut pwm);
    
    void setColors(int greenLEDs, int redLEDs);
    
 private:
    SPI SPI_Selections;
    DigitalOut ChipSelectForTesting;
    PwmOut PWM_Selections;
    
 };
 
 #endif // Click_BarGraph2_H