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