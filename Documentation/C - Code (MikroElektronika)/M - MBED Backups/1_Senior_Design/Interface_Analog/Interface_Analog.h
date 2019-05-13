#ifndef Interface_Analog_H
#define Interface_Analog

class Interface_Analog {
 public:
    Interface_Analog(short int socket, short int click);
    
    void setSocketSelection(short int socket);
    void setClickSelection(short int click);
    
    short int getSocketSelection();
    short int getClickSelection();
    string getPinAnalog();
    
 private:
    short int socketSelection;
    short int clickSelection;
 };
 
 #endif // Interface_Analog_H