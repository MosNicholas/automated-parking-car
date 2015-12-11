
// Internal helper functions to control the motor
void forward(int rear1, int rear2, int powerLevel) {
  digitalWrite(rear1, LOW);
  analogWrite(rear2, powerLevel);
}

void reverse(int rear1, int rear2, int powerLevel) {
  analogWrite(rear1, powerLevel);
  digitalWrite(rear2, LOW);
}

void halt(int rear1, int rear2, int front1, int front2) {
  digitalWrite(rear1, LOW);
  digitalWrite(rear2, LOW);
  digitalWrite(front1, LOW);
  digitalWrite(front2, LOW);
}

void left(int front1, int front2, int powerLevel) {
  analogWrite(front1, powerLevel);
  digitalWrite(front2, LOW);
}

void right(int front1, int front2, int powerLevel) {
  digitalWrite(front1, LOW);
  analogWrite(front2, powerLevel);
}

void straight(int front1, int front2) {
  analogWrite(front1, 0);
  analogWrite(front2, 0);
}

void move(int rear1, int rear2, int front1, int front2, int stearingPowerLevel, int speedPowerLevel, int leftRight, int forwardReverse) {
  switch (leftRight) {
    case GO_LEFT:
      left(front1, front2, stearingPowerLevel);
      break;
    case GO_STRAIGHT:
      straight(front1, front2);
      break;
    case GO_RIGHT:
      right(front1, front2, stearingPowerLevel);
      break;
  }
  
  switch (forwardReverse) {
    case GO_REVERSE:
      reverse(rear1, rear2, speedPowerLevel);
      break;
    case GO_STOP:
      halt(rear1, rear2, front1, front2);
      break;
    case GO_FORWARD:
      forward(rear1, rear2, speedPowerLevel);
      break;
  }
}

///////////////////////// EXTERNAL/PUBLIC METHODS //////////////////////////////////

// This function just switches off the car motors.
void stopCar() {
  setMovement(0, 0);
}

/*
  This is the external/public method used to move the car.
  
  @param leftRight: determines whether the car will go left, right, or straight
  @param forwardReverse: determines whether the car will go forward, reverse, or stop
  
  NOTE:
  
  leftRight is mapped as follows:
    GO_LEFT => left
    GO_STRAIGHT => straight
    GO_RIGHT => right
  
  forwardReverse is mapped as follows:
    GO_FORWARD => forward
    GO_STOP => stop
    GO_REVERSE => reverse
    
  The values GO_* are defined as global variables.
*/
void setMovement(int leftRight, int forwardReverse) {
  setMovement(speedPowerLevel, stearingPowerLevel, leftRight, forwardReverse);
}

/*
  This is the external/public method used to move the car. It differs from the above in that we will call
  this method when we want to control the power applied to each motor as well.
  
  @param speedPowerLevel: amount of power applied to the rear motor, proportional to the speed of the car
  @param stearingPowerLevel: the amount of power applied to the front motor, proportional to the degree
                              of stearing
  @param leftRight: determines whether the car will go left, right, or straight
  @param forwardReverse: determines whether the car will go forward, reverse, or stop
*/
void setMovement(int speedPowerLevel, int stearingPowerLevel, int leftRight, int forwardReverse) {
  move(rearMotorPin1, rearMotorPin2, frontMotorPin1, frontMotorPin2, stearingPowerLevel, speedPowerLevel, leftRight, forwardReverse);
}
