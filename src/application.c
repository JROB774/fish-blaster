// HELPER FUNCTIONS

INTERNAL void render_cursor ()
{
    int x = get_mouse_x()-(SPR_CURSOR_0.w/2);
    int y = get_mouse_y()-(SPR_CURSOR_0.h/2);
    render_bitmap(x,y,PAL_CURSOR,&SPR_CURSOR_0);
}
INTERNAL void render_score (int y)
{
    const char* SCORE_TEXT = "%06d";

    int w = get_text_w(SCORE_TEXT,gScore);
    int h = TILE_H;
    int x = (SCREEN_W-w)/2;

    render_bitmap(x-SPR_SCOREBGL.w,y, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(x,y,w,h, get_palette_color(PAL_BLACK,0));
    render_bitmap(x+w,y, PAL_BLACK, &SPR_SCOREBGR);
    render_text(x,y, PAL_TEXT_SHADE, SCORE_TEXT, gScore);
}
INTERNAL void shoot ()
{
    int x = get_mouse_x()-6;
    int y = get_mouse_y()-6;
    int w = 12;
    int h = 12;

    create_effect(EFX_BUBBLE, x,y,w,h, 2,3);
    play_sound(SND_SHOOT,0);
    shake_camera(1,1,0.1f);
}

// GAME

INTERNAL void update_game (float dt)
{
    update_spawner(dt);
    update_entity (dt);
    update_effect (dt);

    // Handle shooting.
    bool shot = false;
    if (button_pressed(LMB))
    {
        shoot();
        shot = true;
    }

    collide_entity(shot);
}
INTERNAL void render_game (float dt)
{
    begin_camera();
    render_effect(dt);
    render_entity(dt);
    end_camera();

    render_cursor();
    render_score(2);
}

// GAME OVER

INTERNAL void update_gameover (float dt)
{
    if (button_pressed(LMB))
    {
        start_game(dt);
        shoot(); // Do this second so effects don't get wiped.
    }
}
INTERNAL void render_gameover (float dt)
{
    render_cursor();
    render_score((SCREEN_H-TILE_H)/2);
}

// APPLICATION

INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);

    seed_random();
    start_game();

    return true;
}
INTERNAL void quit_application ()
{
    SDL_ShowCursor(SDL_ENABLE);
}
INTERNAL void update_application (float dt)
{
    update_camera(dt);

    switch (gAppState)
    {
        case (APP_STATE_GAME    ): update_game    (dt); break;
        case (APP_STATE_GAMEOVER): update_gameover(dt); break;
    }
}
INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));

    switch (gAppState)
    {
        case (APP_STATE_GAME    ): render_game    (dt); break;
        case (APP_STATE_GAMEOVER): render_gameover(dt); break;
    }
}

// STATES

INTERNAL void start_game ()
{
    memset(gEntity, 0, sizeof(gEntity));
    memset(gEffect, 0, sizeof(gEffect));

    create_spawner();

    gScore = 0;
    gAppState = APP_STATE_GAME;
}
INTERNAL void game_over ()
{
    gAppState = APP_STATE_GAMEOVER;
}
