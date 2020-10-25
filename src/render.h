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

typedef struct Clip__ { int x, y, w, h; } Clip;

#define PAL_MODE_DEFAULT  0
#define PAL_MODE_SLOWDOWN 1
#define PAL_MODE_GAMEBOY  2
#define PAL_MODE_1BIT     3

// INTERFACE

INTERNAL bool      init_renderer     ();
INTERNAL void      quit_renderer     ();
INTERNAL void      render_clear      (ARGBColor color);
INTERNAL void      render_display    ();
INTERNAL void      render_bitmap     (int x,  int y,  int palette_index, const Clip* clip);
INTERNAL void      render_text       (int x,  int y,  int palette_index, const char* text, ...);
INTERNAL void      render_point      (int x,  int y,                         ARGBColor color);
INTERNAL void      render_line       (int x1, int y1, int x2, int y2,        ARGBColor color);
INTERNAL void      render_rect       (int x,  int y,  int w,  int h,         ARGBColor color);
INTERNAL void      render_fill       (int x,  int y,  int w,  int h,         ARGBColor color);
INTERNAL void      render_circle     (int x,  int y,  int r,  int thickness, ARGBColor color);
INTERNAL void      set_palette_mode  (int palette_mode);
INTERNAL SDL_Rect  get_viewport      ();
INTERNAL int       get_text_w        (const char* text, ...);
INTERNAL int       get_text_h        (const char* text, ...);
INTERNAL ARGBColor get_palette_color (int palette_index, int color_index);
INTERNAL void      shake_camera      (int x, int y, float duration);
INTERNAL void      update_camera     (float dt);
INTERNAL void      begin_camera      ();
INTERNAL void      end_camera        ();

// SPRITES

