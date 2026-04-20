#include "db.h"
#include <Arduino.h>
#include <math.h>

#define SAMPLES 100
//ADC-pinnien määritys
#define Microphone 34
#define amplifier 35
#define final 39

long long sum=0;
float db;

float get_rms(void){ //
  for(int i=0; i<SAMPLES; i++){
    int val=analogRead(final);
    sum += val*val;
    }

  float rms=sqrt((float)sum/SAMPLES);
  Serial.print("keskiarvo 100 naytteesta: ");
  Serial.println(rms);
  sum=0;
  return rms;
}

float get_voltage(float rms) { // Muuttaa keskiarvon jännitteeksi
  float voltage=rms*(3.3/4095.0);
  Serial.print("Voltage: ");
  Serial.print(voltage, 3); 
  Serial.println(" V");
  return voltage;
}
//Desibeli ohjelma
float get_db(float voltage){
  float vref=3.3;
  db=20*log10(voltage/vref);
  return db;
}