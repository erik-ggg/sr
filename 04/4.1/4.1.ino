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
  if (yValue >= 800) {
    servo.write(0);
    Serial.println("arriba");
  }
  else if (yValue <= 100) {
    servo.write(180);
    Serial.println("abajo");
  }
  else {
    servo.write(90);
  }
}

/**
 * Sends repeteatly the stop signal for calibrate purpouse
 */
void calibrate() {
  servo.write(90); 
}
