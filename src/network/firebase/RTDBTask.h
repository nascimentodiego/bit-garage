#include <Arduino.h>
#include <Scheduler.h>
#include <LeanTask.h>
#include "RTDBClient.h"

class RealTimeDataBaseTask : public LeanTask
{
protected:
    void setup()
    {
        if (biT.isReadyMode())
            fbRealTime.setup(); 
    }

    void loop()
    {
        if (biT.isReadyMode())
            fbRealTime.loop();
    }

private:
    FirebaseRealTime fbRealTime;
    
} realTimeDataBaseTask;