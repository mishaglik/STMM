#ifndef TIME_H
#define TIME_H
#include <stdint.h>

uint32_t millis(void);
uint32_t micros(void);


void delay(uint32_t ms);
void delay_micro(uint32_t us);

#endif /* TIME_H */
