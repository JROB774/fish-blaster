#ifndef APPLICATION_H
#define APPLICATION_H

#define APP_VERSION_MAJOR 1
#define APP_VERSION_MINOR 0
#define APP_VERSION_PATCH 1

typedef enum  AppState__ { APP_STATE_MENU, APP_STATE_GAME, APP_STATE_LOSE, APP_STATE_TOTAL                                                }  AppState;
typedef enum MenuState__ { MENU_STATE_MAIN, MENU_STATE_OPTIONS, MENU_STATE_RESET, MENU_STATE_SCORES, MENU_STATE_CREDITS, MENU_STATE_TOTAL } MenuState;

#define CODE_LENGTH 5
#define CODE_RETRO "gameb"
#define CODE_BLOOD "blood"
#define CODE_NOPAL "nopal"

#define MAX_SCORE 999999

#define LOSE_COOLDOWN 0.25f

GLOBAL struct
{
    AppState state;
    MenuState menu_state;

    int score;
    int frame;

    MusicID current_track;

    char code[CODE_LENGTH];
    int code_length;

    bool code_retro_enabled;
    bool code_blood_enabled;
    bool code_nopal_enabled;

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
INTERNAL void start_lose ();

// Sets and plays the desired music track.
INTERNAL void set_music_track (MusicID track);

#endif /* APPLICATION_H */
