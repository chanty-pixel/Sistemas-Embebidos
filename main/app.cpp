/**
 * @file app.cpp
 * @brief Implementación de la aplicación principal.
 */
#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "tasks.h"
#include "sensors.h"
#include "fsm.h"
#include "io.h"
#include "app.h"

// Tasks
static Task tBtn, tDht, tLdr, tBlink, tLog, tFlame;

// ===================== BOTÓN =====================
static void pollButton() {
  static int stable = HIGH;
  static int lastRead = HIGH;
  static uint32_t lastChange = 0;

  int now = digitalRead(BTN_PIN);

  if (now != lastRead) {
    lastRead = now;
    lastChange = millis();
  }

  if ((millis() - lastChange) > BTN_DEBOUNCE_MS && now != stable) {
    int prevStable = stable;
    stable = now;
    if (prevStable == HIGH && stable == LOW) btnEvent = true; // latch
  }
}

// ===================== TASK CALLBACKS =====================
static void TaskBtnFn()   { pollButton(); }
static void TaskDhtFn()   { Sensors_ReadDhtCached(); }
static void TaskLdrFn()   { Sensors_ReadLight(); }
static void TaskFlameFn() {
  Sensors_ReadFlame();

  if (flameDetected) {
    Serial.println("[FLAME] Detectada");
  }
}   // AQUÍ, nivel global

static void TaskLogFn() {
  if (!started) return;

  Serial.print("[SENS] T="); Serial.print(tempC);
  Serial.print("C H=");      Serial.print(humP);
  Serial.print("% Lraw=");   Serial.print(luzRaw);
  Serial.print(" base=");    Serial.print(luzBase);
  Serial.print(" delta=");   Serial.print(luzRaw - luzBase);
  Serial.print(" pct=");     Serial.print(luzPct);
  Serial.print("% flame=");
  Serial.println(flameDetected ? "YES" : "NO");
}

static void TaskBlinkFn() {
  int st = stateMachine.GetState();

  if (st == INICIO) {
    if (!ledOn && (millis() - blinkMs) >= INICIO_OFF_MS) {
      ledOn = true; blinkMs = millis();
      IO_SetRGB(0,255,0);
    } else if (ledOn && (millis() - blinkMs) >= INICIO_ON_MS) {
      ledOn = false; blinkMs = millis();
      IO_SetRGB(0,0,0);
    }
  }

  if (st == ALERTA) {
    if (!ledOn) {
      if ((millis() - blinkMs) >= ALERT_OFF_MS) {
        ledOn = true; blinkMs = millis();
        IO_SetRGB(0,0,255);
      }
    } else {
      if ((millis() - blinkMs) >= ALERT_ON_MS) {
        ledOn = false; blinkMs = millis();
        IO_SetRGB(0,0,0);
        alertAttempts++;

        if (alertAttempts >= 3 && !isnan(tempC) && tempC >= TEMP_SALIR_ALERTA && tempC <= TEMP_ALARMA_C) {
          alertAttempts = 0;
        }
      }
    }
  }

  if (st == ALARMA) {
    if (!ledOn && (millis() - blinkMs) >= ALARM_OFF_MS) {
      ledOn = true; blinkMs = millis();
      IO_SetRGB(255,0,0);
    } else if (ledOn && (millis() - blinkMs) >= ALARM_ON_MS) {
      ledOn = false; blinkMs = millis();
      IO_SetRGB(0,0,0);
    }
  }
}

// ===================== APP =====================
void App_Setup() {
  Serial.begin(115200);
  delay(1200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  analogReadResolution(12);

  IO_Init();

  Sensors_Init();
  Sensors_CalibrateLightBaseline();

  FSM_Setup();
  stateMachine.SetState(INICIO, false, true);

  Task_Init(tBtn,   TASK_BTN_MS,   TaskBtnFn);
  Task_Init(tDht,   TASK_DHT_MS,   TaskDhtFn);
  Task_Init(tLdr,   TASK_LDR_MS,   TaskLdrFn);
  Task_Init(tFlame, 50,            TaskFlameFn); // task flama (50ms)
  Task_Init(tBlink, TASK_BLINK_MS, TaskBlinkFn);
  Task_Init(tLog,   TASK_LOG_MS,   TaskLogFn);

  Serial.println("======================================");
  Serial.println(" Sistema listo. Presiona el BOTON para iniciar.");
  Serial.println(" (En el primer toque: arranca + pasa a TEMP)");
  Serial.println("======================================");
}

void App_Loop() {
  uint32_t now = millis();

  Task_Update(tBtn, now);
  Task_Update(tBlink, now);

  if (!started) {
    if (btnEvent) {
      started = true;
      Serial.println("[OK] Iniciando... pasando a TEMP");
    }
    return;
  }

  // flama primero (habilita/inhabilita lógica de luz)
  Task_Update(tFlame, now);

  Task_Update(tDht, now);
  Task_Update(tLdr, now);
  Task_Update(tLog, now);

  stateMachine.Update();

  static int prevState = INICIO;
  int curState = stateMachine.GetState();
  if (prevState == TEMP && curState == HUMEDAD) nextTempToHum = false;
  if (prevState == TEMP && curState == LUZ)     nextTempToHum = true;
  prevState = curState;

  btnEvent = false;
}