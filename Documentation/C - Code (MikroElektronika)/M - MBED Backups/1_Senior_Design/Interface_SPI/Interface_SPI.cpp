#include "Interface_SPI.h"

// Channel 1 = Regular SPI, Channel 2 = UART to SPI

Interface_SPI(short int socket, short int click, short int channel){
    socketSelection = socket;
    clickSelection = click;
    channelSelection = channel;
//    PinSCK  = (channel == 1)? "PTA15" : "PTD5";
//    PinMISO = (channel == 1)? "PTA17" : "PTD7";
//    PinMOSI = (channel == 1)? "PTA16" : "PTD6";
}

void Interface_SPI::setSocketSelection(short int socket){
    socketSelection = socket;
}

void Interface_SPI::setClickSelection(short int socket){
    clickSelection = click;
}

void Interface_SPI::resetChannel(short int channel){
    channelSelection = channel;
}

short int Interface_SPI::getSocketSelection(){
    return socketSelection;
}

short int Interface_SPI::getClickSelection(){
    return clickSelection;
}

string Interface_SPI::getPinSCK(){
    if (channel == 1) return PTA15;
    else return PTD5;
}

string Interface_SPI::getPinMISO(){
    if (channel == 1) return PTA17;
    else return PTD7;
}

string Interface_SPI::getPinMOSI(){
    if (channel == 1) return PTA16;
    else return PTD6;
}
