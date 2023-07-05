#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <HTTPClient.h>
#include "ultrassonic.hpp"
#include "motorsensing.hpp"
#include "RTS_settings.h"

#define SEND_INTERVAL_TIME 1000 // segundos... Coloquei alto pra não ficar usando a cota gratuita lá kkk

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
    void uploadInfos();
    void Task_Upload_Status(void *parameters);
    
}