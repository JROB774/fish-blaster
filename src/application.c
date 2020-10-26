// HELPERS

INTERNAL void render_cursor ()
{
    gApp.cursor_prev_x = gApp.cursor_x;
    gApp.cursor_prev_y = gApp.cursor_y;
    gApp.cursor_x      = get_mouse_x();
    gApp.cursor_y      = get_mouse_y();

    // If the cursor moved then draw a trail behind it.
    if (gApp.cursor_x != gApp.cursor_prev_x && gApp.cursor_y != gApp.cursor_prev_y)
    {
        render_line(gApp.cursor_x,  gApp.cursor_y,   gApp.cursor_prev_x, gApp.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gApp.cursor_x+1,gApp.cursor_y,   gApp.cursor_prev_x, gApp.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gApp.cursor_x-1,gApp.cursor_y,   gApp.cursor_prev_x, gApp.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gApp.cursor_x,  gApp.cursor_y+1, gApp.cursor_prev_x, gApp.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gApp.cursor_x,  gApp.cursor_y-1, gApp.cursor_prev_x, gApp.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
    }

    // Draw the actual cursor graphic.
    bool visible = true;
    if (gApp.god_time > 0.0f)
    {
        if (gApp.frame % 2 == 0) // Flicker when invincible!
        {
            visible = false;
        }
    }
    if (visible)
    {
        int x = gApp.cursor_x-(SPR_CURSOR_0.w/2);
        int y = gApp.cursor_y-(SPR_CURSOR_0.h/2);
        render_bitmap(x,y,PAL_CURSOR,&SPR_CURSOR_0);
    }
}
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
    render_bitmap(16,y, (gApp.life >= 1) ? PAL_HEART : PAL_NOHEART, &SPR_HEART);
    render_bitmap(24,y, (gApp.life >= 2) ? PAL_HEART : PAL_NOHEART, &SPR_HEART);

    // Draw the player's current bonus.
    render_bitmap(SCREEN_W-48,y, PAL_BLACK, &SPR_SCOREBGL);
    render_fill(SCREEN_W-32,y,16,8, get_palette_color(PAL_BLACK,0));
    render_bitmap(SCREEN_W-16,y, PAL_BLACK, &SPR_SCOREBGR);
    if (gApp.current_item == ITEM_NONE)
    {
        render_text(SCREEN_W-32,y, PAL_TEXT_SHADE, "--");
    }
    else
    {
        bool visible = true;
        if (floor(gApp.item_time) <= 0.0f) // Flash the icon and text just before the item goes away.
        {
            if (gApp.frame % 2 == 0)
            {
                visible = false;
            }
        }
        if (visible)
        {
            render_text(SCREEN_W-24,y, PAL_TEXT_SHADE, "%d", CAST(int, floor(gApp.item_time)));
            switch (gApp.current_item)
            {
                case (ITEM_TIME): render_bitmap(SCREEN_W-32,y,PAL_ICO_TIME, &SPR_ICO_TIME_0); break;
                case (ITEM_MULT): render_bitmap(SCREEN_W-32,y,PAL_ICO_MULT, &SPR_ICO_MULT_0); break;
                case (ITEM_RAPD): render_bitmap(SCREEN_W-33,y,PAL_ICO_RAPD, &SPR_ICO_RAPD_0); break; // Kind of hacky but we draw this 1px left so it doesn't hug/touch the number to the right!
                case (ITEM_SPRD): render_bitmap(SCREEN_W-32,y,PAL_ICO_SPRD, &SPR_ICO_SPRD_0); break;
            }
        }
    }
}
INTERNAL void shoot ()
{
    int mx = get_mouse_x();
    int my = get_mouse_y();

    int x = mx-6;
    int y = my-6;
    int w = 12;
    int h = 12;

    create_effect(EFX_BUBBLE, x,y,w,h, 2,3);
    create_effect(EFX_SHOT, mx,my,1,1, 1,1);
    play_sound(SND_NSHOT[random_int_range(0,ARRAYSIZE(SND_NSHOT)-1)],0);
    shake_camera(1,1,0.1f);
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
    update_entity (dt);
    update_effect (dt);

    // Handle shooting.
    if (button_pressed(LMB))
    {
        if (is_cursor_in_screen_bounds())
        {
            shoot();
        }
    }
}
INTERNAL void render_menu (float dt)
{
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

    render_cursor();
}

// APP_STATE_GAME

