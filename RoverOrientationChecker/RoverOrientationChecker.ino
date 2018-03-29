#include <CurieIMU.h>

int lastOrientation = -1;

void setup() {
  Serial.begin(9600);

  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);

}

void loop() {
  updateRoverOrientation();
}

static void updateRoverOrientation() {
  int orientation = -1; // Orientation of the board
  String orientationString;

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
      orientationString = "Rover upright";
    } else {
      orientation = 1;
      orientationString = "Rover face down";
    }
  } else if ((absY > absZ) && (absY > absZ)) {
    // Base orientation on Y axis. Horizontal axis
    if (y > 0) {
      orientation = 2;
      orientationString = "Rover on it's left side, right wheels up";
    } else {
      orientation = 3;
      orientationString = "Rover on it's right side, left wheels up";
    }
  } else {
    // Base orientation on X axis. Vertical axis
    if (x < 0) {
      orientation = 4;
      orientationString = "Rover on it's front side, back wheels up";
    } else {
      orientation = 5;
      orientationString = "Rover on it's back side, front wheels up";
    }
  }

  // Update orientation
  if (orientation != lastOrientation) {
//    Serial.print("Rover's Current Position: ");
//    Serial.println(orientationString);
    Serial.println(orientation);
    lastOrientation = orientation;
  }
}

