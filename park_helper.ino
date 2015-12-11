
// Constants for finding parking spot
const int MIN_DEPTH_DELTA = 150;
const int MIN_TIME_DELTA = 1000;
const int MAX_PARKING_SEARCH_TIME = 2000;

// Constants for backing up:
const int HALF_PARKING_SPOT_SENSOR_VALUE = MIN_DEPTH_DELTA;
const int CLOSE_TO_WALL_SENSOR_VALUE = 90;
const int MAX_REVERSING_TIME = MIN_TIME_DELTA/2;

/*
 Since we are mapping sensor values to 0-1023, we can use an absolute value
 as the minimum depth delta required to identify a parking spot.
 
 We will have 5 states:
   0 for parking not found
   1 for potential parking spot start
   2 for being in a potential parking spot
   3 for potential parking spot end
   4 for end of parking spot
   
 We have 3 cases:
   1. We started parking in a parking spot, so we don't have relative values
   2. Once we started parking, we don't have data about previous wall 
      distances/voltages
      
 Problems:
   How do we know the distance is great enough? 
     --> we control motor strength, so we can just look at time delta
*/

/*
 @arg sideSensorPin, type: int, pin number for side sensor
 @arg frontSensorPin, type: int, pin number for front sensor
 
 @return, type: int
   0: fail to find parking spot
   1: parking spot found
*/
int findParkingSpot(int sideSensorPin, int frontSensorPin) {
  int state = 0;
  int pPreviousSensorValue = getAvgSensorValue(sideSensorPin);
  int previousSensorValue = getAvgSensorValue(sideSensorPin);
  int startTime, currentTime;
  unsigned long functionCallTime = millis();
  
  while (state != 4 && millis() - functionCallTime < MAX_PARKING_SEARCH_TIME) {
    
    int currentSensorValue = getAvgSensorValue(sideSensorPin);

    if (DEBUG) { debugger(PARKING_IDENTIFIER_DEBUG, 2, state, currentSensorValue); }
    
    if (state > 0) { currentTime = millis(); }
    
    if (state == 0 && previousSensorValue - currentSensorValue >= MIN_DEPTH_DELTA) {
      state = 1;
      startTime = millis();
    } else if (state == 1 && pPreviousSensorValue - currentSensorValue >= MIN_DEPTH_DELTA) {
      state = 2;
    } else if (state == 1 && pPreviousSensorValue - currentSensorValue < MIN_DEPTH_DELTA) {
      state = 0;
    } else if (state == 2 && currentSensorValue - previousSensorValue >= MIN_DEPTH_DELTA) {
      state = 3;
    } else if (state == 3 && currentSensorValue - pPreviousSensorValue < MIN_DEPTH_DELTA) {
      state = 2;
    } else if (state == 3 && currentTime - startTime >= MIN_TIME_DELTA) {
      state = 4;
    } else if (state == 3 && currentTime - startTime < MIN_TIME_DELTA) {
      state = 0;
    }
    
    pPreviousSensorValue = previousSensorValue;
    previousSensorValue = currentSensorValue;
    
    delay(10);
  }
  
  return (state == 4);
}

void park(int sideSensorPin, int frontSensorPin, int rearSensorPin) {
  setMovement(0, 130, 0, 1);
  int parkingSpot = findParkingSpot(sideSensorPin, frontSensorPin);
  stopCar();
  if (parkingSpot == 1) { // Do some magic. Park.
    // we found a parking spot, we need to start parking
    // If we haven't found a parking spot in the allotted time, just stop and don't do anything.
    unsigned long time = millis();
    int state = 0;
    while (state != 4) {
      int rearSensorReading = getAvgSensorValue(rearSensorPin);
      int frontSensorReading = getAvgSensorValue(frontSensorPin);
      
      if (DEBUG) { debugger(PARKING_ALGO_DEBUG, 3, state, rearSensorReading, frontSensorReading); }
      
      if (state == 0) {
        // First step: reverse back, left
        setMovement(MAX_MOTORS_STRENGTH, MAX_MOTORS_STRENGTH, GO_LEFT, GO_REVERSE);
        state = 1;
      } else if (state == 1 && (rearSensorReading < HALF_PARKING_SPOT_SENSOR_VALUE || millis() - time > MAX_REVERSING_TIME)) {
         // reverse, change direction.
        setMovement(MAX_MOTORS_STRENGTH, MAX_MOTORS_STRENGTH, GO_RIGHT, GO_REVERSE);
        time = millis();
        state = 2;
      } else if (state == 2 && (rearSensorReading < CLOSE_TO_WALL_SENSOR_VALUE || millis() - time > MAX_REVERSING_TIME)) {
        state = 3;
      } else if (state == 3) {
        stopCar();
      }
    }
  }
}
