#pragma once

typedef enum {
    GpioStartEventOtgOff = 0,
    GpioStartEventOtgOn,
    GpioStartEventManualControl,
    GpioStartEventUsbUart,
    GpioStartEventNeopixel,

    GpioCustomEventErrorBack,

    GpioUsbUartEventConfig,
} GpioCustomEvent;
