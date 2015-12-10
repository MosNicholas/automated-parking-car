
// Constants for bluetooth controls
const char LEFT = 'a';
const char REVERSE = 's';
const char RIGHT = 'd';
const char FORWARD = 'w';
const char PARK = 'p';
const char STOP = 'x';
const char UP_MOTOR = 'u';
const char DOWN_MOTOR = 'n';
const char CALIBRATE = 'c';
const char READ_RAW_SENSOR = 'r';
const char READ_CALIBRATED_SENSOR = 'e';
const char TOGGLE_DEBUG = 't';

// Constants for pin mappings
const int frontSensorPin = P1_3;
const int rearSensorPin = P1_4;
const int sideSensorPin = P1_5;
const int LED = P1_6;
const int rearMotorPin1 = P2_2;
const int rearMotorPin2 = P2_1;
const int frontMotorPin1 = P2_5;
const int frontMotorPin2 = P2_4;

// Constants controlling motion: left/straight/right, reverse/stop/forward
const int GO_LEFT = -1;
const int GO_STRAIGHT = 0;
const int GO_RIGHT = 1;

const int GO_REVERSE = -1;
const int GO_STOP = 0;
const int GO_FORWARD = 1;

// Constants for DEBUGging:
boolean DEBUG = true;
const int INPUT_CHAR_DEBUG = 0;
const int MOTOR_STRENGTH_DEBUG = 1;
const int CALIBRATED_SENSOR_DEBUG = 2;
const int RAW_SENSOR_DEBUG = 3;
const int MIN_MAX_SENSOR_DEBUG = 4;
const int PARKING_IDENTIFIER_DEBUG = 5;

// Values for the
const int MAX_MOTOR_STRENGTH = 250;
int bufSize;
char input;
int motorStrength = 150;
int forwardReverse = 1; // 1 => forward, -1 => reverse
int leftRight = 0; // -1 left, 0 = straight, 1 => right

void debugger(int message, int numArgs, ...) {
  va_list argList;
  va_start(argList, numArgs);
  switch (message) {
    case INPUT_CHAR_DEBUG:
      Serial.print("Input char: ");
      Serial.println(input);
      break;
    case MOTOR_STRENGTH_DEBUG:
      Serial.print("New motor strength: ");
      Serial.println(motorStrength);
      break;
    case CALIBRATED_SENSOR_DEBUG:
      Serial.print("Average value: ");
      Serial.println(getAvgSensorValue(sideSensorPin));
      break;
    case RAW_SENSOR_DEBUG:
      Serial.print("Raw sensor value: ");
      Serial.println(analogRead(sideSensorPin));
      break;
    case MIN_MAX_SENSOR_DEBUG: {
      int sensorMin = va_arg(argList, int);
      int sensorMax = va_arg(argList, int);
      Serial.print("min: ");
      Serial.print(sensorMin);
      Serial.print(", max: ");
      Serial.println(sensorMax);
    } break;
    case PARKING_IDENTIFIER_DEBUG: {
      int state = va_arg(argList, int);
      int currentSensorValue = va_arg(argList, int);
      Serial.print("state: ");
      Serial.print(state);
      Serial.print(", voltage: ");
      Serial.println(currentSensorValue);
    } break;
  } 
}

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
  bufSize = Serial.available();
  
  if (bufSize > 0) {
    input = Serial.read();
    
    if (DEBUG) { debugger(INPUT_CHAR_DEBUG, 0); }
    
    if (input == LEFT) {
      setMovement(motorStrength, -1, forwardReverse);
      leftRight = -1;
    } else if (input == REVERSE) {
      setMovement(motorStrength, leftRight, -1);
      forwardReverse = -1;
    } else if (input == RIGHT) {
      setMovement(motorStrength, 1, forwardReverse);
      leftRight = 1;
    } else if (input == FORWARD) {
      setMovement(motorStrength, leftRight, 1);
      forwardReverse = 1;
    } else if (input == PARK) {
      park(sideSensorPin, frontSensorPin, rearSensorPin);
      forwardReverse = 1;
      leftRight = 0;
    } else if (input == STOP) {
      setMovement(0, 0, 0);
      forwardReverse = 1;
      leftRight = 0;
    } else if (input == UP_MOTOR) {
      motorStrength = min(motorStrength + 50, MAX_MOTOR_STRENGTH);
      if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 0); }
    } else if (input == DOWN_MOTOR) {
      motorStrength = motorStrength - 50;
      if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 0); }
    } else if (input == CALIBRATE) {
      digitalWrite(LED, HIGH);
      calibrateSensor(sideSensorPin);
      digitalWrite(LED, LOW);
    } else if (DEBUG && input == READ_RAW_SENSOR) {
      debugger(RAW_SENSOR_DEBUG, 0);
    } else if (DEBUG && input == READ_CALIBRATED_SENSOR) {
      debugger(CALIBRATED_SENSOR_DEBUG, 0);
    } else if (input == TOGGLE_DEBUG) {
      DEBUG = !DEBUG;
    } else {}
  }
}

void setMovement(int motorStrength, int leftRight, int forwardReverse) {
  move(rearMotorPin1, rearMotorPin2, frontMotorPin1, frontMotorPin2, motorStrength, leftRight, forwardReverse);
}
