#include "led.h"

void led_init(void){ 
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    led_all_off(); //kaikki pois
}
void led_set_red(int state){
     gpio_set_level(RED_LED, state);
}

void led_set_yellow(int state){
    gpio_set_level(YELLOW_LED, state);
}
void led_set_green(int state){
    gpio_set_level(GREEN_LED, state);

}
void led_all_off(void){
    led_set_green(0);
    led_set_yellow(0);
    led_set_red(0);
   
}
