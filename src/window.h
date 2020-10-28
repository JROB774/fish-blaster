#ifndef WINDOW_H
#define WINDOW_H

GLOBAL struct
{
    SDL_Window* window;
    int cached_width;  // The size before being fullscreen.
    int cached_height; // The size before being fullscreen.
    bool running;
    bool fullscreen;

} gWindow;

INTERNAL bool init_window     ();
INTERNAL void quit_window     ();
INTERNAL void show_window     ();
INTERNAL void hide_window     ();

INTERNAL void set_fullscreen  (bool enable);
INTERNAL bool is_fullscreen   ();

INTERNAL void set_window_size (int width, int height);
INTERNAL int  get_window_w    ();
INTERNAL int  get_window_h    ();

INTERNAL int  get_window_cached_w ();
INTERNAL int  get_window_cached_h ();

#endif /* WINDOW_H */
