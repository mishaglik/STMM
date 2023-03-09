#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include <gpio.h>
#include <seg7.h>
#include <button.h>

enum Phase
{
    Greetings = 0,
    ReadySetGo,
    Fight,
    Scores,
    Finale,
    End,
    Registration,
};

struct GameGreetingsData
{
    uint32_t offset;
};

struct GameRSGData
{
    uint32_t idx;
};

struct GameFightData
{
    uint32_t winner;
};

struct GameScoresData
{
    struct GameFightData fd;
    uint32_t loop;
};

struct GameFinaleData
{
    uint32_t animation_start;
    uint32_t animation_loop;
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
    uint16_t score[2];
    union 
    {
        struct GameGreetingsData greetingData;
        struct GameRSGData rsgData;
        struct GameFightData fightData;
        struct GameScoresData scoresData;
        struct GameFinaleData finaleData;
    };
};

#endif /* GAME_H */
