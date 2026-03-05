/**
 * @file config.h
 * @brief Configuración del sistema.
 *
 * Este archivo define constantes globales utilizadas en todo el sistema,
 * incluyendo pines de hardware, umbrales de sensores y parámetros
 * de funcionamiento.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ===================== PINES =====================
#define DHTPIN      4
#define DHTTYPE     DHT22
#define LDR_PIN     34
#define FLAME_PIN 32

#define RGB_R_PIN   25
#define RGB_G_PIN   26
#define RGB_B_PIN   27

#define BTN_PIN     23  // botón a GND con INPUT_PULLUP

// ===================== UMBRALES =====================
#define TEMP_ALERTA_C       28.0
#define TEMP_ALARMA_C       30.0
#define HUM_ALERTA_PCT      80.0
#define TEMP_SALIR_ALERTA   26.0

#define LUZ_DELTA_TRIG      250   // ajusta 150..600

// ===================== TIEMPOS =====================
#define INICIO_ON_MS    1000
#define INICIO_OFF_MS   1000

#define TEMP_TO_HUM_MS  4000
#define TEMP_TO_LUZ_MS  5000

#define HUM_TO_TEMP_MS  3000
#define LUZ_TO_TEMP_MS  2000

#define ALERT_ON_MS     500
#define ALERT_OFF_MS    400

#define ALARM_ON_MS     100
#define ALARM_OFF_MS    300

#define BTN_DEBOUNCE_MS 60

// ===================== PERIODOS TASK =====================
#define TASK_BTN_MS   10     // revisar botón
#define TASK_DHT_MS   2000   // DHT22 mínimo 2s
#define TASK_LDR_MS   200    // luz
#define TASK_LOG_MS   1000   // serial
#define TASK_BLINK_MS 10     // blink suave

// ===================== PWM LEDC =====================
#define PWM_FREQ 5000
#define PWM_RES  8

#endif // CONFIG_H