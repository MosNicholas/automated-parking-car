
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

void move(int rear1, int rear2, int front1, int front2, int powerLevel, int leftRight, int forwardReverse) {
  // left = -1, straight = 0, right = 1
  // reverse = -1, stop = 0, forward = 1
  if (leftRight == GO_LEFT) {
    left(front1, front2, powerLevel);
  } else if (leftRight == GO_STRAIGHT) {
    straight(front1, front2);
  } else if (leftRight == GO_RIGHT) {
    right(front1, front2, powerLevel); 
  } else {}
  
  if (forwardReverse == GO_REVERSE) {
    reverse(rear1, rear2, powerLevel);
  } else if (forwardReverse == GO_STOP) {
    halt(rear1, rear2, front1, front2);
  } else if (forwardReverse == GO_FORWARD) {
    forward(rear1, rear2, powerLevel);
  } else {}
}