GLOBAL const Clip SPR_CURSOR_0   = {  0, 104,  8,  8 };
GLOBAL const Clip SPR_CURSOR_1   = {  8, 104,  8,  8 };
GLOBAL const Clip SPR_CURSOR_2   = { 16, 104,  8,  8 };
GLOBAL const Clip SPR_CURSOR_3   = { 24, 104,  8,  8 };
GLOBAL const Clip SPR_CURSOR_4   = { 32, 104,  8,  8 };
GLOBAL const Clip SPR_SCOREBGL   = { 96,   0, 16,  8 };
GLOBAL const Clip SPR_SCOREBGR   = { 96,   8, 16,  8 };
GLOBAL const Clip SPR_HEART      = {  0,  16,  8,  8 };
GLOBAL const Clip SPR_CRATE      = { 48,  16, 16, 16 };
GLOBAL const Clip SPR_FISH_R_0   = {  0,   0, 16,  8 };
GLOBAL const Clip SPR_FISH_R_1   = { 16,   0, 16,  8 };
GLOBAL const Clip SPR_FISH_L_0   = { 32,   0, 16,  8 };
GLOBAL const Clip SPR_FISH_L_1   = { 48,   0, 16,  8 };
GLOBAL const Clip SPR_JELLY_0    = { 64,  16, 16, 16 };
GLOBAL const Clip SPR_JELLY_1    = { 80,  16, 16, 16 };
GLOBAL const Clip SPR_URCHIN_0   = { 64,  32, 16, 16 };
GLOBAL const Clip SPR_URCHIN_1   = { 80,  32, 16, 16 };
GLOBAL const Clip SPR_BLOOD_0    = {  0,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_1    = {  8,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_2    = { 16,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_3    = { 24,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_4    = { 32,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_5    = { 40,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_6    = { 48,   8,  8,  8 };
GLOBAL const Clip SPR_BLOOD_7    = { 56,   8,  8,  8 };
GLOBAL const Clip SPR_SHOT_0     = {  0,  80,  8,  8 };
GLOBAL const Clip SPR_SHOT_1     = {  0,  88,  8,  8 };
GLOBAL const Clip SPR_SHOT_2     = {  0,  96,  8,  8 };
GLOBAL const Clip SPR_BUBBLE_0   = { 64,   0,  8,  8 };
GLOBAL const Clip SPR_BUBBLE_1   = { 72,   0,  8,  8 };
GLOBAL const Clip SPR_BUBBLE_2   = { 64,   8,  8,  8 };
GLOBAL const Clip SPR_BUBBLE_3   = { 72,   8,  8,  8 };
GLOBAL const Clip SPR_BONE_0     = { 80,   0,  8,  8 };
GLOBAL const Clip SPR_BONE_1     = { 88,   0,  8,  8 };
GLOBAL const Clip SPR_BONE_2     = { 80,   8,  8,  8 };
GLOBAL const Clip SPR_BONE_3     = { 88,   8,  8,  8 };
GLOBAL const Clip SPR_CHIP_0     = {  0,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_1     = {  8,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_2     = { 16,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_3     = { 24,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_4     = { 32,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_5     = { 40,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_6     = { 48,  40,  8,  8 };
GLOBAL const Clip SPR_CHIP_7     = { 56,  40,  8,  8 };
GLOBAL const Clip SPR_SCORE10    = {  0,  32, 16,  8 };
GLOBAL const Clip SPR_SCORE20    = { 16,  32, 16,  8 };
GLOBAL const Clip SPR_SCORE40    = { 32,  32, 16,  8 };
GLOBAL const Clip SPR_SCORE80    = { 48,  32, 16,  8 };
GLOBAL const Clip SPR_ICO_LIFE_0 = {  0,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_LIFE_1 = {  8,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_TIME_0 = { 16,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_TIME_1 = { 24,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_MULT_0 = { 32,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_MULT_1 = { 40,  16,  8,  8 };
GLOBAL const Clip SPR_ICO_RAPD_0 = {  0,  24,  8,  8 };
GLOBAL const Clip SPR_ICO_RAPD_1 = {  8,  24,  8,  8 };
GLOBAL const Clip SPR_ICO_SPRD_0 = { 16,  24,  8,  8 };
GLOBAL const Clip SPR_ICO_SPRD_1 = { 24,  24,  8,  8 };
GLOBAL const Clip SPR_ICO_BOOM_0 = { 32,  24,  8,  8 };
GLOBAL const Clip SPR_ICO_BOOM_1 = { 40,  24,  8,  8 };

// ANIMATIONS

GLOBAL const Clip* ANM_FISH_R  [] = { &SPR_FISH_R_0,     &SPR_FISH_R_1                                                                                       };
GLOBAL const Clip* ANM_FISH_L  [] = { &SPR_FISH_L_0,     &SPR_FISH_L_1                                                                                       };
GLOBAL const Clip* ANM_JELLY   [] = { &SPR_JELLY_0,      &SPR_JELLY_1                                                                                        };
GLOBAL const Clip* ANM_URCHIN  [] = { &SPR_URCHIN_0,     &SPR_URCHIN_1                                                                                       };
GLOBAL const Clip* ANM_BLOOD   [] = { &SPR_BLOOD_0,      &SPR_BLOOD_1,   &SPR_BLOOD_2,  &SPR_BLOOD_3, &SPR_BLOOD_4, &SPR_BLOOD_5, &SPR_BLOOD_6, &SPR_BLOOD_7 };
GLOBAL const Clip* ANM_SHOT    [] = { &SPR_SHOT_0,       &SPR_SHOT_1,    &SPR_SHOT_2                                                                         };
GLOBAL const Clip* ANM_BUBBLE  [] = { &SPR_BUBBLE_0,     &SPR_BUBBLE_1,  &SPR_BUBBLE_2, &SPR_BUBBLE_3                                                        };
GLOBAL const Clip* ANM_BONE    [] = { &SPR_BONE_0,       &SPR_BONE_1,    &SPR_BONE_2,   &SPR_BONE_3                                                          };
GLOBAL const Clip* ANM_CHIP    [] = { &SPR_CHIP_0,       &SPR_CHIP_1,    &SPR_CHIP_2,   &SPR_CHIP_3,  &SPR_CHIP_4,  &SPR_CHIP_5,  &SPR_CHIP_6,  &SPR_CHIP_7  };
GLOBAL const Clip* ANM_ICO_LIFE[] = { &SPR_ICO_LIFE_0,   &SPR_ICO_LIFE_1                                                                                     };
GLOBAL const Clip* ANM_ICO_TIME[] = { &SPR_ICO_TIME_0,   &SPR_ICO_TIME_1                                                                                     };
GLOBAL const Clip* ANM_ICO_MULT[] = { &SPR_ICO_MULT_0,   &SPR_ICO_MULT_1                                                                                     };
GLOBAL const Clip* ANM_ICO_RAPD[] = { &SPR_ICO_RAPD_0,   &SPR_ICO_RAPD_1                                                                                     };
GLOBAL const Clip* ANM_ICO_SPRD[] = { &SPR_ICO_SPRD_0,   &SPR_ICO_SPRD_1                                                                                     };
GLOBAL const Clip* ANM_ICO_BOOM[] = { &SPR_ICO_BOOM_0,   &SPR_ICO_BOOM_1                                                                                     };

// PALETTES

#define PAL_BACKGROUND   0
#define PAL_WHITE        1
#define PAL_BLACK        2
#define PAL_TEXT_SHADE   3
#define PAL_CURSOR       3
#define PAL_NOHEART      5
#define PAL_HEART        6
#define PAL_CRATE       10
#define PAL_FISH_0       6
#define PAL_FISH_1       7
#define PAL_FISH_2       8
#define PAL_JELLY        8
#define PAL_JELLYFLASH0 12
#define PAL_JELLYFLASH1 13
#define PAL_URCHIN       7
#define PAL_BLOOD_0      4
#define PAL_BLOOD_1      5
#define PAL_SHOT         3
#define PAL_BUBBLE       8
#define PAL_BONE         9
#define PAL_CHIP        10
#define PAL_ICO_LIFE     6
#define PAL_ICO_TIME     8
#define PAL_ICO_MULT    11
#define PAL_ICO_RAPD     7
#define PAL_ICO_SPRD     7
#define PAL_ICO_BOOM    12

#endif /* RENDER_H */
