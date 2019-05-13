#include "mbed.h"
#include "RFD77402.h"
//#include "stdint.h"
//#include "stdlib.h"

    // Socket Numbering:
    // 0 is top left
    // 1 is top right
    // 2 is bottom left
    // 3 is bottom right

    // Set up Terminal
    Serial pc(USBTX, USBRX);

    // Pin Assignment for all channels
    AnalogIn socket0_Analog(PTB3);
    AnalogIn socket1_Analog(PTB1);
    AnalogIn socket2_Analog(PTB2);
    AnalogIn socket3_Analog(PTB0);
            
    I2C shared_I2C(PTE0, PTE1); // sda, sdl
    
    PwmOut Socket0_PWM(PTA5);
    PwmOut Socket2_PWM(PTA4); 
    PwmOut Socket3_PWM(PTC9);
    PwmOut Socket4_PWM(PTC8);
    
    SPI regularSPI(PTA16, PTA17, PTA15); // mosi, miso, sclk
    SPI UART_to_SPI(PTD6, PTD7, PTD5); // mosi, miso, sclk
    
    DigitalOut regularSPI_CS_Bit0(PTC0);
    DigitalOut regularSPI_CS_Bit1(PTC4);
    DigitalOut regularSPI_CS_Bit2(PTC10);
    DigitalOut regularSPI_CS_Bit3(PTC11);
    
    // Current Socket Global
    short int current_socket = 0;
    float lagTimeBetweenClicks = 2;
    
    // Global Click and Socket Selections
    short int socket0_click = 0;
    short int socket0_selection = 0;
    short int socket1_click = 0;
    short int socket1_selection = 0;
    short int socket2_click = 0;
    short int socket2_selection = 0;
    short int socket3_click = 0;
    short int socket3_selection = 0;
    
    // Global variables for clicks
    int green = 0xFF;
    int red = 0x0;
    
    // ENUM of available Clicks
    enum clicks : short int { 
                  Temp_Hum     = 1, USB_UART     = 2, Color5       = 3, 
                  BarGraph2    = 4, Accel5       = 5, Gaussmeter   = 6,
                  LightRanger3 = 7, Alcohol      = 8, AirQuality   = 9,
                  MicroSD      = 10
                };
void click_Color5_getColorReadingsI2C(){
    
    pc.printf("Getting color readings from Color5 Click...\r\n\r\n");
    int addresses[6];
    
    //x axis
    addresses[0] = 0x03;
    addresses[1] = 0x04;
    
    //y axis
    addresses[2] = 0x05;
    addresses[3] = 0x06;
    
    //z axis
    addresses[4] = 0x07;
    addresses[5] = 0x08;
    
    char redAxisBits0to7 = 0;
    char redAxisBits8to11 = 0;
    char greenAxisBits0to7 = 0;
    char greenAxisBits8to11 = 0;
    char blueAxisBits0to7 = 0;
    char blueAxisBits8to11 = 0;
    
    shared_I2C.read(addresses[0], &redAxisBits8to11, 1);
    shared_I2C.read(addresses[1], &redAxisBits0to7, 1);
    shared_I2C.read(addresses[2], &greenAxisBits8to11, 1);
    shared_I2C.read(addresses[3], &greenAxisBits0to7, 1);
    shared_I2C.read(addresses[4], &blueAxisBits8to11, 1);
    shared_I2C.read(addresses[5], &blueAxisBits0to7, 1);
    
    float redAxisReading = (redAxisBits8to11 << 8) + redAxisBits0to7;
    float greenAxisReading = (greenAxisBits8to11 << 8) + greenAxisBits0to7;
    float blueAxisReading = (blueAxisBits8to11 << 8) + blueAxisBits0to7;
    
    pc.printf("Red level: %f\r\n", redAxisReading);
    pc.printf("Green level: %f\r\n", greenAxisReading);
    pc.printf("Blue level: %f\r\n", blueAxisReading);
}
     
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
    regularSPI_CS_Bit0 = 1;
    //regularSPI_CS_Bit1 = 1;
    wait(lagTimeBetweenClicks - 1.5f);
    regularSPI_CS_Bit0 = 0;
    //regularSPI_CS_Bit1 = 0;
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

