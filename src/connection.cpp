#include "connection.hpp"
#include "ultrassonic.hpp"
#include "motorsensing.hpp"

// NOME DA REDE WIFI
const char* ssid     = "Andre";
// SENHA DA REDE WIFI
const char* password = "papagaio";

// supabase credentials
String API_URL = "https://gxxsgcigwtxtmgnwldep.supabase.co";
String API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imd4eHNnY2lnd3R4dG1nbndsZGVwIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODQ3NzM0NjAsImV4cCI6MjAwMDM0OTQ2MH0.iYgOl9V7-_BclNSydIRP8maPq1f64BwKbwpC3JFbXUE";
String TableName = "maintable";
const int httpsPort = 443;

namespace Connection{
    HTTPClient https;
    WiFiClientSecure client;

    void setup(void){
        // HTTPS is used without checking credentials 
        client.setInsecure();

        WiFi.begin(ssid, password);
        #if PRINT_DEBUG
        Serial.print("Connecting to ");
        Serial.println(ssid);
        #endif
        while (WiFi.status() != WL_CONNECTED){
            delay(500);
            #if PRINT_DEBUG
            Serial.print(".");
            #endif
        }
        #if PRINT_DEBUG
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());   
        #endif
    }

    void uploadInfos(){
        MotorSensing::motorInfoStruct motor_info;
        int water_level;

        https.begin(client,API_URL+"/rest/v1/"+TableName);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Prefer", "return=representation");
        https.addHeader("apikey", API_KEY);
        https.addHeader("Authorization", "Bearer " + API_KEY);

        water_level = Ultrassonic::GetWaterLevel();
        motor_info =  MotorSensing::getMotorInfoValue();

        int httpCode = https.POST("{\"temperature\":" + String(motor_info.temperature)+ ",\"corrente\":"+ String(motor_info.current)+",\"nivel\":" + String(water_level)+"}" );   //Send the request
        https.end();
    }

    void Task_Upload_Status(void *parameters){
        struct timeval t0,t1, dt;

        while(1){
            gettimeofday(&t0,NULL);
            if(WiFi.status() == WL_CONNECTED){
                Connection::uploadInfos();
            }
            else
            {
                #if PRINT_DEBUG
                Serial.println("Desconectado da internet");
                #endif
            }
            gettimeofday(&t1,NULL);
            timersub(&t1, &t0, &dt);
            Serial.print("Task_Upload_Status:");
            Serial.print(dt.tv_sec);
            Serial.print(".");
            Serial.println(dt.tv_usec);
            vTaskDelay(UPLOAD_STATUS_PERIOD*SEND_INTERVAL_TIME/portTICK_PERIOD_MS);
        }
  
    }

    
}

