#ifndef INC_BUTTON_H
#define INC_BUTTON_H
#include <stdbool.h>
#include <stddef.h>
#include "gpio.h"

#define BUTTON_GIST_VALUE 5

struct Button
{
    GPIO* gpio;
    uint32_t pin;
    int32_t state;
    void (*callback)(bool, void*);
    void* arg;
};

void button_init(struct Button* button, GPIO* gpio, uint32_t pin);
void button_set_callback(struct Button* button, void (*callback)(bool, void*), void* arg);
bool button_get_value(struct Button* button);
void button_update(struct Button* button);
#endif /* INC_BUTTON_H */
