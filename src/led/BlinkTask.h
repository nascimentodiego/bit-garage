#include <Arduino.h>
#include "GlobalVars.h"
#include <Scheduler.h>
#include <Task.h>

class BlinkTask : public Task {
protected:
    void setup() {
        state = LOW;

        pinMode(2, OUTPUT);
    }

    void loop() {
        state = state == HIGH ? LOW : HIGH;
        digitalWrite(2, state);


        if(biT.isRecordMode()){
            blinkFrequency = 100;
        }else if(biT.isSetupMode()){
            blinkFrequency = 1000;
        }
        
        delay(blinkFrequency);
    }

private:
    uint8_t state;
    long blinkFrequency;
} blinkTask;