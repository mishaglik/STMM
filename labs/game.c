#include "gpio.h"
#include "button.h"
#include "seg7.h"
#include "time.h"

#include "game.h"

#define GAME_GREETING_UPDATE_DELAY 1000
#define GAME_GREETING_TEXT "HELLO, GAMERS! Press any button to START. HELL"




static void game_greeting_init   (struct Game* game);
static void game_greetings_update(struct Game* game);
static void game_display_time   (struct Game* game);

static void game_button_handler(bool state, struct Button* button, void* data);

const struct PhaseF PhaseInfo[] =
{
    {game_greeting_init, game_greetings_update},
    {game_display_time , game_display_time},
};


static void game_greeting_init(struct Game* game)
{
    game->greetingData.offset = 0;
}

void game_init(struct Game* game, const uint32_t pin_map[], GPIO gpio)
{
    seg7_init(&game->display, pin_map, gpio);
    button_init(&game->gamer_button[0], gpio, pin_map[12]);
    button_init(&game->gamer_button[1], gpio, pin_map[13]);

    button_set_callback(&game->gamer_button[0], game_button_handler, game);
    button_set_callback(&game->gamer_button[1], game_button_handler, game);

    GPIOx_SET_MODE(gpio, pin_map[14], GPIO_MODE_OUTPUT);
    GPIOx_SET_OTYPE(gpio, pin_map[14], GPIO_OTYPE_OD);

    game->phase = 0;
    PhaseInfo[game->phase].init(game);
    game->last_update = millis();
}

void game_greetings_update(struct Game* game)
{
    if(millis() - game->last_update > GAME_GREETING_UPDATE_DELAY)
    {
        seg7_set_str(&game->display, GAME_GREETING_TEXT + game->greetingData.offset++); 
        if(game->greetingData.offset == sizeof(GAME_GREETING_TEXT) - 5)
        {
            game->greetingData.offset = 0;
        }
        game->last_update = millis();
    }
}


void game_update(struct Game* game)
{
    seg7_update(&game->display);
    button_update(&game->gamer_button[0]);
    button_update(&game->gamer_button[1]);

    PhaseInfo[game->phase].update(game);
}

static void game_display_time(struct Game* game)
{
    seg7_set_num(&game->display, millis() / 100);
}

static void game_button_handler(bool state, struct Button* button, void* data)
{
    struct Game* game = data;
    // uint32_t id = button - game->gamer_button;

    switch(game->phase)
    {
    case Greetings:
        if(state)
        {
            game->phase++;
            PhaseInfo[game->phase].init(game);
        }
    break;
    default:
    break;
    }
}