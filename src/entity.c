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
#define FISH_ANM_SPEED   0.2f
#define FISH_SPEED 50
#define FISH_SCORE 10
#define FISH_DIR_L 0
#define FISH_DIR_R 1
#define FISH_MIN_BLOOD 10
#define FISH_MAX_BLOOD 15
#define FISH_MIN_GIB   1
#define FISH_MAX_GIB   4

INTERNAL void create_fish (Entity* entity)
{
    entity->palette = random_int_range(PAL_FISH_0,PAL_FISH_2);
    entity->dir = random_int_range(FISH_DIR_L,FISH_DIR_R);
    entity->t = FISH_ANM_SPEED;
    entity->frame = 0;

    if (entity->dir == FISH_DIR_L) entity->x = SCREEN_W;
    if (entity->dir == FISH_DIR_R) entity->x = 0 - SPR_FISH_R_0.w;

    entity->y = random_int_range(12, SCREEN_H-SPR_FISH_R_0.h);
}
INTERNAL void update_fish (Entity* entity, float dt)
{
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
INTERNAL const Clip* render_fish (Entity* entity, float dt)
{
    entity->t -= dt;
    if (entity->t <= 0.0f)
    {
        entity->t = FISH_ANM_SPEED;
        entity->frame++;
        if (entity->frame >= ARRAYSIZE(ANM_FISH_R)) // Doesn't matter if we use left or right, they're the same length!
        {
            entity->frame = 0;
        }
    }
    return ((entity->dir == FISH_DIR_L) ? ANM_FISH_L[entity->frame] : ANM_FISH_R[entity->frame]);
}
INTERNAL void collide_fish (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    if (shot)
    {
        int x = CAST(int,entity->x)+2;
        int y = CAST(int,entity->y)+1;
        int w = 12;
        int h = 6;

        if (rect_vs_rect_collision(mx,my,mw,mh, x,y,w,h))
        {
            // Kill the fish.
            create_effect(EFX_BLOOD, x,y,w,h, FISH_MIN_BLOOD,FISH_MAX_BLOOD);
            create_effect(EFX_GIB, x,y,w,h, FISH_MIN_GIB,FISH_MAX_GIB);
            create_effect(EFX_SCORE10, x+w/2,y+h/2,1,1, 1,1);
            play_sound(SND_SQUEAK[random_int_range(0,ARRAYSIZE(SND_SQUEAK)-1)],0);
            gApp.score += FISH_SCORE;
            entity->alive = false;
        }
    }
}

// ENT_SQUID

INTERNAL void create_squid (Entity* entity)
{
    // @Incomplete: ...
}
INTERNAL void update_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
}
INTERNAL const Clip* render_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
    return NULL;
}
INTERNAL void collide_squid (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    // @Incomplete: ...
}

// ENT_URCHIN

#define URCHIN_SPAWN_START      8.0f
#define URCHIN_SPAWN_RATE       1.0f
#define URCHIN_INCREMENT_START 10.0f
#define URCHIN_INCREMENT_RATE  10.0f
#define URCHIN_ANM_SPEED        0.2f
#define URCHIN_MAX_START 5
#define URCHIN_WIDTH    16
#define URCHIN_HEIGHT   16
#define URCHIN_SPEED    40

