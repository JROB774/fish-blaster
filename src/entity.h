#ifndef ENTITY_H
#define ENTITY_H

typedef enum EntityID__
{
    ENT_FISH,
    ENT_SQUID,
    ENT_URCHIN,
    ENT_TOTAL

} EntityID;

typedef struct Entity__
{
    EntityID type;
    float x,y,vx,vy;
    int palette;
    int frame;
    int dir;
    float t;
    float t2;
    bool alive;
    bool active;

} Entity;

typedef struct Spawner__
{
    float fish_spawn_timer;

    float urchin_spawn_timer;
    float urchin_increment_timer;
    int   urchin_max_count;
    int   urchin_count;

} Spawner;

#define ENTITY_MAX 128

GLOBAL Entity  gEntity[ENTITY_MAX];
GLOBAL Spawner gSpawner;

INTERNAL bool have_space_for_num_entities (int num);

INTERNAL Entity*  create_entity (EntityID id);
INTERNAL void     update_entity (float dt);
INTERNAL void     render_entity (float dt);
INTERNAL void    collide_entity (bool shot); // Whether a shot was fired or its just normal collision.
INTERNAL void    create_spawner ();
INTERNAL void    update_spawner (float dt);

#endif /* ENTITY_H */
