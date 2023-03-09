#ifndef BLINKLED_MEMMAP_GPIO_H
#define BLINKLED_MEMMAP_GPIO_H
#include "common.h"


#define IO0   0
#define IO1   1
#define IO2   2
#define IO3   3
#define IO4   4
#define IO5   5
#define IO6   6
#define IO7   7
#define IO8   8
#define IO9   9
#define IO10 10
#define IO11 11
#define IO12 12
#define IO13 13
#define IO14 14
#define IO15 15

#pragma pack(1)
struct GPIO
{
    word MODER;
    word OTYPER;
    word OSPEEDR;
    word PUPDR;
    word IDR;
    word ODR;
    word BSRR;
    word LCKR;
    word AFRL;
    word AFRH;
    word BRR;
};
#pragma pack(0)

// typedef struct GPIO GPIO;
typedef volatile struct GPIO* GPIO;


/**
 * @brief GPIO Registers.
 * 
 */
#define GPIOA ((volatile struct GPIO*)(uintptr_t)(0x48000000U)) 
#define GPIOB ((volatile struct GPIO*)(uintptr_t)(0x48000400U)) 
#define GPIOC ((volatile struct GPIO*)(uintptr_t)(0x48000800U)) 
#define GPIOD ((volatile struct GPIO*)(uintptr_t)(0x48000C00U)) 
#define GPIOE ((volatile struct GPIO*)(uintptr_t)(0x48001000U)) 
#define GPIOF ((volatile struct GPIO*)(uintptr_t)(0x48001400U)) 

// Moder types
#define GPIO_MODE_INPUT  0b00
#define GPIO_MODE_OUTPUT 0b01
#define GPIO_MODE_ALTF   0b10
#define GPIO_MODE_ANALOG 0b11

#define GPIO_OTYPE_PP 0 //Push-pull
#define GPIO_OTYPE_OD 1 //Open-drain

#define GPIO_OSPEED_LOW    0b00
#define GPIO_OSPEED_MEDIUM 0b10
#define GPIO_OSPEED_HIGH   0b11

#define GPIO_PUPD_NONE     0b00
#define GPIO_PUPD_PULLUP   0b01
#define GPIO_PUPD_PULLDOWN 0b10

#define GPIOx_SET_MODE(GPIO, PORT, MODE)    MODIFY_REG((GPIO)->MODER, (0b11) << 2*(PORT), (MODE) << 2*(PORT))
#define GPIOx_SET_OTYPE(GPIO, PORT, OTYPE)  MODIFY_BIT((GPIO)->OTYPER, PORT, OTYPE)
#define GPIOx_SET_OSPEED(GPIO, PORT, SPEED) MODIFY_REG((GPIO)->OSPEEDR, (0b11) << 2*(PORT), (SPEED) << 2*(PORT))
#define GPIOx_SET_PUPD(GPIO, PORT, PUPD)    MODIFY_REG((GPIO)->PUPDR, (0b11) << 2*(PORT), (PUPD)  << 2*(PORT))
#define GPIOx_SET_OUTPUT(GPIO, PORT, VALUE) ((GPIO)->BSRR |= ((VALUE) ? 1 : 0x10000U) << PORT)
#define GPIOx_SET_OUT_HIGH(GPIO, PORT)      ((GPIO)->BSRR = (1U << (PORT)))
#define GPIOx_SET_OUT_LOW(GPIO, PORT )      ((GPIO)->BSRR = (1U << ((PORT) + 16)))
#define GPIOx_GET_INPUT(GPIO, PORT)         READ_BIT((GPIO)->IDR, (PORT))


//-------------------------------- GPIO COPYPASTE ------------------------------------------

#endif /* 01_BLINKLED_MEMMAP_GPIO_H */
