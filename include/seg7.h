#ifndef _7SEG_H
#define _7SEG_H
#include "stdint.h"
#include "gpio.h"
struct Seg7Display
{
    uint32_t map[8 + 4];
    volatile GPIO* gpio;
    uint32_t display[4];
    uint32_t mask;
    uint8_t current;
    int enabled;
};

extern const uint8_t SEG7_ASCII[128];

uint32_t seg7_map_sym(uint8_t sym, uint8_t pos, const uint32_t map[]);

void seg7_init(struct Seg7Display* display, const uint32_t map[], volatile GPIO* gpio);

void seg7_set_num(struct Seg7Display* display, uint32_t num);

void seg7_set_num_up(struct Seg7Display* display, uint32_t num);

void seg7_set_num_dn(struct Seg7Display* display, uint32_t num);

void seg7_set_dot(struct Seg7Display* display, uint16_t dot);

void seg7_set_str(struct Seg7Display* display, char str[4]);

void seg7_fill(struct Seg7Display* display, uint8_t seg7_value);

void seg7_fill_up(struct Seg7Display* display, uint8_t seg7_value);

void seg7_fill_dn(struct Seg7Display* display, uint8_t seg7_value);

void seg7_update(struct Seg7Display* display);

#endif /* 7SEG_H */
