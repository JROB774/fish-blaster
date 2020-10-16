#ifndef WINDOW_H
#define WINDOW_H

GLOBAL struct
{
    SDL_Window* window;

} gWindow;

INTERNAL void init_window ();
INTERNAL void quit_window ();
INTERNAL void show_window ();
INTERNAL void hide_window ();

INTERNAL int get_window_width  ();
INTERNAL int get_window_height ();

#endif /* WINDOW_H */
