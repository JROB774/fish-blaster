#ifndef APPLICATION_H
#define APPLICATION_H

typedef enum AppState__ { APP_STATE_GAME, APP_STATE_GAMEOVER, APP_STATE_TOTAL } AppState;

#define CODE_LENGTH 5
#define CODE_RETRO "retro"
#define CODE_BLOOD "blood"

GLOBAL struct
{
    int cursor_x, cursor_prev_x;
    int cursor_y, cursor_prev_y;

    AppState state;
    int score;

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

// Switches the state of the game.
INTERNAL void start_game ();
INTERNAL void game_over  ();

#endif /* APPLICATION_H */
