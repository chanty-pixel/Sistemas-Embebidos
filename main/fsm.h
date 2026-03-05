/**
 * @file fsm.h
 * @brief Definición de la máquina de estados del sistema.
 *
 * Este módulo define los estados y la instancia de la máquina
 * de estados encargada de controlar el comportamiento del sistema.
 */
 
#ifndef FSM_H
#define FSM_H

#include "StateMachineLib.h"

/**
 * @enum State
 * @brief Estados posibles del sistema.
 */
enum State {
  INICIO = 0,
  TEMP = 1,
  HUMEDAD = 2,
  LUZ = 3,
  ALERTA = 4,
  ALARMA = 5
};

/**
 * @brief Instancia global de la máquina de estados.
 */
extern StateMachine stateMachine;

/**
 * @brief Inicializa la máquina de estados.
 */
void FSM_Setup();

#endif // FSM_H