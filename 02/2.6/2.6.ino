#include <TM1637.h>
#include <Keypad.h>
//password
const String correctPass = "9876";
const int FROM_OUTSIDE = 0;
const int FROM_INSIDE = 1;

// movement sensor
int pinMS = 8;

// digitalTube
int pinClk = 10;
int pinDio = 9;
TM1637 screen(pinClk, pinDio);

// keypad
const byte nRows = 4;
const byte nColumns = 4;
byte pRows[nRows] = {2, 3, 4, 5};
byte pColumns[nColumns] = {A0, A1, A2, A3};
char keys[nRows][nColumns] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
Keypad keybd = Keypad(makeKeymap(keys), pRows, pColumns, nRows, nColumns);

// leds
int ledR = 11;
int ledG = 12;
int ledB = 13;
int offColor = 0;
int onColor = 255;

// ultrasounds
long distance;
long responseTime;
int pinTrig = 6;
int pinEcho = 7;

// global vars
int changeTimeBit = 0; // if the user is changing the time of the door
int entryBit = 0;      // if user is entering
int inBit = 0;         // means nobody went inside the building
bool isSomebodyOutside = false;
bool isSomebodyInside = false;
char keyPressed = '\0';
int leaveBit = 0;     // if user is leaving
double mls;           // time passed from the door was opened
String newTime = "";  // new time introduce from the user
int openBit = 0;      // if the door is opened value = 1
String pass = "";     // the password introduced by the user
int people = 0;       // counter of how many went inside
int closeTime = 5000; // by default is 5000

void setup()
{
  Serial.begin(9600);
  // leds
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, LOW);

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
void entry()
{
  if (openBit == 0)
  {
    if (isDigit(keyPressed))
    {
      pass += keyPressed;
    }
    if (pass.length() == 4)
    {
      //       if the pass is correct
      if (pass.equals(correctPass))
      {
        Serial.println("correct pass");
        entryBit = 1;
        openDoor(FROM_OUTSIDE);
      }
      else
      {
        for (int i = 0; i < 3; i++)
        {
          Serial.println("incorrect pass");
          turnOffLed();
          delay(500);
          changeLedColorGreen();
          delay(500);
        }
      }
      pass = "";
    }
  }
  // if the password was correct the door is open
  if (openBit == 1)
  {
    // checks the sensors
    checkUserGetCloseToDoor();
    checkUserGoInside();
    if ((millis() - mls >= closeTime || keybd.getKey() == 'C') && !isSomebodyOutside && !isSomebodyInside && inBit == 1)
    {
      resetValues();
      people++;
      screen.display(0, people);
    }
  }
}

/**
 * This funtion controls the path if the user go from inside to outside
 */
void leave()
{
  // If the inside sensor detects, first door will open
  // and we waits until user go outside
  checkUserGoOutside();
  if (openBit == 1)
  {
    checkUserGetCloseToDoor();
    if (isSomebodyOutside && !isSomebodyInside)
    {
      resetValues();
    }
  }
}

/**
 * Opens the door, sets de openBit to 1 and start the chronometer
 */
void openDoor(int from)
{
  delay(1000);
  //  Serial.println("entro");
  if (from == FROM_OUTSIDE)
    changeLedColorRed();
  if (from == FROM_INSIDE)
    changeLedColorBlue();
  openBit = 1;
  mls = millis();
}

/**
 * If the inside sensor detects something it opens the door and sets de leaveBit to 1
 * so the leave path is chosen
 */
void checkUserGoOutside()
{
  if (digitalRead(pinMS) == HIGH)
  {
    Serial.println("Somebody go out!");
    openDoor(FROM_INSIDE);
    isSomebodyInside = true;
    openBit = 1;
    leaveBit = 1;
  }
  else
  {
    isSomebodyInside = false;
  }
}

/**
 * Checks if somebody is outside the door
 */
void checkUserGetCloseToDoor()
{
  // if there is something close to 10cm the door will not close
  // after 5 seconds the counter will reset to 0
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  responseTime = pulseIn(pinEcho, HIGH);
  distance = int(0.01716 * responseTime);
  if (distance < 10)
  {
    Serial.println("sensor fuera");
    isSomebodyOutside = true;
  }
  else
  {
    isSomebodyOutside = false;
  }
  delay(100);
}

/**
 * Checks if somebody went inside.
 * Sets the inBit to 1
 */
void checkUserGoInside()
{
  if (digitalRead(pinMS) == HIGH)
  {
    Serial.println("sensor dentro");
    isSomebodyInside = true;
    inBit = 1;
  }
  else
  {
    isSomebodyInside = false;
  }
}

/**
 * Close the door after one path
 * Resets the value of the global variables and the leds.
 */
void resetValues()
{
  changeLedColorGreen();
  openBit = 0;
  inBit = 0;
  entryBit = 0;
  leaveBit = 0;
}

/**
 * Allows the user to change the time for the door to automatically close
 */
void detectsAndChangeTime()
{
  // if the user press "*" and bit == 0 we start to change the close time
  if (keyPressed == '*' && changeTimeBit == 0)
  {
    changeTimeBit = 1;
  }
  else if (changeTimeBit == 1 && isDigit(keyPressed))
  {
    newTime += keyPressed;
  }
  // if time is being updated and press "*"
  else if (keyPressed == '*' && changeTimeBit == 1)
  {
    changeTimeBit = 0;
    closeTime = newTime.toInt();
    newTime = "";
    Serial.println(String(closeTime));
  }
}

void changeLedColorGreen()
{
  analogWrite(ledR, offColor);
  analogWrite(ledG, onColor);
  analogWrite(ledB, offColor);
  delay(15);
}

void changeLedColorRed()
{
  analogWrite(ledR, onColor);
  analogWrite(ledG, offColor);
  analogWrite(ledB, offColor);
  delay(15);
}

void changeLedColorBlue()
{
  analogWrite(ledR, offColor);
  analogWrite(ledG, offColor);
  analogWrite(ledB, onColor);
  delay(15);
}

void turnOffLed()
{
  analogWrite(ledR, offColor);
  analogWrite(ledG, offColor);
  analogWrite(ledB, offColor);
  delay(15);
}

void loop()
{
  keyPressed = keybd.getKey();
  detectsAndChangeTime();
  // if time is not being changed
  if (changeTimeBit == 0)
  {
    if (leaveBit == 0)
    {
      entry();
    }
    if (entryBit == 0)
    {
      leave();
    }
  }
}
