#include "led.h"
#define SAMPLES 100


void setup() {
  Serial.begin(115200);
  led_init(); 
}
void loop() {
 long sum=0;
  int voiceStart=analogRead(34);//mikrofonin alusta ääni
  //int voiceMax=analogRead(35);//vahvistusten jälkeen
  int voiceEnd=analogRead(39);//Lopusta
  //Serial.print("Alku: ");
  //Serial.println(voiceStart);

 // Serial.print("Vahvistus: ");
  //Serial.println(voiceMax);
  //Serial.print("Loppu: ");
  Serial.println(voiceEnd);
  led_set_green();
  delay(100);
  led_set_yellow();
   delay(100);
  led_set_red();
   delay(100);
   led_all_off();

for(int i=0; i<SAMPLES; i++){
    int val=analogRead(39);
  sum += val*val;
}
float rms=sqrt(sum/SAMPLES);
Serial.print("keskiarvo 100 naytteesta: ");
Serial.println(rms);
float voltage=rms*(3.3/4095);

delay(50);

Serial.print("Voltage: ");
Serial.print(voltage, 3); //
Serial.println(" V");



}
