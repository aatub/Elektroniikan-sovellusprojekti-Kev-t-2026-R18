#ifndef PEOPLE_COUNTER_H
#define PEOPLE_COUNTER_H

#include <Arduino.h>

// Pins
const int TRIG_PIN_RIGHT = 4;
const int ECHO_PIN_RIGHT = 5;
const int TRIG_PIN_LEFT = 12;
const int ECHO_PIN_LEFT = 14;

// Constants
const int BASELINE_SAMPLES = 10;
const float SOUND_SPEED_CM_US = 0.034;
const float DETECTION_TOLERANCE = 5.0;  // cm
const unsigned long TIMEOUT_US = 26000;
const int SENSOR_READ_DELAY_MS = 100;
const int BASELINE_UPDATE_DELAY_MS = 30000;  // 30 seconds
const int DISPLAY_UPDATE_DELAY_MS = 1000;

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
