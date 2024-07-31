#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSerial(2, 3);
Servo H_SERVO;
Servo V_SERVO;

#define Flame A0
#define DigitalFlame 7
#define JoystickX A3
#define JoystickY A4
#define JoystickSwitch A2
#define Buz 6
#define Led 4

char PreviousMode = ' ';
char CurrentMode = 'M';
char FireButton;
bool On = false;
int FireRate;
int angelX;
int angelY;

void CheckFlame();
void AutomaticMotion();
void JoystickManualMotion();
void ManualMotion();
void CheckMode();

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(DigitalFlame, INPUT);
  pinMode(JoystickSwitch, INPUT);
  digitalWrite(JoystickSwitch, HIGH);
  H_SERVO.attach(9);
  H_SERVO.write(90);
  delay(400);
  V_SERVO.attach(10);
  V_SERVO.write(40);
  delay(400);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  CheckMode();
}

void CheckMode() {
  if (BTSerial.available()) {
    CurrentMode = BTSerial.read();
  }
  
  if (CurrentMode != PreviousMode) {
    On = false;  // Stop the current mode
    delay(100);  // Short delay to ensure the current mode stops
    Serial.flush();
    BTSerial.flush();

    PreviousMode = CurrentMode;  // Update previous mode

    switch (CurrentMode) {
      case 'A':
        On = true;
        AutomaticMotion();
        break;
      case 'M':
        On = true;
        ManualMotion();
        break;
      case 'J':
        On = true;
        JoystickManualMotion();
        break;
    }
  }
  delay(50);
}

void CheckFlame() {
  // Serial.println(map(analogRead(Flame), 0, 1023, 255, 0));
  if (analogRead(Flame) < 450) {
    digitalWrite(Led, HIGH);
    while (true) {
      FireRate = map(analogRead(Flame), 0, 1023, 255, 0);
      analogWrite(Buz, FireRate);
      // BTSerial.println(FireRate);
      if (BTSerial.available())
        FireButton = BTSerial.read();
      if ((digitalRead(JoystickSwitch) == LOW && CurrentMode == 'J') || 
          (FireButton == 'S' && (CurrentMode == 'A' || CurrentMode == 'M'))) {
        FireButton = 'Z';
        break;
      }
    }
  } 
  else {
    digitalWrite(Led, LOW);
    analogWrite(Buz, LOW);
  }
}

void AutomaticMotion() {
  while (On) {
    delay(100);
    for (int i = 5; i <= 175; i += 10) {
      H_SERVO.write(i);
      Serial.println(i);
      delay(30);
      for (int j = 10; j <= 60; j++) {
        V_SERVO.write(j);
        CheckFlame();   
        Serial.println(j);
        CheckMode();
        delay(30);
      }
      delay(50);  
      for (int j = 60; j >= 10; j--) {
        V_SERVO.write(j);
        CheckFlame();
        Serial.println(j);
        CheckMode();
        delay(30);
      }
    }
    delay(1000);
    for (int i = 175; i >= 5; i -= 10) {
      H_SERVO.write(i);
      Serial.println(i);
      delay(30);
      for (int j = 10; j <= 60; j++) {
        V_SERVO.write(j);
        CheckFlame();
        CheckMode();
        Serial.println(j);
        delay(30);
      }
      delay(50);  
      for (int j = 60; j >= 10; j--) {
        V_SERVO.write(j);
        CheckFlame();
        CheckMode();
        Serial.println(j);
        delay(30);
      }
    }
    delay(4000);
  }
}

void JoystickManualMotion() {
  while (On && CurrentMode == 'J') {
    angelX = map(analogRead(JoystickX), 0, 1023, 170, 10);
    delay(25);
    angelY = map(analogRead(JoystickY), 0, 1023, 60, 10);
    delay(25);
    if (angelX <= 170 && angelX >= 10) H_SERVO.write(angelX);
    delay(25);
    if (angelY <= 60 && angelY >= 10) V_SERVO.write(angelY);
    delay(25);
    CheckFlame();
    delay(30);
    CheckMode();
    delay(30);
  }
}

void ManualMotion() {
  while (On && CurrentMode == 'M') {
    if (BTSerial.available()) {
      String s = BTSerial.readStringUntil('\n');
      if (s.length() == 1) {
        CurrentMode = s.charAt(0);
        Serial.println(CurrentMode);
        break;
      }
      String letter = s.substring(0, 1);
      String numbers = s.substring(1);
      int num = numbers.toInt();
      if (letter.equals("H")) {
        Serial.println(num);
        H_SERVO.write(num);
        delay(30);
      } else if (letter.equals("V")) {
        Serial.println(num);
        V_SERVO.write(num);
        delay(30);
      }
    }
    CheckFlame();
    delay(30);
  }
  CheckMode();
}