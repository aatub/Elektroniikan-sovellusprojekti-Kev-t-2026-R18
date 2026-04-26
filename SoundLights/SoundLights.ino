#include "led.h"
#include "db.h"
#include "PeopleCounter.h"

#define SAMPLES 100

TaskHandle_t SoundLevelHandle=NULL;

void setup() {
  Serial.begin(115200);
  led_init(); 
  //initializePeopleCounter();
  init_soundlevel();
}
void loop() {
}

  xTaskCreate(
  start_sound_level, //task-funktio
  "SoundlevelTask",
  4096,
  NULL,
  1,
  &SoundLevelHandle
  );
}
void loop() {
}
void start_sound_level(void *parameter){//Laskee keskiarvon ja sytyttää tarpeelliset ledit
  while(true){
    float rms=get_rms();
    led_all_off();
    if (rms < 2500.0){
              led_set_green();
          }
          else if( rms >=2500.0 && rms<3000.0){
              led_set_yellow();
          }
          else{
              led_set_red();
          }
        vTaskDelay(50/portTICK_PERIOD_MS);
  }
}
