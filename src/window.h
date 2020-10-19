#ifndef WINDOW_H
#define WINDOW_H

GLOBAL struct
{
    SDL_Window* window;
    bool running;
    bool fullscreen;

} gWindow;

INTERNAL bool init_window    ();
INTERNAL void quit_window    ();
INTERNAL void show_window    ();
INTERNAL void hide_window    ();

INTERNAL void set_fullscreen (bool enable);
INTERNAL bool is_fullscreen  ();

INTERNAL int  get_window_w   ();
INTERNAL int  get_window_h   ();

#endif /* WINDOW_H */
