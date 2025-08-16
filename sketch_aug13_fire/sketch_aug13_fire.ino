#include <Servo.h>

// Create a servo object
Servo myServo;

// Define pin connections
const int trigPin = 9;
const int echoPin = 10;

const int rainDOPin = 3;     // Rain sensor digital output
const int flameDOPin = 2;    // Flame sensor digital output
const int buzzerPin = 8;
const int servoPin = 6;

// Variables
long duration;
int distance;
int rainStatus;
int flameStatus;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(rainDOPin, INPUT);
  pinMode(flameDOPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0); // Initial servo position

  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // ---------- Ultrasonic Sensor ----------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 15 && distance > 0) {
    Serial.println("Object Detected! Opening door...");
    myServo.write(90); // Open
    digitalWrite(buzzerPin, HIGH); // Optional sound
    delay(1000);
    digitalWrite(buzzerPin, LOW);
  } else {
    myServo.write(0); // Closed
  }

  // ---------- Rain Sensor ----------
  rainStatus = digitalRead(rainDOPin);
  if (rainStatus == LOW) {
    Serial.println("Rain Detected!");
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
  }

  // ---------- Flame Sensor ----------
  flameStatus = digitalRead(flameDOPin);
  if (flameStatus == LOW) {
    Serial.println("🔥 Flame Detected!");
    digitalWrite(buzzerPin, HIGH);
    delay(1500);
    digitalWrite(buzzerPin, LOW);
  }

  delay(1000);
}