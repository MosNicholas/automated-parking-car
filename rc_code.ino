
// Constants for bluetooth controls
const char LEFT = 'a';
const char STRAIGHT = 'q';
const char RIGHT = 'd';
const char REVERSE = 's';
const char FORWARD = 'w';
const char STOP = 'x';
const char PARK = 'p';
const char GO_FASTER = 'u';
const char GO_SLOWER = 'n';
const char TURN_MORE = 'i';
const char TURN_LESS = 'm';
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

// Constants for debugging:
boolean DEBUG = true;
const int INPUT_CHAR_DEBUG = 0;
const int MOTOR_STRENGTH_DEBUG = 1;
const int CALIBRATED_SENSOR_DEBUG = 2;
const int RAW_SENSOR_DEBUG = 3;
const int MIN_MAX_SENSOR_DEBUG = 4;
const int PARKING_IDENTIFIER_DEBUG = 5;
const int PARKING_ALGO_DEBUG = 6;

// Values for the
const int MAX_MOTORS_STRENGTH = 250;
int bufSize;
char input;
int stearingPowerLevel = 150; // power applied to the front motor
int speedPowerLevel = 150; // power applied to the rear motor
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
    case MOTOR_STRENGTH_DEBUG: {
      int motorStrength = va_arg(argList, int);
      Serial.print("New motor strength: ");
      Serial.println(motorStrength);
    } break;
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
    case PARKING_ALGO_DEBUG: {
      int state = va_arg(argList, int);
      int rearSensorValue = va_arg(argList, int);
      int frontSensorValue = va_arg(argList, int);
      Serial.print("state: ");
      Serial.print(state);
      Serial.print(", rear voltage: ");
      Serial.print(rearSensorValue);
      Serial.print(", front voltage: ");
      Serial.println(frontSensorValue);
    }
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
    
    switch (input) {
      case LEFT:
        setMovement(GO_LEFT, forwardReverse);
        leftRight = GO_LEFT;
        break;
      case STRAIGHT:
        setMovement(GO_STRAIGHT, forwardReverse);
        leftRight = GO_STRAIGHT;
        break;
      case RIGHT:
        setMovement(GO_RIGHT, forwardReverse);
        leftRight = GO_RIGHT;
        break;
      case REVERSE:
        setMovement(leftRight, GO_REVERSE);
        forwardReverse = GO_REVERSE;
        break;
      case FORWARD:
        setMovement(leftRight, GO_FORWARD);
        forwardReverse = GO_FORWARD;
        break;
      case PARK:
        park(sideSensorPin, frontSensorPin, rearSensorPin);
      case STOP:
        stopCar();
        forwardReverse = GO_FORWARD;
        leftRight = GO_STRAIGHT;
        break;
      case GO_FASTER:
        speedPowerLevel = min(speedPowerLevel + 50, MAX_MOTORS_STRENGTH);
        if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 1, speedPowerLevel); }
        break;
      case GO_SLOWER:
        speedPowerLevel = speedPowerLevel - 50;
        if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 1, speedPowerLevel); }
        break;
      case TURN_MORE:
        stearingPowerLevel = min(stearingPowerLevel + 50, MAX_MOTORS_STRENGTH);
        if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 1, stearingPowerLevel); }
        break;
      case TURN_LESS:
        stearingPowerLevel = stearingPowerLevel - 50;
        if (DEBUG) { debugger(MOTOR_STRENGTH_DEBUG, 1, stearingPowerLevel); }
        break;
      case CALIBRATE:
        digitalWrite(LED, HIGH);
        calibrateSensor(sideSensorPin);
        digitalWrite(LED, LOW);
        break;
      case READ_RAW_SENSOR:
        if (DEBUG) { debugger(RAW_SENSOR_DEBUG, 0); }
        break;
      case READ_CALIBRATED_SENSOR:
        if (DEBUG) { debugger(CALIBRATED_SENSOR_DEBUG, 0); }
        break;
      case TOGGLE_DEBUG:
        DEBUG = !DEBUG;
        break;
    }
  }
}
