#ifndef Connection_h
#define Connection_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid     = "NOME INTERNET";
const char* password = "SENHA INTERNET";

// supabase credentials
String API_URL = "https://gxxsgcigwtxtmgnwldep.supabase.co";
String API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imd4eHNnY2lnd3R4dG1nbndsZGVwIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODQ3NzM0NjAsImV4cCI6MjAwMDM0OTQ2MH0.iYgOl9V7-_BclNSydIRP8maPq1f64BwKbwpC3JFbXUE";
String TableName = "maintable";
const int httpsPort = 443;

namespace Connection{
    HTTPClient https;
    WiFiClientSecure client;
    // Sending interval of the packets in seconds
    int sendinginterval = 120; // 2 minutes

    void setup(void);
    void uploadInfos(float current, 
                    float temperature,
                    int level);

}

void Connection::setup(void){
    // HTTPS is used without checking credentials 
    client.setInsecure();

    Serial.print("Connecting to ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   
}

void Connection::uploadInfos(float current, float temperature, int level){

    if (WiFi.status() == WL_CONNECTED) {
        // Send the a post request to the server
        https.begin(client,API_URL+"/rest/v1/"+TableName);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Prefer", "return=representation");
        https.addHeader("apikey", API_KEY);
        https.addHeader("Authorization", "Bearer " + API_KEY);

        // int httpCode = https.POST("{\"temperature\":" + String(t)+ ",\"humidity\":"+ String(h)+",\"moisture\":" + String(1024 - m)+"}" );   //Send the request
        int httpCode = https.POST("{\"temperature\":" + String(temperature)+ ",\"corrente\":"+ String(current)+",\"nivel\":" + String(level)+"}" );   //Send the request
        String payload = https.getString(); 
        Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
        https.end();

    }
    // else{
    //     Serial.println("Error in WiFi connection");
    // }
    
    //wait to send the next request
    vTaskDelay(1000*sendinginterval);
}

#endif