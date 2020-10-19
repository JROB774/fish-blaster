#ifndef RENDERER_H
#define RENDERER_H

// @INCOMPLETE: IMPLEMENT THE NEW PALETTE SYSTEM WHERE ITS AN IMAGE FILE INSTEAD...

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
#define MAKE_RGBA(r,g,b,a) (CAST(ARGBColor,(((b)<<24)|((g)<<16)|((r)<<8)|(a   ))))
#define MAKE_RGB( r,g,b)   (CAST(ARGBColor,(((b)<<24)|((g)<<16)|((r)<<8)|(0xFF))))

typedef struct Bitmap__
{
    int w,h;
    int* pixels; // Indicies [0-3] into a 4-color palette specified on render.

} Bitmap;

typedef struct Clip__
{
    int x, y, w, h;

} Clip;

typedef struct Font__
{
    Bitmap bitmap;
    Clip glyphs[128];
    int glyph_w;
    int glyph_h;
} Font;

// The size of the "screen" we render to internally.
// This can be scaled up based on the window size.
#define SCREEN_W 160 // Width
#define SCREEN_H 144 // Height
#define SCREEN_S   3 // Scale

GLOBAL struct
{
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  target;
    SDL_Rect      viewport;

} gRenderer;

// GENERAL INTERFACE

INTERNAL void init_renderer ();
INTERNAL void quit_renderer ();

// BITMAP INTEFACE

// IMPORTANT: We only support 4-bit bitmaps (right now?) attempting to load any other will fail an assertion!
INTERNAL void load_bitmap_from_file (Bitmap* bitmap, const char* file_name);
INTERNAL void free_bitmap           (Bitmap* bitmap);

// FONT INTERFACE

INTERNAL void load_font_from_file (Font* font, int gw, int gh, const char* file_name);
INTERNAL void free_font           (Font* font);

// RENDER INTERFACE

INTERNAL SDL_Rect get_viewport ();
INTERNAL ARGBColor* get_screen ();

INTERNAL void render_clear   (ARGBColor color);
INTERNAL void render_display ();

INTERNAL void render_bitmap  (Bitmap* bitmap, int x, int y, const ARGBColor palette[4], Clip* clip);

INTERNAL void render_point   (int x,  int y,                  ARGBColor color);
INTERNAL void render_line    (int x1, int y1, int x2, int y2, ARGBColor color);
INTERNAL void render_rect    (int x,  int y,  int w,  int h,  ARGBColor color);
INTERNAL void render_fill    (int x,  int y,  int w,  int h,  ARGBColor color);

#endif /* RENDERER_H */
