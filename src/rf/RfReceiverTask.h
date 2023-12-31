#include <Arduino.h>
#include <Scheduler.h>
#include <Task.h>
#include "RfReceiveControl.h"

class RfReceiverTask : public Task
{
protected:
    void setup()
    {
        if (biT.isSetupMode() || biT.isRecordMode())
        {
            rfReceiverControl.init();
            count = 0;
        }  
    }

    void loop()
    {
        if (biT.isRecordMode()){
            rfReceiverControl.recordCode();
            count++;
        }else{
             count = 0;
        }

        if(count > 10){
            storage.putOperationMode(SETUP_BIT_MODE);
            biT.setMode(SETUP_BIT_MODE);
        }
        delay(500);
    }

private:
    RfReceiveControl rfReceiverControl;
    int count;
    
} rfReceiverTask;