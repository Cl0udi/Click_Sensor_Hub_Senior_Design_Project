#ifndef Interface_I2C_H
#define Interface_I2C

class Interface_I2C {
 public:
    Interface_I2C(short int socket, short int click);
    
    void setSocketSelection(short int socket);
    void setClickSelection(short int click);
    
    short int getSocketSelection();
    short int getClickSelection();
    string getPinSCL();
    string getPinSDA();
    
 private:
    short int socketSelection;
    short int clickSelection;
 };
 
 #endif // Interface_I2C_H