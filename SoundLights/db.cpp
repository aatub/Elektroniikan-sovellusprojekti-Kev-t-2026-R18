#include "db.h"
#include "led.h"
#include <Arduino.h>
#include <math.h>

#define SAMPLES 100
//ADC-pinnien määritys
//#define Microphone 34
//#define amplifier 35
#define final 39
#define buzzer 19 //toinen jalka groundiin

float sharedDb = 0;
float prev_db=0;
SemaphoreHandle_t dbMutex = NULL;
struct CalibrationPoint {
      float adc;
    	float db;
  };
CalibrationPoint calibration[] = { //manuaalinen kalibrointitaulukko
    {40,   30},   // hiljaisuus
    {50,   40},  
    {160,  60},
    {300,  70},   // normaali puhe
    {500,  80},   // kova puhe
    {900,  92},
    {1300, 100},  // maksimi
};

const int N = sizeof(calibration) / sizeof(calibration[0]);

void init_soundlevel(void){
    dbMutex = xSemaphoreCreateMutex();
    TaskHandle_t SoundLevelHandle=NULL;
    xTaskCreate(
      start_sound_level, //task-funktio
      "SoundlevelTask",
      4096,
      NULL,
      1,
      &SoundLevelHandle
    );
}
  
float get_db_rms(float x) {
    if (x <= calibration[0].adc) return calibration[0].db; 

  for (int i = 0; i < N - 1; i++) {
    if (x <= calibration[i + 1].adc) {

      float x0 = calibration[i].adc;
      float y0 = calibration[i].db;
      float x1 = calibration[i + 1].adc;
      float y1 = calibration[i + 1].db;

      float t = (x - x0) / (x1 - x0);

      return y0 + t * (y1 - y0);
    }
  }
  return calibration[N - 1].db;
}
float get_rms(void){ //
long sum=0;
int samples[SAMPLES];
  for(int i=0; i<SAMPLES; i++){
    samples[i]=analogRead(final);
    sum +=samples[i];
    delayMicroseconds(150);
    }
    float mean =sum/(float)SAMPLES;
    //Offsetin poisto
    long long sq_sum=0;
    for(int i=0; i<SAMPLES; i++){
      float val=samples[i]-mean;
      sq_sum += val*val;
    }
  float rms=sqrt(sq_sum/(float)SAMPLES);
  sum=0;
  return rms;
}

void start_sound_level(void *parameter){//Laskee keskiarvon ja sytyttää tarpeelliset ledit
  while(true){
    float rms=get_rms();
    //Serial.print("RMS: ");
    //Serial.println(rms);    
    led_all_off();
    if (rms < 300){ //sytyttää ledit vihreä <=2500 keltainen >=2500 ja <3000 ja punainen >3000
              led_set_green();
          }
          else if( rms >=300 && rms<1000.0){
              led_set_yellow();
          }
          else{
              tone(buzzer,500,100);
              led_set_red();
              
          }
          float db=get_db_rms(rms);
          //db= db * 0.8 + prev_db * 0.2; //tasoittaa desibelin heiluntaa aiemmasta arvosta 20% ja uudesta 80%
          prev_db=db;
          //Serial.print("DB: ");
          //Serial.println(db);
                  if (dbMutex != NULL) {
            xSemaphoreTake(dbMutex, portMAX_DELAY);
            sharedDb = db;
            xSemaphoreGive(dbMutex);
        }
        vTaskDelay(50/portTICK_PERIOD_MS);

  }
}
