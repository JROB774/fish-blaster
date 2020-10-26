#ifndef APPLICATION_H
#define APPLICATION_H

typedef enum AppState__ { APP_STATE_MENU, APP_STATE_GAME, APP_STATE_GAMEOVER, APP_STATE_TOTAL } AppState;

#define CODE_LENGTH 5
#define CODE_RETRO "retro"
#define CODE_BLOOD "blood"
#define CODE_1BITS "1bits"

#define GAMEOVER_COOLDOWN 0.25f

GLOBAL struct
{
    AppState state;

    int score;
    int frame;

    char code[CODE_LENGTH];
    int code_length;

    bool code_retro_enabled;
    bool code_blood_enabled;
    bool code_1bits_enabled;

    bool flash_white;

} gApp;

INTERNAL bool   init_application ();
INTERNAL void   quit_application ();
INTERNAL void handle_application (SDL_Event* event);
INTERNAL void update_application (float dt);
INTERNAL void render_application (float dt);

// Not reaally sure where to put this?
INTERNAL void flash_screen_white ();

// Switches the state of the game.
INTERNAL void start_menu ();
INTERNAL void start_game ();
INTERNAL void game_over  ();

#endif /* APPLICATION_H */
