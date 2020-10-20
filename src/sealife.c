// COLLISION

INTERNAL bool point_vs_rect_collision (float px, float py, float rx, float ry, float rw, float rh)
{
    return ((px >= rx) && (py >= ry) && (px < (rx+rw)) && (py < (ry+rh)));
}

INTERNAL bool rect_vs_rect_collision (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    return ((ax < (bx+bw)) && ((ax+aw) > bx) && (ay < (by+bh)) && ((ay+ah) > by));
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
INTERNAL void collide_fish ()
{
    int mx = CAST(int,get_mouse_x())-2;
    int my = CAST(int,get_mouse_y())-2;
    int mw = 4;
    int mh = 4;

    for (int i=0; i<FISH_MAX; ++i)
    {
        Fish* fish = gEntityFish+i;
        if (fish->alive)
        {
            int fx = CAST(int,fish->x)+1;
            int fy = CAST(int,fish->y)+1;
            int fw = 13;
            int fh =  6;

            if (rect_vs_rect_collision(mx,my,mw,mh, fx,fy,fw,fh))
            {
                // Kill the fish.
                fish->alive = false;
            }
        }
    }
}
