#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int soundPin = A0;
int people = 0;
int lastStatePlus = HIGH;
int lastStateMinus = HIGH;
int buzzerPin = 9;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int sound = analogRead(soundPin); //lukee 0-1023 analogisen arvon
  int dB = map(sound, 0, 1023, 30, 100); //0 = 30db, 1023 = 100db --- ESP32: 0–4095 (ei 1023!)

  int currentPlus = digitalRead(7);
  int currentMinus = digitalRead(8);

  if (lastStatePlus == HIGH && currentPlus == LOW) {
    people++;
    delay(50);
  }

  if (lastStateMinus == HIGH && currentMinus == LOW && people > 0) {
    people--;
    delay(50);
  }

  lcd.setCursor(10, 0);

  if(dB<60){
    lcd.print("OK ");
  } else if(dB<85){
    lcd.print("MID ");
  } else{
    lcd.print("LOUD ");
  }

// buzzer

  static bool alarmOn = false;

  if(dB>85){
    alarmOn = true;
  } 
  if(dB<80){
    alarmOn = false;
  }

  if(alarmOn){
    tone(buzzerPin, 3000);
  } else {
    noTone(buzzerPin);  
  }
  Serial.println(dB);

  //tilan päivitys
  lastStatePlus = currentPlus;
  lastStateMinus = currentMinus;

  lcd.setCursor(0, 0);
  lcd.print("dB: ");
  lcd.print(dB);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("People: ");
  lcd.print(people);
  lcd.print("   ");

  delay(200);
}

