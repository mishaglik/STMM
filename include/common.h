#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#define PTR(x) ((volatile uint32_t*)(uintptr_t)(x))

#define SET_BIT(REG, BIT)     ((REG) |=  (1U << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))
#define READ_BIT(REG, BIT)    (((REG) >> BIT) & 1U)
#define ISSET_BIT(REG, BIT)   ((REG) & (1U << (BIT)))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define MODIFY_REG(REG, MODIFYMASK, VALUE) ((REG) = ((REG) & ~(MODIFYMASK)) | ((VALUE) & (MODIFYMASK)))
#define MODIFY_BIT(REG, BIT, VALUE) ((REG) = ((REG) & (1u << (BIT))) | ((VALUE) << (BIT)))

#define WAIT_FOR(x) do { while(!(x)) { __asm__ volatile ("nop"); } } while(0)

#define SIZEOF_ARR(x) (sizeof(x) / sizeof((x)[0]))

typedef uint32_t word;
typedef uint16_t hword;
typedef uint8_t  byte;

#endif /* COMMON_H */
