
// Constants to control sensor readings & calibration
const int NUM_READINGS = 50;
const int NUM_MILLISECONDS = 5000;

// private/internal variables used to calibrate the sensor
int sensorMin = 1, sensorMax = 25;

/* 
  Public method to calibrate the sensors.
  
  @param sensorPin: the pin for the input sensor
*/
void calibrateSensor(int sensorPin) {
  if (DEBUG) { debugger(CALIBRATING_DEBUG, 0); }
  int _sensorMin = 1023, _sensorMax = 0;
  unsigned long startTime = millis();
  while (millis() - startTime < NUM_MILLISECONDS) {
    int sensorValue = analogRead(sensorPin);
    if (sensorValue > _sensorMax) {
      _sensorMax = sensorValue;
    } else if (sensorValue < _sensorMin) {
      _sensorMin = sensorValue;
    }
  }
  sensorMin = _sensorMin;
  sensorMax = _sensorMax;

  if (DEBUG) { debugger(MIN_MAX_SENSOR_DEBUG, 2, sensorMin, sensorMax); }
}

/*
  This is a public/external method used to get a scaled reading from the sensor. We map the sensor reading
  onto a scale of the calibrated sensor, and map that onto a scale of 0-1023.
  
  @param sensorPin: the pin for the input sensor
  @return int: a value between 0-1023 representing the calibrated sensor value on a larger range.
*/
int scaledSensorValue(int sensorPin) {
  int sensorValue = map(analogRead(sensorPin), sensorMin, sensorMax, 0, 1023);
  return constrain(sensorValue, 0, 1023);
}

/*
  This is a public/external method used to get an average scaled reading from the sensor. We take sensor
  readings, and average the values returned to minimize the effects of noise in the sensor.
  
  @param sensorPin: the pin for the input sensor
  @return int: a value between 0-1023 representing the average calibrated sensor value
*/
int getAvgSensorValue(int sensorPin) {
  unsigned long avgSensorValue = 0;
  for (int i = 1; i <= NUM_READINGS; i++) {
    avgSensorValue = (avgSensorValue * (i-1) + scaledSensorValue(sensorPin))/i;
  }
  return avgSensorValue;
}
