#include <Keypad.h> 
const String correctPass = "9876";
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

int ledR = 6;
int ledG = 13;

char keyPressed = '\0';
int openBit = 0;
double mls;
String pass = "";

void setup() {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  digitalWrite(ledR, LOW);
  pinMode(ledG, OUTPUT);
  digitalWrite(ledG, HIGH);
}

void loop() {
  keyPressed = keybd.getKey(); 
  if (isDigit(keyPressed)) {
    pass += keyPressed;
  }
  if (pass.length() == 4) {
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
    if(millis() - mls >= 5000 || keybd.getKey() == 'C') {
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      openBit = 0;
    }
  }
}
