#include <CurieIMU.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

int lastOrientation = -1;
String data;
String impact_detected;
String lastCoordinate = "";
float latitude;
float longitude;

TinyGPS gps;
SoftwareSerial softSerial(2,3);

static void updateRoverOrientation();
static void updateRoverLocation(float valid, float invalid, int len, int prec);
static void smartDelay(unsigned long ms);

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  CurieIMU.begin();
  // Orientation Checker
  CurieIMU.setAccelerometerRange(2); // Set Acceleromter Range
  // Impact Detector
  CurieIMU.attachInterrupt(eventCallback);
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 1500); // 1.5g
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50); // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);
}

void loop() {
  // Collect and convert the latitude and longitude
  gps.f_get_position(&latitude, &longitude);
  data = "latitude:";
  updateRoverLocation(latitude, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  data += "longitude:";
  updateRoverLocation(longitude, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  Serial.println(data);
  updateRoverOrientation();

  // Delay for 5 seconds
  smartDelay(5000);
}

// Delay reading of GPS data
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (softSerial.available())
      gps.encode(softSerial.read());
  } while (millis() - start < ms);
}

static void updateRoverOrientation() {
  int orientation = -1; // Orientation of the board

  // Read accelerometer values
  int x = CurieIMU.readAccelerometer(X_AXIS);
  int y = CurieIMU.readAccelerometer(Y_AXIS);
  int z = CurieIMU.readAccelerometer(Z_AXIS);

  // Calculate the absolute values, to determine the largest
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);

  if ((absZ > absX) && (absZ > absY)) {
    // Base orientation on Z axis. Flat axis
    if (z > 0) {
      orientation = 0;
    } else {
      orientation = 1;
    }
  } else if ((absY > absZ) && (absY > absZ)) {
    // Base orientation on Y axis. Horizontal axis
    if (y > 0) {
      orientation = 2;
    } else {
      orientation = 3;
    }
  } else {
    // Base orientation on X axis. Vertical axis
    if (x < 0) {
      orientation = 4;
    } else {
      orientation = 5;
    }
  }

  // Update orientation
  if (orientation != lastOrientation) {
    lastOrientation = orientation;
    data += "orientation:" + String(lastOrientation) + ",";
    Serial.println(data);
  }
}

static void updateRoverLocation(float valid, float invalid, int len, int prec) {
  if (valid == invalid) {
  } else {
    String coordinate = String(valid, prec);
    if (coordinate != lastCoordinate){
      lastCoordinate = coordinate;
      data += coordinate + ",";
    }
  }
}

// Impact detector function
static void eventCallback(void) {
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE)){
      impact_detected = "back"; 
    }
    if (CurieIMU.shockDetected(X_AXIS, NEGATIVE)) {
      impact_detected = "front";
    }
    if (CurieIMU.shockDetected(Y_AXIS, POSITIVE)){
      impact_detected = "right";
    }
    if (CurieIMU.shockDetected(Y_AXIS, NEGATIVE)) {
      impact_detected = "left";
    }
    if (CurieIMU.shockDetected(Z_AXIS, POSITIVE)){ 
      impact_detected = "top";
    }
    data += "impact:" + String(impact_detected);
    Serial.println(data);
  }
}
