
const int NUM_READINGS = 100;
const int NUM_MILLISECONDS = 5000;

int sensorMin = 1023, sensorMax = 0;

int defineSensorMinMax(int sensorPin) {
  // to be called in the setup function, 
  //finds the min and max values of the sensor readings
  int startTime = millis();
  while (millis() - startTime < NUM_MILLISECONDS) {
    int sensorValue = analogRead(sensorPin);
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    } else if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
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
  int avgSensorValue = 0;
  for (int i = 1; i <= NUM_READINGS; i++) {
    avgSensorValue = (avgSensorValue * (i-1) + scaledSensorValue(sensorPin))/i;
  }
  return avgSensorValue;
}
