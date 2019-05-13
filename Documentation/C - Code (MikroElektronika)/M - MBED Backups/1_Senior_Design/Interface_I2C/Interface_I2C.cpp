#include "Interface_I2C.h"

Interface_I2C(short int socket, short int click){
    socketSelection = socket;
    clickSelection = click;
}

void Interface_I2C::setSocketSelection(short int socket){
    socketSelection = socket;
}

void Interface_I2C::setClickSelection(short int socket){
    clickSelection = click;
}

short int Interface_I2C::getSocketSelection(short int socket){
    return socketSelection;
}

short int Interface_I2C::getClickSelection(short int socket){
    return clickSelection;
}

string Interface_I2C::getPinSCL(short int socket){
    return "PTE1";
}

string Interface_I2C::getPinSDA(short int socket){
    return "PTE0";
}