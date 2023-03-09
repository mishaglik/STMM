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
    IO13, IO14, IO15, IO0
};

void board_gpio_init()
{
    // (1) Configure PA1-PA12 as output:
    REG_RCC->AHBENR |= RCC_AHBENR_IOPAEN;
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
/*
    struct Button button;
    button_init(&button, GPIOA, IO0);
    // GPIOx_SET_MODE(GPIOA, IO0, GPIO_MODE_INPUT);

    button_set_callback(&button, stopticking, &ticking);

    // Init display rendering:
    struct Seg7Display seg7;

    seg7_init(&seg7, Seg7Map, GPIOA);
    seg7_fill(&seg7, 0xFF);
    seg7_update(&seg7);

    uint32_t tick = 0;
    uint32_t number = 0;
    while (1)
    {
        button_update(&button);
        // Update display state:
        if (ticking && (tick % 500U) == 0U)
        {
            if (number < 1000U)
            {
                seg7_set_num(&seg7, number);
                // seg7_fill(&seg7, SEG7_ASCII[0x10 + (number % 6)]);
                seg7_set_str(&seg7, "HELLO, WORLD!  HEL" + number % 14);
                number++;
            } else {
                ticking = false;
            }
        }
        if(!ticking)
        {
            // seg7_set_str(&seg7, "\x01\x01\x01\x01");
        }

        seg7_update(&seg7);
        button_update(&button);

        // Adjust ticks every ms:
        void time_update(void);
        time_update();
        tick++;
    }
    */
}
