#ifndef WINDOW_H
#define WINDOW_H

GLOBAL struct
{
    SDL_Window* window;
    int width, height;

} gWindow;

INTERNAL void init_window ();
INTERNAL void quit_window ();
INTERNAL void show_window ();
INTERNAL void hide_window ();

#endif /* WINDOW_H */
