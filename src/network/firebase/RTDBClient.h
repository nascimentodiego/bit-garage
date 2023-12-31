#ifndef RTDB_CLIENT_H
#define RTDB_CLIENT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "./GlobalVars.h"
#include "./config/BitConfig.h"
#include "./rf/RFTrasmitControl.h"
#include "FirebaseConsts.h"

/* 1. Define the WiFi credentials */
String WIFI_SSID = "FIGUE";
String WIFI_PASSWORD = "1imortal";

String DOOR_A = "";
String DOOR_B = "";

class FirebaseRealTime
{
public:
    void setup()
    {
        WIFI_SSID = storage.getSSID();
        WIFI_PASSWORD = storage.getWifiPass();

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        Serial.print("Connecting to Wi-Fi");
        while (WiFi.status() != WL_CONNECTED)
        {
            count++;
            if(count>30){
               storage.putOperationMode(SETUP_BIT_MODE);
               biT.resetDevice();     
            }
            Serial.print(".");
            delay(500);
        }
        Serial.println();
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        Serial.println();

        Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

        /* Assign the api key (required) */
        config.api_key = API_KEY;

        /* Assign the user sign in credentials */
        auth.user.email = CREDENTIAL_USER;
        auth.user.password = CREDENTIAL_PASS;

        /* Assign the RTDB URL (required) */
        config.database_url = DATABASE_URL;

        /* Assign the callback function for the long running token generation task */
        config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

        // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
        Firebase.reconnectNetwork(true);

        // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
        // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
        fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

        // Connect
        Firebase.begin(&config, &auth);

        // The data under the node being stream (parent path) should keep small
        // Large stream payload leads to the parsing error due to memory allocation.
        
        if (!Firebase.beginStream(fbdo,  DEVICE_ID_PATH))
            Serial.printf("sream begin error, %s\n\n", fbdo.errorReason().c_str());

        rfTransmit.init();
    }
    void loop()
    {
        // Read
        if (Firebase.ready())
        {
            if (!Firebase.readStream(fbdo))
                Serial.printf("sream read error, %s\n\n", fbdo.errorReason().c_str());

            if (fbdo.streamTimeout())
            {
                Serial.println("stream timed out, resuming...\n");

                if (!fbdo.httpConnected())
                    Serial.printf("error code: %d, reason: %s\n\n", fbdo.httpCode(), fbdo.errorReason().c_str());
            }

            if (fbdo.streamAvailable())
            {
                // Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\nvalue, %s\n\n",
                //               fbdo.streamPath().c_str(),
                //               fbdo.dataPath().c_str(),
                //               fbdo.dataType().c_str(),
                //               fbdo.eventType().c_str(),
                //               fbdo.stringData().c_str());

                String event = fbdo.dataPath().c_str();
                String value = fbdo.stringData().c_str();

                if (event == "/A/value")
                {
                    Serial.println("Evento A disparado");
                    Serial.println("");
                    Serial.printf("Valor:  %s ", value);
                    if (DOOR_A != value)
                    {
                        DOOR_A = value;
                        Serial.println("Liga o Led");

                        // String rfDoorACode = "1010100110101001001110010101"; // storage.getDoorARFCode();
                        // char *code = "1010100110101001001110010101";
                        // char *rfCode;

                        String rfDoorACode = storage.getDoorARFCode();
                        sendCode(rfDoorACode);

                        char *cstr = new char[rfDoorACode.length() + 1];
                        strcpy(cstr, rfDoorACode.c_str());

                        rfTransmit.send(cstr);
                    }
                }
                else if (event == "/B/value")
                {
                    Serial.println("Evento B disparado");
                    Serial.println("");
                    Serial.printf("Valor:  %s ", value);

                    String rfDoorACode = storage.getDoorBRFCode();
                    sendCode(rfDoorACode);
                }
            }

            // After calling stream.keepAlive, now we can track the server connecting status
            if (!fbdo.httpConnected())
            {
                // Server was disconnected!
                Serial.println("Server was disconnected!");
            }
        }
    }

private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    RfTrasmitControl rfTransmit;
    int count;

    void sendCode(String rfDoorACode)
    {
        char *cstr = new char[rfDoorACode.length() + 1];
        strcpy(cstr, rfDoorACode.c_str());

        rfTransmit.send(cstr);
    }
};

#endif // RTDB_CLIENT_H