/**
 * @file globals.cpp
 * @brief Definición de variables globales del sistema.
 */
#include "globals.h"

uint32_t stateEnterMs = 0;
uint32_t blinkMs = 0;
bool ledOn = false;

bool btnEvent = false;
bool nextTempToHum = true;
bool flameDetected = false;

uint8_t alertAttempts = 0;

float tempC = NAN;
float humP  = NAN;

int luzRaw  = 0;
int luzPct  = 0;
int luzBase = 0;

uint32_t lastDhtReadMs = 0;

bool started = false;
bool promptShown = false;