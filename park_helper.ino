
const int MIN_DEPTH = 150;
const int MIN_TIME_DELTA = 1000;
const int MAX_PARKING_SEARCH_TIME = 5000;

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
    
    if (state > 0) { currentTime = millis(); }
    
    if (state == 0 && currentSensorValue - previousSensorValue >= MIN_DEPTH) {
      state = 1;
      startTime = millis();
    } else if (state == 1 && currentSensorValue - pPreviousSensorValue >= MIN_DEPTH) {
      state = 2;
    } else if (state == 1 && currentSensorValue - pPreviousSensorValue < MIN_DEPTH) {
      state = 0;
    } else if (state == 2 && currentSensorValue - previousSensorValue <= -MIN_DEPTH) {
      state = 3;
    } else if (state == 3 && currentSensorValue - pPreviousSensorValue > -MIN_DEPTH) {
      state = 2;
    } else if (state == 3 && currentTime - startTime >= MIN_TIME_DELTA) {
      state = 4;
    } else if (state == 3 && currentTime - startTime < MIN_TIME_DELTA) {
      state = 0;
    }
    
    pPreviousSensorValue = previousSensorValue;
    previousSensorValue = currentSensorValue;
    
    delay(30);
  }
  
  return (state == 4);
}

void park(int sideSensorPin, int frontSensorPin) {
  setMovement(130, 0, 1);
  int parkingSpot = findParkingSpot(sideSensorPin, frontSensorPin);
  setMovement(0, 0, 0);
  if (parkingSpot == 1) {
    // we found a parking spot, we need to start parking
//    setMovement(100, -1, -1); // start reversing backwards
    // do some magic
    // park.
  } else {
    // we haven't found a parking spot, keep searching? 
    // Maybe set in a while loop?
  }
}


