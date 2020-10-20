#ifndef EFFECT_H
#define EFFECT_H

typedef enum EffectID__
{
    EFX_BUBBLE,
    EFX_TOTAL

} EffectID;

typedef struct Effect__
{
    EffectID type;
    const Clip* clip;
    int palette;
    float x,y,vx,vy;
    bool alive;
    float t;

} Effect;

#define EFFECT_MAX 128

GLOBAL Effect gEffect[EFFECT_MAX];

INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count);
INTERNAL void update_effect (float dt);
INTERNAL void render_effect (float dt);

#endif /* EFFECT_H */
