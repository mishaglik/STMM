#include "time.h"

static uint32_t millis_ = 0;

uint32_t millis()
{
    return millis_;
}

static void to_get_more_accuracy_pay_2202_2013_2410_3805_1ms()
{
    for (uint32_t i = 0; i < ONE_MILLISECOND/3U; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

void time_update()
{
    to_get_more_accuracy_pay_2202_2013_2410_3805_1ms();
    millis_++;
}