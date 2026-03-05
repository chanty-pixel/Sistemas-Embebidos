/**
 * @file app.h
 * @brief Control principal de la aplicación.
 *
 * Este módulo coordina la inicialización y ejecución de todos
 * los componentes del sistema.
 */

#ifndef APP_H
#define APP_H

/**
 * @brief Inicializa todos los módulos del sistema.
 */
void App_Setup();

/**
 * @brief Ejecuta el ciclo principal de la aplicación.
 */
void App_Loop();

#endif // APP_H