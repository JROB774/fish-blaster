#ifndef RENDERER_H
#define RENDERER_H

// The size of the "screen" we render to internally.
// This can be scaled up based on the window size.
#define SCREEN_W 160 // Width
#define SCREEN_H 144 // Height
#define SCREEN_S   3 // Scale

#define TILE_W 8
#define TILE_H 8

typedef U32 ARGBColor;

// Some useful macros for converting separate components into a U32 ARGB color.
#define MAKE_RGBA(r,g,b,a) (CAST(ARGBColor,(((b)<<24)|((g)<<16)|((r)<<8)|(a   ))))
#define MAKE_RGB( r,g,b)   (CAST(ARGBColor,(((b)<<24)|((g)<<16)|((r)<<8)|(0xFF))))

#define COLOR_WHITE   0xFFFFFFFF
#define COLOR_BLACK   0xFF000000
#define COLOR_RED     0xFFFF0000
#define COLOR_GREEN   0xFF00FF00
#define COLOR_BLUE    0xFF0000FF
#define COLOR_CYAN    0xFF00FFFF
#define COLOR_MAGENTA 0xFFFF00FF
#define COLOR_YELLOW  0xFFFFFF00

typedef struct Clip__ { int x, y, w, h; } Clip;

// INTERFACE

INTERNAL bool     init_renderer  ();
INTERNAL void     quit_renderer  ();
INTERNAL void     render_clear   (ARGBColor color);
INTERNAL void     render_display ();
INTERNAL void     render_bitmap  (int x, int y, int palette_index, const Clip* clip);
INTERNAL void     render_text    (int x, int y, int palette_index, const char* text, ...);
INTERNAL SDL_Rect get_viewport   ();

// PALETTES

#define PAL_TEXT_WHITE  0
#define PAL_TEXT_BLACK  1
#define PAL_TEXT_SHADOW 2
#define PAL_CURSOR      2
#define PAL_FISH_0      3
#define PAL_FISH_1      4
#define PAL_FISH_2      5

// SPRITES

GLOBAL const Clip SPR_CURSOR   = { 32, 104,  8, 8 };
GLOBAL const Clip SPR_FISH_R_0 = {  0,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_R_1 = { 16,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_L_0 = { 32,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_L_1 = { 48,   0, 16, 8 };

#endif /* RENDERER_H */
