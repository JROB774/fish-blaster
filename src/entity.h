#ifndef ENTITY_H
#define ENTITY_H

// ENTITY

typedef enum EntityID__
{
    ENT_FISH,
    ENT_TOTAL

} EntityID;

typedef struct Entity__
{
    EntityID type;
    float x,y;
    int palette;
    int frame;
    int dir;
    bool alive;
    float t;

} Entity;

#define ENTITY_MAX 512

GLOBAL Entity gEntity[ENTITY_MAX];

INTERNAL void  create_entity (EntityID id);
INTERNAL void  update_entity (float dt);
INTERNAL void  render_entity (float dt);
INTERNAL void collide_entity (bool shot); // Whether a shot was fired ot its just normal collision.

// SPAWNER

typedef struct Spawner__
{
    float t_fish;

} Spawner;

GLOBAL Spawner gSpawner;

INTERNAL void create_spawner ();
INTERNAL void update_spawner (float dt);

#endif /* ENTITY_H */
