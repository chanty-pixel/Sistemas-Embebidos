/**
 * @file sensors.h
 * @brief Lectura de sensores del sistema.
 *
 * Este módulo permite obtener información de temperatura,
 * humedad, nivel de luz y detección de llama.
 */

#ifndef SENSORS_H
#define SENSORS_H


/**
 * @brief Inicializa los sensores del sistema.
 */
void Sensors_Init();

/**
 * @brief Lee temperatura y humedad usando caché temporal.
 */
void Sensors_ReadDhtCached();

/**
 * @brief Lee el nivel de luz del sensor LDR.
 */
void Sensors_ReadLight();

/**
 * @brief Calibra la línea base del sensor de luz.
 */
void Sensors_CalibrateLightBaseline();

/**
 * @brief Lee el estado del sensor de llama.
 */
void Sensors_ReadFlame();

#endif // SENSORS_H