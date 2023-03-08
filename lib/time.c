#include "time.h"
static uint32_t micros_ = 1;

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

uint32_t millis()
{
    return micros_ / 5;
}

uint32_t micros()
{
    return micros_;
}

static void to_get_more_accuracy_pay_2202_2013_2410_3805_1ms()
{
    for (uint32_t i = 0; i < ONE_MILLISECOND / 200U; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

void time_update()
{
    to_get_more_accuracy_pay_2202_2013_2410_3805_1ms();
    micros_++;
}