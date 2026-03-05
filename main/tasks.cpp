/**
 * @file tasks.cpp
 * @brief Implementación del planificador de tareas.
 */
#include "tasks.h"

void Task_Init(Task &t, uint32_t periodMs, TaskFn fn) {
  t.periodMs = periodMs;
  t.lastRunMs = 0;
  t.fn = fn;
}

void Task_Update(Task &t, uint32_t nowMs) {
  if (!t.fn) return;
  if (nowMs - t.lastRunMs >= t.periodMs) {
    t.lastRunMs = nowMs;
    t.fn();
  }
}