void click_Accel5_getMovementReadingsI2C(){
    
    pc.printf("Getting acceleration readings from Accel5 Click...\r\n\r\n");
    char addresses[6];
    
    // x axis
    addresses[0] = 0x04;
    addresses[1] = 0x05;
    
    // y axis
    addresses[2] = 0x06;
    addresses[3] = 0x07;
    
    // z axis
    addresses[4] = 0x08;
    addresses[5] = 0x09;
    
    // slave info
    int slave_address = 0x14;
    char slave_cmd = 0x05;
    
    char xAxisBits0to7 = 0;
    char xAxisBits8to11 = 0;
    char yAxisBits0to7 = 0;
    char yAxisBits8to11 = 0;
    char zAxisBits0to7 = 0;
    char zAxisBits8to11 = 0;
    
    shared_I2C.write(slave_address, &slave_cmd, 1);
    shared_I2C.read(addresses[0], &xAxisBits0to7, 1);
    shared_I2C.read(addresses[1], &xAxisBits8to11, 1);
    shared_I2C.read(addresses[2], &yAxisBits0to7, 1);
    shared_I2C.read(addresses[3], &yAxisBits8to11, 1);
    shared_I2C.read(addresses[4], &zAxisBits0to7, 1);
    shared_I2C.read(addresses[5], &zAxisBits8to11, 1);
    
    float xAxisReading = (xAxisBits8to11 << 8) + xAxisBits0to7;
    float yAxisReading = (yAxisBits8to11 << 8) + yAxisBits0to7;
    float zAxisReading = (zAxisBits8to11 << 8) + zAxisBits0to7;
    
    pc.printf("X axis acceleration: %f\r\n", xAxisReading);
    pc.printf("Y axis acceleration: %f\r\n", yAxisReading);
    pc.printf("Z axis acceleration: %f\r\n", zAxisReading);
}

void click_Gaussmeter_getGaussReadingsI2C(){
    
    pc.printf("Getting Gauss readings from Gaussmeter Click...\r\n\r\n");
    int addresses[3];
    
    //x axis
    addresses[0] = 0x04;
    
    //y axis
    addresses[1] = 0x05;
    
    //z axis
    addresses[2] = 0x06;
    
    
    char xAxisBits0to15[2] = {0};
    char yAxisBits0to15[2] = {0};
    char zAxisBits0to15[2] = {0};
    
    shared_I2C.read(addresses[0], xAxisBits0to15, 2);
    shared_I2C.read(addresses[1], yAxisBits0to15, 2);
    shared_I2C.read(addresses[2], zAxisBits0to15, 2);
    
    float xAxisReading = (xAxisBits0to15[0] << 8) + xAxisBits0to15[1];
    float yAxisReading = (yAxisBits0to15[0] << 8) + yAxisBits0to15[1];
    float zAxisReading = (zAxisBits0to15[0] << 8) + zAxisBits0to15[1];
    
    pc.printf("X axis reading: %f\r\n", xAxisReading);
    pc.printf("Y axis reading: %f\r\n", yAxisReading);
    pc.printf("Z axis reading: %f\r\n", zAxisReading);
}

void click_LightRanger_getDistanceReadingsI2C(){
    RFD77402 lightRangerObject;
    lightRangerObject.begin(shared_I2C);
    lightRangerObject.takeMeasurement();
    
    uint16_t distanceReading = lightRangerObject.getDistance();
//    int distance = distanceReading;
//    pc.printf("LighRanger3 Distance: %d \r\n", xMag);
    pc.printf(" LighRanger3 Distance: %f \r\n", (float)distanceReading);
}

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
        case 0: analogValue = socket0_Analog * 10;
        case 1: analogValue = socket1_Analog * 10;
        case 2: analogValue = socket2_Analog * 10;
        case 3: analogValue = socket3_Analog * 10; 
    }
    
    resistanceSensor = (resistanceANtoGND/analogValue)*5 - resistanceANtoGND;
    wait(lagTimeBetweenClicks);
    pc.printf(" Analog value: %f \r\n", (float)analogValue/2);
    //pc.printf(" Resistance Sensor value: %f \r\n", (float)resistanceSensor);
}

void performClickAction(short int click, short int selection){
    switch(click){
        case Temp_Hum: break; // discontinued by MikroElektronika, must have some sort of defect
        case USB_UART: break;
        case Color5: 
            switch(selection){
                case 0: click_Color5_getColorReadingsI2C(); break; // gets red, green, blue readings in that order
            }break;
        case BarGraph2:
            switch(selection){
                case 0: click_BarGraph2_setColors(0x155,0x2AA); break;// red, green
                case 1: click_BarGraph2_colorPattern(); break; // cycles one bar at a time all colors
            }break;
        case Accel5: 
            switch(selection){
                case 0: click_Accel5_getMovementReadingsI2C(); break; // Gets x, y, z axis readings
            }break;
        case Gaussmeter:
            switch(selection){
                case 0: click_Gaussmeter_getGaussReadingsI2C(); break; // Gets x, y, z axis readings
            }break;
        case LightRanger3: switch(selection){
                case 0: click_LightRanger_getDistanceReadingsI2C(); break; // Measures distance from LightRanger
            }break;
        case Alcohol:
            switch(selection){
                case 0: click_Alcohol_MeasureAlcoholLevel(); break; // DO NOT USE unless you test analog value, it is possible to get more than 3.3V reading that will fry board
            }break;
        case AirQuality:
            switch(selection){
                case 0: click_AirQuality_MeasureAirQualityLevel(); break; // analog value based on variable resistor, resistance must be edited each time in function
            }break;
        case MicroSD: break;
    }
}

