#include "Interface_Analog.h"

Interface_Analog(short int socket, short int click){
    socketSelection = socket;
    clickSelection = click;
}

void Interface_Analog::setSocketSelection(short int socket){
    socketSelection = socket;
}

void Interface_Analog::setClickSelection(short int socket){
    clickSelection = click;
}

short int Interface_Analog::getSocketSelection(short int socket){
    return socketSelection;
}

short int Interface_Analog::getClickSelection(short int socket){
    return clickSelection;
}

string Interface_Analog::getPinAnalog(short int socket){
    switch (socket){
        case 0:
            return "PTB3";
            break;
        case 1:
            return "PTB2";
            break;
        case 2:
            return "PTB1";
            break;
        case 3:
            return "PTB0";
            break;
        default:
            return "Invalid socket in Interface_Analog object.";
            break;
    }
}
