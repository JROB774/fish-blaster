#ifndef ENTITY_H
#define ENTITY_H

// COLLISION

INTERNAL bool point_vs_rect_collision (float px, float py, float rx, float ry, float rw, float rh);
INTERNAL bool rect_vs_rect_collision  (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh);

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
#define FISH_ANIM_SPEED  0.2f
#define FISH_MAX   128
#define FISH_SPEED 50
#define FISH_SCORE 10

typedef struct Fish__
{
    float x,y;
    int palette;
    int frame;
    HDir dir;
    bool alive;
    float t;

} Fish;

GLOBAL Fish gEntityFish[FISH_MAX];

INTERNAL void  create_fish ();
INTERNAL void  update_fish (float dt);
INTERNAL void  render_fish (float dt);
INTERNAL void collide_fish ();

#endif /* ENTITY_H */
