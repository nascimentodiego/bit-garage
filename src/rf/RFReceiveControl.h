#ifndef RF_RECEIVE_CONTROL_H
#define RF_RECEIVE_CONTROL_H

#include <Arduino.h>
#include <RCSwitch.h>
#include <string.h>
#include "GlobalVars.h"
#include "./util/StringHelper.h"

class RfReceiveControl
{
public:
    void init()
    {
        if (biT.isRecordMode() || biT.isSetupMode())
        {
            rfReceiver = RCSwitch();
            rfReceiver.enableReceive(GPIO);
            slot = storage.getSlotRecord();
        }
    }

    void recordCode()
    {
        if (rfReceiver.available())
        {
            output(rfReceiver.getReceivedValue(), rfReceiver.getReceivedBitlength(), rfReceiver.getReceivedDelay(), rfReceiver.getReceivedRawdata(), rfReceiver.getReceivedProtocol());
            char *binarayCode = dec2binWzerofill(rfReceiver.getReceivedValue(), rfReceiver.getReceivedBitlength());

            if (biT.isRecordMode())
            {
                storage.putCode(binarayCode);
                storage.putOperationMode(SETUP_BIT_MODE);
                biT.setMode(SETUP_BIT_MODE);

                Serial.println("Code save::");
                Serial.printf(binarayCode);
                Serial.println("");
                Serial.printf("SLOT - %s", storage.getSlotRecord());
            }
            rfReceiver.resetAvailable();
        }
    }

private:
    RCSwitch rfReceiver;
    String slot;
    const int GPIO = 13; // ESP8266 == Pin 13 | D1 min = D7
};

#endif // RF_RECEIVE_CONTROL_H