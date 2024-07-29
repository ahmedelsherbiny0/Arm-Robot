#include <Servo.h>
Servo H_SERVO;
Servo V_SERVO;

#define Flame A0
#define DigitalFlame 7
#define H_joystick A3
#define Buz 6
#define led 2

void CheckFlame();

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(DigitalFlame, INPUT);
  H_SERVO.attach(9);
  H_SERVO.write(5);
  delay(400);
  V_SERVO.attach(10);
  V_SERVO.write(5);
  delay(400);
  Serial.begin(9600);
}

void loop() {
  // sensor 
  // int SensorValue = analogRead(Flame);
  // int Buz_PWM = map(SensorValue, 0, 1023, 0, 255);
  // Serial.print("Sensor Value: ");
  // Serial.print(SensorValue);
  // Serial.print(" => Mapped Angle: ");
  // Serial.println(angle);
  // if (angle < 200) {
  //   analogWrite(Buz, 255 - angle);
  //   digitalWrite(led, HIGH);
  // }
  // else {
  //   analogWrite(Buz, 0);
  //   digitalWrite(led, LOW);
  // }



  // Automatic Motion          * We Will Make It Function *

  for (int i = 5; i <= 175; i += 10) {
    H_SERVO.write(i);
    delay(50);
    for (int i = 5; i <= 60; i++) {
      V_SERVO.write(i);
      CheckFlame();
      delay(50);
    }
    delay(50);  
    for (int i = 60; i >= 5; i--) {
      V_SERVO.write(i);
      CheckFlame();
      delay(50);
    }
  }
  delay(1000);
  for (int i = 175; i >= 5; i--) {
    H_SERVO.write(i);
    delay(50);
    for (int i = 5; i <= 60; i++) {
      V_SERVO.write(i);
      CheckFlame();
      delay(50);
    }
    delay(50);  
    for (int i = 60; i >= 5; i--) {
      V_SERVO.write(i);
      CheckFlame();
      delay(50);
    }
  }
  delay(4000);
  
  // End Of Automatic Motion 




  // int H_joystickValue = analogRead(H_joystick);
  // int angle = map(H_joystickValue, 0, 1023, 5, 175);
  // H_SERVO.write(angle);
  // Serial.print("Joy stick Read => ");
  // Serial.println(analogRead(H_joystick));
  // Serial.print("Motor Value => ");
  // Serial.println(angle);
}


void CheckFlame() {
  if (analogRead(Flame) < 600) {
    digitalWrite(led, HIGH);
    Serial.println(analogRead(Flame));
    analogWrite(Buz, map(analogRead(Flame), 0, 1023, 0, 255));
    while(true) {
      // We Will Creat Button In the App To Break This While Loop And Make The Arm Continue The Motion
    }
  }
  else
    digitalWrite(led, LOW);
    analogWrite(Buz, LOW);
}
