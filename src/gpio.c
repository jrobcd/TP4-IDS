/************************************************************************************************
Copyright (c) 2024, José Roberto Castro <jroberto.castrod@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file gpio.c
 ** @brief Definición de la función principal del programa
 **/

/* === Headers files inclusions =============================================================== */

#include "gpio.h"
#include "hal.h"
#include <stddef.h>
#include <stdlib.h>

/* === Macros definitions ====================================================================== */

#ifndef GPIO_MAX_INSTANCES
#define GPIO_MAX_INSTANCES 16
#endif

/* === Private data type declarations ========================================================== */

struct gpio_s {
    uint8_t port; // GPIO puerto
    uint8_t bit;  // GPIO bit
    bool output;  // output state
#ifdef USE_STATIC_MEM
    bool used;
#endif
};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

#ifdef USE_DYNAMIC_MEM
static gpio_t gpioAllocate(void) {
    static struct gpio_h instances[GPIO_MAX_INSTANCES] = {0};

    gpio_t self = NULL;
    for (int index = 0; index < GPIO_MAX_INSTANCES; index++) {
        if (!instances[index].used) {
            self = &instances[index];
            self->used = true;
            break;
        }
    }
    return self;
}
#endif

/* === Public function implementation ========================================================== */

gpio_t gpioCreate(uint8_t port, uint8_t bit) {
#ifdef USE_DYNAMIC_MEM
    gpio_t self = malloc(sizeof(struct gpio_s));
#else
    gpio_t self = allocateInstance();
#endif

    if (self) {
        self->port = port;
        self->bit = bit;
        self->output = false;
    }
    return self;
}

void gpioSetOutput(gpio_t self, bool output) {
    self->output = output;
    hal_gpio_set_direction(self->port, self->bit, output);
}

void gpioSetState(gpio_t self, bool state) {
    if (self->output) {
        hal_gpio_set_output(self->port, self->bit, state);
    }
}

bool gpioGetState(gpio_t self) {
    return hal_gpio_get_input(self->port, self->bit);
}

/* === End of documentation ==================================================================== */
