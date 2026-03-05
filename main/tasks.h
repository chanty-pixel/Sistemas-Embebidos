/**
 * @file tasks.h
 * @brief Planificador simple de tareas periódicas.
 */

#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>

/**
 * @brief Tipo de función utilizada por una tarea.
 */
typedef void (*TaskFn)();

/**
 * @struct Task
 * @brief Representa una tarea periódica.
 */
struct Task {
  uint32_t periodMs;
  uint32_t lastRunMs;
  TaskFn fn;
};

/**
 * @brief Inicializa una tarea.
 */
void Task_Init(Task &t, uint32_t periodMs, TaskFn fn);

/**
 * @brief Ejecuta la tarea si corresponde.
 */
void Task_Update(Task &t, uint32_t nowMs);

#endif // TASKS_H