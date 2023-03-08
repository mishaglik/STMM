#include "button.h"

void button_init(struct Button* button, GPIO* gpio, uint32_t pin)
{
    button->gpio = gpio;
    button->pin = pin;
    GPIOx_SET_MODE(gpio, pin, GPIO_MODE_INPUT);
    button->state = GPIOx_GET_INPUT(gpio, pin) ? BUTTON_GIST_VALUE : -BUTTON_GIST_VALUE;
    button->callback = NULL;
}

void button_set_callback(struct Button* button, void (*callback)(bool, void*), void* arg)
{
    button->callback = callback;
    button->arg = arg;
}

bool button_get_value(struct Button* button)
{
    return button->state > 0;
}


void button_update(struct Button* button)
{
    int val = GPIOx_GET_INPUT(button->gpio, button->pin);
    if(val)
    {
        if(button->state == BUTTON_GIST_VALUE) return;
        button->state++;
        if(!button->state) {
            button->state = BUTTON_GIST_VALUE;
            if(button->callback) button->callback(true, button->arg);
        };
    }
    else
    {
        if(button->state == -BUTTON_GIST_VALUE) return;
        button->state--;
        if(!button->state) button->state = -BUTTON_GIST_VALUE;
            if(button->callback) button->callback(false, button->arg);
    }
}