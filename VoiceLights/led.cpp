#include "led.h"
#include <Arduino.h>

//Led-pinnien määritys
#define RED_LED 18
#define YELLOW_LED 17
#define GREEN_LED 16

void led_init(void){ 
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    led_all_off(); //kaikki pois
}
void led_set_red(int state){
     pinMode(RED_LED, state);
}

void led_set_yellow(int state){
    pinMode(YELLOW_LED, state);
}
void led_set_green(int state){
    pinMode(GREEN_LED, state);

}
void led_all_off(void){
    led_set_green(LOW);
    led_set_yellow(LOW);
    led_set_red(LOW);
   
}
