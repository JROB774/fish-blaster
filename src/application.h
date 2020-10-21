#ifndef APPLICATION_H
#define APPLICATION_H

typedef enum AppState__ { APP_STATE_GAME, APP_STATE_GAMEOVER, APP_STATE_TOTAL } AppState;

GLOBAL struct
{
    AppState state;
    int score;

} gApp;

INTERNAL bool   init_application ();
INTERNAL void   quit_application ();
INTERNAL void update_application (float dt);
INTERNAL void render_application (float dt);

// Switches the state of the game.
INTERNAL void start_game ();
INTERNAL void game_over  ();

#endif /* APPLICATION_H */
