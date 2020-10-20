#ifndef SEALIFE_H
#define SEALIFE_H

// FISH

#define FISH_SPEED 50
#define FISH_MAX 128

typedef struct Fish__
{
    float x,y;
    int palette;
    HDir dir;
    bool alive;
    float t;

} Fish;

GLOBAL Fish gEntityFish[FISH_MAX];

INTERNAL void create_fish ();
INTERNAL void update_fish (float dt);
INTERNAL void render_fish (float dt);

#endif /* SEALIFE_H */
