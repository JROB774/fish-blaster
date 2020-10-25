#ifndef EFFECT_H
#define EFFECT_H

typedef enum EffectID__
{
    EFX_GIB_BLOOD,
    EFX_BLOOD,
    EFX_GIB,
    EFX_CHIP_0,
    EFX_CHIP_1,
    EFX_BUBBLE,
    EFX_SHOT,
    EFX_SCORE10,
    EFX_SCORE20,
    EFX_SCORE40,
    EFX_SCORE80,
    EFX_ICO_LIFE,
    EFX_ICO_TIME,
    EFX_ICO_MULT,
    EFX_ICO_RAPD,
    EFX_ICO_SPRD,
    EFX_ICO_BOOM,
    EFX_TOTAL

} EffectID;

typedef struct Effect__
{
    EffectID type;
    int palette;
    int frame;
    float x,y,vx,vy;
    bool alive;
    bool invis;
    float t;

} Effect;

#define EFFECT_MAX 4096

GLOBAL Effect gEffect[EFFECT_MAX];

INTERNAL void create_effect    (EffectID id, int x, int y, int w, int h, int min_count, int max_count);
INTERNAL void update_effect    (float dt);
INTERNAL void render_effect_lo (float dt); // Drawn before entities.
INTERNAL void render_effect_hi (float dt); // Drawn after entities.

#endif /* EFFECT_H */
