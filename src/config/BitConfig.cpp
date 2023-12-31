#ifndef BIT_CONFIG_CPP
#define BIT_CONFIG_CPP

#include <Arduino.h>
#include "BitConfig.h"

BitConfig::BitConfig(){
    mode = READY_BIT_MODE;
} 

void BitConfig::setMode(int pMode){
    mode = pMode;
}

void BitConfig::resetDevice(){
    ESP.restart();
}

bool  BitConfig::isReadyMode(){
    return mode == READY_BIT_MODE;
}

bool BitConfig::isSetupMode(){
    return mode == SETUP_BIT_MODE;
}

bool BitConfig::isRecordMode(){
    return mode == RECORD_BIT_MODE;
}

#endif // BIT_CONFIG_CPP