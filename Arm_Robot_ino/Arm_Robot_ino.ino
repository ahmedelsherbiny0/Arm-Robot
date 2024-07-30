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
int angelX;
int angelY; 

void CheckFlame();
void AutomaticMotion();
void JoystickManualMotion();

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
  // Bluetooth
  // if (Serial.available()) {
  //   BTSerial.println(Serial.read());
  // }
  // if (BTSerial.available()) {
  //   Serial.println(BTSerial.read());
  // }

  // if(BTSerial.available())
  // {
  //   char in = BTSerial.read();
  //   if(in == 'X'){
  //     H_SERVO.write(120);
  //     delay(50);
  //   }
  //   else if(in == 'Y')
  //   {
  //     H_SERVO.write(60);
  //     delay(50);  
  //   }
  // }
  // delay(50);
  // Serial.println(analogRead(Flame));



  // AutomaticMotion();

  // JoystickManualMotion();

}


void CheckFlame() {
  if (analogRead(Flame) < 500) {
    digitalWrite(Led, HIGH);
    Serial.println(analogRead(Flame));
    analogWrite(Buz, map(analogRead(Flame), 0, 1023, 255, 0));
    while(true) {
      // We Will Creat A Button In the App To Break This While Loop And Make The Arm Continue The Motion
      if (digitalRead(JoystickSwitch) == LOW)             // To Continue The Motion From Joystick Switch   
        break;
    }
  }
  else
    digitalWrite(Led, LOW);
    analogWrite(Buz, LOW);
}

void AutomaticMotion() {
  while(true) {
    for (int i = 5; i <= 175; i += 10) {
    H_SERVO.write(i);
    delay(30);
    for (int i = 10; i <= 60; i++) {
      V_SERVO.write(i);
      CheckFlame();
      delay(30);
      Serial.println(i);
    }
    delay(50);  
    for (int i = 60; i >= 10; i--) {
      V_SERVO.write(i);
      CheckFlame();
      delay(30);
      Serial.println(i);
    }
  }
  delay(1000);
  for (int i = 175; i >= 5; i-= 10) {
    H_SERVO.write(i);
    delay(30);
    for (int i = 10; i <= 60; i++) {
      V_SERVO.write(i);
      CheckFlame();
      delay(30);
    }
    delay(50);  
    for (int i = 60; i >= 10; i--) {
      V_SERVO.write(i);
      CheckFlame();
      delay(30);
    }
  }
  delay(4000);
  }
}

void JoystickManualMotion() {
  while(true) {
    angelX = map(analogRead(JoystickX), 0, 1023, 170, 10);
    delay(20);
    angelY = map(analogRead(JoystickY), 0, 1023, 60, 10);
    delay(20);
    if (angelX <= 170 && angelX >= 10)
      H_SERVO.write(angelX);
    delay(20);
    if (angelY <= 60 && angelY >= 10)
      V_SERVO.write(angelY);
    delay(20);
    CheckFlame();
    delay(20);
    Serial.println(angelX);
    Serial.println(angelY);
    delay(20);
  }
}