#include <Servo.h> 

#define button_pin 9
#define X_pin A0
#define Y_pin A1

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(button_pin, INPUT_PULLUP); 
  servo.attach(8);
 }

void loop() {  
//  calibrate();
  detectJoystickYMove();
 
}

/**
 * Detects the Y value from the Joystick and moves the servo.
 * If the yValue stops the servo receive the stop signal.
 */
void detectJoystickYMove() {  
  int yValue = analogRead(Y_pin);
  Serial.println(String(yValue));
  moveServo(yValue);
}

/**
 * Moves the servo. The speed will be establish depending on
 * the Y value that the joystick sends.
 */
void moveServo(int yValue) {  
  if (yValue > 923) {
    servo.write(0);
  }
  else if (yValue > 612 && yValue < 923) {
    servo.write(85); // 10, 50, 70, 85
  }
  else if (yValue > 490 && yValue < 612) {
    servo.write(85); // 20, 85
  }
  // stop signal  
  else if (yValue >= 480 && yValue <= 490) {
    servo.write(90);
  }
  else if (yValue > 412 && yValue < 480) {
    servo.write(95); // 95
  }
  else if (yValue > 100 && yValue < 412) {
    servo.write(95); // 170, 100, 95
  }
  else if (yValue < 100) {
    servo.write(180);
  }
}

/**
 * Sends repeteatly the stop signal for calibrate purpouse
 */
void calibrate() {
  servo.write(90); 
}
