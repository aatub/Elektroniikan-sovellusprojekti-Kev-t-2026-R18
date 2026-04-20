#include "led.h"
#define SAMPLES 100


void setup() {
  Serial.begin(115200);
  led_init(); 

  //tänne task alhaalla olevasta
  /*
  xTaskCreate(
    SoundLevel, //task-funktio
    "SoundlevelTask",
    4096,
    NULL,
    1,
    &SoundLevelHandle
    );
  ) */
}
void loop() {
 long sum=0;
  //int voiceStart=analogRead(34);//mikrofonin alusta ääni
  //int voiceMax=analogRead(35);//vahvistusten jälkeen :
  //int voiceEnd=analogRead(39);//Lopusta
  //Serial.print("Alku: ");
  //Serial.println(voiceStart);

 // Serial.print("Vahvistus: ");
  //Serial.println(voiceMax);
  //Serial.print("Loppu: ");
  

for(int i=0; i<SAMPLES; i++){
    int val=analogRead(39); //Pinni scheman mukaan!
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
}

//TaskHandle_t SoundLevelHandle=NULL; //
/*void SoundLevel(void *parameter){
  while(true){
  unsigned long long sum=0;
  //int voiceStart=analogRead(34);//mikrofonin alusta ääni
  //int voiceMax=analogRead(35);//vahvistusten jälkeen
  //int voiceEnd=analogRead(39);//Lopusta

  float get_rms(void){
  for(int i=0; i<SAMPLES; i++){
    int val=analogRead(39);
    sum += val*val;
    }

  float rms=sqrt((float)sum/SAMPLES);
  Serial.print("keskiarvo 100 naytteesta: ");
  Serial.println(rms);
  return rms;
}

//TÄSTÄ VOISI TEHDÄ ALIOHJELMAN/
  float get_voltage(float rms) {
    float voltage=rms*(3.3/4095);
    Serial.print("Voltage: ");
    Serial.print(voltage, 3); //
    Serial.println(" V");
}
//Desibeli ohjelma
void get_db{
  rms=get_rms();
  voltage=get_voltage(rms);
  vref=3.3;
  db=10*log10(voltage/vref);
  return db;
}
//DB 

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
}*/