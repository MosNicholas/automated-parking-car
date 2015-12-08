
const char LEFT = 'a';
const char REVERSE = 's';
const char RIGHT = 'd';
const char FORWARD = 'w';
const char PARK = 'p';
const char STOP = 'x';
const char UP_MOTOR = 'u';
const char DOWN_MOTOR = 'n';
const char READ_SENSOR = 'r';
const char CALIBRATE = 'c';

const int frontSensorPin = P1_3;
const int rearSensorPin = P1_4;
const int sideSensorPin = P1_5;
const int LED = P1_6;
const int rearMotorPin1 = P2_4;
const int rearMotorPin2 = P2_5;
const int frontMotorPin1 = P2_2;
const int frontMotorPin2 = P2_1;

int bufSize;
char input;
int motorStrength = 150;

void setup() {
  Serial.begin(9600);
  pinMode(frontSensorPin, INPUT);
  pinMode(rearSensorPin, INPUT);
  pinMode(sideSensorPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(rearMotorPin1, OUTPUT);
  pinMode(rearMotorPin2, OUTPUT);
  pinMode(frontMotorPin1, OUTPUT);
  pinMode(frontMotorPin2, OUTPUT);
  
  digitalWrite(LED, LOW);
  digitalWrite(rearMotorPin1, LOW);
  digitalWrite(rearMotorPin2, LOW);
  digitalWrite(frontMotorPin1, LOW);
  digitalWrite(frontMotorPin2, LOW);
}

void loop() {
//  int avgFrontSensorValue = getAvgSensorValue(frontSensorPin);
//  int avgRearSensorValue = getAvgSensorValue(rearSensorPin);
//  
//  Serial.print("Front sensor voltage: ");
//  Serial.println(avgFrontSensorValue);
//  
//  Serial.print("Rear sensor voltage: ");
//  Serial.println(avgRearSensorValue);
  
//  if (avgSensorValue > 100) {
//    move(rearMotorPin1, rearMotorPin2, frontMotorPin1, frontMotorPin2, avgSensorValue, 0, 0);
//  } else {
//    move(rearMotorPin1, rearMotorPin2, frontMotorPin1, frontMotorPin2, 0, 0, 0);
//  }
  
  bufSize = Serial.available();
  
  int forwardReverse = 1; // 1 => forward, -1 => reverse
  
  if (bufSize > 0) {
    input = Serial.read();
    Serial.print("input char: ");
    Serial.println(input);
    if (input == LEFT) {
      setMovement(motorStrength, -1, forwardReverse);
    } else if (input == REVERSE) {
      setMovement(motorStrength, 0, -1);
      forwardReverse = -1;
    } else if (input == RIGHT) {
      setMovement(motorStrength, 1, forwardReverse);
    } else if (input == FORWARD) {
      setMovement(motorStrength, 0, 1);
      forwardReverse = 1;
    } else if (input == PARK) {
      park(sideSensorPin, frontSensorPin);
    } else if (input == STOP) {
      setMovement(0, 0, 0);
    } else if (input == UP_MOTOR) {
      motorStrength = motorStrength + 50;
      Serial.print("new motor strength: ");
      Serial.println(motorStrength);
    } else if (input == DOWN_MOTOR) {
      motorStrength = motorStrength - 50;
      Serial.print("new motor strength: ");
      Serial.println(motorStrength);
    } else if (input == READ_SENSOR) {
      Serial.print("Average value: ");
      Serial.println(getAvgSensorValue(sideSensorPin));
    } else if (input == CALIBRATE) {
      digitalWrite(LED, HIGH);
      calibrateSensor(sideSensorPin);
      digitalWrite(LED, LOW);
    } else {}
  }
}

void setMovement(int motorStrength, int leftRight, int forwardReverse) {
  move(rearMotorPin1, rearMotorPin2, frontMotorPin1, frontMotorPin2, motorStrength, leftRight, forwardReverse);
}
