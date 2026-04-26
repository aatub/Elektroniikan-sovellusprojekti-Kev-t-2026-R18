#ifndef DB_H
#define DB_H

//aliohjelmat
void init_soundlevel(void);//alustaa soundlevel taskin
float get_db_rms(float); //laskee rms:n avulla desibelin
float get_rms(void); //laskee keskiarvon 100:n näytteen jälkeen
void start_sound_level(void *parameter);
#endif