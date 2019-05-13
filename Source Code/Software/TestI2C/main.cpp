#include "mbed.h"
#include "RFD77402.h"

// Set up Terminal
Serial pc(USBTX, USBRX);

I2C shared_I2C(PTE0, PTE1); // sda, scl

// Current Socket Global
short int current_socket = 0;
float lagTimeBetweenClicks = 1.0f;

void readRegister_I2C(uint8_t dev_addr, int reg_addr, uint8_t* data, int len){
    char t[1] = {reg_addr};
    shared_I2C.write(dev_addr, t, 1, true);
    shared_I2C.read(dev_addr, (char *)data, len);
}
 
void writeRegister_I2C(uint8_t dev_addr, uint8_t* data, int len){
    shared_I2C.write(dev_addr, (char*)data, len);
}

void click_Gaussmeter_getGaussReadingsI2C(){
    
    int addresses[3];
    addresses[0] = 0x04; //x axis
    addresses[1] = 0x05; //y axis - not used
    addresses[2] = 0x06; //z axis - not used
    uint8_t dev_addr = 0x0C; // device address
    uint8_t singleMeasurementCMD[1];
    singleMeasurementCMD[0] = 0x3F; // 011ZYXT bits according to what we read
    uint8_t gaussReadingsXYZ[6] = {0};
    uint8_t data[7] = {0};
    
    // Write register command, AH = 0x00, AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5(0x60, 0x00, 0x5C, 0x00)
    // Address register, (0x00 << 2)
    uint8_t config[4] = {0};
    config[0] = 0x60;
    config[1] = 0x00;
    config[2] = 0x5C;
    config[3] = 0x00;
    writeRegister_I2C(dev_addr, config, 4);

    // Read 1 Byte of data
    // status byte
    readRegister_I2C(dev_addr, 0x00, data, 1);

    // Write register command, AH = 0x02, AL = 0xB4, RES for magnetic measurement = 0(0x60, 0x02, 0xB4, 0x08)
    // Address register, (0x02 << 2)
    config[0] = 0x60;
    config[1] = 0x02;
    config[2] = 0xB4;
    config[3] = 0x08;
    writeRegister_I2C(dev_addr, config, 4);

    // Read 1 Byte of data
    // Status byte
    readRegister_I2C(dev_addr, 0x00, data, 1);

    // Start single meaurement mode, X, Y, Z-Axis enabled(0x3E)
    config[0] = 0x3E;
    writeRegister_I2C(dev_addr, config, 1);

    // Read 1 byte of data
    // Status byte
    readRegister_I2C(dev_addr, 0x00, data, 1);
    wait(1);

    // Send read measurement command, X, Y, Z-Axis enabled(0x4E)
    config[0] = 0x4E;
    writeRegister_I2C(dev_addr, config, 1);
                                  
    
    // Set chip to single measurement mode
    // writeRegister_I2C(dev_addr, singleMeasurementCMD, 1);
    
    // Read 6 bytes. 
    // First two X bytes, then two Y bytes, finally two Z bytes
    readRegister_I2C(dev_addr, addresses[0], gaussReadingsXYZ, 6);
    
    float xAxisReading = (gaussReadingsXYZ[0] << 8) + gaussReadingsXYZ[1];
    float yAxisReading = (gaussReadingsXYZ[2] << 8) + gaussReadingsXYZ[3];
    float zAxisReading = (gaussReadingsXYZ[4] << 8) + gaussReadingsXYZ[5];
    
    float xTest = (data[0] << 8) + data[1];
    float yTest = (data[2] << 8) + data[3];
    float zTest = (data[4] << 8) + data[5];
    
    pc.printf("\r\nX axis reading: %f\r\n", xAxisReading);
    pc.printf("Y axis reading: %f\r\n", yAxisReading);
    pc.printf("Z axis reading: %f\r\n", zAxisReading);
    
    pc.printf("\r\nTest X axis reading: %f\r\n", xTest);
    pc.printf("Test Y axis reading: %f\r\n", yTest);
    pc.printf("Test Z axis reading: %f\r\n", zTest);
}

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

