#include "CurieIMU.h"

void setup() {
  Serial.begin(9600);
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 1500); // 1.5g
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50); // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);
}

void loop() {
  
}

static void eventCallback(void) {
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE)){ 
      Serial.println("Shock detected on Rover's rear");
    }
    if (CurieIMU.shockDetected(X_AXIS, NEGATIVE)) {
      Serial.println("Shock detected on Rover's front");
    }
    if (CurieIMU.shockDetected(Y_AXIS, POSITIVE)){ 
      Serial.println("Shock detected on Rover's right side");
    }
    if (CurieIMU.shockDetected(Y_AXIS, NEGATIVE)) {
      Serial.println("Shock detected on Rover's left side");
    }
    if (CurieIMU.shockDetected(Z_AXIS, POSITIVE)){ 
      Serial.println("Shock detected on Rover's top");
    }
  }
}

