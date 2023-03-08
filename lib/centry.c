#include <rcc.h>
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;

int main();

static void board_clocking_init()
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

void _premain(void)
{
    unsigned long *src, *dst;

    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
        *(dst++) = *(src++);

    board_clocking_init();
    main();
}