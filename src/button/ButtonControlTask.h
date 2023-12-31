#include <Arduino.h>
#include "GlobalVars.h"
#include <Scheduler.h>
#include <Task.h>

class ButtonTask : public Task {
protected:
    void setup() {
        count = 0;
        // Wemos D1 R1 = Pin D1 | ESP8266 = Pin 5 
        pinMode(5, INPUT_PULLUP); // Botão para entrar em mnodo de configuração (Usando o resistor interno para PULLUP)

    }

    void loop() {
        if(digitalRead(5)){
            delay(200); 
        } else {
            if(debug)
                Serial.printf("Pull-Up(5)::count=%i",count);
            
            delay(200); 
            count++;
        }

        if(count == 3){
            if(biT.isReadyMode()){
              storage.putOperationMode(SETUP_BIT_MODE);  
            } else{
              storage.putOperationMode(READY_BIT_MODE);
            }
            biT.resetDevice();    
        }
    }

private:
    int count;
} buttonTask;