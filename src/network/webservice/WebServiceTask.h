#include <Arduino.h>
#include "GlobalVars.h"
#include <Scheduler.h>
#include <Task.h>
#include "WebService.h"

class WebServiceTask : public Task {
    protected:  
      void setup() {
      if (biT.isSetupMode() || biT.isRecordMode())       
          webService.setup();
      }

      void loop(){
        if (biT.isSetupMode() || biT.isRecordMode())     
          webService.loop();
      }
    private:
    WebService webService;  
} webServiceTask;