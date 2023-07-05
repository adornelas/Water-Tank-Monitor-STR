#include "lcd.hpp"

namespace LCD {

    LiquidCrystal_I2C lcd(0x27, 16, 2);

    int WaterLevel;

    void setup() {
        lcd.init();
        lcd.backlight();
    }

    void Task_LCD(void *parameters){
        int water_level;
        while (1)
        {
            lcd.setCursor(0, 0);
            lcd.print("WLevel:");
            lcd.setCursor(8, 0);
            lcd.print("Very Low");
            lcd.setCursor(0, 1);
            lcd.print("Motor is OFF");
        }
    }

}