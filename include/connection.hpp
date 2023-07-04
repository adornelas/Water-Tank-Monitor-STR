#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <HTTPClient.h>

#define SEND_INTERVAL_TIME 120 // 2 minutos

// Replace with your network credentials
extern const char* ssid;
extern const char* password;

// supabase credentials
extern String API_URL;
extern String API_KEY;
extern String TableName;
extern const int httpsPort;

namespace Connection{
    extern HTTPClient https;
    extern WiFiClientSecure client;

    void setup(void);
    void uploadInfos(float current, 
                    float temperature,
                    int level);

}