#ifndef APPLICATION_H
#define APPLICATION_H

typedef enum AppState__ { APP_STATE_GAME, APP_STATE_GAMEOVER, APP_STATE_TOTAL } AppState;

#define CODE_LENGTH 5
#define CODE_RETRO "retro"
#define CODE_BLOOD "blood"

#define GAMEOVER_COOLDOWN 0.25f

#define MAX_LIFE 2

#define GOD_DURATION  1.00f
#define ITEM_DURATION 9.99f

typedef enum ItemID__
{
    ITEM_NONE,
    ITEM_TIME,
    ITEM_MULT,
    ITEM_SPRD,
    ITEM_RAPD

} ItemID;

GLOBAL struct
{
    int cursor_x, cursor_prev_x;
    int cursor_y, cursor_prev_y;

    AppState state;

    ItemID current_item;
    float item_time;

    float shoot_cooldown;

    int score;
    int life;
    int frame;

    float god_time;

    char code[CODE_LENGTH];
    int code_length;

    bool code_retro_enabled;
    bool code_blood_enabled;

} gApp;

INTERNAL bool   init_application ();
INTERNAL void   quit_application ();
INTERNAL void handle_application (SDL_Event* event);
INTERNAL void update_application (float dt);
INTERNAL void render_application (float dt);

INTERNAL void cursor_hit ();

// Switches the state of the game.
INTERNAL void start_game ();
INTERNAL void game_over  ();

#endif /* APPLICATION_H */
