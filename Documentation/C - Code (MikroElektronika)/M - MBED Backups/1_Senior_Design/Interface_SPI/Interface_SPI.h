#ifndef Interface_SPI_H
#define Interface_SPI

class Interface_SPI {
 public:
    Interface_SPI(short int socket, short int click, short int channel);
    
    void setSocketSelection(short int socket);
    void setClickSelection(short int click);
    void resetChannel(short int channel);
    
    short int getSocketSelection();
    short int getClickSelection();
    string getPinSCK();
    string getPinMISO();
    string getPinMOSI();
    
 private:
    short int socketSelection;
    short int clickSelection;
    short int channelSelection;
 };
 
 #endif // Interface_SPI_H