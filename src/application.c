#define GAME_BACKGROUND_COLOR 0xFF2F4B99

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
    update_spawner(dt);
    update_fish(dt);

    // Handle shooting.
    if (button_pressed(LMB))
    {
        collide_fish();
    }
}

INTERNAL void render_application (float dt)
{
    render_clear(GAME_BACKGROUND_COLOR);

    render_fish(dt);

    // Render the in-game mouse cursor/crosshair.
    int cx = get_mouse_x()-(SPR_CURSOR_0.w/2);
    int cy = get_mouse_y()-(SPR_CURSOR_0.h/2);
    render_bitmap(cx,cy,PAL_CURSOR,&SPR_CURSOR_0);

    // Render the score to the top-center of the screen.
    const char* SCORE_TEXT = "%06d";

    int sw = get_text_w(SCORE_TEXT,gScore);
    int sh = TILE_H;
    int sx = (SCREEN_W-sw)/2;
    int sy = 2;

    render_bitmap(sx-SPR_SCOREBGL.w,sy, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(sx,sy,sw,sh, COLOR_BLACK);
    render_bitmap(sx+sw,sy, PAL_BLACK, &SPR_SCOREBGR);
    render_text(sx,sy, PAL_TEXT_SHADE, SCORE_TEXT, gScore);
}