INTERNAL void create_urchin (Entity* entity)
{
    entity->palette = PAL_URCHIN;
    entity->t = URCHIN_ANM_SPEED;
    entity->x = random_int_range(0, SCREEN_W-URCHIN_WIDTH);
    entity->y = SCREEN_H;
    entity->frame = 0;
    entity->active = false;

    entity->vx = 0;
    entity->vy = -URCHIN_SPEED;
    rotate_vec2(&entity->vx, &entity->vy, random_float_range(-(M_PI/4), (M_PI/4)));
}
INTERNAL void update_urchin (Entity* entity, float dt)
{
    // Urchin bouncing.
    if (entity->t2 > 0.0f) entity->t2 -= dt;
    if (entity->active && entity->t2 <= 0.0f)
    {
        for (int i=0; i<ENTITY_MAX; ++i)
        {
            Entity* other = gEntity+i;
            if (entity != other) // Do not collide with self!
            {
                if (other->alive)
                {
                    if (other->type == ENT_URCHIN)
                    {
                        int ax = CAST(int,entity->x)+4;
                        int ay = CAST(int,entity->y)+4;
                        int aw = 8;
                        int ah = 8;
                        int bx = CAST(int,other->x)+4;
                        int by = CAST(int,other->y)+4;
                        int bw = 8;
                        int bh = 8;

                        bool collision = false;
                        if (rect_vs_rect_collision(ax+(entity->vx*dt),ay,aw,ah, bx,by,bw,bh)) // Horizontal.
                        {
                            entity->vx *= -1;
                            other->vx *= -1;
                            collision = true;
                        }
                        if (rect_vs_rect_collision(ax,ay+(entity->vy*dt),aw,ah, bx,by,bw,bh)) // Vertical.
                        {
                            entity->vy *= -1;
                            other->vy *= -1;
                            collision = true;
                        }
                        if (collision)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    // Slowly move the urchin.
    entity->x += entity->vx * dt;
    entity->y += entity->vy * dt;

    int x = entity->x+(URCHIN_WIDTH/2);
    int y = entity->y+(URCHIN_HEIGHT/2);

    // Once we're on screen we can activate vertical bouncing.
    if (!entity->active && (y < SCREEN_H)) entity->active = true;

    // Wall bouncing.
    if ((x < 0) || (x > SCREEN_W)) // Horizontal bounce.
    {
        entity->vx = -(entity->vx);
    }
    if (entity->active) // Vetical bounce.
    {
        if ((y < 0) || (y > SCREEN_H))
        {
            entity->vy = -(entity->vy);
        }
    }
}
INTERNAL const Clip* render_urchin (Entity* entity, float dt)
{
    entity->t -= dt;
    if (entity->t <= 0.0f)
    {
        entity->t = URCHIN_ANM_SPEED;
        entity->frame++;
        if (entity->frame >= ARRAYSIZE(ANM_URCHIN))
        {
            entity->frame = 0;
        }
    }
    return ANM_URCHIN[entity->frame];
}
INTERNAL void collide_urchin (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    int x = CAST(int,entity->x)+4;
    int y = CAST(int,entity->y)+4;
    int w = 8;
    int h = 8;

    mx += mw/2;
    my += mh/2;

    if (point_vs_rect_collision(mx,my, x,y,w,h))
    {
        game_over();
    }
}

// SCHOOLS

INTERNAL void spawn_school_of_fish_formation_smallv ()
{
    // If we don't have enough space for the formation just spawn one fish like normal.
    if (!have_space_for_num_entities(3))
    {
        create_entity(ENT_FISH);
        return;
    }

    Entity* a = create_entity(ENT_FISH);
    Entity* b = create_entity(ENT_FISH);
    Entity* c = create_entity(ENT_FISH);

    // Put all the fish in a vertical line and the same direction.
    b->y = a->y - SPR_FISH_L_0.h, b->dir = a->dir;
    c->y = a->y + SPR_FISH_L_0.h, c->dir = a->dir;

    // Place the fish in the V shape.
    if (a->dir == FISH_DIR_L)
    {
        b->x = a->x + SPR_FISH_L_0.w;
        c->x = a->x + SPR_FISH_L_0.w;
    }
    else
    {
        b->x = a->x - SPR_FISH_R_0.w;
        c->x = a->x - SPR_FISH_R_0.w;
    }
}

INTERNAL void spawn_school_of_fish_formation_line ()
{
    // If we don't have enough space for the formation just spawn one fish like normal.
    if (!have_space_for_num_entities(4))
    {
        create_entity(ENT_FISH);
        return;
    }

    Entity* a = create_entity(ENT_FISH);
    Entity* b = create_entity(ENT_FISH);
    Entity* c = create_entity(ENT_FISH);
    Entity* d = create_entity(ENT_FISH);

    // Put all the fish on the same vertical axis and direction.
    b->y = a->y, b->dir = a->dir;
    c->y = a->y, c->dir = a->dir;
    d->y = a->y, d->dir = a->dir;

    // Place all the fish behind each other in a line.
    if (a->dir == FISH_DIR_L)
    {
        b->x = a->x + SPR_FISH_L_0.w;
        c->x = b->x + SPR_FISH_L_0.w;
        d->x = c->x + SPR_FISH_L_0.w;
    }
    else
    {
        b->x = a->x - SPR_FISH_R_0.w;
        c->x = b->x - SPR_FISH_R_0.w;
        d->x = c->x - SPR_FISH_R_0.w;
    }
}

INTERNAL void spawn_school_of_fish_formation_largev ()
{
    // If we don't have enough space for the formation just spawn one fish like normal.
    if (!have_space_for_num_entities(5))
    {
        create_entity(ENT_FISH);
        return;
    }

    Entity* a = create_entity(ENT_FISH);
    Entity* b = create_entity(ENT_FISH);
    Entity* c = create_entity(ENT_FISH);
    Entity* d = create_entity(ENT_FISH);
    Entity* e = create_entity(ENT_FISH);

    // Put all the fish in a vertical line and the same direction.
    b->y = a->y - SPR_FISH_L_0.h,   b->dir = a->dir;
    c->y = a->y + SPR_FISH_L_0.h,   c->dir = a->dir;
    d->y = a->y - SPR_FISH_L_0.h*2, d->dir = a->dir;
    e->y = a->y + SPR_FISH_L_0.h*2, e->dir = a->dir;

    // Place the fish in the V shape.
    if (a->dir == FISH_DIR_L)
    {
        b->x = a->x + SPR_FISH_L_0.w;
        c->x = a->x + SPR_FISH_L_0.w;
        d->x = a->x + SPR_FISH_L_0.w*2;
        e->x = a->x + SPR_FISH_L_0.w*2;
    }
    else
    {
        b->x = a->x - SPR_FISH_R_0.w;
        c->x = a->x - SPR_FISH_R_0.w;
        d->x = a->x - SPR_FISH_R_0.w*2;
        e->x = a->x - SPR_FISH_R_0.w*2;
    }
}

// ENTITIES

INTERNAL bool have_space_for_num_entities (int num)
{
    int count = 0;
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (!entity->alive)
        {
            count++;
            if (count >= num)
            {
                break;
            }
        }
    }
    return (count >= num);
}

INTERNAL Entity* create_entity (EntityID id)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (!entity->alive)
        {
            entity->type = id;
            entity->frame = 0;
            entity->vx = 0;
            entity->vy = 0;
            entity->t = 0.0f;
            entity->alive = true;

            // Specific creation logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH  ): create_fish  (entity); break;
                case (ENT_SQUID ): create_squid (entity); break;
                case (ENT_URCHIN): create_urchin(entity); break;
            }

            return entity;
        }
    }
    return NULL;
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
                case (ENT_FISH  ): update_fish  (entity, dt); break;
                case (ENT_SQUID ): update_squid (entity, dt); break;
                case (ENT_URCHIN): update_urchin(entity, dt); break;
            }
        }
    }
}

