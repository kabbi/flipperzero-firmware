#pragma once

#include <furi_hal_gpio.h>

void neopixel_configure();

void neopixel_set_color(uint8_t index, uint32_t color);

void neopixel_clear();

void neopixel_update();
