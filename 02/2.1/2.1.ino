#include <Keypad.h> 

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
int ledG = 7;

int openBit = 0;
double mls;

void setup() {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  digitalWrite(ledR, LOW);
  pinMode(ledG, OUTPUT);
  digitalWrite(ledG, HIGH);
}

void loop() {
  if (openBit == 0) {
    if ( keybd.getKey() == 'A' ) {
      mls = millis();
      openBit = 1;
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, LOW);
    }
  }
  if (openBit == 1) {
    if(millis() - mls >= 5000 || keybd.getKey() == 'C') {
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      openBit = 0;
    }
  }
}