INTERNAL int entity_z_compare (const void* a, const void* b)
{
    const Entity* ea = CAST(Entity*, a);
    const Entity* eb = CAST(Entity*, b);

    if (ea->type < eb->type) return -1;
    if (ea->type > eb->type) return  1;

    return 0;
}

INTERNAL void render_entity (float dt)
{
    // Sort the entity list so they render in the correct order. The order
    // is determiend by the order of the IDs in the EntityID enumeration.
    qsort(gEntity, ENTITY_MAX, sizeof(Entity), entity_z_compare);

    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific render logic for the different entity types.
            const Clip* frame = NULL;
            switch (entity->type)
            {
                case (ENT_FISH  ): frame = render_fish  (entity, dt); break;
                case (ENT_SQUID ): frame = render_squid (entity, dt); break;
                case (ENT_URCHIN): frame = render_urchin(entity, dt); break;
            }
            render_bitmap(entity->x,entity->y,entity->palette,frame);
        }
    }
}

INTERNAL void collide_entity (bool shot)
{
    // Don't handle collisions if the cursor is out of bounds.
    if (!is_mouse_in_screen_bounds()) return;

    // We use a box around the mouse point for collision as it feels better to give
    // some leeway when shooting entities, rather than needing a point collision.
    int x = CAST(int,get_mouse_x())-2;
    int y = CAST(int,get_mouse_y())-2;
    int w = 4;
    int h = 4;

    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific collision logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH  ): collide_fish  (entity, x,y,w,h, shot); break;
                case (ENT_SQUID ): collide_squid (entity, x,y,w,h, shot); break;
                case (ENT_URCHIN): collide_urchin(entity, x,y,w,h, shot); break;
            }
        }
    }
}

INTERNAL void create_spawner ()
{
    gSpawner.fish_spawn_timer = FISH_SPAWN_START;

    gSpawner.urchin_spawn_timer = URCHIN_SPAWN_START;
    gSpawner.urchin_increment_timer = URCHIN_INCREMENT_START;
    gSpawner.urchin_max_count = URCHIN_MAX_START;
    gSpawner.urchin_count = 0;
}

INTERNAL void update_spawner (float dt)
{
    // Spawn fish at fixed intervals.
    gSpawner.fish_spawn_timer -= dt;
    if (gSpawner.fish_spawn_timer <= 0.0f)
    {
        // Small percentage chance to spawn a school of fish in a formation.
        gSpawner.fish_spawn_timer = FISH_SPAWN_RATE;
        if (random_int_range(1,100) <= 5)
        {
            switch (random_int_range(0,2))
            {
                case (0): spawn_school_of_fish_formation_smallv(); break;
                case (1): spawn_school_of_fish_formation_line  (); break;
                case (2): spawn_school_of_fish_formation_largev(); break;
            }
        }
        else
        {
            create_entity(ENT_FISH);
        }
    }

    // Spawn urchin at fixed intervals.
    gSpawner.urchin_spawn_timer -= dt;
    if (gSpawner.urchin_spawn_timer <= 0.0f)
    {
        gSpawner.urchin_spawn_timer = URCHIN_SPAWN_RATE;
        if (gSpawner.urchin_count < gSpawner.urchin_max_count)
        {
            gSpawner.urchin_count++;
            create_entity(ENT_URCHIN);
        }
    }
    // Increate the max number of urchin at fixed intervals.
    gSpawner.urchin_increment_timer -= dt;
    if (gSpawner.urchin_increment_timer <= 0.0f)
    {
        gSpawner.urchin_max_count++;
        gSpawner.urchin_increment_timer = URCHIN_INCREMENT_RATE;
    }
}
