#include "lcd.hpp"

namespace LCD {

    LiquidCrystal_I2C lcd(0x27, 16, 2);

    int WaterLevel;

    void setup() {
        lcd.init();
        lcd.backlight();

        lcd.setCursor(0, 0);
        lcd.print("Modo ");
        lcd.setCursor(5, 0);
        lcd.print("STOP ");
        lcd.setCursor(10, 0);
        lcd.print("Alt:");
        lcd.setCursor(14, 0);
        lcd.print("00");

        lcd.setCursor(0, 1);
        lcd.print("Corr:");
        lcd.setCursor(5, 1);
        lcd.print("0.0 ");
        lcd.setCursor(9, 1);
        lcd.print("Temp:");
        lcd.setCursor(14, 1);
        lcd.print("00");
    }

    void Task_LCD(void *parameters){
        srcSystem system_state;
        MotorSensing::motorInfoStruct motor_info;
        int water_level;
        while (1)
        {
            system_state = Button::GetState();
            writeState(system_state);
            water_level = Ultrassonic::GetWaterLevel();
            writeWaterLevel(water_level);
            motor_info =  MotorSensing::getMotorInfoValue();
            writeCurrent(motor_info.current);
            writeTemperature(motor_info.temperature);
            vTaskDelay(500/portTICK_PERIOD_MS);
        }
    }

    void writeState(srcSystem system_state)
    {
        if(system_state.State == STOP_MODE)
        {
            lcd.setCursor(5, 0);
            lcd.print("STOP ");
        }
        else if(system_state.State == AUTOMATIC_MODE)
        {
            lcd.setCursor(5, 0);
            lcd.print("AUTO ");
        }
        else if(system_state.State == MANUAL_MODE)
        {
            lcd.setCursor(5, 0);
            lcd.print("MAN  ");
        }
    }

    void writeWaterLevel(int water_level)
    {
        if(water_level >= 0 && water_level <= 9)
        {
            lcd.setCursor(14, 0);
            lcd.print(" ");
            lcd.setCursor(15, 0);
            lcd.print(String(water_level));
        }
        else if(water_level >= 10 && water_level <= 14)
        {
            lcd.setCursor(14, 0);
            lcd.print(String(water_level));
        }
    }

    void writeCurrent(float current)
    {
        if(abs(current) >= 0.0 && abs(current) <= 2.0)
        {
            lcd.setCursor(5, 1);
            lcd.print(String(abs(current),1));
            lcd.setCursor(8, 1);
            lcd.print(" ");
        }
    }

    void writeTemperature(float temperature)
    {
        // TODO: Setar temperatura negativa caso haja erro na medição
        if(temperature >= 0.0 && temperature <= 99.0)
        {
            lcd.setCursor(14, 1);
            Serial.print("IMPRIMINDO:");
            Serial.println(String(temperature,0));
            lcd.print(String(temperature,0));
        }
        else
        {
            lcd.setCursor(14, 1);
            lcd.print("ER");
        }
    }

}