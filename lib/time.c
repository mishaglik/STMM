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
    return micros_ * 200;
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
    delay_micro(50);
    micros_++;
}


void delay(uint32_t ms)
{
    if(ms < 1) return;
    for(uint32_t i = 0; i < ms; ++i)
    {
        for(uint32_t busy = 0; busy < CPU_FREQENCY / 1000 / 13; ++busy)
        {
            __asm__ volatile ("");
        }
    }
}

void delay_micro(uint32_t us)
{
    if(us) us--;
    for(uint32_t i = 0; i < us; ++i)
    {
        //48 - 13 = 35 
        __asm__ volatile("add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n" 
                         "add r1, #0\n"); 
    }
}
/*
delay_micro:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}  // 3 tick
	sub	sp, sp, #16   // 1 tick 
	add	r7, sp, #0    // 1 tick
	str	r0, [r7, #4]  // 2 tick
	movs	r3, #0    // 1 tick
	str	r3, [r7, #12] // 2 tick
	b	.L28          // 3 tick
    // TOTAL 17 ticks IN 
.L29:
	ldr	r3, [r7, #12]  // 2 tick
	adds	r3, r3, #1 // 1 tick 
	str	r3, [r7, #12]  // 2 tick
.L28:  
	ldr	r2, [r7, #12]  // 2 tick
	ldr	r3, [r7, #4]   // 2 tick
	cmp	r2, r3
	bcc	.L27
.L30:
	nop               // 1 tick
    nop               // 1 tick
    // 11 ticks OUT
	mov	sp, r7        // 1 tick
	add	sp, sp, #16   // 1 tick 
	@ sp needed
	pop	{r7, pc}      // 6 ticks
	.size	delay_micro, .-delay_micro
	.ident	"GCC: (Arch Repository) 12.2.0"
*/