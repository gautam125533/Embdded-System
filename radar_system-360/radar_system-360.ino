#include <Servo.h>

// Ultrasonic Sensor 1 pins
const int trigPin1 = 2;
const int echoPin1 = 9;

// Ultrasonic Sensor 2 pins (opposite side)
const int trigPin2 = 3;
const int echoPin2 = 10;

// Buzzer
const int buzzer = 7;

// Servo
Servo myServo;

// Variables
long duration;
int distance1, distance2;
int threshold = 40;  // cm distance to trigger buzzer

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(buzzer, OUTPUT);

  myServo.attach(12);   // Servo signal pin
  Serial.begin(9600);   // For Processing
}

void loop() {
  // Sweep 0 → 180
  for (int i = 0; i <= 180; i++) {
    myServo.write(i);
    delay(0);

    distance1 = getDistance(trigPin1, echoPin1);
    distance2 = getDistance(trigPin2, echoPin2);

    // Send data for Processing
    sendData(i, distance1);        // Sensor 1 (0–180°)
    sendData(i + 180, distance2);  // Sensor 2 (180–360°)

    // Buzzer detection
    checkBuzzer(distance1);
    checkBuzzer(distance2);
  }

  // Sweep 180 → 0
  for (int i = 180; i >= 0; i--) {
    myServo.write(i);
    delay(30);

    distance1 = getDistance(trigPin1, echoPin1);
    distance2 = getDistance(trigPin2, echoPin2);

    sendData(i, distance1);
    sendData(i + 180, distance2);

    checkBuzzer(distance1);
    checkBuzzer(distance2);
  }
}

// Function: Ultrasonic distance measurement
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 20000); // 20ms timeout
  if (duration == 0) return 999; // No object
  return duration * 0.034 / 2;   // cm
}

// Function: Send angle + distance over Serial
void sendData(int angle, int dist) {
  Serial.print(angle);
  Serial.print(",");
  Serial.print(dist);
  Serial.print(".");
}

// Function: Buzzer control (faster beep when closer)
void checkBuzzer(int dist) {
  if (dist > 0 && dist < threshold) {
    int beepDelay = map(dist, 5, threshold, 100, 700); // closer = faster
    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
    delay(beepDelay);
  }
}
