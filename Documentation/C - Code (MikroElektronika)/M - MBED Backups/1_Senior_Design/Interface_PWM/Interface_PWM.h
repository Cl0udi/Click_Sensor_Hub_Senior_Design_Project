#ifndef Interface_PWM_H
#define Interface_PWM

class Interface_PWM {
 public:
    Interface_PWM(short int socket, short int click);
    
    void setSocketSelection(short int socket);
    void setClickSelection(short int click);
    
    short int getSocketSelection();
    short int getClickSelection();
    string getPinPWM();
    
 private:
    short int socketSelection;
    short int clickSelection;
 };
 
 #endif // Interface_PWM_H