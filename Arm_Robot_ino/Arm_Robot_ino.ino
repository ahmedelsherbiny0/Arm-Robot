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
void ManualMotion();
void CheckMode();
bool On = false;
char PreviousMode; 
char CurrentMode;
char FireButton;
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

void CheckMode(){
if(BTSerial.available()) {
    CurrentMode = BTSerial.read();
    // Serial.print("Received character: ");
    // Serial.println(CurrentMode);
    if(CurrentMode == 'A') {
      On = false;
      PreviousMode='A';
      On=true;
      AutomaticMotion();
      delay(50);
    }
    else if(CurrentMode == 'M') {
      On=false;
      PreviousMode='M';
      On=true;
      ManualMotion();
      delay(50);  
    }
    else if(CurrentMode == 'J') {
      On=false;
      PreviousMode='J';
      On = true;
      JoystickManualMotion();
      delay(50);  
    }
  }
  delay(50);
  
}

void CheckFlame() {
  Serial.println(map(analogRead(Flame), 0, 1023, 255, 0));
  if (analogRead(Flame) < 450) {
    digitalWrite(Led, HIGH);
    while(true) {
      // We Will Creat A Button In the App To Break This While Loop And Make The Arm Continue The Motion
      analogWrite(Buz, map(analogRead(Flame), 0, 1023, 255, 0));
      // Serial.println(analogRead(Flame));
      if(BTSerial.available()) 
      FireButton = BTSerial.read();
      if ((digitalRead(JoystickSwitch) == LOW && PreviousMode=='J' ) || (FireButton=='S' && PreviousMode=='A' ) || (FireButton=='S' && PreviousMode=='M' ))  {           // To Continue The Motion From Joystick Switch   
        FireButton = 'Z';
        break;
      }
    }
  }
  else
    digitalWrite(Led, LOW);
    analogWrite(Buz, LOW);
}

void AutomaticMotion() {
  while(On) {
    for (int i = 5; i <= 175 ; i += 10) {
    H_SERVO.write(i);
    delay(30);
    for (int i = 10; i <= 60 ; i++) {
      V_SERVO.write(i);
      CheckFlame();
      CheckMode();
      delay(30);
      if(!On)
      return 0;
      // Serial.println(i);
    }
    delay(50);  
    for (int i = 60; i >= 10; i--) {
      V_SERVO.write(i);
      CheckFlame();
      CheckMode();
      delay(30);
      if(!On)
      return 0;
      // Serial.println(i);
    }
  }
  delay(1000);
  for (int i = 175; i >= 5; i-= 10) {
    H_SERVO.write(i);
    delay(30);
    for (int i = 10; i <= 60; i++) {
      V_SERVO.write(i);
      CheckFlame();
      CheckMode();
      delay(30);
      if(!On)
      return 0;
    }
    delay(50);  
    for (int i = 60; i >= 10; i--) {
      V_SERVO.write(i);
      CheckFlame();
      CheckMode();
      delay(30);
      if(!On)
      return 0;
    }
  }
  delay(4000);
  }
}

void JoystickManualMotion() {
  while(On) {
    angelX = map(analogRead(JoystickX), 0, 1023, 170, 10);
    delay(25);
    angelY = map(analogRead(JoystickY), 0, 1023, 60, 10);
    delay(25);
    if (angelX <= 170 && angelX >= 10)
      H_SERVO.write(angelX);
    delay(25);
    if (angelY <= 60 && angelY >= 10)
      V_SERVO.write(angelY);
    delay(25);
    CheckFlame();
    delay(30);
    CheckMode();
    delay(25);
    // Serial.println(angelX);
    // Serial.println(angelY);
    delay(25);
  }
}

void ManualMotion() {
  while (On) {
    if (BTSerial.available()) {
      // Read the incoming string from Bluetooth
      String s = BTSerial.readStringUntil('\n'); // Assuming commands are newline-terminated
      if(s.length()==1){
        CurrentMode=s.charAt(0);
        break;
      }
      
      String letter = s.substring(0, 1); // Assuming the letter is at the start
      String numbers = s.substring(1);    // Assuming the numbers start from the second character
      
      int num = numbers.toInt(); // Convert the numeric part to an integer
      CheckFlame();
      if (letter.equals("H")) {
        Serial.println(num);
        H_SERVO.write(num);
        delay(30);
      } 
      else if (letter.equals("V")) {
        Serial.println(num);
        V_SERVO.write(num);
        delay(30);
      }
    }
    delay(30);
  }
   CheckMode(); 
}