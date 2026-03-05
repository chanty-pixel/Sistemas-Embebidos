/**
 * @file main.ino
 * @brief Punto de entrada del programa.
 *
 * Este archivo contiene las funciones principales del entorno Arduino.
 * Desde aquí se inicializa y ejecuta la aplicación principal del sistema.
 */

#include "app.h"

/**
 * @brief Inicializa el sistema completo.
 */
void setup() {
  App_Setup();
}

/**
 * @brief Bucle principal del sistema.
 *
 * Ejecuta continuamente la lógica principal de la aplicación.
 */
void loop() {
  App_Loop();
}