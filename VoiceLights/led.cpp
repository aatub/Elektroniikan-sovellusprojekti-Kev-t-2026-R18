#include "led.h"
#include <Arduino.h>

//Led-pinnien määritys
#define RED_LED 16
#define YELLOW_LED 17
#define GREEN_LED 18

void led_init(void){ 
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    led_all_off(); //kaikki pois
}
void led_set_red(void){
    digitalWrite(GREEN_LED, HIGH);
}
void led_set_yellow(void){
    digitalWrite(YELLOW_LED, HIGH);
} //asettaa keltaisen ledin päälle
void led_set_green(void){
    digitalWrite(RED_LED, HIGH);
} // asettaa vihreän ledin päälle
void led_all_off(void){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
}
