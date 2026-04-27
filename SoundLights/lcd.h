#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include "PeopleCounter.h"
#include "db.h"

class LCDDisplay {
private:
    uint8_t addr = 0x3E; //i2c address
    void sendCommand(uint8_t cmd); //commandbyte -> lcd
    void sendData(uint8_t data); //databyte -> lcd
    void clear();
    void setCursor(uint8_t col, uint8_t row); // sets cursor
    void print(const char* str); //prints string to lcd
    void printNumber(int num);

public:
    LCDDisplay(); //constructor
    void begin(); //lcd ready
    void update(); //updates lcd
};

#endif