// COLLISION HELPERS

INTERNAL bool point_vs_rect_collision (float px, float py, float rx, float ry, float rw, float rh)
{
    return ((px >= rx) && (py >= ry) && (px < (rx+rw)) && (py < (ry+rh)));
}

INTERNAL bool rect_vs_rect_collision (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    return ((ax < (bx+bw)) && ((ax+aw) > bx) && (ay < (by+bh)) && ((ay+ah) > by));
}

// ENT_FISH

#define FISH_SPAWN_START 0.0f
#define FISH_SPAWN_RATE  0.5f
#define FISH_ANIM_SPEED  0.2f
#define FISH_SPEED 50
#define FISH_SCORE 10
#define FISH_DIR_L 0
#define FISH_DIR_R 1

INTERNAL void create_fish (Entity* entity)
{
    entity->palette = random_int_range(PAL_FISH_0,PAL_FISH_2);
    entity->dir = random_int_range(FISH_DIR_L,FISH_DIR_R);
    entity->t = 0.0f;

    if (entity->dir == FISH_DIR_L) entity->x = SCREEN_W;
    if (entity->dir == FISH_DIR_R) entity->x = 0 - SPR_FISH_R_0.w;

    entity->y = random_int_range(12, SCREEN_H-SPR_FISH_R_0.h);
}
INTERNAL void update_fish (Entity* entity, float dt)
{
    entity->t += dt;

    // Move the fish and deactivate when off-screen.
    if (entity->dir == FISH_DIR_L)
    {
        entity->x -= FISH_SPEED * dt;
        if (entity->x < -SPR_FISH_L_0.w) entity->alive = false;
    }
    if (entity->dir == FISH_DIR_R)
    {
        entity->x += FISH_SPEED * dt;
        if (entity->x > SCREEN_W) entity->alive = false;
    }
}
INTERNAL void render_fish (Entity* entity, float dt)
{
    // Determine what animation frame to play for the fish.
    const Clip* frame = NULL;
    if (entity->dir == FISH_DIR_L)
    {
        if (entity->t < 0.2f) frame = &SPR_FISH_L_0;
        else
        {
            frame = &SPR_FISH_L_1;
            if (entity->t > FISH_ANIM_SPEED*2) entity->t = 0.0f;
        }
    }
    if (entity->dir == FISH_DIR_R)
    {
        if (entity->t < FISH_ANIM_SPEED) frame = &SPR_FISH_R_0;
        else
        {
            frame = &SPR_FISH_R_1;
            if (entity->t > FISH_ANIM_SPEED*2) entity->t = 0.0f;
        }
    }

    render_bitmap(entity->x,entity->y, entity->palette, frame);
}
INTERNAL void collide_fish (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    if (shot)
    {
        int fx = CAST(int,entity->x)+1;
        int fy = CAST(int,entity->y)+1;
        int fw = 13;
        int fh =  6;

        if (rect_vs_rect_collision(mx,my,mw,mh, fx,fy,fw,fh))
        {
            // Kill the fish.
            create_effect(EFX_BLOOD, fx+2,fy+1,fw-4,fh-2, 10,15);
            play_sound(SND_HIT,0);
            gScore += FISH_SCORE;
            entity->alive = false;
        }
    }
}

// ENTITY

INTERNAL void create_entity (EntityID id)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (!entity->alive)
        {
            entity->alive = true;
            entity->type = id;
            entity->t = 0.0f;

            // Specific creation logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH): create_fish(entity); break;
            }

            break;
        }
    }
}

INTERNAL void update_entity (float dt)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific update logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH): update_fish(entity, dt); break;
            }
        }
    }
}

INTERNAL void render_entity (float dt)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific render logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH): render_fish(entity, dt); break;
            }
        }
    }
}

INTERNAL void collide_entity (bool shot)
{
    // We use a box around the mouse point for collision as it feels better to give
    // some leeway when shooting entities, rather than needing a point collision.
    int mx = CAST(int,get_mouse_x())-2;
    int my = CAST(int,get_mouse_y())-2;
    int mw = 4;
    int mh = 4;

    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific collision logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH): collide_fish(entity, mx,my,mw,mh, shot); break;
            }
        }
    }
}

// SPAWNER

INTERNAL void create_spawner ()
{
    gSpawner.t_fish = FISH_SPAWN_START;
}

INTERNAL void update_spawner (float dt)
{
    if (gSpawner.t_fish > 0.0f) gSpawner.t_fish -= dt;
    else
    {
        create_entity(ENT_FISH);
        gSpawner.t_fish = FISH_SPAWN_RATE;
    }
}
