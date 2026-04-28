#include "lcd.h"
#include "db.h"

LCDDisplay::LCDDisplay() {
    Serial.println("LCD constructor");
}

void LCDDisplay::begin() {
    Wire.begin(21, 22); //sda, scl

    delay(50);

    //lcd initializer
    sendCommand(0x38);
    sendCommand(0x39); 
    sendCommand(0x14);
    sendCommand(0x70); // contrast
    sendCommand(0x56);
    sendCommand(0x6C);

    delay(200);

    sendCommand(0x0C); // display ON
    sendCommand(0x01); // clear
    delay(2);
    sendCommand(0x06); // entry mode
}

/*
0x38	8-bit, 2-rivinen näyttö
0x39	Laajennettu komentotila
0x14	Sisäinen oskillaattori
0x70	Kontrasti
0x56	Virransyötön asetukset
0x6C	Follower control
0x0C	Näyttö päälle
0x01	Tyhjennä näyttö
0x06	Kursori siirtyy oikealle
*/
void LCDDisplay::update() {
    setCursor(0, 0);
    print("People:");
    printNumber(globalPeople);
    print("         ");  // täyttää lopun rivin tyhjällä

    setCursor(0, 1);
    print("dB:");
    if (dbMutex != NULL) {
        xSemaphoreTake(dbMutex, portMAX_DELAY);
        float db = sharedDb;
        xSemaphoreGive(dbMutex);
        printNumber((int)db);
        print("         ");  // täyttää lopun rivin tyhjällä
    }
}
void LCDDisplay::sendCommand(uint8_t cmd) {
    Wire.beginTransmission(addr); //i2c -> lcd
    Wire.write(0x00); //next byte
    Wire.write(cmd); //sends byte
    Wire.endTransmission();
    delayMicroseconds(30);
}

void LCDDisplay::sendData(uint8_t data) {
    Wire.beginTransmission(addr);
    Wire.write(0x40);
    Wire.write(data);
    Wire.endTransmission();
    delayMicroseconds(30);
}

void LCDDisplay::clear() {
    sendCommand(0x01); //clear
    delay(2);
}

void LCDDisplay::setCursor(uint8_t col, uint8_t row) {
    uint8_t offset[] = {0x00, 0x40}; //alkuosoitteet lcd muistissa
    sendCommand(0x80 | (col + offset[row]));
}

void LCDDisplay::print(const char* str) {
    while (*str) sendData(*str++);
}

void LCDDisplay::printNumber(int num) {
    char buf[10];
    sprintf(buf, "%d", num);
    print(buf);
}


