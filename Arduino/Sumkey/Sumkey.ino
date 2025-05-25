// File: sumobot_hockeybot.ino

// Motor driver pins
const int ENA = 11;
const int ENB = 6;
const int IN1 = 10;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;

// IR sensor
const int IR_PIN = 5;

// Ultrasonic pins
const int TRIG_PIN = 4;
const int ECHO_PIN = 3;

// Mode selector pin
const int MODE_PIN = 2; // LOW = Sumo, HIGH = Hockey (Bluetooth Controlled)

char command;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(MODE_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  bool mode = digitalRead(MODE_PIN);

  if (mode == LOW) {
    sumoMode();
  } else {
    hockeyModeBluetooth();
  }
}

void sumoMode() {
  if (digitalRead(IR_PIN) == LOW) {
    moveBackward();
    delay(400);
    turnRight();
    delay(400);
  } else {
    int distance = readDistance();
    if (distance < 20 && distance > 0) {
      moveForward();
    } else {
      turnLeft();
    }
  }
}

void hockeyModeBluetooth() {
  if (Serial.available()) {
    command = Serial.read();

    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopBot(); break;
    }
  }
}

int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  int distance = duration * 0.034 / 2;
  return distance;
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void stopBot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
