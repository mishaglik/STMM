#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include <gpio.h>
#include <seg7.h>
#include <button.h>

enum Phase
{
    Greetings = 0,
    End,
    Registration,
};

struct Score
{
    uint16_t first;
    uint16_t second;
};

struct GameGreetingsData
{
    uint32_t offset;
};

struct Game;

struct PhaseF
{
    void (*init)(struct Game*);
    void (*update)(struct Game*);
};

struct Game
{
    struct Seg7Display display;
    struct Button gamer_button[2];
    GPIO gpio;
    uint32_t led_pin;
    enum Phase phase;
    uint32_t last_update;
    union 
    {
        struct GameGreetingsData greetingData;
    };
    
};

#endif /* GAME_H */
