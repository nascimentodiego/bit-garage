#ifndef RF_TRANSMIT_CONTROL_H
#define RF_TRANSMIT_CONTROL_H

#include <Arduino.h>
#include <RCSwitch.h>
#include <string.h>

class RfTrasmitControl
{
public:
    void init()
    {
        rfTrasmitControl = RCSwitch();
        rfTrasmitControl.enableTransmit(GPIO);
        rfTrasmitControl.setProtocol(6);
    }

    void send(char *rfCode)
    {
        rfTrasmitControl.send(rfCode);
    }

private:
    RCSwitch rfTrasmitControl;
    const int GPIO = 12; // ESP8266 == Pin 12 | D1 min = D6
};

#endif //RF_TRANSMIT_CONTROL_H