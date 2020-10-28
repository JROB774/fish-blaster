// HELPERS

INTERNAL void render_score (int y)
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
}

INTERNAL void render_hud (int y)
{
    render_score(y);

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

typedef enum MenuButton__
{
    MENU_BUTTON_PLAY,
    MENU_BUTTON_OPTIONS,
    MENU_BUTTON_SCORES,
    MENU_BUTTON_CREDITS,
    MENU_BUTTON_EXIT,
    MENU_BUTTON_BACK,
    MENU_BUTTON_RETRY,
    MENU_BUTTON_MENU,
    MENU_BUTTON_SOUND,
    MENU_BUTTON_MUSIC,
    MENU_BUTTON_CURSOR,
    MENU_BUTTON_FULLSCREEN,
    MENU_BUTTON_RESET,
    MENU_BUTTON_TOTAL

} MenuButton;

// Stores the state values needed for lerping button backgrounds.
GLOBAL float gMenuButtonTarget[MENU_BUTTON_TOTAL];
GLOBAL float gMenuButtonCurrent[MENU_BUTTON_TOTAL];

#define MENU_BUTTON_BUFFER_SIZE 64

INTERNAL bool do_menu_button (int* x, int* y, MenuButton button, bool center, float dt, const char* text, ...)
{
    assert(x && y);

    char text_buffer[MENU_BUTTON_BUFFER_SIZE] = {0};

    // Format the arguments into a formatted string in a buffer.
    va_list args;
    va_start(args, text);
    vsnprintf(text_buffer, MENU_BUTTON_BUFFER_SIZE, text, args);
    va_end(args);

    int rw = get_text_w(text_buffer);
    int rh = TILE_H;
    int rx = (center) ? ((SCREEN_W-rw)/2) : *x;
    int ry = *y;

    int mx = get_mouse_x();
    int my = get_mouse_y();

    // Handle hovering and pressing the button.
    bool pressed = false;
    if (point_vs_rect_collision(mx,my, rx,ry,rw,rh))
    {
        if (gMenuButtonTarget[button] == 0.0f)
        {
            play_sound_channel(SND_SWISH,0,1);
            gMenuButtonTarget[button] = rw;
        }

        if (button_pressed(LMB))
        {
            pressed = true;
        }
    }
    else
    {
        gMenuButtonTarget[button] = 0.0f;
    }

    // Animate the backing of the button.
    gMenuButtonCurrent[button] = lerp(gMenuButtonCurrent[button], gMenuButtonTarget[button], 20*dt);
    if (gMenuButtonCurrent[button] > 1.0f)
    {
        int bx = rx+((rw-(round(gMenuButtonCurrent[button])))/2);
        int by = ry;
        int bw = ceil(gMenuButtonCurrent[button]);
        int bh = rh;

        render_bitmap(bx-SPR_SCOREBGL.w,by,PAL_BLACK,&SPR_SCOREBGL);
        render_fill(bx,by,bw,bh,get_palette_color(PAL_BLACK,0));
        render_bitmap(bx+bw,by,PAL_BLACK,&SPR_SCOREBGR);
    }

    render_text(rx,ry,PAL_TEXT_SHADE,text_buffer);

    *x  = rx;
    *y += rh+2;

    return pressed;
}
INTERNAL void do_menu_score_label (int* x, int* y, U32 score)
{
    assert(x && y);

    const char* SCORE_TEXT = "%06d";

    int rw = get_text_w(SCORE_TEXT,score);
    int rh = TILE_H;
    int rx = (SCREEN_W-rw)/2;
    int ry = *y;

    render_bitmap(rx-SPR_SCOREBGL.w,ry,PAL_BLACK,&SPR_SCOREBGL);
    render_fill(rx,ry,rw,rh,get_palette_color(PAL_BLACK,0));
    render_bitmap(rx+rw,ry,PAL_BLACK,&SPR_SCOREBGR);
    render_text(rx,ry,PAL_TEXT_SHADE,SCORE_TEXT,score);

    *x  = rx;
    *y += rh+2;
}

INTERNAL void reset_menu_button_state ()
{
    memset(gMenuButtonTarget, 0, sizeof(gMenuButtonTarget));
    memset(gMenuButtonCurrent, 0, sizeof(gMenuButtonCurrent));
}

INTERNAL void start_menu_main ()
{
    gApp.menu_state = MENU_STATE_MAIN;
    reset_menu_button_state();
}
INTERNAL void start_menu_options ()
{
    gApp.menu_state = MENU_STATE_OPTIONS;
    reset_menu_button_state();
}
INTERNAL void start_menu_scores ()
{
    gApp.menu_state = MENU_STATE_SCORES;
    reset_menu_button_state();
}
INTERNAL void start_menu_credits ()
{
    gApp.menu_state = MENU_STATE_CREDITS;
    reset_menu_button_state();
}

INTERNAL void update_menu (float dt)
{
    // Go to the menu if the player wants.
    if (gApp.menu_state != MENU_STATE_MAIN)
    {
        if (key_pressed(ESCAPE))
        {
            start_menu_main();
            return;
        }
    }

    update_spawner(dt);
    update_player (dt);
    update_entity (dt);
    update_effect (dt);
}
INTERNAL void render_menu (float dt)
{
    begin_camera();
    render_effect_lo(dt);
    render_entity   (dt);
    render_effect_hi(dt);
    end_camera();

    int x = 0;
    int y = 0;

    switch (gApp.menu_state)
    {
        case (MENU_STATE_MAIN):
        {
            render_bitmap(0,12,PAL_TEXT_SHADE,&SPR_TITLE);

            y = 76;

            if (do_menu_button(&x,&y, MENU_BUTTON_PLAY,    true, dt, "PLAY"   )) start_game();
            if (do_menu_button(&x,&y, MENU_BUTTON_OPTIONS, true, dt, "OPTIONS")) start_menu_options();
            if (do_menu_button(&x,&y, MENU_BUTTON_SCORES,  true, dt, "SCORES" )) start_menu_scores();
            if (do_menu_button(&x,&y, MENU_BUTTON_CREDITS, true, dt, "CREDITS")) start_menu_credits();
            if (do_menu_button(&x,&y, MENU_BUTTON_EXIT,    true, dt, "EXIT"   )) gWindow.running = false;
        } break;
        case (MENU_STATE_OPTIONS):
        {
            y = 48;

            if (do_menu_button(&x,&y, MENU_BUTTON_FULLSCREEN, true, dt, "FULLSCREEN %s",   is_fullscreen() ? "ON" : "OFF"  )) set_fullscreen(!is_fullscreen());
            if (do_menu_button(&x,&y, MENU_BUTTON_CURSOR,     true, dt, "CURSOR TYPE %d",  gPlayer.current_cursor          )) set_player_cursor_type(++gPlayer.current_cursor);
            if (do_menu_button(&x,&y, MENU_BUTTON_SOUND,      true, dt, "SOUND VOLUME %d", CAST(int,get_sound_volume()*100))) set_sound_volume(get_sound_volume()+0.1f);
            if (do_menu_button(&x,&y, MENU_BUTTON_MUSIC,      true, dt, "MUSIC VOLUME %d", CAST(int,get_music_volume()*100))) set_music_volume(get_music_volume()+0.1f);
            if (do_menu_button(&x,&y, MENU_BUTTON_RESET,      true, dt, "RESET"                                            )) reset_settings();

            x = TILE_W+4;
            y = SCREEN_H-TILE_H-4;
            if (do_menu_button(&x,&y,MENU_BUTTON_BACK, false, dt, "BACK")) start_menu_main();
        } break;
        case (MENU_STATE_SCORES):
        {
            y = 23;
            for (int i=0; i<MAX_SCORES; ++i) do_menu_score_label(&x,&y,gScores[i]);
            x = TILE_W+4;
            y = SCREEN_H-TILE_H-4;
            if (do_menu_button(&x,&y,MENU_BUTTON_BACK, false, dt, "BACK")) start_menu_main();
        } break;
        case (MENU_STATE_CREDITS):
        {
            x = TILE_W+4;
            y = SCREEN_H-TILE_H-4;
            if (do_menu_button(&x,&y,MENU_BUTTON_BACK, false, dt, "BACK")) start_menu_main();
        } break;
    }

    render_player(dt);
}

// APP_STATE_GAME

INTERNAL void update_game (float dt)
{
    // // Restart the game if the player wants.
    if (key_pressed(RESTART))
    {
        start_game();
        return;
    }
    // Go to the menu if the player wants.
    if (key_pressed(ESCAPE))
    {
        start_menu();
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
    render_hud(2);
}

// APP_STATE_LOSE

INTERNAL void update_lose (float dt)
{
    // // Restart the game if the player wants.
    if (key_pressed(RESTART))
    {
        start_game();
        return;
    }
    // Go to the menu if the player wants.
    if (key_pressed(ESCAPE))
    {
        start_menu();
        return;
    }

    update_spawner(dt);
    update_player (dt);
    update_entity (dt);
    update_effect (dt);
}
INTERNAL void render_lose (float dt)
{
    begin_camera();
    render_effect_lo(dt);
    render_entity   (dt);
    render_effect_hi(dt);
    end_camera();

    int x = 0;
    int y = (SCREEN_H-TILE_H)/2;

    render_score(y);
    if (gApp.score == gScores[0]) // If we're the new highest score...
    {
        const char* HIGHSCORE_TEXT = "NEW HIGHSCORE!";

        int sw = get_text_w(HIGHSCORE_TEXT);
        int sh = TILE_H;
        int sx = (SCREEN_W-sw)/2;
        int sy = y-TILE_H-2;

        render_bitmap(sx-SPR_SCOREBGL.w,sy, PAL_BLACK, &SPR_SCOREBGL);
        render_fill(sx,sy,sw,sh, get_palette_color(PAL_BLACK,0));
        render_bitmap(sx+sw,sy, PAL_BLACK, &SPR_SCOREBGR);
        render_text(sx,sy,PAL_TEXT_SHADE,HIGHSCORE_TEXT);
    }

    y += 32;

    if (do_menu_button(&x,&y,MENU_BUTTON_RETRY,true,dt,"RETRY")) start_game();
    if (do_menu_button(&x,&y,MENU_BUTTON_MENU, true,dt,"MENU" )) start_menu();

    render_player(dt);
}

// APPLICATION

INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);

    seed_random();
    start_menu ();
    init_player();
    load_scores();

    create_spawner();

    play_music(MUS_TRACK[random_int_range(0,ARRAYSIZE(MUS_TRACK)-1)],-1);

    return true;
}
INTERNAL void quit_application ()
{
    save_scores();
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
                 strncmp(gApp.code, CODE_NOPAL, gApp.code_length) != 0)
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
                    if (gApp.code_retro_enabled) gApp.code_nopal_enabled = false;
                    play_sound(SND_CODE,0);
                }
                if (strncmp(gApp.code, CODE_BLOOD, gApp.code_length) == 0)
                {
                    gApp.code_blood_enabled = !gApp.code_blood_enabled;
                    play_sound(SND_CODE,0);
                }
                if (strncmp(gApp.code, CODE_NOPAL, gApp.code_length) == 0)
                {
                    gApp.code_nopal_enabled = !gApp.code_nopal_enabled;
                    if (gApp.code_nopal_enabled) gApp.code_retro_enabled = false;
                    play_sound(SND_CODE,0);
                }

                // Special case for the RETRO and NOPAL code when setting the palette.
                if (gApp.code_retro_enabled)
                {
                    set_palette_mode(PAL_MODE_GAMEBOY);
                }
                else if (gApp.code_nopal_enabled)
                {
                    set_palette_mode(PAL_MODE_NOPALETTE);
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
        case (APP_STATE_MENU): update_menu(dt); break;
        case (APP_STATE_GAME): update_game(dt); break;
        case (APP_STATE_LOSE): update_lose(dt); break;
    }

    // Make sure the score cannot go over its maximum value.
    if (gApp.score > MAX_SCORE)
    {
        gApp.score = MAX_SCORE;
    }
}
INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));

    switch (gApp.state)
    {
        case (APP_STATE_MENU): render_menu(dt); break;
        case (APP_STATE_GAME): render_game(dt); break;
        case (APP_STATE_LOSE): render_lose(dt); break;
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
    reset_menu_button_state();

    gApp.state = APP_STATE_MENU;

    gApp.menu_state = MENU_STATE_MAIN;

    gPlayer.cooldown_time = 0.0f;
    gPlayer.god_time = 0.0f;
    gPlayer.current_item = ITEM_NONE;

    if (!gApp.code_retro_enabled && !gApp.code_nopal_enabled)
    {
        set_palette_mode(PAL_MODE_DEFAULT);
    }
}
INTERNAL void start_game ()
{
    gApp.state = APP_STATE_GAME;

    clear_entity();

    create_spawner();
    create_player();

    gApp.score = 0;
}
INTERNAL void start_lose ()
{
    gApp.state = APP_STATE_LOSE;

    gPlayer.cooldown_time = LOSE_COOLDOWN;
    gPlayer.god_time = 0.0f;
    gPlayer.current_item = ITEM_NONE;

    add_highscore(gApp.score);

    if (!gApp.code_retro_enabled && !gApp.code_nopal_enabled)
    {
        set_palette_mode(PAL_MODE_DEFAULT);
    }
}
