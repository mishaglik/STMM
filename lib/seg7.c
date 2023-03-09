#include "seg7.h"
#include "common.h"
#include "stdint.h"
#include "gpio.h"
#include "time.h"

enum Seg7Pins
{
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    DP,
    P0,
    P1,
    P2,
    P3,
};


const uint8_t SEG7_ASCII[128] =
{
//       x0    x1    x2     x3       x4    x5    x6    x7        x8    x9    xA    xB        xC    xD    xE    xF
/*0x*/ 0x00, 0xFF, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
/*1x*/ 0x01, 0x02, 0x04, 0x08,     0x10, 0x20, 0x40, 0x80,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
/*2x*/ 0x00, 0x86, 0x22, 0x7E,     0x6D, 0xD2, 0x46, 0x20,     0x29, 0x0B, 0x21, 0x70,     0x10, 0x40, 0x80, 0x52, 
/*3x*/ 0x3F, 0x06, 0x5B, 0x4F,     0x66, 0x6D, 0x7D, 0x07,     0x7F, 0x6F, 0x09, 0x0D,     0x61, 0x48, 0x43, 0xD3,

/*4x*/ 0x5F, 0x77, 0x7C, 0x39,     0x5E, 0x79, 0x71, 0x3D,     0x76, 0x30, 0x1E, 0x75,     0x38, 0x15, 0x37, 0x3F, 
/*5x*/ 0x73, 0x6B, 0x33, 0x6D,     0x78, 0x3E, 0x3E, 0x2A,     0x76, 0x6E, 0x5B, 0x39,     0x64, 0x0F, 0x23, 0x08, 
/*6x*/ 0x02, 0x5F, 0x7C, 0x58,     0x5E, 0x7B, 0x71, 0x6F,     0x74, 0x10, 0x0C, 0x75,     0x06, 0x14, 0x54, 0x5C, 
/*7x*/ 0x73, 0x67, 0x50, 0x6D,     0x78, 0x1C, 0x1C, 0x14,     0x76, 0x6E, 0x5B, 0x46,     0x30, 0x70, 0x01, 0x00,
};

uint32_t seg7_map_sym(uint8_t sym, uint8_t pos, const uint32_t map[])
{
    if(pos >= 4)
    {
        return 0;
    }

    uint32_t ans = (map[P0] | map[P1] | map[P2] | map[P3]) & ~map[P0 + pos];
    for(uint32_t i = 0; sym; sym >>= 1, i++)
    {
        if(sym & 1) ans |= map[i];
    }
    return ans;
}

void seg7_init(struct Seg7Display* display, const uint32_t map[], GPIO gpio)
{
    display->current = 0;
    for(uint32_t i = 0; i < SIZEOF_ARR(display->map); ++i)
    {
        display->map[i] = (1u << map[i]);
        display->mask |=  (1u << map[i]);
        GPIOx_SET_MODE (gpio, map[i], GPIO_MODE_OUTPUT);
        GPIOx_SET_OTYPE(gpio, map[i], GPIO_OTYPE_PP);
    }
    display->gpio = gpio;
    display->lastUpdate = millis();
}

void seg7_set_num(struct Seg7Display* seg7, uint32_t number)
{
    for(uint32_t i = 0; i < 4; ++i)
    {
        seg7->display[i] = seg7_map_sym(SEG7_ASCII['0' + number % 10], i, seg7->map);
        number /= 10;
    }
}

void seg7_set_num_up(struct Seg7Display* seg7, uint32_t number)
{
    seg7->display[0] = seg7_map_sym(SEG7_ASCII['0' + number      % 10], 0, seg7->map);
    seg7->display[1] = seg7_map_sym(SEG7_ASCII['0' + number / 10 % 10], 1, seg7->map);
}

void seg7_set_num_dn(struct Seg7Display* seg7, uint32_t number)
{
    seg7->display[2] = seg7_map_sym(SEG7_ASCII['0' + number      % 10], 2, seg7->map);
    seg7->display[3] = seg7_map_sym(SEG7_ASCII['0' + number / 10 % 10], 3, seg7->map);
}

void seg7_set_dot(struct Seg7Display* seg7, uint16_t dot)
{
    for(uint32_t i = 0; i < 4; ++i)
    {
        seg7->display[i] &= ~seg7->map[DP];
    }
    if(dot < 4)
        seg7->display[dot] |= seg7->map[DP];
}

void seg7_set_str(struct Seg7Display* seg7, const char str[4])
{
    for(uint32_t i = 0; i < 4; ++i)
    {
        seg7->display[3 - i] = seg7_map_sym(SEG7_ASCII[str[i]], 3 - i, seg7->map);
        if(!str[i])
        {
            for(; i < 4; ++i)
                seg7->display[3 - i] = 0;
            return; 
        }
    }
}

void seg7_fill(struct Seg7Display* seg7, uint8_t seg7_value)
{
    for(uint32_t i = 0; i < 4; ++i)
    {
        seg7->display[i] = seg7_map_sym(seg7_value, i, seg7->map);
    }
}

void seg7_fill_up(struct Seg7Display* seg7, uint8_t seg7_value)
{
    for(uint32_t i = 0; i < 2; ++i)
    {
        seg7->display[i] = seg7_map_sym(seg7_value, i, seg7->map);
    }
}

void seg7_fill_dn(struct Seg7Display* seg7, uint8_t seg7_value)
{
    for(uint32_t i = 2; i < 4; ++i)
    {
        seg7->display[i] = seg7_map_sym(seg7_value, i, seg7->map);
    }
}

void seg7_update(struct Seg7Display* seg7)
{
    MODIFY_REG(GPIOA->ODR, seg7->mask, seg7->display[seg7->current++ & (4 - 1)]);
}
