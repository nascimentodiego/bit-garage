#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>
#include "WebServiceConsts.h"
#include "./storage/Preferences.h"
#include "./GlobalVars.h"

IPAddress ip(192, 168, 4, 4);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

void handleBody()
{
    if (server.hasArg("plain") == false)
    { // Check if body received
        server.send(200, "text/plain", "/bitconfig/wifi -> Body not received");
        return;
    }

    String message = "/bitconfig/wifi -> Body received:\n";
    String responseBody = "{\"device_id\":\"";
    responseBody += DEVICE_ID;
    responseBody +="\"}";

    message += "\n";
    message += responseBody;


    server.send(200, "text/plain", responseBody);

    JSONVar myObject = JSON.parse(server.arg("plain"));
    if (JSON.typeof(myObject) == "undefined")
    {
        Serial.println("Parsing input failed!");
        return;
    }

    Serial.println(message);
    Serial.println("");
    if (myObject.hasOwnProperty("ssid"))
    {
        Serial.println("ssid -> ");
        String ssid = myObject["ssid"];
        Serial.print(ssid);

        // Save SSID
        storage.putWiFiSSID(ssid);
    }
    Serial.println("");
    if (myObject.hasOwnProperty("pass"))
    {
        Serial.println("pass -> ");
        String pass = myObject["pass"];
        Serial.print(pass);

        // SAVE WiFi Password
        storage.putWifiPass(pass);
    }
    // if (myObject.hasOwnProperty("pass") && myObject.hasOwnProperty("ssid"))
    // {
    //     storage.putOperationMode(READY_BIT_MODE);
    //     biT.resetDevice();
    // }
};

void handleRecordBody()
{
    if (server.hasArg("plain") == false)
    { // Check if body received
        server.send(200, "text/plain", "/bitconfig/record -> Body not received");
        return;
    }

    String message = "/bitconfig/record -> Body Record received:\n";
    String responseBody = "{\"device_id\":\"";
    responseBody += DEVICE_ID;
    responseBody +="\"}";

    message += "\n";
    message += responseBody;


    server.send(200, "text/plain", responseBody);

    JSONVar myObject = JSON.parse(server.arg("plain"));
    if (JSON.typeof(myObject) == "undefined")
    {
        Serial.println("Parsing input failed!");
        return;
    }

    Serial.println(message);
    Serial.println("");
    if (myObject.hasOwnProperty("slot"))
    {
        Serial.println("slot -> ");
        String slotFromBody = myObject["slot"];
        Serial.print(slotFromBody);
        if(slotFromBody == "A")
            storage.recordSlotA();
        else 
            storage.recordSlotB();

        storage.putOperationMode(RECORD_BIT_MODE);    
        biT.setMode(RECORD_BIT_MODE);
    }
};

void handleFinishBody()
{
    String message = "/bitconfig/finish -> Body Record received:\n";
    String responseBody = "{\"device_id\":\"";
    responseBody += DEVICE_ID;
    responseBody +="\"}";

    message += "\n";
    message += responseBody;

    server.send(200, "text/plain", responseBody);

    storage.putOperationMode(READY_BIT_MODE);    
    biT.setMode(READY_BIT_MODE);
    biT.resetDevice();
};

class WebService
{
public:
    void setup()
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid, password, 3, 0);
        WiFi.softAPConfig(ip, gateway, subnet);

        server.on("/bitconfig/wifi", handleBody);\
        server.on("/bitconfig/record", handleRecordBody);
         server.on("/bitconfig/finish", handleFinishBody);
        server.begin();

        Serial.println(" ");
        Serial.println("Servidor Iniciado !!!");
        Serial.println(WiFi.softAPIP());
    }

    void loop()
    {
        server.handleClient();
    }
} webService;