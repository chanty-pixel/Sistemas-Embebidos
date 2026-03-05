/**
 * @file globals.h
 * @brief Variables globales compartidas del sistema.
 *
 * Este archivo declara las variables globales utilizadas por
 * los distintos módulos del sistema.
 */

#ifndef GLOBALS_H
#define GLOBALS_H


#include <Arduino.h>

// Timers estado/parpadeo
extern uint32_t stateEnterMs;
extern uint32_t blinkMs;
extern bool ledOn;
extern bool flameDetected;

// Eventos / control
extern bool btnEvent;
extern bool nextTempToHum;
extern uint8_t alertAttempts;

// Sensores
extern float tempC;
extern float humP;
extern int luzRaw;
extern int luzPct;
extern int luzBase;

// DHT cache
extern uint32_t lastDhtReadMs;

// Gate inicio
extern bool started;
extern bool promptShown;

#endif // GLOBALS_H