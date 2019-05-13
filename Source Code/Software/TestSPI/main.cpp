#include "mbed.h"
 

DigitalOut regularSPI_CS_Bit0(PTC10); //CSA
DigitalOut regularSPI_CS_Bit1(PTC11); //CSB
DigitalOut regularSPI_CS_Bit2(PTC0);
DigitalOut regularSPI_CS_Bit3(PTC4);

PwmOut Socket0_PWM(PTC9);
PwmOut Socket1_PWM(PTC8);
PwmOut Socket2_PWM(PTA5);
PwmOut Socket3_PWM(PTA4);
    
SPI regularSPI(PTA16, PTA17, PTA15); // mosi, miso, sclk

// Current Socket Global
short int current_socket = 0;
float lagTimeBetweenClicks = 2;

void click_BarGraph2_setColors(int redLEDs, int greenLEDs){
        
    long control_bargraph = (redLEDs << 10) + greenLEDs;
    uint8_t byte0 = control_bargraph & 0xFF;
    uint8_t byte1 = (control_bargraph >> 8) & 0xFF;
    uint8_t byte2 = (control_bargraph >> 16) & 0xF;
        
//    switch(current_socket){
//        case 0: Socket0_PWM.write(1);
//        case 1: Socket1_PWM.write(1);
//        case 2: Socket2_PWM.write(1);
//        case 3: Socket3_PWM.write(1); 
//    }
        
    regularSPI.write(byte2);
    regularSPI.write(byte1);
    regularSPI.write(byte0);
    
    switch(current_socket){
        case 0: regularSPI_CS_Bit0 = 1;
        case 1: regularSPI_CS_Bit1 = 1;
        case 2: regularSPI_CS_Bit2 = 1;
        case 3: regularSPI_CS_Bit3 = 1; 
    }
    
    wait(lagTimeBetweenClicks);

    switch(current_socket){
        case 0: regularSPI_CS_Bit0 = 0;
        case 1: regularSPI_CS_Bit1 = 0;
        case 2: regularSPI_CS_Bit2 = 0;
        case 3: regularSPI_CS_Bit3 = 0; 
    }
}

void click_BarGraph2_colorPattern(){
    
    long redLEDs = 0xFFC00; 
    long greenLEDs =  0x3FF;
    
    click_BarGraph2_setColors(redLEDs,greenLEDs);
    
    for(int i = 0; i < 10; i++){
        redLEDs = redLEDs >> 1;
        greenLEDs = greenLEDs >> 1;
        click_BarGraph2_setColors((int)redLEDs,(int)greenLEDs);
    }
    
}
 
int main() {
    
    pc.baud(115200);
    pc.printf(" Program succesfully booted.\n");
    
    pwm.write(1);
    pwm1.write(1);
    pwm2.write(1);
    pwm3.write(1);
    
    
    while(true){
        pc.printf("\n Intializing Bargraph pattern...\n");
        click_BarGraph2_colorPattern();
        pc.printf("\n Bargraph pattern finished.\n");
        wait(lagTimeBetweenClicks);
        
    }
}

// It WORKS! This makes the BarGraph2 Click display random values.