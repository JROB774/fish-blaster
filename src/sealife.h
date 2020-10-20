#ifndef SEALIFE_H
#define SEALIFE_H

// SPAWNER

typedef struct Spawner__
{
    float t_fish;

} Spawner;

GLOBAL Spawner gSpawner;

INTERNAL void create_spawner ();
INTERNAL void update_spawner (float dt);

// FISH

#define FISH_SPAWN_START 0.0f
#define FISH_SPAWN_RATE  0.5f
#define FISH_MAX   128
#define FISH_SPEED 50

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
