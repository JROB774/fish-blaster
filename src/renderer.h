#ifndef RENDERER_H
#define RENDERER_H

#define COLOR_WHITE   0xFFFFFFFF
#define COLOR_BLACK   0xFF000000
#define COLOR_RED     0xFFFF0000
#define COLOR_GREEN   0xFF00FF00
#define COLOR_BLUE    0xFF0000FF
#define COLOR_CYAN    0xFF00FFFF
#define COLOR_MAGENTA 0xFFFF00FF
#define COLOR_YELLOW  0xFFFFFF00

typedef U32 ARGBColor;

// Some useful macros for converting separate components into a U32 ARGB color.
#define MAKE_RGBA(r,g,b,a) (CAST(U32,(((b)<<24)|((g)<<16)|((r)<<8)|(a   ))))
#define MAKE_RGB( r,g,b)   (CAST(U32,(((b)<<24)|((g)<<16)|((r)<<8)|(0xFF))))

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
INTERNAL void   clear_renderer (ARGBColor color);
INTERNAL void display_renderer ();

#endif /* RENDERER_H */
