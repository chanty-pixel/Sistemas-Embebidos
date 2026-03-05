/**
 * @file sensors.cpp
 * @brief Implementación de lectura de sensores.
 */
#include <Arduino.h>
#include <DHT.h>

#include "config.h"
#include "globals.h"
#include "sensors.h"

static DHT dht(DHTPIN, DHTTYPE);

void Sensors_Init() {
  pinMode(FLAME_PIN, INPUT);   // HW491 DO
  dht.begin();
  delay(2000);
}

void Sensors_ReadDhtCached() {
  if (millis() - lastDhtReadMs < TASK_DHT_MS) return;
  lastDhtReadMs = millis();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t)) tempC = t;
  if (!isnan(h)) humP = h;
}

void Sensors_ReadLight() {
  luzRaw = analogRead(LDR_PIN);
  luzPct = map(luzRaw, 0, 4095, 0, 100);
  luzPct = constrain(luzPct, 0, 100);
}

void Sensors_CalibrateLightBaseline() {
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(LDR_PIN);
    delay(20);
  }
  luzBase = sum / 20;
}

void Sensors_ReadFlame() {
  int v = digitalRead(FLAME_PIN);
  flameDetected = (v == HIGH);
}