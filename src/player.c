INTERNAL void shoot ()
{
    int mx = get_mouse_x();
    int my = get_mouse_y();

    // Region for spawning bubbles.
    int bx = mx-6;
    int by = my-6;
    int bw = 12;
    int bh = 12;

    create_effect(EFX_BUBBLE, bx,by,bw,bh, 2,3);
    create_effect(EFX_SHOT, mx,my,1,1, 1,1);

    shake_camera(1,1,0.1f);

    if (gPlayer.current_item == ITEM_RAPD) play_sound(SND_RSHOT[random_int_range(0,ARRAYSIZE(SND_RSHOT)-1)],0);
    else play_sound(SND_NSHOT[random_int_range(0,ARRAYSIZE(SND_NSHOT)-1)],0);

    // Region that is deadly for entities.
    int sx = mx-2;
    int sy = my-2;
    int sw = 4;
    int sh = 4;

    collide_entity_vs_shot(sx,sy,sw,sh);
}

INTERNAL void init_player ()
{
    gPlayer.cursor_x      = get_mouse_x();
    gPlayer.cursor_y      = get_mouse_y();
    gPlayer.cursor_prev_x = gPlayer.cursor_x;
    gPlayer.cursor_prev_y = gPlayer.cursor_y;
}

INTERNAL void create_player ()
{
    gPlayer.current_item = ITEM_NONE;
    gPlayer.item_time = 0.0f;
    gPlayer.life = MAX_LIFE;
    gPlayer.god_time = 0;
}

INTERNAL void update_player (float dt)
{
    // Update tiomers.
    if (gPlayer.god_time > 0.0f)
    {
        gPlayer.god_time -= dt;
    }
    if (gPlayer.item_time > 0.0f)
    {
        gPlayer.item_time -= dt;
        if (gPlayer.item_time < 0.0f)
        {
            gPlayer.current_item = ITEM_NONE;
            // We also reset the palette because ITEM_TIME changes it.
            if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
            {
                set_palette_mode(PAL_MODE_DEFAULT);
            }
        }
    }
    if (gPlayer.cooldown_time > 0.0f)
    {
        gPlayer.cooldown_time -= dt;
    }

    // Special case for the time item that towards the end the custom palette
    // starts blinking back to the original to show the power is ending.
    if (gPlayer.current_item == ITEM_TIME)
    {
        if (!gApp.code_retro_enabled && !gApp.code_1bits_enabled)
        {
            if (floor(gPlayer.item_time) <= 0.0f)
            {
                if (gApp.frame % 2 == 0) set_palette_mode(PAL_MODE_DEFAULT);
                else set_palette_mode(PAL_MODE_SLOWDOWN);
            }
        }
    }

    // Handle shooting.
    bool shot = false;
    if (is_player_in_screen_bounds())
    {
        if (gPlayer.god_time <= 0.0f) // You cannot shoot after being hit!.
        {
            if (gPlayer.cooldown_time <= 0.0f)
            {
                if (gPlayer.current_item == ITEM_RAPD)
                {
                    if (button_down(LMB))
                    {
                        gPlayer.cooldown_time = 0.1f;
                        shoot();
                        shot = true;
                    }
                }
                else
                {
                    if (button_pressed(LMB))
                    {
                        if (gApp.state == APP_STATE_GAMEOVER)
                        {
                            start_game();
                        }
                        shoot();
                        shot = true;
                    }
                }
            }
        }
    }

    collide_entity_vs_player(get_mouse_x(),get_mouse_y());
}

INTERNAL void render_player (float dt)
{
    gPlayer.cursor_prev_x = gPlayer.cursor_x;
    gPlayer.cursor_prev_y = gPlayer.cursor_y;
    gPlayer.cursor_x      = get_mouse_x();
    gPlayer.cursor_y      = get_mouse_y();

    // If the cursor moved then draw a trail behind it.
    if (gPlayer.cursor_x != gPlayer.cursor_prev_x && gPlayer.cursor_y != gPlayer.cursor_prev_y)
    {
        render_line(gPlayer.cursor_x,  gPlayer.cursor_y,   gPlayer.cursor_prev_x, gPlayer.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gPlayer.cursor_x+1,gPlayer.cursor_y,   gPlayer.cursor_prev_x, gPlayer.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gPlayer.cursor_x-1,gPlayer.cursor_y,   gPlayer.cursor_prev_x, gPlayer.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gPlayer.cursor_x,  gPlayer.cursor_y+1, gPlayer.cursor_prev_x, gPlayer.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
        render_line(gPlayer.cursor_x,  gPlayer.cursor_y-1, gPlayer.cursor_prev_x, gPlayer.cursor_prev_y, get_palette_color(PAL_CURSOR,2));
    }

    // Draw the actual cursor graphic.
    bool visible = true;
    if (gPlayer.god_time > 0.0f)
    {
        if (gApp.frame % 2 == 0) // Flicker when invincible!
        {
            visible = false;
        }
    }
    if (visible)
    {
        int x = gPlayer.cursor_x-(SPR_CURSOR_0.w/2);
        int y = gPlayer.cursor_y-(SPR_CURSOR_0.h/2);
        render_bitmap(x,y,PAL_CURSOR,&SPR_CURSOR_0);
    }
}

INTERNAL void hit_player ()
{
    if (gPlayer.god_time <= 0.0f) // If we don't have invincibility frames.
    {
        gPlayer.god_time = GOD_DURATION;
        gPlayer.life--;

        play_sound(SND_SMACK,0);
        shake_camera(2,2,0.25f);

        if (gPlayer.life <= 0)
        {
            game_over();
        }
    }
}

INTERNAL bool is_player_in_screen_bounds ()
{
    int x = get_mouse_x()-3, y = get_mouse_y()-3, w = 6, h = 6;
    return rect_vs_rect_collision(x,y,w,h, 0,0,SCREEN_W,SCREEN_H);
}
