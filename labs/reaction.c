#include <stdint.h>
#include <stdbool.h>    

#include "gpio.h"
#include "rcc.h"
#include "button.h"
#include "seg7.h"
//-------------------
// 7-segment display
//-------------------

const uint32_t Seg7Map[] =
{
    IO11, IO7, IO4, IO2,
    IO1, IO10, IO5, IO3,
    IO6, IO8, IO9, IO12,
};

// Display state:

//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

void board_clocking_init()
{
    
    // (1) Clock HSE and wait for oscillations to setup.
    REG_RCC->CR |= RCC_CR_HSE_ON;
    
    WAIT_FOR(REG_RCC->CR & RCC_CR_HSE_READY);
    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    // 2 - is div factor 
    // -1 is shift according std

    // (3) Select PREDIV output as PLL input (4 MHz):
    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    // (6) Configure AHB frequency to 48 MHz:
    // (8) Set APB frequency to 24 MHz

    REG_RCC->CFGR = RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL_VAL(12) | RCC_CFGR_HPRE_NO_DIVIDE | RCC_CFRG_PPRE_DIV_2;
    MODIFY_REG(REG_RCC->CFGR2, RCC_CFGR2_PREDIV_MASK, 2 - 1);

    // (5) Enable PLL:
    REG_RCC->CR |= RCC_CR_PLL_ON;
    WAIT_FOR(REG_RCC->CR & RCC_CR_PLL_READY);

    // (7) Select PLL as SYSCLK source:
    // Before it was 00 - HSI because of RESET state.
    REG_RCC->CFGR |= RCC_CFRG_SW_PLL;
    WAIT_FOR((REG_RCC->CFGR & RCC_CFRG_SWS_MASK) == RCC_CFRG_SWS_PLL);
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PA1-PA12 as output:
    REG_RCC->AHBENR |= RCC_AHBENR_IOPAEN;

    GPIOx_SET_MODE(GPIOA, 0, GPIO_MODE_INPUT);
    // for(uint32_t i = 1; i < 13; ++i)
    // {
        // GPIOx_SET_MODE(GPIOA, i, GPIO_MODE_OUTPUT);
        // GPIOx_SET_OTYPE(GPIOA, i, GPIO_OTYPE_PP);
    // }
  
    // Configure PA0 as pull-down pin:
    GPIOx_SET_PUPD(GPIOA, 0, GPIO_PUPD_PULLDOWN);
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
    struct Button button;
    board_clocking_init();

    board_gpio_init();

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
        // Render display state:
        // SEG7_set_number_quarter(&seg7, tick);

        // SEG7_push_display_state_to_mc(&seg7);

        seg7_update(&seg7);
        button_update(&button);

        // Adjust ticks every ms:
        to_get_more_accuracy_pay_2202_2013_2410_3805_1ms();
        tick++;

    }
}
