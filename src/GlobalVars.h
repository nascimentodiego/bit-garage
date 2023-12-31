#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <Arduino.h>
#include "./config/BitConfig.h"
#include "./storage/Preferences.h"

BitConfig biT = BitConfig{};
Storage storage;
bool debug = true;
#endif // GLOBAL_VARS_H