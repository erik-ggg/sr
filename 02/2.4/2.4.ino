#include <Keypad.h> 
//password
const String correctPass = "9876";

// keypad
const byte nRows = 4;
const byte nColumns = 4;
byte pRows[nRows] = {2, 3, 4, 5};
byte pColumns[nColumns] = {A0, A1, A2, A3};
char keys[nRows][nColumns] = {  
  {'1','2','3','A'},  
  {'4','5','6','B'},  
  {'7','8','9','C'},  
  {'*','0','#','D'} 
}; 
Keypad keybd = Keypad(makeKeymap(keys), pRows, pColumns, nRows, nColumns); 

// leds
int ledR = 6;
int ledG = 13;

// ultrasounds
long distance;
long responseTime;
int pinTrig = 8;
int pinEcho = 7;

// global vars
char keyPressed = '\0';
int openBit = 0;
double mls;
String pass = "";
bool isSomebody = false;

void setup() {
  Serial.begin(9600);
  // leds
  pinMode(ledR, OUTPUT);
  digitalWrite(ledR, LOW);
  pinMode(ledG, OUTPUT);
  digitalWrite(ledG, HIGH);

  // ultrasounds
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
}

void loop() {
  keyPressed = keybd.getKey(); 
  if (isDigit(keyPressed)) {
    pass += keyPressed;
  }
  if (pass.length() == 4) {
    // if the pass is correct
    if (pass.equals(correctPass)) {
      digitalWrite(ledG, LOW);
      delay(1000);
      digitalWrite(ledR, HIGH);
      openBit = 1;
      mls = millis();
    } else {
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledG, LOW);
        delay(500);
        digitalWrite(ledG, HIGH);
        delay(500);
      }
    }
    pass = "";
  }  
  if (openBit == 1) {
  // if there is something close to 10cm the door will not close
  // after 5 seconds the counter will reset to 0      
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  responseTime = pulseIn(pinEcho, HIGH);
  distance = int(0.01716*responseTime);
  Serial.println("distance: " + String(distance));
  if (distance < 10) {
    Serial.println("hay alguien");
    isSomebody = true;
    mls = millis();
  } else {
    Serial.println("no hay nadie");
    isSomebody = false;
  }  
  delay(100);
    if((millis() - mls >= 5000 || keybd.getKey() == 'C') && !isSomebody) {
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      openBit = 0;
    }
  }
}
