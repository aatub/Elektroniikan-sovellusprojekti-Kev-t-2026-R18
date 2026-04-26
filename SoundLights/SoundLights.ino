#include "led.h"
#include "db.h"
#include "PeopleCounter.h"



void setup() {
  Serial.begin(115200);
  led_init(); 
  //initializePeopleCounter();
  init_soundlevel();
}
void loop() {
}
