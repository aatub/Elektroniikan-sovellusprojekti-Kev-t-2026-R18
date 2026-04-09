#ifndef LED_H
#define LED_H

//aliohjelmat
void led_init(void); //alustaa ledit
void led_set_red(int state); //asettaa punaisen ledin 0=pois 1=päällä
void led_set_yellow(int state); //asettaa keltaisen ledin 0=pois 1=päällä
void led_set_green(int state); // asettaa vihreän ledin 0=pois 1=päällä
void led_all_off(void); //sammuttaa ledit


#endif