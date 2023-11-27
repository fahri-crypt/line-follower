#include <Arduino.h>

const unsigned int Emiter = 22;
const unsigned int lines[] = {24, 26, 28, 30, 32, 34, 36, 38};
const unsigned int numLines = sizeof(lines) / sizeof(lines[0]);

const uint8_t maxSpeedA = 150;
const uint8_t maxSpeedB = 150;
const uint8_t baseSpeedA = 100;
const uint8_t baseSpeedB = 100;

int modePin = 8;
int bPhasePin = 6; // kanan
int bEnblPin = 7;
int aPhasePin = 4; // kiri
int aEnblPin = 5;

void setup() {
  Serial.begin(9600);
  pinMode(Emiter, OUTPUT);
  digitalWrite(Emiter, HIGH);

  for (unsigned int i = 0; i < numLines; i++) {
    pinMode(lines[i], INPUT);
  }

  pinMode(modePin, OUTPUT);
  pinMode(aPhasePin, OUTPUT);
  pinMode(aEnblPin, OUTPUT);
  pinMode(bPhasePin, OUTPUT);
  pinMode(bEnblPin, OUTPUT);
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
  // Pengaturan kecepatan motor
  int motorSpeedA, motorSpeedB;
  int baseSpeed = (baseSpeedA + baseSpeedB) / 2;
  int speedShift = 20; // Amount to reduce speed for each shift

  // Logika kontrol motor berdasarkan pembacaan sensor
  if (sensorValues[3] == HIGH && sensorValues[4] == HIGH) {
    // Jika kedua sensor tengah terdeteksi, maju lurus
    motorSpeedA = motorSpeedB = baseSpeed;
  } else if (sensorValues[7] == HIGH) {
    // Jika sensor paling kiri terdeteksi, belok ke kiri
    motorSpeedA = 0;
    motorSpeedB = maxSpeedB - speedShift;
  } else if (sensorValues[6] == HIGH) {
    // Jika sensor sebelah kiri sedikit terdeteksi, belok sedikit ke kiri
    motorSpeedA = baseSpeedA;
    motorSpeedB = maxSpeedB - speedShift;
  } else if (sensorValues[5] == HIGH) {
    // Jika sensor sebelah kiri lebih sedikit terdeteksi, belok lebih sedikit ke kiri
    motorSpeedA = maxSpeedA - speedShift;
    motorSpeedB = baseSpeedB;
  } else if (sensorValues[2] == HIGH) {
    // Jika sensor sebelah kanan lebih sedikit terdeteksi, belok lebih sedikit ke kanan
    motorSpeedA = baseSpeedA;
    motorSpeedB = maxSpeedB - speedShift;
  } else if (sensorValues[1] == HIGH) {
    // Jika sensor sebelah kanan sedikit terdeteksi, belok sedikit ke kanan
    motorSpeedA = maxSpeedA - speedShift;
    motorSpeedB = baseSpeedB;
  } else if (sensorValues[0] == HIGH) {
    // Jika sensor paling kanan terdeteksi, belok ke kanan
    motorSpeedA = maxSpeedA - speedShift;
    motorSpeedB = 0;
  } else {
    // Jika tidak ada sensor yang terdeteksi, berhenti
    motorSpeedA = motorSpeedB = 0;
  }

  // Print kecepatan motor ke Serial Monitor
  Serial.print("Motor Speed A: ");
  Serial.print(motorSpeedA);
  Serial.print(", Motor Speed B: ");
  Serial.println(motorSpeedB);

  // Set kecepatan motor menggunakan driver motor DRV8835
  analogWrite(aEnblPin, motorSpeedA);
  analogWrite(bEnblPin, motorSpeedB);

  // Set arah putaran motor menggunakan fase (phase) pada DRV8835
  digitalWrite(aPhasePin, (motorSpeedA > 0) ? HIGH : LOW);
  digitalWrite(bPhasePin, (motorSpeedB > 0) ? HIGH : LOW);

  // Set mode driver motor (mode) untuk kontrol kecepatan
  digitalWrite(modePin, HIGH);
}