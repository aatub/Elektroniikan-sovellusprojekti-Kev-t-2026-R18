#ifndef DB_H
#define DB_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern float sharedDb;
extern SemaphoreHandle_t dbMutex;

//aliohjelmat
void init_soundlevel(void);//alustaa soundlevel taskin ja kalibroi db mittarin
float get_db_rms(float); //laskee rms:n avulla desibelin
float get_rms(void); //laskee tehollisarvon 100 näytteen jälkeen
void start_sound_level(void *parameter);
#endif