#include <AFMotor.h>

AF_DCMotor motorA(3);  // Motor connected to M1 port on HW130 motor shield
AF_DCMotor motorB(4);  // Motor connected to M2 port on HW130 motor shield

const unsigned int Emiter = 22;
const unsigned int lines[] = {24, 26, 28, 30, 32, 34, 36, 38};
const unsigned int numLines = sizeof(lines) / sizeof(lines[0]);

int modePin = 8;

void setup() {
  Serial.begin(9600);
  pinMode(Emiter, OUTPUT);
  digitalWrite(Emiter, HIGH);

  for (unsigned int i = 0; i < numLines; i++) {
    pinMode(lines[i], INPUT);
  }

  pinMode(modePin, OUTPUT);
}

void loop() {
  int values[numLines];

  for (unsigned int i = 0; i < numLines; i++) {
    values[i] = digitalRead(lines[i]);
    Serial.print(values[i]);
    Serial.print(" ");
  }

  Serial.println();

  // Proses pembacaan sensor dan kontrol motor
  lineFollower(values);

  delay(500);
}

void lineFollower(int sensorValues[]) {
  // Logika kontrol motor berdasarkan pembacaan sensor
  if (sensorValues[3] == HIGH && sensorValues[4] == HIGH) {
    // Jika kedua sensor tengah terdeteksi, maju lurus
    motorA.setSpeed(100);
    motorB.setSpeed(100);
    motorA.run(FORWARD);
    motorB.run(FORWARD);
  } else if (sensorValues[7] == HIGH) {
    // Jika sensor paling kiri terdeteksi, belok ke kiri
    motorA.setSpeed(0);
    motorB.setSpeed(100);
    motorA.run(RELEASE);
    motorB.run(FORWARD);
  } else if (sensorValues[6] == HIGH) {
    // Jika sensor sebelah kiri sedikit terdeteksi, belok sedikit ke kiri
    motorA.setSpeed(100);
    motorB.setSpeed(80);
    motorA.run(FORWARD);
    motorB.run(FORWARD);
  } // Add more conditions as needed

  // Print kecepatan motor ke Serial Monitor
  Serial.print("Motor Speed A: ");
  // Serial.print(motorA.readSpeed());
  Serial.print(", Motor Speed B: ");
  // Serial.println(motorB.readSpeed());
}
