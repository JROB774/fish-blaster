#ifndef EFFECT_H
#define EFFECT_H

typedef enum EffectID__
{
    EFX_GIB_BLOOD,
    EFX_BLOOD,
    EFX_GIB,
    EFX_BUBBLE,
    EFX_SHOT,
    EFX_TOTAL

} EffectID;

typedef struct Effect__
{
    EffectID type;
    int palette;
    int frame;
    float x,y,vx,vy;
    bool alive;
    float t;

} Effect;

#define EFFECT_MAX 2048

GLOBAL Effect gEffect[EFFECT_MAX];

INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count);
INTERNAL void update_effect (float dt);
INTERNAL void render_effect (float dt);

#endif /* EFFECT_H */
