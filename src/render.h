#ifndef RENDER_H
#define RENDER_H

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
INTERNAL void     render_bitmap  (int x,  int y,  int palette_index, const Clip* clip);
INTERNAL void     render_text    (int x,  int y,  int palette_index, const char* text, ...);
INTERNAL void     render_point   (int x,  int y,                  ARGBColor color);
INTERNAL void     render_line    (int x1, int y1, int x2, int y2, ARGBColor color);
INTERNAL void     render_rect    (int x,  int y,  int w,  int h,  ARGBColor color);
INTERNAL void     render_fill    (int x,  int y,  int w,  int h,  ARGBColor color);
INTERNAL SDL_Rect get_viewport   ();
INTERNAL int      get_text_w     (const char* text, ...);
INTERNAL int      get_text_h     (const char* text, ...);
INTERNAL void     shake_camera   (int x, int y, float duration);
INTERNAL void     update_camera  (float dt);
INTERNAL void     begin_camera   ();
INTERNAL void     end_camera     ();

// PALETTES

#define PAL_WHITE      0
#define PAL_BLACK      1
#define PAL_TEXT_WHITE 2
#define PAL_TEXT_BLACK 3
#define PAL_TEXT_SHADE 4
#define PAL_CURSOR     4
#define PAL_BLOOD      5
#define PAL_FISH_0     6
#define PAL_FISH_1     7
#define PAL_FISH_2     8
#define PAL_BUBBLE     8

// SPRITES

GLOBAL const Clip SPR_CURSOR_0 = {  0, 104,  8, 8 };
GLOBAL const Clip SPR_CURSOR_1 = {  8, 104,  8, 8 };
GLOBAL const Clip SPR_CURSOR_2 = { 16, 104,  8, 8 };
GLOBAL const Clip SPR_CURSOR_3 = { 24, 104,  8, 8 };
GLOBAL const Clip SPR_CURSOR_4 = { 32, 104,  8, 8 };
GLOBAL const Clip SPR_FISH_R_0 = {  0,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_R_1 = { 16,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_L_0 = { 32,   0, 16, 8 };
GLOBAL const Clip SPR_FISH_L_1 = { 48,   0, 16, 8 };
GLOBAL const Clip SPR_SCOREBGL = { 80,   0, 16, 8 };
GLOBAL const Clip SPR_SCOREBGR = { 80,   8, 16, 8 };
GLOBAL const Clip SPR_BUBBLE_0 = { 64,   0,  8, 8 };
GLOBAL const Clip SPR_BUBBLE_1 = { 72,   0,  8, 8 };
GLOBAL const Clip SPR_BUBBLE_2 = { 64,   8,  8, 8 };
GLOBAL const Clip SPR_BUBBLE_3 = { 72,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_0  = {  0,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_1  = {  8,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_2  = { 16,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_3  = { 24,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_4  = { 32,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_5  = { 40,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_6  = { 48,   8,  8, 8 };
GLOBAL const Clip SPR_BLOOD_7  = { 56,   8,  8, 8 };

// ANIMATIONS

GLOBAL const Clip* ANM_FISH_R[] = { &SPR_FISH_R_0, &SPR_FISH_R_1                                                                                      };
GLOBAL const Clip* ANM_FISH_L[] = { &SPR_FISH_L_0, &SPR_FISH_L_1                                                                                      };
GLOBAL const Clip* ANM_BUBBLE[] = { &SPR_BUBBLE_0, &SPR_BUBBLE_1, &SPR_BUBBLE_2, &SPR_BUBBLE_3                                                        };
GLOBAL const Clip* ANM_BLOOD [] = { &SPR_BLOOD_0,  &SPR_BLOOD_1,  &SPR_BLOOD_2,  &SPR_BLOOD_3, &SPR_BLOOD_4, &SPR_BLOOD_5, &SPR_BLOOD_6, &SPR_BLOOD_7 };

#endif /* RENDER_H */
