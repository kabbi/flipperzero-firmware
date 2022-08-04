#pragma once

#include <gui/view.h>

typedef struct GpioNeopixel GpioNeopixel;
typedef void (*GpioNeopixelOkCallback)(InputType type, void* context);

GpioNeopixel* gpio_neopixel_alloc();

void gpio_neopixel_free(GpioNeopixel* gpio_neopixel);

View* gpio_neopixel_get_view(GpioNeopixel* gpio_neopixel);

void gpio_neopixel_set_ok_callback(GpioNeopixel* gpio_neopixel, GpioNeopixelOkCallback callback, void* context);
