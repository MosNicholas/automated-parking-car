
const int NUM_READINGS = 50;
const int NUM_MILLISECONDS = 5000;

int sensorMin, sensorMax;

int calibrateSensor(int sensorPin) {
  // to be called in the setup function, 
  //finds the min and max values of the sensor readings
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
  Serial.print("min: ");
  Serial.println(sensorMin);
  Serial.print("max: ");
  Serial.println(sensorMax);
}

int scaledSensorValue(int sensorPin) {
  int sensorValue = map(analogRead(sensorPin), sensorMin, sensorMax, 0, 1023);
  return constrain(sensorValue, 0, 1023);
}

int getAvgSensorValue(int sensorPin) {
  unsigned long avgSensorValue = 0;
  for (int i = 1; i <= NUM_READINGS; i++) {
    avgSensorValue = (avgSensorValue * (i-1) + scaledSensorValue(sensorPin))/i;
  }
  return avgSensorValue;
}
