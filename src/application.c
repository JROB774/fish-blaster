// HELPERS

INTERNAL void render_hud (int y, bool extra)
{
    // Draw the players'current score.
    const char* SCORE_TEXT = "%06d";
    int sw = get_text_w(SCORE_TEXT,gApp.score);
    int sh = TILE_H;
    int sx = (SCREEN_W-sw)/2;
    int sy = y;
    render_bitmap(sx-SPR_SCOREBGL.w,sy, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(sx,sy,sw,sh, get_palette_color(PAL_BLACK,0));
    render_bitmap(sx+sw,sy, PAL_BLACK, &SPR_SCOREBGR);
    render_text(sx,sy, PAL_TEXT_SHADE, SCORE_TEXT, gApp.score);

    // Don't draw the other parts of the display!
    if (!extra) return;

    // Draw the player's current health.
    render_bitmap(0,y, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(16,y,16,8, get_palette_color(PAL_BLACK,0));
    render_bitmap(32,y, PAL_BLACK, &SPR_SCOREBGR);
    render_bitmap(16,y, (gPlayer.life >= 1) ? PAL_HEART : PAL_NOHEART, &SPR_HEART);
    render_bitmap(24,y, (gPlayer.life >= 2) ? PAL_HEART : PAL_NOHEART, &SPR_HEART);

    // Draw the player's current bonus.
    render_bitmap(SCREEN_W-48,y, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(SCREEN_W-32,y,16,8, get_palette_color(PAL_BLACK,0));
    render_bitmap(SCREEN_W-16,y, PAL_BLACK, &SPR_SCOREBGR);
    if (gPlayer.current_item == ITEM_NONE)
    {
        render_text(SCREEN_W-32,y, PAL_TEXT_SHADE, "--");
    }
    else
    {
        bool visible = true;
        if (floor(gPlayer.item_time) <= 0.0f) // Flash the icon and text just before the item goes away.
        {
            if (gApp.frame % 2 == 0)
            {
                visible = false;
            }
        }
        if (visible)
        {
            render_text(SCREEN_W-24,y, PAL_TEXT_SHADE, "%d", CAST(int, floor(gPlayer.item_time)));
            switch (gPlayer.current_item)
            {
                case (ITEM_TIME): render_bitmap(SCREEN_W-32,y,PAL_ICO_TIME, &SPR_ICO_TIME_0); break;
                case (ITEM_MULT): render_bitmap(SCREEN_W-32,y,PAL_ICO_MULT, &SPR_ICO_MULT_0); break;
                case (ITEM_RAPD): render_bitmap(SCREEN_W-33,y,PAL_ICO_RAPD, &SPR_ICO_RAPD_0); break; // Kind of hacky but we draw this 1px left so it doesn't hug/touch the number to the right!
                case (ITEM_SPRD): render_bitmap(SCREEN_W-32,y,PAL_ICO_SPRD, &SPR_ICO_SPRD_0); break;
            }
        }
    }
}

// APP_STATE_MENU

INTERNAL bool do_menu_button (int* x, int* y, const char* text, float* target, float* current, float dt)
{
    assert(x && y && target && current);

    int rw = get_text_w(text);
    int rh = TILE_H;
    int rx = (SCREEN_W-rw)/2;
    int ry = *y;

    int mx = get_mouse_x();
    int my = get_mouse_y();

    // Handle hovering and pressing the button.
    bool pressed = false;
    if (point_vs_rect_collision(mx,my, rx,ry,rw,rh))
    {
        if (*target == 0.0f)
        {
            play_sound_channel(SND_SWISH,0,1);
            *target = rw;
        }

        if (button_pressed(LMB))
        {
            pressed = true;
        }
    }
    else
    {
        *target = 0.0f;
    }

    // Animate the backing of the button.
    *current = lerp(*current, *target, 20*dt);
    if (*current > 1.0f)
    {
        int bx = rx+((rw-(round(*current)))/2);
        int by = ry;
        int bw = ceil(*current);
        int bh = rh;

        render_bitmap(bx-SPR_SCOREBGL.w,by,PAL_BLACK,&SPR_SCOREBGL);
        render_fill(bx,by,bw,bh,get_palette_color(PAL_BLACK,0));
        render_bitmap(bx+bw,by,PAL_BLACK,&SPR_SCOREBGR);
    }

    render_text(rx,ry,PAL_TEXT_SHADE,text);

    *x  = rx;
    *y += rh+2;

    return pressed;
}

INTERNAL void update_menu (float dt)
{
    update_spawner(dt);
    update_player (dt);
    update_entity (dt);
    update_effect (dt);
}
INTERNAL void render_menu (float dt)
{
    // @Incomplete: Remove static/persistent variables!!!

    static float play_target = 0.0f;
    static float options_target = 0.0f;
    static float scores_target = 0.0f;
    static float credits_target = 0.0f;
    static float exit_target = 0.0f;

    static float play_current = 0.0f;
    static float options_current = 0.0f;
    static float scores_current = 0.0f;
    static float credits_current = 0.0f;
    static float exit_current = 0.0f;

    begin_camera();
    render_effect_lo(dt);
    render_entity(dt);
    render_effect_hi(dt);
    end_camera();

    render_bitmap(0,12,PAL_TEXT_SHADE,&SPR_TITLE);

    int x =  0;
    int y = 76;

    if (do_menu_button(&x,&y,"PLAY",    &/*gApp.*/play_target,   &/*gApp.*/play_current,    dt)) start_game();
    if (do_menu_button(&x,&y,"OPTIONS", &/*gApp.*/options_target,&/*gApp.*/options_current, dt)) {}
    if (do_menu_button(&x,&y,"SCORES",  &/*gApp.*/scores_target, &/*gApp.*/scores_current,  dt)) {}
    if (do_menu_button(&x,&y,"CREDITS", &/*gApp.*/credits_target,&/*gApp.*/credits_current, dt)) {}
    if (do_menu_button(&x,&y,"EXIT",    &/*gApp.*/exit_target,   &/*gApp.*/exit_current,    dt)) gWindow.running = false;

    render_player(dt);
}

// APP_STATE_GAME

INTERNAL void update_game (float dt)
{
    // Go to the menu if the player wants.
    if (key_pressed(ESCAPE))
    {
        start_menu();
        return;
    }
    // Restart the game if the player wants.
    if (key_pressed(RESTART))
    {
        start_game();
        return;
    }

    // Half the speed of everything if the slowdown item was recieved.
    float gamedt = dt;
    if (gPlayer.current_item == ITEM_TIME)
    {
        gamedt /= 2.0f;
    }

    update_spawner(gamedt);
    update_player (    dt);
    update_entity (gamedt);
    update_effect (gamedt);
}
INTERNAL void render_game (float dt)
{
    // Half the speed of everything if the slowdown item was recieved.
    float gamedt = dt;
    if (gPlayer.current_item == ITEM_TIME)
    {
        gamedt /= 2.0f;
    }

    begin_camera();
    render_effect_lo(gamedt);
    render_entity   (gamedt);
    render_effect_hi(gamedt);
    end_camera();

    render_player(dt);
    render_hud(2,true);
}

// APP_STATE_GAMEOVER

INTERNAL void update_gameover (float dt)
{
    // Go to the menu if the player wants.
    if (key_pressed(ESCAPE))
    {
        start_menu();
        return;
    }

    update_player(dt);
}
INTERNAL void render_gameover (float dt)
{
    render_player(dt);

    begin_camera();
    render_hud((SCREEN_H-TILE_H)/2,false);
    end_camera();
}

// APPLICATION

INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);

    seed_random();
    start_menu();
    init_player();

    return true;
}
INTERNAL void quit_application ()
{
    SDL_ShowCursor(SDL_ENABLE);
}
INTERNAL void handle_application (SDL_Event* event)
{
    // Handle checking secret code input and enabling the codes.
    if (event->type == SDL_KEYDOWN)
    {
        int keycode = event->key.keysym.sym;
        if ((keycode >= 'a' && keycode <= 'z') || (keycode >= '0' && keycode <= '9'))
        {
            if (gApp.code_length >= CODE_LENGTH) gApp.code_length = 0;
            gApp.code[gApp.code_length++] = CAST(char, keycode);

            // Make sure we're matching at least one of the codes so far and if not
            // reset the length of our input to zero to avoid messing up the buffer.
             if (strncmp(gApp.code, CODE_RETRO, gApp.code_length) != 0 &&
                 strncmp(gApp.code, CODE_BLOOD, gApp.code_length) != 0 &&
                 strncmp(gApp.code, CODE_1BITS, gApp.code_length) != 0)
            {
                gApp.code_length = 0;
                gApp.code[gApp.code_length++] = CAST(char, keycode);
            }

            // If we've entered a full code then check which one matches and toggle it.
            if (gApp.code_length == CODE_LENGTH)
            {
                if (strncmp(gApp.code, CODE_RETRO, gApp.code_length) == 0)
                {
                    gApp.code_retro_enabled = !gApp.code_retro_enabled;
                    if (gApp.code_retro_enabled) gApp.code_1bits_enabled = false;
                    play_sound(SND_CODE,0);
                }
                if (strncmp(gApp.code, CODE_BLOOD, gApp.code_length) == 0)
                {
                    gApp.code_blood_enabled = !gApp.code_blood_enabled;
                    play_sound(SND_CODE,0);
                }
                if (strncmp(gApp.code, CODE_1BITS, gApp.code_length) == 0)
                {
                    gApp.code_1bits_enabled = !gApp.code_1bits_enabled;
                    if (gApp.code_1bits_enabled) gApp.code_retro_enabled = false;
                    play_sound(SND_CODE,0);
                }

                // Special case for the RETRO and 1BITS code when setting the palette.
                if (gApp.code_retro_enabled)
                {
                    set_palette_mode(PAL_MODE_GAMEBOY);
                }
                else if (gApp.code_1bits_enabled)
                {
                    set_palette_mode(PAL_MODE_1BIT);
                }
                else
                {
                    if (gPlayer.current_item == ITEM_TIME)
                    {
                        set_palette_mode(PAL_MODE_SLOWDOWN);
                    }
                    else
                    {
                        set_palette_mode(PAL_MODE_DEFAULT);
                    }
                }
            }
        }
    }
}
INTERNAL void update_application (float dt)
{
    // Debug keys for spawning the different entity types.
    #if defined(BUILD_DEBUG)
    if (key_pressed(SDLK_F1 )) create_entity(ENT_CRATE_LIFE);
    if (key_pressed(SDLK_F2 )) create_entity(ENT_CRATE_TIME);
    if (key_pressed(SDLK_F3 )) create_entity(ENT_CRATE_MULT);
    if (key_pressed(SDLK_F4 )) create_entity(ENT_CRATE_RAPD);
    if (key_pressed(SDLK_F5 )) create_entity(ENT_CRATE_SPRD);
    if (key_pressed(SDLK_F6 )) create_entity(ENT_CRATE_BOOM);
    if (key_pressed(SDLK_F7 )) create_entity(ENT_FISH);
    if (key_pressed(SDLK_F8 )) create_entity(ENT_SQUID);
    if (key_pressed(SDLK_F9 )) create_entity(ENT_JELLY);
    if (key_pressed(SDLK_F10)) create_entity(ENT_URCHIN);
    if (key_pressed(SDLK_F11)) create_entity(ENT_BOOM);
    #endif

    gApp.frame++;

    update_camera(dt);

    switch (gApp.state)
    {
        case (APP_STATE_MENU    ): update_menu    (dt); break;
        case (APP_STATE_GAME    ): update_game    (dt); break;
        case (APP_STATE_GAMEOVER): update_gameover(dt); break;
    }
}
INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));

    switch (gApp.state)
    {
        case (APP_STATE_MENU    ): render_menu    (dt); break;
        case (APP_STATE_GAME    ): render_game    (dt); break;
        case (APP_STATE_GAMEOVER): render_gameover(dt); break;
    }

    if (gApp.flash_white)
    {
        render_clear(get_palette_color(PAL_WHITE,0));
        gApp.flash_white = false;
    }
}

// MISCELLANEOUS

INTERNAL void flash_screen_white ()
{
    gApp.flash_white = true;
}

// STATE CHANGES

INTERNAL void start_menu ()
{
    gApp.state = APP_STATE_MENU;

    memset(gEntity, 0, sizeof(gEntity));
    memset(gEffect, 0, sizeof(gEffect));

    create_spawner();
}
INTERNAL void start_game ()
{
    gApp.state = APP_STATE_GAME;

    memset(gEntity, 0, sizeof(gEntity));
    memset(gEffect, 0, sizeof(gEffect));

    create_spawner();
    create_player();

    gApp.score = 0;
}
INTERNAL void game_over ()
{
    gApp.state = APP_STATE_GAMEOVER;

    gPlayer.cooldown_time = GAMEOVER_COOLDOWN;
    gPlayer.god_time = 0.0f;
    gPlayer.current_item = ITEM_NONE;

    if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
    {
        set_palette_mode(PAL_MODE_DEFAULT);
    }
}
