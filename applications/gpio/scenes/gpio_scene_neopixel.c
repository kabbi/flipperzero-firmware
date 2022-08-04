#include "../gpio_app_i.h"

void gpio_scene_neopixel_ok_callback(InputType type, void* context) {
    furi_assert(context);
    GpioApp* app = context;

    if(type == InputTypePress) {
        notification_message(app->notifications, &sequence_set_green_255);
    } else if(type == InputTypeRelease) {
        notification_message(app->notifications, &sequence_reset_green);
    }
}

void gpio_scene_neopixel_on_enter(void* context) {
    GpioApp* app = context;
    neopixel_configure();
    gpio_neopixel_set_ok_callback(app->gpio_neopixel, gpio_scene_neopixel_ok_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, GpioAppViewGpioNeopixel);
}

bool gpio_scene_neopixel_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void gpio_scene_neopixel_on_exit(void* context) {
    UNUSED(context);
    // TODO: Unconfigure
}