//short int getNumberInRangeFromUser(short int floor, short int ceil){
//    short int userInputShortInt;
//    pc.scanf("%h", userInputShortInt);
//    while (userInputShortInt < floor || userInputShortInt > ceil){
//            pc.printf("Please enter number between %h and %h.\n", floor, ceil);
//            c.scanf("%h", userInputShortInt);
//    }
//    return userInputShortInt;
//}
//
//void printAvailableClicks(){
//    pc.printf("0 - Temp_Hum\n");
//    pc.printf("1 - USB_UART\n");
//    pc.printf("2 - Color5\n");
//    pc.printf("3 - BarGraph2\n");
//    pc.printf("4 - Accel5\n");
//    pc.printf("5 - Gaussmeter\n");
//    pc.printf("6 - LightRanger3\n");
//    pc.printf("7 - Alcohol\n");
//    pc.printf("8 - AirQuality\n");
//    pc.printf("9 - MicroSD\n");  
//}
//
//void printAvailableClickFunctions(short int click){
//    switch(click){
//        case Temp_Hum: break;
//        case USB_UART: break;
//        case Color5: break;
//        case BarGraph2: break;
//            pc.printf("0 - Set Colors");
//            pc.printf("1 - Display Pattern");
//            break;
//        case Accel5: break;
//        case Gaussmeter: break;
//        case LightRanger3: break;
//        case Alcohol:
//            pc.printf("0 - Measure Alcohol Level");
//            break;
//        case AirQuality:
//            pc.printf("0 - Measure Air Quality Level");
//            break;
//        case MicroSD: break;
//        default: break;
//    } 
//}
//
//short int returnNumberOfAvailableClickFunctions(short int click){
//    switch(click){
//        case Temp_Hum: break;
//        case USB_UART: break;
//        case Color5: break;
//        case BarGraph2: break;
//            pc.printf("0 - Set Colors");
//            pc.printf("1 - Display Pattern");
//            break;
//        case Accel5: break;
//        case Gaussmeter: break;
//        case LightRanger3: break;
//        case Alcohol:
//            pc.printf("0 - Measure Alcohol Level");
//            break;
//        case AirQuality:
//            pc.printf("0 - Measure Air Quality Level");
//            break;
//        case MicroSD: break;
//    } 
//}
//
//bool gui(){
//    pc.printf("Welcome to Click Sensor Hub! :)\n\n");
//    pc.printf("Here is a list of our available clicks:\n\n");
//    printAvailableClicks();
//    
//}
    
int main(){
    pc.baud(115200);
    pc.printf(" Program succesfully booted. \r\n");
    Socket0_PWM.write(1);
//    Socket1_PWM.write(1);
//    Socket2_PWM.write(1);
//    Socket3_PWM.write(1);

    // For GUI and UART, not needed for demo
//    bool clickOptionsSet = gui();
//    while(!clickOptionsSet){
//        clickOptionsSet = gui();
//    }
//    regularSPI_CS_Bit0 = 0;  //PTC10
//    regularSPI_CS_Bit1 = 1;  //PTC11
//    UART_to_SPI_CS_Bit0 = 0; //PTC0
//    UART_to_SPI_CS_Bit1 = 1; //PTC4

    while(true){
        
        // Socket 0
        current_socket = 0;
        pc.printf("\n Displaying BarGraph pattern... \r\n");
        current_socket = 0;
        performClickAction(BarGraph2,1);
        wait(lagTimeBetweenClicks);
        performClickAction(BarGraph2,0);
        wait(lagTimeBetweenClicks);
        pc.printf("\n BarGraph pattern displayed. \r\n");
        
        // Socket 1
        current_socket = 1;
        pc.printf("\n Taking Air Quality reading... \r\n");
        performClickAction(AirQuality,0);
        pc.printf("\n Air Quality reading taken. \r\n");
        wait(lagTimeBetweenClicks);
        
        // Socket 2
        current_socket = 2;
        pc.printf("\n Taking Distance reading... \r\n");
        performClickAction(LightRanger3,0);
        pc.printf("\n Distance reading taken. \r\n");
        wait(lagTimeBetweenClicks);
    }
}