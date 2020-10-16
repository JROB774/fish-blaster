#ifndef RENDERER_H
#define RENDERER_H

// The size of the "screen" we render to internally.
// This can be scaled up based on the window size.
#define SCREEN_W 256
#define SCREEN_H 240

GLOBAL struct
{
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  target;

} gRenderer;

INTERNAL void    init_renderer ();
INTERNAL void    quit_renderer ();
INTERNAL void   clear_renderer (U32 color);
INTERNAL void display_renderer ();

#endif /* RENDERER_H */
