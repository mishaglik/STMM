#include "gpio.h"
#include "button.h"
#include "seg7.h"
#include "time.h"

#include "game.h"

#define GAME_GREETING_UPDATE_DELAY 1000
#define GAME_GREETING_TEXT "HELLO, GAMERS! Press any button to START. HELL"
#define GAME_FIGHT_PRE_MS 10000

#define GAME_SCORES_WINNER_DELAY_MS 500
#define GAME_SCORES_LOSER_DELAY_MS 1000

#define GAME_PLAY_UNTILL 2

struct TimedText
{
    const char* str;
    uint32_t time;
};

static const struct TimedText GAME_RSD_TTEXT[] =
{
    {"FAKE", 1000},
    {"",     1000},
    {"FAKE", 1000},
    {"",     1000},
    {"FAKE", 1000},
    {"",     1000},
    {"FAKE", 1000},
    {"RDY!", 5000},
    {"",     2000},
    {"SET!", 3000},
    {"",     1000},
    {"GO !",    0},
};

#define GAME_RSG_START 7

#define GAME_SCORES_LOOPS 10
#define GAME_SCORES_DELAY 1000

#define GAME_FINALE_ANIM_DELAY 300
#define GAME_FINALE_EAT_DELAY 10000

static void game_greeting_init   (struct Game* game);
static void game_greetings_update(struct Game* game);
static void game_rsg_init        (struct Game* game);
static void game_rsg_update      (struct Game* game);
static void game_fight_init      (struct Game* game);
static void game_fight_update    (struct Game* game);
static void game_scores_init     (struct Game* game);
static void game_scores_update   (struct Game* game);
static void game_looper_init     (struct Game* game);
static void game_looper_update   (struct Game* game);
static void game_display_time    (struct Game* game);

static void game_button_handler(bool state, struct Button* button, void* data);

const struct PhaseF PhaseInfo[] =
{
    {game_greeting_init, game_greetings_update},
    {game_rsg_init,      game_rsg_update},
    {game_fight_init,    game_fight_update},
    {game_scores_init,   game_scores_update},
    {game_looper_init,   game_looper_update},
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

    game->led_pin = pin_map[14];
    GPIOx_SET_MODE (gpio, game->led_pin, GPIO_MODE_OUTPUT);
    GPIOx_SET_OTYPE(gpio, game->led_pin, GPIO_OTYPE_PP);
    GPIOx_SET_PUPD (gpio, game->led_pin, GPIO_PUPD_NONE);

    GPIOx_SET_MODE (GPIOC, IO8, GPIO_MODE_OUTPUT);
    GPIOx_SET_OTYPE(GPIOC, IO8, GPIO_OTYPE_PP);

    GPIOx_SET_MODE (GPIOC, IO9, GPIO_MODE_OUTPUT);
    GPIOx_SET_OTYPE(GPIOC, IO9, GPIO_OTYPE_PP);

    game->phase = 0;
    game->gpio = gpio;
    PhaseInfo[game->phase].init(game);
    game->last_update = millis();

    game->score[0] = game->score[1] = 0;
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
    seg7_set_dot(&game->display, 1);
}

static void game_button_handler(bool state, struct Button* button, void* data)
{
    struct Game* game = data;
    uint32_t id = button - game->gamer_button;

    switch(game->phase)
    {
    case Greetings:
        if(!state && !button_get_value(game->gamer_button + 0) && !button_get_value(game->gamer_button + 1))
        {
            game->phase++;
            PhaseInfo[game->phase].init(game);
        }
    break;
    case ReadySetGo:
        if(state)
        {
            game->rsgData.idx = 0;
            game->last_update = 0;
        }
    break;
    case Fight:
        if(state)
        {
            game->score[1 - id]++;
            game->fightData.winner = 1 - id;
            game->phase++;
            PhaseInfo[game->phase].init(game);
        }
    default:
    break;
    }
}

static void game_show_score(struct Game* game)
{
    seg7_set_num_up(&game->display, game->score[0]);
    seg7_set_num_dn(&game->display, game->score[1]);
}

static void game_rsg_init   (struct Game* game)
{
    seg7_set_str(&game->display, GAME_RSD_TTEXT[GAME_RSG_START].str);
    game->rsgData.idx = GAME_RSG_START;
    game->last_update = millis();
}

static void game_timed_text_update(struct Game* game, struct TimedText ttext[], uint32_t* idx)
{
    if(millis() - game->last_update > ttext[*idx].time)
    {
        (*idx)++;
        seg7_set_str(&game->display, ttext[*idx].str);
        game->last_update = millis();
    }
}

