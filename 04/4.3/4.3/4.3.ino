#include <Servo.h> 

#define button_pin 11
#define X_pin A0
#define Y_pin A1

int modeBit = 0; // 0 auto, 1 manual

Servo servo;
boolean isStarted = true;

void setup() {
  Serial.begin(9600);
  servo.attach(8);
  // joystick
  pinMode(button_pin, INPUT_PULLUP); 
  // collision sensors
  pinMode(4, INPUT);   
  pinMode(7, INPUT);   
 }

void loop() {  
//  calibrate();
  start();
  if (modeBit == 0) moveServo(digitalRead(4), digitalRead(7));
  else if (modeBit == 1) detectJoystickYMove();
  changeMode();
} 

/**
 * Detects the Y value from the Joystick and moves the servo.
 * If the yValue stops the servo receive the stop signal.
 */
void detectJoystickYMove() {  
  int yValue = analogRead(Y_pin);
  if (yValue >= 800) {
    servo.write(0);
  }
  else if (yValue <= 100) {
    servo.write(180);
  }
  else {
    servo.write(90);
  }
}

/**
 * If joystick button is clicked the mode will change to the other one
 */
void changeMode() {
  if (digitalRead(button_pin) == 0) {
    if (modeBit == 0) modeBit = 1;
    else if (modeBit == 1) {
      modeBit = 0;      
      servo.write(0);
    }
  }
  delay(100);
}

/**
 * Starts the servo.
 */
void start() {
  if (isStarted == true) {
    isStarted = false;
    servo.write(0);
  }
}

/**
 * Moves the servo. The direction will be specified by the collision sensors.
 */
void moveServo(int moveLeft, int moveRight) {
  if (moveLeft == 0) {
    servo.write(180);
  }
  else if (moveRight == 0) {
    servo.write(0);
  }
}

/**
 * Sends repeteatly the stop signal for calibrate purpouse
 */
void calibrate() {
  servo.write(90); 
}
