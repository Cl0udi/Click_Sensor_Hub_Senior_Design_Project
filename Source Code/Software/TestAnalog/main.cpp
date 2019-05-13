#include "mbed.h"
#include "stdint.h"
#include "stdlib.h"

Serial pc(USBTX, USBRX);

// Pin Assignment for all channels
AnalogIn socket0_Analog(PTB3);
AnalogIn socket1_Analog(PTB2);
AnalogIn socket2_Analog(PTB1);
AnalogIn socket3_Analog(PTB0);

// Current Socket Global
short int current_socket = 0;
float lagTimeBetweenClicks = 2;

void click_Alcohol_MeasureAlcoholLevel(){
    float resistanceANtoGND = 7800; //measured 3/13/2019 from AN to GND in Ohms
    float resistanceSensor = 0;
    float analogValue = 0;
    
    switch(current_socket){
        case 0: analogValue = socket0_Analog;
        case 1: analogValue = socket1_Analog;
        case 2: analogValue = socket2_Analog;
        case 3: analogValue = socket3_Analog; 
    }
    
    resistanceSensor = (resistanceANtoGND/analogValue)*5 - resistanceANtoGND;
    wait(lagTimeBetweenClicks);
    pc.printf("%f", analogValue);
    pc.printf("/n");
}

void click_AirQuality_MeasureAirQualityLevel(){
    float resistanceANtoGND = 9960; //measured 3/4/2019 from AN to GND in Ohms
    float resistanceSensor = 0;
    float analogValue = 0;
    
    switch(current_socket){
        case 0: analogValue = socket0_Analog; break;
        case 1: analogValue = socket1_Analog; break;
        case 2: analogValue = socket2_Analog; break;
        case 3: analogValue = socket3_Analog; break;
    }
    
    resistanceSensor = (resistanceANtoGND/analogValue)*5 - resistanceANtoGND;
    wait(lagTimeBetweenClicks);
    pc.printf("Analog value: %f", analogValue);
    pc.printf("Resistance Sensor value: %f", resistanceSensor);
    pc.printf("/n");
}
 
int main() {
    
    pc.baud(115200);
    pc.printf(" Program succesfully booted.\n");
    
    while(true){
        pc.printf("\n Taking Alcohol Level reading...\n");
        click_Alcohol_MeasureAlcoholLevel();
        pc.printf("\n Alcohol Level reading taken.\n");
        wait(lagTimeBetweenClicks);
        
        pc.printf("\n Taking Alcohol Level reading...\n");
        click_AirQuality_MeasureAirQualityLevel();
        pc.printf("\n Alcohol Level reading taken.\n");
        wait(lagTimeBetweenClicks);
    }
    
    return 0;
}

// It WORKS! This makes the BarGraph2 Click display random values.