static void game_rsg_update (struct Game* game)
{
    if(game->rsgData.idx > GAME_RSG_START && (button_get_value(game->gamer_button + 0) || button_get_value(game->gamer_button + 1)))
    {
        game->rsgData.idx = GAME_RSG_START;
        seg7_set_str(&game->display, GAME_RSD_TTEXT[GAME_RSG_START].str);
        game->last_update = millis();
    }

    game_timed_text_update(game, GAME_RSD_TTEXT, &game->rsgData.idx);
    
    if(game->rsgData.idx == SIZEOF_ARR(GAME_RSD_TTEXT) - 1)
    {
        game->phase++;
        PhaseInfo[game->phase].init(game);
    }
}

static void game_fight_init   (struct Game* game)
{
    GPIOx_SET_OUT_HIGH(game->gpio, game->led_pin);
    game->last_update = millis();
}

static void game_fight_update (struct Game* game)
{
    if(millis() - game->last_update > GAME_FIGHT_PRE_MS)
    {
        GPIOx_SET_OUT_LOW(game->gpio, game->led_pin);
        game_show_score(game);
        game->last_update = millis();
    }
}

static void game_scores_init   (struct Game* game)
{
    GPIOx_SET_OUT_LOW(game->gpio, game->led_pin);
    game_show_score(game);
    game->scoresData.loop = 0;
    game->scoresData.led1state = game->scoresData.led2state = 0;
    game->scoresData.led1upd = game->scoresData.led2upd = millis();
}

static void game_scores_update (struct Game* game)
{
    if(millis() - game->last_update > GAME_SCORES_DELAY)
    {
        game->scoresData.loop++;
        if(game->scoresData.loop & 1)
        {
            (game->scoresData.fd.winner ? seg7_fill_dn : seg7_fill_up)(&game->display, 0);
        }
        else
        {
            game_show_score(game);
        }

        if(game->scoresData.loop == GAME_SCORES_LOOPS)
        {
            GPIOx_SET_OUT_LOW(GPIOC, IO8);
            GPIOx_SET_OUT_LOW(GPIOC, IO9);
            game->phase++;
            PhaseInfo[game->phase].init(game);
        }

        game->last_update = millis();
    }

    if(millis() - game->scoresData.led1upd > (game->scoresData.fd.winner ? GAME_SCORES_WINNER_DELAY_MS : GAME_SCORES_LOSER_DELAY_MS))
    {
        game->scoresData.led1state ^= 1;
        GPIOx_SET_OUTPUT(GPIOC, IO8, game->scoresData.led1state);
        game->scoresData.led1upd = millis();
    }

    if(millis() - game->scoresData.led2upd > (!game->scoresData.fd.winner ? GAME_SCORES_WINNER_DELAY_MS : GAME_SCORES_LOSER_DELAY_MS))
    {
        game->scoresData.led2state ^= 1;
        GPIOx_SET_OUTPUT(GPIOC, IO9, game->scoresData.led2state);
        game->scoresData.led2upd = millis();
    }
}

static void game_looper_init   (struct Game* game)
{
    if(game->score[0] < GAME_PLAY_UNTILL && game->score[1] < GAME_PLAY_UNTILL)
    {
        game->phase = ReadySetGo;
        PhaseInfo[game->phase].init(game);
        return;
    }

    game->finaleData.animation_start = millis();
    game->finaleData.animation_loop  = 0;

}

static uint8_t GAME_ANIMATION[] = 
{
    // 0x01, 0x02, 0x40, 0x10, 0x08, 0x04, 0x40, 0x20
    // 0x21, 0x03, 0x42, 0x50, 0x18, 0x0C, 0x44, 0x60
    // 0x61, 0x23, 0x43, 0x52, 0x58, 0x1C, 0x4C, 0x64
    // 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x03, 0x06, 0x0C, 0x18, 0x30, 0x21,
};

static void game_looper_update (struct Game* game)
{
    if(millis() - game->last_update > GAME_FINALE_ANIM_DELAY)
    {
        game_show_score(game);
        uint32_t eaten = (millis() - game->finaleData.animation_start) / GAME_FINALE_EAT_DELAY;
        if(eaten > 4)
        {
            game->phase++;
            PhaseInfo[game->phase].init(game);
            return;
        }

        for(uint32_t i = 0; i < eaten; ++i)
        {
            game->display.display[3-i] = seg7_map_sym(GAME_ANIMATION[game->finaleData.animation_loop % SIZEOF_ARR(GAME_ANIMATION)], 3-i, &game->display.map);
        }

        game->finaleData.animation_loop++;
        game->last_update = millis();
    }
}