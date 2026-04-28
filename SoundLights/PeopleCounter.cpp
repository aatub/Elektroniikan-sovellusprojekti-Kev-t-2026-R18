#include "PeopleCounter.h"

// ===== Global Variables =====
SensorData sensorData = {0};
SemaphoreHandle_t dataMutex = NULL;
int globalPeople = 5;

// ===== RTOS Task Functions =====
void taskReadSensors(void *parameter) {
  while (true) {
    // Read Right
    float distRight = readUltrasonicDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
    vTaskDelay(pdMS_TO_TICKS(30)); // 30ms "Silence" period for echoes to dissipate
    
    // Read Left
    float distLeft = readUltrasonicDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
    vTaskDelay(pdMS_TO_TICKS(30)); 

    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      sensorData.distanceRight = distRight;
      sensorData.distanceLeft = distLeft;
      xSemaphoreGive(dataMutex);
    }
    
    vTaskDelay(pdMS_TO_TICKS(20)); // Overall task frequency
  }
}

void taskDetectPeople(void *parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  
  while (true) {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      detectMovement();
      xSemaphoreGive(dataMutex);
    }
    
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_READ_DELAY_MS));
  }
}

void taskUpdateBaseline(void *parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  
  while (true) {
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(BASELINE_UPDATE_DELAY_MS));
    
    Serial.println("Updating baseline...");
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      calculateBaseline();
      xSemaphoreGive(dataMutex);
    }
  }
}

void taskDisplayStatus(void *parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  
  while (true) {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      Serial.printf("People Count: %u | R: %.2f cm | L: %.2f cm\n",
                    sensorData.peopleCount,
                    sensorData.distanceRight,
                    sensorData.distanceLeft);
      globalPeople = sensorData.peopleCount;
      xSemaphoreGive(dataMutex);
    }
    
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_DELAY_MS));
  }
}

// ===== Helper Functions =====
float readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  unsigned long duration = pulseIn(echoPin, HIGH, TIMEOUT_US);
  return (duration * SOUND_SPEED_CM_US) / 2.0;
}

void calculateBaseline() {
  float sumRight = 0;
  float sumLeft = 0;
  
  Serial.println("Calculating baseline...");
  
  for (int i = 0; i < BASELINE_SAMPLES; i++) {
    float distRight = readUltrasonicDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
    float distLeft = readUltrasonicDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
    
    sumRight += distRight;
    sumLeft += distLeft;
    
    Serial.printf("Sample %d | R: %.2f | L: %.2f\n", i, distRight, distLeft);
    delay(50);
  }
  
  sensorData.baselineRight = sumRight / BASELINE_SAMPLES;
  sensorData.baselineLeft = sumLeft / BASELINE_SAMPLES;
  sensorData.prevRight = sensorData.baselineRight;
  sensorData.prevLeft = sensorData.baselineLeft;
  
  Serial.printf("Baseline set | R: %.2f | L: %.2f\n",
                sensorData.baselineRight,
                sensorData.baselineLeft);
}

void detectMovement() {
  bool rightBlocked = (sensorData.distanceRight < (sensorData.baselineRight - DETECTION_TOLERANCE));
  bool leftBlocked = (sensorData.distanceLeft < (sensorData.baselineLeft - DETECTION_TOLERANCE));

  static unsigned long lastTriggerTime = 0; 
  unsigned long currentTime = millis();

  if (currentTime - lastTriggerTime < DEBOUCE_DELAY_MS) { 
    return;
  }

  if (rightBlocked) {
    sensorData.peopleCount++;
    lastTriggerTime = currentTime;
    Serial.println("ENTRY");
  }
  else if (leftBlocked) { 
    if (sensorData.peopleCount > 0) {
      sensorData.peopleCount--;
    }
    lastTriggerTime = currentTime;
    Serial.println("EXIT");
  }
}

void initializePeopleCounter() {
  // Configure pins
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  
  // Create mutex
  dataMutex = xSemaphoreCreateMutex();
  
  if (dataMutex == NULL) {
    Serial.println("ERROR:  mutex!");
    return;
  }
  
  // Baseline calculation
  calculateBaseline();
  
  // RTOS tasks
  BaseType_t taskResult;
  
  taskResult = xTaskCreate(taskReadSensors, "ReadSensors", 8182, NULL, 2, NULL);
  if (taskResult != pdPASS) {
    Serial.println("ERROR: ReadSensors");
    return;
  }
  
  taskResult = xTaskCreate(taskDetectPeople, "DetectPeople", 4096, NULL, 2, NULL);
  if (taskResult != pdPASS) {
    Serial.println("ERROR: DetectPeople");
    return;
  }
  
  taskResult = xTaskCreate(taskUpdateBaseline, "UpdateBaseline", 8192, NULL, 1, NULL);
  if (taskResult != pdPASS) {
    Serial.println("ERROR: UpdateBaseline");
    return;
  }
  
  taskResult = xTaskCreate(taskDisplayStatus, "DisplayStatus", 4096, NULL, 1, NULL);
  if (taskResult != pdPASS) {
    Serial.println("ERROR: DisplayStatus");
    return;
  }
  
  Serial.println("RTOS tasks created successfully. System running...");
}
