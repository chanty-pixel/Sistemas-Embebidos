/**
 * @file io.cpp
 * @brief Implementación del control de salida RGB.
 */
#include <Arduino.h>
#include "config.h"
#include "io.h"

void IO_Init() {
  ledcAttach(RGB_R_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(RGB_G_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(RGB_B_PIN, PWM_FREQ, PWM_RES);
  IO_SetRGB(0, 0, 0);
}

void IO_SetRGB(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(RGB_R_PIN, r);
  ledcWrite(RGB_G_PIN, g);
  ledcWrite(RGB_B_PIN, b);
}