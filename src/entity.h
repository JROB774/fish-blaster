#ifndef ENTITY_H
#define ENTITY_H

typedef enum EntityID__
{
    ENT_FISH,
    ENT_URCHIN,
    ENT_SQUID,
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

typedef struct Spawner__
{
    float t_fish;
    float t_urchin;
    float t_squid;

} Spawner;

#define ENTITY_MAX 512

GLOBAL Entity  gEntity[ENTITY_MAX];
GLOBAL Spawner gSpawner;

INTERNAL void  create_entity (EntityID id);
INTERNAL void  update_entity (float dt);
INTERNAL void  render_entity (float dt);
INTERNAL void collide_entity (bool shot); // Whether a shot was fired ot its just normal collision.
INTERNAL void create_spawner ();
INTERNAL void update_spawner (float dt);

#endif /* ENTITY_H */
