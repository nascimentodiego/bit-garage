#ifndef BIT_CONFIG_H
#define BIT_CONFIG_H

#include <Arduino.h>

#define SETUP_BIT_MODE 1
#define READY_BIT_MODE 2
#define RECORD_BIT_MODE 3
#define DEVICE_ID "eecbfa70-f21b-43f4-963e-8f63dde9dbdf"
#define DEVICE_ID_PATH "/eecbfa70-f21b-43f4-963e-8f63dde9dbdf"

class BitConfig {
        volatile int mode;
    public:
        BitConfig();
        void setMode(int);
        bool isReadyMode();
        bool isSetupMode();
        bool isRecordMode();
        void resetDevice();
};

#endif // BIT_CONFIG_H