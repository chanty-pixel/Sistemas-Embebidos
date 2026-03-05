/**
 * @file fsm.cpp
 * @brief Implementación de la máquina de estados del sistema.
 */
#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "fsm.h"
#include "io.h"

// Máquina global (6 estados, 11 transiciones)
StateMachine stateMachine(6, 11);

static inline bool timeoutMs(uint32_t ms) { return (millis() - stateEnterMs) >= ms; }

static inline bool condTempAlerta() { return (!isnan(tempC) && tempC > TEMP_ALERTA_C); }
static inline bool condHumAlerta()  { return (!isnan(humP)  && humP > HUM_ALERTA_PCT); }

// Luz SOLO se evalúa si hay flama
static inline bool condLuzAlerta() {
  if (!flameDetected) return false;
  return (abs(luzRaw - luzBase) > LUZ_DELTA_TRIG);
}

// Escalamiento / salida por temperatura
static inline bool condEscalaAlarma()     { return (!isnan(tempC) && tempC > TEMP_ALARMA_C && alertAttempts >= 3); }
static inline bool condSalirAlertaATemp() { return (!isnan(tempC) && tempC < TEMP_SALIR_ALERTA && alertAttempts >= 3); }

// NUEVO: salida inmediata de ALERTA si desaparece la flama
static inline bool condFlamaDesaparece() {
  return !flameDetected;
}

static inline void enterCommon() {
  stateEnterMs = millis();
  blinkMs = millis();
  ledOn = false;
  // OJO: NO borramos btnEvent aquí
}

// OnEnter: apaga LED al entrar
static void onEnterInicio()   { enterCommon(); IO_SetRGB(0,0,0); alertAttempts = 0; Serial.println("[ENTER] INICIO"); }
static void onEnterTemp()     { enterCommon(); IO_SetRGB(0,0,0); Serial.println("[ENTER] TEMP"); }
static void onEnterHumedad()  { enterCommon(); IO_SetRGB(0,0,0); Serial.println("[ENTER] HUMEDAD"); }
static void onEnterLuz()      { enterCommon(); IO_SetRGB(0,0,0); Serial.println("[ENTER] LUZ"); }
static void onEnterAlerta()   { enterCommon(); IO_SetRGB(0,0,0); Serial.println("[ENTER] ALERTA"); }
static void onEnterAlarma()   { enterCommon(); IO_SetRGB(0,0,0); Serial.println("[ENTER] ALARMA"); }

void FSM_Setup() {

  // 1) INICIO -> TEMP (Botón)
  stateMachine.AddTransition(INICIO, TEMP, [](){ return btnEvent; });

  // PRIORIDAD: Si hay flama, entrar a LUZ inmediatamente
  // (antes de timeouts para que gane prioridad)
  stateMachine.AddTransition(TEMP, LUZ, [](){ return flameDetected; });

  // (Opcional recomendado) Si estás en HUMEDAD y aparece flama, también ir a LUZ
  stateMachine.AddTransition(HUMEDAD, LUZ, [](){ return flameDetected; });

  // Timeouts normales TEMP <-> HUMEDAD/LUZ
  stateMachine.AddTransition(TEMP, HUMEDAD, [](){ return nextTempToHum && timeoutMs(TEMP_TO_HUM_MS); });
  stateMachine.AddTransition(TEMP, LUZ,     [](){ return (!nextTempToHum) && timeoutMs(TEMP_TO_LUZ_MS); });

  stateMachine.AddTransition(HUMEDAD, TEMP, [](){ return timeoutMs(HUM_TO_TEMP_MS); });
  stateMachine.AddTransition(LUZ, TEMP,     [](){ return timeoutMs(LUZ_TO_TEMP_MS); });

  // Alertas por sensores
  stateMachine.AddTransition(TEMP, ALERTA,    [](){ return condTempAlerta(); });
  stateMachine.AddTransition(HUMEDAD, ALERTA, [](){ return condHumAlerta(); });

  // Luz -> ALERTA solo si hay flama
  stateMachine.AddTransition(LUZ, ALERTA,     [](){ return condLuzAlerta(); });

  // NUEVO: si estás en ALERTA y se va la flama, sales de ALERTA a TEMP
  // Lo ponemos antes de otras salidas para que tenga prioridad
  stateMachine.AddTransition(ALERTA, TEMP,    [](){ return condFlamaDesaparece(); });

  // ALERTA -> ALARMA o -> TEMP por temperatura (cuando completa intentos)
  stateMachine.AddTransition(ALERTA, ALARMA,  [](){ return condEscalaAlarma(); });
  stateMachine.AddTransition(ALERTA, TEMP,    [](){ return condSalirAlertaATemp(); });

  // ALARMA -> INICIO (Botón)
  stateMachine.AddTransition(ALARMA, INICIO,  [](){ return btnEvent; });

  // Actions On Enter
  stateMachine.SetOnEntering(INICIO, onEnterInicio);
  stateMachine.SetOnEntering(TEMP, onEnterTemp);
  stateMachine.SetOnEntering(HUMEDAD, onEnterHumedad);
  stateMachine.SetOnEntering(LUZ, onEnterLuz);
  stateMachine.SetOnEntering(ALERTA, onEnterAlerta);
  stateMachine.SetOnEntering(ALARMA, onEnterAlarma);

  // Debug Leaving
  stateMachine.SetOnLeaving(INICIO,  [](){ Serial.println("[LEAVE] INICIO"); });
  stateMachine.SetOnLeaving(TEMP,    [](){ Serial.println("[LEAVE] TEMP"); });
  stateMachine.SetOnLeaving(HUMEDAD, [](){ Serial.println("[LEAVE] HUMEDAD"); });
  stateMachine.SetOnLeaving(LUZ,     [](){ Serial.println("[LEAVE] LUZ"); });
  stateMachine.SetOnLeaving(ALERTA,  [](){ Serial.println("[LEAVE] ALERTA"); });
  stateMachine.SetOnLeaving(ALARMA,  [](){ Serial.println("[LEAVE] ALARMA"); });
}