#include<Servo.h>

/// Pin definitions
// - Sensor
int TriggerPin = 8;
int EchoPin = 9;

// - Motor
int motorSignalPin = 10;

// - LEDS
int GreenLED = 6;
int RedLED = 5;

// Variables
const int startingAngle = 90;
const int minimumAngle = 6;
const int maximumAngle = 175;
const int rotationSpeed = 2;

bool state = LOW;

Servo motor;

void setup(void){
  // LEDS
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);

  // Sensor
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Motor
  motor.attach(motorSignalPin);

  // Console
  Serial.begin(9600);
}

void loop(void){
  if(Serial.available() > 0){
    int val = Serial.read();
    if(char(val) == '1'){
      state = HIGH;
    }else{
      state = LOW;
    }
  }
  
  if(state){
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, HIGH);

    static int motorAngle = startingAngle;
    static int motorRotateAmount = rotationSpeed;
  
    motor.write(motorAngle);
    delay(10);
  
    WriteOutput(motorAngle, GetDistance());
      
    motorAngle += motorRotateAmount;
      
    if(motorAngle <= minimumAngle || motorAngle >= maximumAngle){
      motorRotateAmount = -motorRotateAmount;
    }
  }else{
    digitalWrite(GreenLED, LOW);
    digitalWrite(RedLED, HIGH);
  }
}

int GetDistance(void){
  // Clears the trigPin condition
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin ACTIVE for 10 microseconds
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(EchoPin, HIGH);
  float distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (to and back)

  return int(distance);
}

void WriteOutput(const int angle, const int distance){
  String angleString = String(angle);
  String distanceString = String(distance);
    
  Serial.println(angleString + "," + distanceString);
}