INTERNAL void update_game (float dt)
{
    // Restart the game if the player wants.
    if (key_pressed(RESTART))
    {
        start_game();
        return;
    }

    if (gApp.god_time > 0.0f)
    {
        gApp.god_time -= dt;
    }
    if (gApp.item_time > 0.0f)
    {
        gApp.item_time -= dt;
        if (gApp.item_time < 0.0f)
        {
            gApp.current_item = ITEM_NONE;
            // We also reset the palette because ITEM_TIME changes it.
            if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
            {
                set_palette_mode(PAL_MODE_DEFAULT);
            }
        }
    }

    // Special case for the time item that towards the end the custom palette
    // starts blinking back to the original to show the power is ending.
    if (gApp.current_item == ITEM_TIME)
    {
        if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
        {
            if (floor(gApp.item_time) <= 0.0f)
            {
                if (gApp.frame % 2 == 0) set_palette_mode(PAL_MODE_DEFAULT);
                else set_palette_mode(PAL_MODE_SLOWDOWN);
            }
        }
    }

    // Half the speed of everything if the slowdown item was recieved.
    float gamedt = dt;
    if (gApp.current_item == ITEM_TIME)
    {
        gamedt /= 2.0f;
    }

    update_spawner(gamedt);
    update_entity (gamedt);
    update_effect (gamedt);

    // Handle shooting.
    bool shot = false;
    if (button_pressed(LMB))
    {
        if (is_cursor_in_screen_bounds())
        {
            if (gApp.god_time <= 0.0f) // You cannot shoot after being hit!.
            {
                if (gApp.shoot_cooldown <= 0.0f)
                {
                    shoot();
                    shot = true;
                }
            }
        }
    }

    collide_entity(shot);
}
INTERNAL void render_game (float dt)
{
    // Half the speed of everything if the slowdown item was recieved.
    float gamedt = dt;
    if (gApp.current_item == ITEM_TIME)
    {
        gamedt /= 2.0f;
    }

    begin_camera();
    render_effect_lo(gamedt);
    render_entity(gamedt);
    render_effect_hi(gamedt);
    end_camera();

    render_cursor();
    render_hud(2,true);
}

// APP_STATE_GAMEOVER

INTERNAL void update_gameover (float dt)
{
    if (button_pressed(LMB))
    {
        if (is_cursor_in_screen_bounds())
        {
            if (gApp.shoot_cooldown <= 0.0f)
            {
                start_game(dt);
                shoot(); // Do this second so effects don't get wiped.
            }
        }
    }
}
INTERNAL void render_gameover (float dt)
{
    render_cursor();

    begin_camera();
    render_hud((SCREEN_H-TILE_H)/2,false);
    end_camera();
}

// APPLICATION

INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);

    gApp.cursor_x      = get_mouse_x();
    gApp.cursor_y      = get_mouse_y();
    gApp.cursor_prev_x = gApp.cursor_x;
    gApp.cursor_prev_y = gApp.cursor_y;

    seed_random();
    start_menu();

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
                    if (gApp.current_item == ITEM_TIME)
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
    // Quit the game.
    if (key_pressed(ESCAPE))
    {
        gWindow.running = false;
        return;
    }

    gApp.frame++;

    if (gApp.shoot_cooldown > 0.0f)
    {
        gApp.shoot_cooldown -= dt;
    }

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

// CURSOR

INTERNAL bool is_cursor_in_screen_bounds ()
{
    int x = get_mouse_x()-3, y = get_mouse_y()-3, w = 6, h = 6;
    return rect_vs_rect_collision(x,y,w,h, 0,0,SCREEN_W,SCREEN_H);
}

INTERNAL void cursor_hit ()
{
    if (gApp.god_time <= 0.0f) // If we don't have invincibility frames.
    {
        gApp.god_time = GOD_DURATION;
        gApp.life--;

        play_sound(SND_SMACK,0);
        shake_camera(2,2,0.25f);

        if (gApp.life <= 0)
        {
            game_over();
        }
    }
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

    gApp.current_item = ITEM_NONE;
    gApp.item_time = 0.0f;
    gApp.score = 0;
    gApp.life = MAX_LIFE;
    gApp.god_time = 0;
}
INTERNAL void game_over ()
{
    gApp.shoot_cooldown = GAMEOVER_COOLDOWN;
    gApp.state = APP_STATE_GAMEOVER;
    gApp.god_time = 0.0f;
    if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
    {
        set_palette_mode(PAL_MODE_DEFAULT);
    }
}
