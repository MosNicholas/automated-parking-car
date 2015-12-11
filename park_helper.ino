
// Constants for finding parking spot
const int MIN_DEPTH_DELTA = 150;
const int MIN_TIME_DELTA = 1000;
const int MAX_PARKING_SEARCH_TIME = 2500;

// Constants for backing up:
const int HALF_PARKING_SPOT_SENSOR_VALUE = 150;
const int CLOSE_TO_WALL_SENSOR_VALUE = 150;
const int MAX_REVERSING_TIME = 1000;
const int MAX_FORWARD_TIME_1 = 900;

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
  Private/internal method used to determine if there is a parking spot nearby. We stop on one of 3 conditions:
    1. We have found a parking spot (and so state == 4)
    2. We have run out of time to find a parking spot. It may be recalled afterwards
    3. There is an obstable in front of us.

  @param sideSensorPin: pin number for side sensor
  @param frontSensorPin: pin number for front sensor
 
  @return:
    0: fail to find parking spot (cases 2 & 3 above)
    1: parking spot found (case 1)
*/
int findParkingSpot(int sideSensorPin, int frontSensorPin) {
  int state = 0;
  int pPreviousSensorValue = getAvgSensorValue(sideSensorPin);
  int previousSensorValue = getAvgSensorValue(sideSensorPin);
  int frontSensorValue = getAvgSensorValue(frontSensorPin);
  int startTime, currentTime;
  
  setMovement(130, 0, 0, 1);
  unsigned long functionCallTime = millis();
  
  while (state != 4 && millis() - functionCallTime < MAX_PARKING_SEARCH_TIME && frontSensorValue > CLOSE_TO_WALL_SENSOR_VALUE) {
    
    int sideSensorValue = getAvgSensorValue(sideSensorPin);
    frontSensorValue = getAvgSensorValue(frontSensorPin);

    if (DEBUG) { debugger(PARKING_IDENTIFIER_DEBUG, 2, state, sideSensorValue); }
    
    if (state > 0) { currentTime = millis(); }
    
    if (state == 0 && previousSensorValue - sideSensorValue >= MIN_DEPTH_DELTA) {
      state = 1;
      startTime = millis();
    } else if (state == 1 && pPreviousSensorValue - sideSensorValue >= MIN_DEPTH_DELTA) {
      state = 2;
    } else if (state == 1 && pPreviousSensorValue - sideSensorValue < MIN_DEPTH_DELTA) {
      state = 0;
    } else if (state == 2 && sideSensorValue - previousSensorValue >= MIN_DEPTH_DELTA) {
      state = 3;
    } else if (state == 3 && sideSensorValue - pPreviousSensorValue < MIN_DEPTH_DELTA) {
      state = 2;
    } else if (state == 3 && currentTime - startTime >= MIN_TIME_DELTA) {
      state = 4;
    } else if (state == 3 && currentTime - startTime < MIN_TIME_DELTA) {
      state = 0;
    }
    
    pPreviousSensorValue = previousSensorValue;
    previousSensorValue = sideSensorValue;
    
    delay(10);
  }
  
  return (state == 4);
}

void park(int sideSensorPin, int frontSensorPin, int rearSensorPin) {
  stopCar();
  
  int parkingSpotFound = 1; // Assume we stopped the car next to a parking spot.
  if (automateFindingParkingSpot) { parkingSpotFound = findParkingSpot(sideSensorPin, frontSensorPin); }
  
  stopCar();
  
  if (parkingSpotFound != 1) { return; }
  
  // We have a parking spot. Do some magic. Park.
  if (automateFindingParkingSpot) { Serial.print("Parking spot found. "); }
  Serial.println("Beginning to park.");

  unsigned long time = millis();
  int state = 0;
  while (state < 5) {
    int rearSensorReading = getAvgSensorValue(rearSensorPin);
    int frontSensorReading = getAvgSensorValue(frontSensorPin);
    
    if (DEBUG) { debugger(PARKING_ALGO_DEBUG, 3, state, rearSensorReading, frontSensorReading); }
    
    if (state == 0) {
      // First step: reverse back, left
      setMovement(180, MAX_MOTORS_STRENGTH, GO_LEFT, GO_REVERSE);
      state = 1;
    } else if (state == 1 && (rearSensorReading < HALF_PARKING_SPOT_SENSOR_VALUE || millis() - time > MAX_REVERSING_TIME)) {
      // reverse, change direction.
      setMovement(110, MAX_MOTORS_STRENGTH, GO_RIGHT, GO_REVERSE);
      time = millis();
      state = 2;
    } else if (state == 2 && (rearSensorReading < CLOSE_TO_WALL_SENSOR_VALUE || millis() - time > MAX_REVERSING_TIME)) {
      state = 3;
      stopCar();
      setMovement(100, 100, GO_LEFT, GO_FORWARD);
      time = millis();
    } else if (state == 3 && (frontSensorReading < CLOSE_TO_WALL_SENSOR_VALUE || millis() - time > MAX_FORWARD_TIME_1)) {
      setMovement(130, 30, GO_RIGHT, GO_FORWARD);
      state = 4;
    } else if (state == 4 && frontSensorReading < CLOSE_TO_WALL_SENSOR_VALUE) {
      stopCar();
      state = 5;
    }
  }
  Serial.println("The car is parked. Have a good day, I hope you enjoyed the ride.");
}
