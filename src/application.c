#define GAME_BACKGROUND_COLOR 0xFF2F4B99

// HELPER FUNCTIONS

INTERNAL void render_cursor ()
{
    int x = get_mouse_x()-(SPR_CURSOR_0.w/2);
    int y = get_mouse_y()-(SPR_CURSOR_0.h/2);
    render_bitmap(x,y,PAL_CURSOR,&SPR_CURSOR_0);
}

INTERNAL void render_score ()
{
    const char* SCORE_TEXT = "%06d";

    int w = get_text_w(SCORE_TEXT,gScore);
    int h = TILE_H;
    int x = (SCREEN_W-w)/2;
    int y = 2;

    render_bitmap(x-SPR_SCOREBGL.w,y, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(x,y,w,h, COLOR_BLACK);
    render_bitmap(x+w,y, PAL_BLACK, &SPR_SCOREBGR);
    render_text(x,y, PAL_TEXT_SHADE, SCORE_TEXT, gScore);
}

// GAME

INTERNAL void update_game (float dt)
{
    update_spawner(dt);
    update_fish(dt);

    // Handle shooting.
    if (button_pressed(LMB))
    {
        // Check for collision with shootable enemies.
        shake_camera(1,1,0.1f);
        collide_fish();
    }
}
INTERNAL void render_game (float dt)
{
    begin_camera();
    render_fish(dt);
    end_camera();

    render_cursor();
    render_score();
}

// APPLICATION

INTERNAL bool init_application ()
{
    seed_random();
    create_spawner();
    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

INTERNAL void quit_application ()
{
    SDL_ShowCursor(SDL_ENABLE);
}

INTERNAL void update_application (float dt)
{
    update_camera(dt);
    update_game(dt);
}

INTERNAL void render_application (float dt)
{
    render_clear(GAME_BACKGROUND_COLOR);
    render_game(dt);
}
