#ifndef ESPSCREEN_HPP
#define ESPSCREEN_HPP


#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


void initLCD();


#endif