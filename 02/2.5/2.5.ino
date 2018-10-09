#include <TM1637.h>
#include <Keypad.h> 
//password
const String correctPass = "9876";

// movement sensor
int pinMS = 9;

// digitalTube
int pinClk = 10;
int pinDio = 11;
TM1637 screen(pinClk,pinDio); 

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
int entryBit = 0; // if user is entering 
int inBit = 0; // means nobody went inside the building
bool isSomebodyOutside = false;
bool isSomebodyInside = false;
char keyPressed = '\0';
int leaveBit = 0; // if user is leaving
double mls; // time passed from the door was opened
int openBit = 0; // if the door is opened value = 1
String pass = ""; // the password introduced by the user
int people = 0; // counter of how many went inside

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

  // movementSensor
  pinMode(pinMS, INPUT);

  // digitalTube
  init();
  screen.init();
  screen.set(BRIGHT_TYPICAL);
  delay(1500);
}

/**
 * This function checks if someone tries to go inside. First checks if the password introduced is correct.
 * After that, if is correct sets the entryBit to 1 and open the door calling the openDoor method (that sets the openBit to 1).
 * If openBit is 1 it starts to check if someone is inside and outside. If the user went inside and leave the sensors the door close.
 * If he went inside but the outside sensor detects something it not close the door.
 */
void entry() {
  keyPressed = keybd.getKey(); 
  Serial.println("entry");
  if (openBit == 0) {
    if (isDigit(keyPressed)) {
      pass += keyPressed;
    }
    if (pass.length() == 4) {
      // if the pass is correct
      if (pass.equals(correctPass)) {
        Serial.println("correct pass");
        entryBit = 1;
        openDoor();      
      } else {
        for (int i = 0; i < 3; i++) {
          Serial.println("incorrect pass");
          digitalWrite(ledG, LOW);
          delay(500);
          digitalWrite(ledG, HIGH);
          delay(500);
        }
      }
      pass = "";
    }  
  }
  if (openBit == 1) {
    checkUserGetCloseToDoor();
    checkUserGoInside();
  
    if((millis() - mls >= 5000 || keybd.getKey() == 'C') && !isSomebodyOutside && !isSomebodyInside && inBit == 1) {
        resetValues();      
        people++;
        screen.display(0, people);
    }
  }
}

void leave() {
  checkUserGoOutside();
  if (openBit == 1) {
    checkUserGetCloseToDoor();
    if (isSomebodyOutside && !isSomebodyInside) {
      resetValues();
    }
  }
}

/**
 * Opens the door, sets de openBit to 1 and start the chronometer
 */
void openDoor() {
  digitalWrite(ledG, LOW);
  delay(1000);
  Serial.println("entro");
  digitalWrite(ledR, HIGH);
  openBit = 1;
  mls = millis();
}

/**
 * If the inside sensor detects something it opens the door and sets de leaveBit to 1
 * so the leave path is chosen
 */
void checkUserGoOutside() {
  if (digitalRead(pinMS) == HIGH) {
//    Serial.println("Somebody go out!");
    openDoor();
    isSomebodyInside = true;
    openBit = 1;
    leaveBit = 1;
  } else {
    isSomebodyInside = false;
  }
}

/**
 * Checks if somebody is outside the door
 */
void checkUserGetCloseToDoor() {
  // if there is something close to 10cm the door will not close
    // after 5 seconds the counter will reset to 0      
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(5);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    responseTime = pulseIn(pinEcho, HIGH);
    distance = int(0.01716*responseTime);
//    Serial.println("distance: " + String(distance));
    if (distance < 10) {
      Serial.println("Distance");
      isSomebodyOutside = true;
    } else {
//      Serial.println("no hay nadie");
      isSomebodyOutside = false;
    }  
    delay(100);
}

/**
 * Checks if somebody went inside.
 * Sets the inBit to 1
 */
void checkUserGoInside() {
  if (digitalRead(pinMS) == HIGH) {
    Serial.println("Movement");
    isSomebodyInside = true;
    inBit = 1;
  } else {
    isSomebodyInside = false;
  }
}

/**
 * Close the door after one path
 * Resets the value of the global variables and the leds.
 */
void resetValues() {
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, HIGH);
  openBit = 0;
  inBit = 0;
  entryBit = 0;
  leaveBit = 0;
}

void loop() {
  if (leaveBit == 0) {
    entry();
  }
  if (entryBit == 0) {
    leave();
  }
}
