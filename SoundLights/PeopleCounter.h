#ifndef PEOPLE_COUNTER_H
#define PEOPLE_COUNTER_H

#include <Arduino.h>

// Pins
const int TRIG_PIN_RIGHT = 14;
const int ECHO_PIN_RIGHT = 27;
const int TRIG_PIN_LEFT = 26;
const int ECHO_PIN_LEFT = 25;

// Constants
const int BASELINE_SAMPLES = 10;
const float SOUND_SPEED_CM_US = 0.034;
const float DETECTION_TOLERANCE = 20.0;  // cm
const unsigned long TIMEOUT_US = 26000;
const int SENSOR_READ_DELAY_MS = 10;
const int BASELINE_UPDATE_DELAY_MS = 30000;  // 30 seconds
const int DISPLAY_UPDATE_DELAY_MS = 1000;
const int DEBOUCE_DELAY_MS = 2000;

// Scruts
struct SensorData {
  float distanceRight;
  float distanceLeft;
  float baselineRight;
  float baselineLeft;
  float prevRight;
  float prevLeft;
  unsigned int peopleCount;
};

// Global
extern SensorData sensorData;
extern SemaphoreHandle_t dataMutex;
extern int globalPeople;

// Sensor reading
float readUltrasonicDistance(int trigPin, int echoPin);

// Baseline calculation
void calculateBaseline();

// Movement detection
void detectMovement();

// RTOS
void taskReadSensors(void *parameter);
void taskDetectPeople(void *parameter);
void taskUpdateBaseline(void *parameter);
void taskDisplayStatus(void *parameter);

// Initialization
void initializePeopleCounter();

#endif // PEOPLE_COUNTER_H