void click_Accel5_getMovementReadingsI2C(){
    
    pc.printf("Getting acceleration readings from Accel5 Click...");
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
    
    // set normal power mode
    char data = 0x02;
    int setupAddress = 0x19;
    shared_I2C.write(setupAddress, &data, 1);
    pc.printf("1");
    
    // setFullScaleRange
    setupAddress = 0x1A;
    shared_I2C.write(setupAddress, &data, 1);
    pc.printf("2");
    
    setupAddress = 0;
    for(int i = 0; i < 256; i++){
        shared_I2C.write(setupAddress, &data, 1);
        setupAddress++;
        pc.printf("3");
    }
        
//    setupAddress = 0x1A;
//    shared_I2C.write(setupAddress, &data, 1);
    
//    shared_I2C.write(slave_address, &slave_cmd, 1);
    shared_I2C.read(addresses[0], &xAxisBits0to7, 1);
    shared_I2C.read(addresses[1], &xAxisBits8to11, 1);
    shared_I2C.read(addresses[2], &yAxisBits0to7, 1);
    shared_I2C.read(addresses[3], &yAxisBits8to11, 1);
    shared_I2C.read(addresses[4], &zAxisBits0to7, 1);
    shared_I2C.read(addresses[5], &zAxisBits8to11, 1);
    
    float xAxisReading = (xAxisBits8to11 << 8) + xAxisBits0to7;
    float yAxisReading = (yAxisBits8to11 << 8) + yAxisBits0to7;
    float zAxisReading = (zAxisBits8to11 << 8) + zAxisBits0to7;
    
    pc.printf("\r\nX axis acceleration: %.4f", xAxisReading);
    pc.printf("Y axis acceleration: %.4f", yAxisReading);
    pc.printf("Z axis acceleration: %.4f\r\n", zAxisReading);
}

void click_Temp2Hum_getTemperatureReadingsI2C(){
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

void testFunction(){ // Gaussmeter code
    
    int addr = 0x0C <<1;
    char config [4];
    char data[7] = {0};
 
    config[0] = 0x60;
    config[1] = 0x00;
    config[2] = 0x5C;
    config[3] = 0x00;
 
    shared_I2C.write(addr, config, 4, false);
 
    shared_I2C.read(addr, data, 1);
 
    config[0] = 0x60;
    config[1] = 0x02;
    config[2] = 0xB4;
    config[3] = 0x02;
 
    shared_I2C.write(addr, config, 4, false);
 
    shared_I2C.read(addr, data, 1);
 
    wait(.25); 
    
    config[0] = 0x3E; // Single measurement mode, ZYX enabled
 
    shared_I2C.write(addr, config, 1 , false);
    shared_I2C.read(addr, data, 1);
 
    wait(.1);
 
    config[0] = 0x4E;
 
    shared_I2C.write(addr, config, 1, false); // Read command, followed by ZYX bits set
    shared_I2C.read(addr, data, 7);
 
    float xMag = ((data[1] * 256) + data[2]);
    float yMag = ((data[3] * 256) + data[4]);
    float zMag = ((data[5] * 256) + data[6]);
 
    pc.printf("\r\nX Axis = %.4f \r\n", xMag);
    pc.printf("Y Axis = %.4f \r\n", yMag);
    pc.printf("Z Axis = %.4f \r\n", zMag);
}

// main() runs in its own thread in the OS
int main(){
    
    pc.baud(115200);
    pc.printf(" Program succesfully booted. \r\n");
    
    float lagTimeBetweenClicks = 2;
    
    while(true){
//        pc.printf("\r\n Taking Gaussmeter reading...\r\n");
//        click_Gaussmeter_getGaussReadingsI2C();
//        pc.printf("\r\n Gaussmeter reading taken.\r\n");
//        wait(lagTimeBetweenClicks);
        
//        pc.printf("\n Taking Temperature reading...\n");
//        click_Temp2Hum_getTemperatureReadingsI2C();
//        pc.printf("\n Temperature reading taken.\n");
//        wait(lagTimeBetweenClicks);
//        
//        pc.printf("\n Taking Color reading...\n");
//        click_Color5_getColorReadingsI2C();
//        pc.printf("\n Color reading taken.\n");
//        wait(lagTimeBetweenClicks);
//        
//        pc.printf("\n Taking Acceleration reading...\n");
//        click_Accel5_getMovementReadingsI2C();
//        pc.printf("\n Acceleration reading taken.\n");
//        wait(lagTimeBetweenClicks);
//        
        pc.printf("\n Taking Distance reading... \r\n");
        click_LightRanger_getDistanceReadingsI2C();
        pc.printf("\n Distance reading taken. \r\n");
        wait(lagTimeBetweenClicks);
        
//        pc.printf("\n Test function reading...");
//        testFunction();
//        pc.printf("\n Test function reading taken.");
//        wait(lagTimeBetweenClicks);
    }
}
