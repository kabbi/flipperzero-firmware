#include "gpio_neopixel.h"
#include "../gpio_item.h"
#include "../neopixel.h"

#include <gui/elements.h>

struct GpioNeopixel {
    View* view;
    GpioNeopixelOkCallback callback;
    void* context;
};

typedef enum {
    EffectStaticRed,
    EffectStaticGreen,
    EffectStaticBlue,
    EffectRainbow,
    EffectLast,
} Effect;

typedef struct {
    Effect effect;
    uint8_t strip_length;
    bool on;
} GpioNeopixelModel;

static bool gpio_neopixel_process_up(GpioNeopixel* gpio_neopixel);
static bool gpio_neopixel_process_down(GpioNeopixel* gpio_neopixel);
static bool gpio_neopixel_process_left(GpioNeopixel* gpio_neopixel);
static bool gpio_neopixel_process_right(GpioNeopixel* gpio_neopixel);
static bool gpio_neopixel_process_ok(GpioNeopixel* gpio_neopixel, InputEvent* event);

static void gpio_neopixel_draw_callback(Canvas* canvas, void* _model) {
    GpioNeopixelModel* model = _model;
    canvas_set_font(canvas, FontPrimary);
    elements_multiline_text_aligned(canvas, 64, 2, AlignCenter, AlignTop, "Gpio Neopixel");
    canvas_draw_icon(canvas, 16, 14, &I_NeopixelPinout_61x11);
    canvas_set_font(canvas, FontSecondary);

    char buff[32];
    snprintf(buff, sizeof(buff), "effect %d size %d", model->effect, model->strip_length);
    elements_multiline_text_aligned(canvas, 64, 32, AlignCenter, AlignTop, buff);
}

static void gpio_neopixel_update(GpioNeopixelModel *model) {
    neopixel_clear();

    if (!model->on) {
        neopixel_update();
        return;
    }

    switch (model->effect) {
        case EffectStaticRed:
            for (uint8_t i = 0; i < model->strip_length; i++) {
                neopixel_set_color(i, 0x001000);
            }
            break;
        case EffectStaticGreen:
            for (uint8_t i = 0; i < model->strip_length; i++) {
                neopixel_set_color(i, 0x000010);
            }
            break;
        case EffectStaticBlue:
            for (uint8_t i = 0; i < model->strip_length; i++) {
                neopixel_set_color(i, 0x100000);
            }
            break;
        default:
            break;
    }
    neopixel_update();
}

static bool gpio_neopixel_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    GpioNeopixel* gpio_neopixel = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        if(event->key == InputKeyUp) {
            consumed = gpio_neopixel_process_up(gpio_neopixel);
        } else if(event->key == InputKeyDown) {
            consumed = gpio_neopixel_process_down(gpio_neopixel);
        } else if(event->key == InputKeyLeft) {
            consumed = gpio_neopixel_process_left(gpio_neopixel);
        } else if(event->key == InputKeyRight) {
            consumed = gpio_neopixel_process_right(gpio_neopixel);
        }
    } else if(event->key == InputKeyOk) {
        consumed = gpio_neopixel_process_ok(gpio_neopixel, event);
    }

    return consumed;
}

static bool gpio_neopixel_process_up(GpioNeopixel* gpio_neopixel) {
    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            model->effect += 1;
            if (model->effect == EffectLast) {
                model->effect = 0;
            }
            gpio_neopixel_update(model);
            return true;
        });
    return true;
}

static bool gpio_neopixel_process_down(GpioNeopixel* gpio_neopixel) {
    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            if (model->effect == 0) {
                model->effect = EffectLast;
            }
            model->effect -= 1;
            gpio_neopixel_update(model);
            return true;
        });
    return true;
}

static bool gpio_neopixel_process_left(GpioNeopixel* gpio_neopixel) {
    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            if (model->strip_length > 0) {
                model->strip_length -= 1;
            }
            gpio_neopixel_update(model);
            return true;
        });
    return true;
}

static bool gpio_neopixel_process_right(GpioNeopixel* gpio_neopixel) {
    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            if (model->strip_length < 8) {
                model->strip_length += 1;
            }
            gpio_neopixel_update(model);
            return true;
        });
    return true;
}

static bool gpio_neopixel_process_ok(GpioNeopixel* gpio_neopixel, InputEvent* event) {
    bool consumed = false;

    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            if(event->type == InputTypePress) {
                model->on = !model->on;
                consumed = true;
            }
            gpio_neopixel->callback(event->type, gpio_neopixel->context);
            return true;
        });

    return consumed;
}

GpioNeopixel* gpio_neopixel_alloc() {
    GpioNeopixel* gpio_neopixel = malloc(sizeof(GpioNeopixel));

    gpio_neopixel->view = view_alloc();
    view_allocate_model(gpio_neopixel->view, ViewModelTypeLocking, sizeof(GpioNeopixelModel));
    view_set_context(gpio_neopixel->view, gpio_neopixel);
    view_set_draw_callback(gpio_neopixel->view, gpio_neopixel_draw_callback);
    view_set_input_callback(gpio_neopixel->view, gpio_neopixel_input_callback);

    return gpio_neopixel;
}

void gpio_neopixel_free(GpioNeopixel* gpio_neopixel) {
    furi_assert(gpio_neopixel);
    view_free(gpio_neopixel->view);
    free(gpio_neopixel);
}

View* gpio_neopixel_get_view(GpioNeopixel* gpio_neopixel) {
    furi_assert(gpio_neopixel);
    return gpio_neopixel->view;
}

void gpio_neopixel_set_ok_callback(GpioNeopixel* gpio_neopixel, GpioNeopixelOkCallback callback, void* context) {
    furi_assert(gpio_neopixel);
    furi_assert(callback);
    with_view_model(
        gpio_neopixel->view, (GpioNeopixelModel * model) {
            UNUSED(model);
            gpio_neopixel->callback = callback;
            gpio_neopixel->context = context;
            return false;
        });
}
