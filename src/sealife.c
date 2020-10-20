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
        create_fish();
        gSpawner.t_fish = FISH_SPAWN_RATE;
    }
}

// FISH

INTERNAL void create_fish ()
{
    // Find an empty slot to create a fish and if there isn't one don't create.
    for (int i=0; i<FISH_MAX; ++i)
    {
        Fish* fish = gEntityFish+i;
        if (!fish->alive)
        {
            fish->palette = PAL_FISH_0 + (rand() % 3);
            fish->dir = rand() % 2;
            fish->alive = true;
            fish->t = 0.0f;

            if (fish->dir == HDIR_R) fish->x = 0 - SPR_FISH_R_0.w;
            if (fish->dir == HDIR_L) fish->x = SCREEN_W;

            fish->y = rand() % (SCREEN_H-SPR_FISH_R_0.h);

            return;
        }
    }
}
INTERNAL void update_fish (float dt)
{
    for (int i=0; i<FISH_MAX; ++i)
    {
        Fish* fish = gEntityFish+i;
        if (fish->alive)
        {
            fish->t += dt;

            // Move the fish and deactivate when off-screen.
            if (fish->dir == HDIR_R)
            {
                fish->x += FISH_SPEED * dt;
                if (fish->x > SCREEN_W) fish->alive = false;
            }
            if (fish->dir == HDIR_L)
            {
                fish->x -= FISH_SPEED * dt;
                if (fish->x < -SPR_FISH_L_0.w) fish->alive = false;
            }
        }
    }
}
INTERNAL void render_fish (float dt)
{
    for (int i=0; i<FISH_MAX; ++i)
    {
        Fish* fish = gEntityFish+i;
        if (fish->alive)
        {
            // Determine what animation frame to play for the fish.
            const Clip* frame = NULL;
            if (fish->dir == HDIR_R)
            {
                if (fish->t < FISH_ANIM_SPEED) frame = &SPR_FISH_R_0;
                else
                {
                    frame = &SPR_FISH_R_1;
                    if (fish->t > FISH_ANIM_SPEED*2) fish->t = 0.0f;
                }
            }
            if (fish->dir == HDIR_L)
            {
                if (fish->t < 0.2f) frame = &SPR_FISH_L_0;
                else
                {
                    frame = &SPR_FISH_L_1;
                    if (fish->t > FISH_ANIM_SPEED*2) fish->t = 0.0f;
                }
            }

            render_bitmap(fish->x,fish->y, fish->palette, frame);
        }
    }
}
