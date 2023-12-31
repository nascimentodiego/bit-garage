#ifndef STORAGE_H
#define STORAGE_H

#include "StorageConsts.h"
#include "./config/BitConfig.h"

class Storage
{
public:
    void putOperationMode(int _mode)
    {
        prefs.begin(preferenceApp);
        prefs.putInt(pref_mode_operation, _mode);
        prefs.end();
    }

    int getOperationMode()
    {
        prefs.begin(preferenceApp);
        int mode = prefs.getInt(pref_mode_operation, READY_BIT_MODE);
        prefs.end();

        return mode;
    }

    void recordSlotA()
    {
        prefs.begin(preferenceApp);
        prefs.putString(pref_record_slot, "A");
        prefs.putInt(pref_mode_operation, RECORD_BIT_MODE);
        prefs.end();
    }

    void recordSlotB()
    {
        prefs.begin(preferenceApp);
        prefs.putString(pref_record_slot, "B");
        prefs.putInt(pref_mode_operation, RECORD_BIT_MODE);
        prefs.end();
    }

    void putCode(char *rfCode)
    {
        String slot = getSlotRecord();

        prefs.begin(preferenceApp);
        if (slot == "A") // strcmp(slot, "A")
        {
            Serial.println("Salvou code A");
            Serial.printf(" - code: %s",rfCode);
            prefs.putString(pref_slot_a, rfCode);
        }
        else
        {
            Serial.println("Salvou code B");
            Serial.printf(" - code: %s",rfCode);
            prefs.putString(pref_slot_b, rfCode);
        }

        prefs.end();
    }

    String getSlotRecord()
    {
        prefs.begin(preferenceApp);
        String slot = prefs.getString(pref_record_slot, "A");
        prefs.end();

        return slot;
    }

    String getDoorARFCode()
    {
        prefs.begin(preferenceApp);
        String rfCode = prefs.getString(pref_slot_a, "");
        prefs.end();

        return rfCode;
    }

    String getDoorBRFCode()
    {
        prefs.begin(preferenceApp);
        String rfCode = prefs.getString(pref_slot_b, "");
        prefs.end();

        return rfCode;
    }

    void putWiFiSSID(String _ssid)
    {
        prefs.begin(preferenceApp);
        prefs.putString(pref_ssid, _ssid);
        prefs.end();
    }

    void putWifiPass(String _pass)
    {
        prefs.begin(preferenceApp);
        prefs.putString(pref_pass, _pass);
        prefs.end();
    }

    String getSSID()
    {
        prefs.begin(preferenceApp);
        String ssid = prefs.getString(pref_ssid, "");
        prefs.end();

        return ssid;
    }

    String getWifiPass()
    {
        prefs.begin(preferenceApp);
        String pass = prefs.getString(pref_pass, "");
        prefs.end();

        return pass;
    }
};

#endif // STORAGE_H