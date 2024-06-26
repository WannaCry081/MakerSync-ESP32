#include "ESPScreen.hpp"


void initLCD() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("<< WELCOME TO >>");
    lcd.setCursor(0, 1);
    lcd.printf(">> MakerSync! <<");
    delay(2500);
}


void displayLCD(const char * line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf(line1);
    lcd.setCursor(0, 1);
    lcd.printf(line2);
}