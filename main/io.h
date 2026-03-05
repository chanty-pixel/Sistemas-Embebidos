/**
 * @file io.h
 * @brief Control de dispositivos de entrada y salida.
 *
 * Este módulo gestiona el control del LED RGB del sistema.
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
 * @brief Inicializa el sistema de salida RGB.
 */
void IO_Init();

/**
 * @brief Establece el color del LED RGB.
 *
 * @param r Intensidad del rojo (0-255)
 * @param g Intensidad del verde (0-255)
 * @param b Intensidad del azul (0-255)
 */
void IO_SetRGB(uint8_t r, uint8_t g, uint8_t b);

#endif // IO_H