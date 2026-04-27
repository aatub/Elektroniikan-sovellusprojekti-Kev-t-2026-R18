#include "led.h"
#include "db.h"
#include "PeopleCounter.h"
#include "lcd.h"

LCDDisplay lcd;

void taskUpdateLCD(void *parameter) {
    while (true) {
        lcd.update();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup() {
    Serial.begin(115200);
    led_init();
    init_soundlevel();
    initializePeopleCounter();

    lcd.begin();  // initialize LCD

    xTaskCreate(taskUpdateLCD, "UpdateLCD", 2048, NULL, 1, NULL);
}

void loop() {}
