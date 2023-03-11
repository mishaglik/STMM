#include <stdint.h>
#include <stdbool.h>    

#include "gpio.h"
#include "rcc.h"
#include "button.h"
#include "seg7.h"
#include "game.h"
//-------------------
// 7-segment display
//-------------------

const uint32_t Seg7Map[] =
{
    IO11, IO7 , IO4 , IO2,
    IO1 , IO10, IO5 , IO3,
    IO6 , IO8 , IO9 , IO12,
    IO13, IO14, IO0
};

void board_gpio_init()
{
    // (1) Configure PA1-PA12 as output:
    REG_RCC->AHBENR |= RCC_AHBENR_IOPAEN;
    REG_RCC->AHBENR |= RCC_AHBENR_IOPCEN;
    // GPIOx_SET_MODE(GPIOA, 0, GPIO_MODE_INPUT);
    // GPIOx_SET_PUPD(GPIOA, 0, GPIO_PUPD_PULLDOWN);
}

// bool ticking;

void stopticking(bool state, void* data)
{
    if(state) (*(bool*)data) ^= 1;
}

//------
// Main
//------
bool ticking = true;

int main()
{
    board_gpio_init();
    struct Game game;
    GPIOx_SET_PUPD(GPIOA, IO13, GPIO_PUPD_PULLDOWN);
    GPIOx_SET_PUPD(GPIOA, IO14, GPIO_PUPD_PULLDOWN);
    game_init(&game, Seg7Map, GPIOA);

    while(1)
    {
        game_update(&game);
        time_update();
    }
}
