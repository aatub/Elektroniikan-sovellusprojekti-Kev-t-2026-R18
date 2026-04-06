#include "led.h"


void app_main() {
    led_init();

    int voiceStart = gpio_get_level(34); //mikrofonin alusta ääni
    int voiceMax = gpio_get_level(35); //vahvistusten jälkeen
    int voiceEnd = gpio_get_level(39); //Lopusta

    printf(voiceStart);
    printf(voiceMax);
    printf(voiceEnd);
    
    double Vref=3.3;
    float db=20.0*log10(voiceMax/Vref);  //Muuttaa pinnin lukemat desibeliksi
    printf("Äänitaso: %.2f dB\n", db);

    //Viive, jolla led valo näkyy, kuinka usein mitataan ääntä
    while(1){
       //led_all_off();

        if (db < 60.0){
            led_set_yellow(0);
            led_set_red(0);
            led_set_green(1);
        }
        else if( db >= 60.0 && db<88.0){
            led_set_green(0);
            led_set_red(0);
            led_set_yellow(1);
        }
        else{
            led_set_green(0);
            led_set_yellow(0);
            led_set_red(1);
        }
    } 
}