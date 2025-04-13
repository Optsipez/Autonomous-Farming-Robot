// ----- Motor Pins (L298N #1) -----
int enA1 = 9;
int in1 = 8;
int in2 = 7;

int enB1 = 3;
int in3 = 5;
int in4 = 4;

// ----- Motor Pins (L298N #2) -----
int enA2 = 10;
int in5 = 6;
int in6 = 2;

int enB2 = 11;
int in7 = 12;
int in8 = 13;

// ----- Soil Moisture Sensor -----
#define sensorPin A0
#define wetSoil 277
#define drySoil 380

// ----- Hobby Motor (Actuator for Sensor Arm) -----
int hobbyMotorPin1 = A1;  // Control pin 1 for actuator
int hobbyMotorPin2 = A2;  // Control pin 2 for actuator

void setup() {
  Serial.begin(9600);

  // Setup motor pins
  pinMode(enA1, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB1, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  pinMode(enA2, OUTPUT); pinMode(in5, OUTPUT); pinMode(in6, OUTPUT);
  pinMode(enB2, OUTPUT); pinMode(in7, OUTPUT); pinMode(in8, OUTPUT);

  // Hobby motor control pins
  pinMode(hobbyMotorPin1, OUTPUT);
  pinMode(hobbyMotorPin2, OUTPUT);

  stopAllMotors();
}

void loop() {
  // Move forward for 5 seconds
  moveAllForward();
  delay(5000);

  // Stop and take soil reading
  stopAllMotors();
  delay(500);
  performSoilCheck();

  // Continue moving
  moveAllForward();
  delay(5000);

  stopAllMotors();
  delay(2000);
}

// ----- Soil Moisture Reading & Actuator -----
void performSoilCheck() {
  Serial.println(" Stopping for soil moisture check...");

  // Lower the soil sensor (rotate hobby motor one direction)
  digitalWrite(hobbyMotorPin1, HIGH);
  digitalWrite(hobbyMotorPin2, LOW);
  delay(1000); // Adjust time as needed to fully lower

  // Stop hobby motor
  digitalWrite(hobbyMotorPin1, LOW);
  digitalWrite(hobbyMotorPin2, LOW);

  // Wait 10 seconds while reading
  for (int i = 0; i < 10; i++) {
    int moisture = analogRead(sensorPin);
    Serial.print("Analog output: ");
    Serial.println(moisture);

    if (moisture < wetSoil) {
      Serial.println("Status: Soil is too wet");
    } else if (moisture < drySoil) {
      Serial.println("Status: Soil moisture is perfect");
    } else {
      Serial.println("Status: Soil is too dry - time to water!");
    }

    delay(1000);
  }

  // Raise the soil sensor (rotate hobby motor opposite direction)
  digitalWrite(hobbyMotorPin1, LOW);
  digitalWrite(hobbyMotorPin2, HIGH);
  delay(1000); // Adjust to raise back up

  // Stop hobby motor
  digitalWrite(hobbyMotorPin1, LOW);
  digitalWrite(hobbyMotorPin2, LOW);

  Serial.println("✅ Soil check complete. Resuming movement...\n");
}

// ----- Mecanum Motor Control -----
void moveAllForward() {
  analogWrite(enA1, 200);
  analogWrite(enB1, 200);
  analogWrite(enA2, 128);
  analogWrite(enB2, 128);

  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  digitalWrite(in5, HIGH); digitalWrite(in6, LOW);
  digitalWrite(in7, HIGH); digitalWrite(in8, LOW);
}

void stopAllMotors() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  digitalWrite(in5, LOW); digitalWrite(in6, LOW);
  digitalWrite(in7, LOW); digitalWrite(in8, LOW);
}


