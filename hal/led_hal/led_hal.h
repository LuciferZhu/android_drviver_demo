#ifndef __LED_HAL_H__
#define __LED_HAL_H__

#include <hardware/hardware.h>

__BEGIN_DECLS

#define LED_HARDWARE_MODULE_ID "led_hal"

typedef struct led_module {
    struct hw_module_t common;
} led_module_t;

typedef struct led_device {
    struct hw_device_t common;
    int (*control)(int enable);
} led_device_t;

static inline int led_hal_open(const struct hw_module_t* module,
        led_device_t** device) {
    return module->methods->open(module, NULL,
            TO_HW_DEVICE_T_OPEN(device));
}

static inline int led_hal_close(led_device_t* device) {
    return device->common.close(&device->common);
}

__END_DECLS

#endif
