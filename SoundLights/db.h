#ifndef DB_H
#define DB_H

//aliohjelmat
void db_init(void); //Alustaa 
float get_rms(void); //laskee keskiarvon 100:n näytteen jälkeen
float get_db(float voltage); //laskee db:n jännitteen avulla
float get_voltage(float rms); //Laskee jännitteen keskiarvon mukaan


#endif