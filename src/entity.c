// ENT_CRATE

#define CRATE_SPAWN_COUNT 18
#define CRATE_WIDTH  16
#define CRATE_HEIGHT 16
#define CRATE_SPEED  30
#define CRATE_MIN_SPAWN_X 8
#define CRATE_MAX_SPAWN_X (SCREEN_W-CRATE_WIDTH-8)
#define CRATE_MIN_CHIP_0 12
#define CRATE_MAX_CHIP_0 20
#define CRATE_MIN_CHIP_1 1
#define CRATE_MAX_CHIP_1 4
#define CRATE_MIN_BUBBLE 3
#define CRATE_MAX_BUBBLE 7

GLOBAL const Rect CRATE_COLLIDER = { 0,0,CRATE_WIDTH,CRATE_HEIGHT };

INTERNAL Rect get_crate_collider (Entity* entity)
{
    Rect collider;
    collider.x = CRATE_COLLIDER.x + entity->x;
    collider.y = CRATE_COLLIDER.y + entity->y;
    collider.w = CRATE_COLLIDER.w;
    collider.h = CRATE_COLLIDER.h;
    return collider;
}
INTERNAL void create_crate (Entity* entity)
{
    entity->palette = PAL_CRATE;
    entity->x = random_int_range(CRATE_MIN_SPAWN_X,CRATE_MAX_SPAWN_X);
    entity->y = -CRATE_HEIGHT;
}
INTERNAL void update_crate (Entity* entity, float dt)
{
    entity->y += CRATE_SPEED * dt;
    if (entity->y > SCREEN_H) // Deactivate the crate if it goes off-screen.
    {
        entity->alive = false;
    }
}
INTERNAL void render_crate (Entity* entity, float dt)
{
    render_bitmap(entity->x,entity->y,entity->palette,&SPR_CRATE);
    // Draw the identifiying icon for the crate.
    if (gApp.frame % 2 == 0)
    {
        switch (entity->type)
        {
            case (ENT_CRATE_LIFE): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_LIFE,ANM_ICO_LIFE[0]); break;
            case (ENT_CRATE_TIME): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_TIME,ANM_ICO_TIME[0]); break;
            case (ENT_CRATE_MULT): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_MULT,ANM_ICO_MULT[0]); break;
            case (ENT_CRATE_RAPD): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_RAPD,ANM_ICO_RAPD[0]); break;
            case (ENT_CRATE_SPRD): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_SPRD,ANM_ICO_SPRD[0]); break;
            case (ENT_CRATE_BOOM): render_bitmap(entity->x+4,entity->y+4,PAL_ICO_BOOM,ANM_ICO_BOOM[0]); break;
            default:
                break;
        }
    }
}
INTERNAL void kill_crate (Entity* entity)
{
    Rect c = get_crate_collider(entity);
    create_effect(EFX_CHIP_0, entity->x+(CRATE_WIDTH/2),entity->y+(CRATE_HEIGHT/2),1,1, CRATE_MIN_CHIP_0,CRATE_MAX_CHIP_0);
    create_effect(EFX_BUBBLE, c.x,c.y,c.w,c.h, CRATE_MIN_BUBBLE,CRATE_MAX_BUBBLE);
    play_sound(SND_BREAK,0);
    play_sound(SND_ITEM,0);
    entity->alive = false;
}
INTERNAL void collide_crate_vs_shot (Entity* entity, int sx, int sy, int sw, int sh)
{
    Rect c = get_crate_collider(entity);
    if (rect_vs_rect_collision(sx,sy,sw,sh, c.x,c.y,c.w,c.h))
    {
        // Determine which item graphic to display and perform the crate's effect.
        EffectID effect_id = 0;
        switch (entity->type)
        {
            case (ENT_CRATE_LIFE):
            {
                if (gPlayer.life < MAX_LIFE) gPlayer.life++;
                effect_id = EFX_ICO_LIFE;
            } break;
            case (ENT_CRATE_TIME):
            {
                gPlayer.current_item = ITEM_TIME;
                gPlayer.item_time = ITEM_DURATION;
                effect_id = EFX_ICO_TIME;
            } break;
            case (ENT_CRATE_MULT):
            {
                gPlayer.current_item = ITEM_MULT;
                gPlayer.item_time = ITEM_DURATION;
                effect_id = EFX_ICO_MULT;
            } break;
            case (ENT_CRATE_RAPD):
            {
                gPlayer.current_item = ITEM_RAPD;
                gPlayer.item_time = ITEM_DURATION;
                effect_id = EFX_ICO_RAPD;
            } break;
            case (ENT_CRATE_SPRD):
            {
                gPlayer.current_item = ITEM_SPRD;
                gPlayer.item_time = ITEM_DURATION;
                effect_id = EFX_ICO_SPRD;
            } break;
            case (ENT_CRATE_BOOM):
            {
                Entity* boom = create_entity(ENT_BOOM);
                boom->x = entity->x+(CRATE_WIDTH/2);
                boom->y = entity->y+(CRATE_HEIGHT/2);
                effect_id = EFX_ICO_BOOM;
            } break;
            default:
            {
                // Nothing...
            } break;
        }

        // Spawn the icon effect for the item that was collected from the crate.
        create_effect(effect_id, entity->x+(CRATE_WIDTH/2),entity->y+(CRATE_HEIGHT/2),1,1, 1,1);

        // Handle swapping the palette.
        if (!gApp.code_retro_enabled && !gApp.code_nopal_enabled) // Don't want to change palette if using the special RETRO or NOPAL codes.
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

        kill_crate(entity);
    }
}

// ENT_FISH

#define FISH_SPAWN_SCHOOL_CHANCE 0.05f
#define FISH_SPAWN_START 0.0f
#define FISH_SPAWN_RATE  0.5f
#define FISH_ANM_SPEED   0.2f
#define FISH_SPEED 50
#define FISH_SCORE 10
#define FISH_DIR_L 0
#define FISH_DIR_R 1
#define FISH_MIN_SPAWN_Y 12
#define FISH_MAX_SPAWN_Y (SCREEN_H-8)
#define FISH_MIN_BLOOD 10
#define FISH_MAX_BLOOD 15
#define FISH_MIN_GIB   1
#define FISH_MAX_GIB   4

GLOBAL const Rect FISH_COLLIDER = { 2,1,12,6 };

INTERNAL Rect get_fish_collider (Entity* entity)
{
    Rect collider;
    collider.x = FISH_COLLIDER.x + entity->x;
    collider.y = FISH_COLLIDER.y + entity->y;
    collider.w = FISH_COLLIDER.w;
    collider.h = FISH_COLLIDER.h;
    return collider;
}
INTERNAL void create_fish (Entity* entity)
{
    entity->palette = random_int_range(PAL_FISH_0,PAL_FISH_2);
    entity->dir = random_int_range(FISH_DIR_L,FISH_DIR_R);
    entity->t = FISH_ANM_SPEED;
    entity->frame = 0;

    if (entity->dir == FISH_DIR_L) entity->x = SCREEN_W;
    if (entity->dir == FISH_DIR_R) entity->x = 0 - SPR_FISH_R_0.w;

    entity->y = random_int_range(FISH_MIN_SPAWN_Y,FISH_MAX_SPAWN_Y);
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
INTERNAL void render_fish (Entity* entity, float dt)
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
    const Clip* clip = ((entity->dir == FISH_DIR_L) ? ANM_FISH_L[entity->frame] : ANM_FISH_R[entity->frame]);
    render_bitmap(entity->x,entity->y,entity->palette,clip);

}
INTERNAL void kill_fish (Entity* entity)
{
    Rect c = get_fish_collider(entity);
    create_effect(EFX_BLOOD, c.x,c.y,c.w,c.h, FISH_MIN_BLOOD,FISH_MAX_BLOOD);
    create_effect(EFX_GIB, c.x,c.y,c.w,c.h, FISH_MIN_GIB,FISH_MAX_GIB);
    play_sound(SND_SQUEAK[random_int_range(0,ARRAYSIZE(SND_SQUEAK)-1)],0);
    gSpawner.crate_spawn_counter++;
    entity->alive = false;
    if (gApp.state == APP_STATE_GAME)
    {
        if (gPlayer.current_item == ITEM_MULT)
        {
            create_effect(EFX_SCORE20, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += (FISH_SCORE*2);
        }
        else
        {
            create_effect(EFX_SCORE10, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += FISH_SCORE;
        }
    }
}
INTERNAL void collide_fish_vs_shot (Entity* entity, int sx, int sy, int sw, int sh)
{
    // Handle killing the fish if its shot at any time.
    Rect c = get_fish_collider(entity);
    if (rect_vs_rect_collision(sx,sy,sw,sh, c.x,c.y,c.w,c.h))
    {
        kill_fish(entity);
    }
}

// ENT_SQUID

INTERNAL Rect get_squid_collider (Entity* entity)
{
    Rect collider;
    // @Incomplete; ...
    return collider;
}
INTERNAL void create_squid (Entity* entity)
{
    // @Incomplete: ...
}
INTERNAL void update_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
}
INTERNAL void render_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
}
INTERNAL void kill_squid (Entity* entity)
{
    // @Incomplete: ...
}
INTERNAL void collide_squid_vs_shot (Entity* entity, int sx, int sy, int sw, int sh)
{
    // @Incomplete: ...
}

// ENT_JELLY

#define JELLY_FLASH_COOLDOWN  1.5f
#define JELLY_FLASH_DURATION  1.0f
#define JELLY_FLASH_LEEWAY    0.1f
#define JELLY_SPAWN_START    30.0f
#define JELLY_SPAWN_RATE      2.0f
#define JELLY_ANM_SPEED       0.2f
#define JELLY_MAX     5
#define JELLY_WIDTH  16
#define JELLY_HEIGHT 16
#define JELLY_SPEED  40
#define JELLY_SCORE  40
#define JELLY_FLASH_SPEED 6
#define JELLY_MIN_SPAWN_X 8
#define JELLY_MAX_SPAWN_X (SCREEN_W-JELLY_WIDTH-8)
#define JELLY_MIN_BLOOD 20
#define JELLY_MAX_BLOOD 35
#define JELLY_MIN_GIB   2
#define JELLY_MAX_GIB   5

GLOBAL const Rect JELLY_COLLIDER = { 2,2,12,12 };

INTERNAL Rect get_jelly_collider (Entity* entity)
{
    Rect collider;
    collider.x = JELLY_COLLIDER.x + entity->x;
    collider.y = JELLY_COLLIDER.y + entity->y;
    collider.w = JELLY_COLLIDER.w;
    collider.h = JELLY_COLLIDER.h;
    return collider;
}
INTERNAL void create_jelly (Entity* entity)
{
    entity->palette = PAL_JELLY;
    entity->frame = 0;
    entity->extra = -1; // Used to store the channel the zap sound is played on.
    entity->t = JELLY_ANM_SPEED;
    entity->t2 = JELLY_FLASH_COOLDOWN;
    entity->x = random_int_range(JELLY_MIN_SPAWN_X,JELLY_MAX_SPAWN_X);
    entity->y = SCREEN_H;
}
INTERNAL void update_jelly (Entity* entity, float dt)
{
    // Handle the timings for making the jelly go electric/deadly.
    entity->t2 -= dt;
    if (entity->t2 <= -JELLY_FLASH_DURATION)
    {
        entity->t2 = JELLY_FLASH_COOLDOWN;
        entity->frame = 0;
        if (entity->extra != -1)
        {
            stop_channel(entity->extra);
            entity->extra = -1;
        }
    }
    // Move the jelly.
    if (entity->t2 > 0.0f) // Don't move the jelly if flashing.
    {
        entity->y -= JELLY_SPEED * dt;
        if (entity->y < -JELLY_HEIGHT) // Deactivate the jelly if ity goes off-screen.
        {
            gSpawner.jelly_count--;
            entity->alive = false;
        }
    }
    // If the jelly just started being electric play the sound.
    if (entity->t2 <= 0.0f)
    {
        if (entity->extra == -1) // -1 is used to mean we haven't done this yet.
        {
            // Play the sound on continuous loop but store the channel so we can stop it later.
            entity->extra = play_sound(SND_ZAP,-1);
        }
    }
}
INTERNAL void render_jelly (Entity* entity, float dt)
{
    if (entity->t2 <= 0.0f) // Jelly is electric!
    {
        if ((++entity->frame) >= JELLY_FLASH_SPEED) entity->frame = 0;
        int palette = (entity->frame < (JELLY_FLASH_SPEED/2)) ? PAL_JELLYFLASH0 : PAL_JELLYFLASH1;
        render_bitmap(entity->x,entity->y,palette,&SPR_JELLY_0);
    }
    else // Jelly is normal!
    {
        entity->t -= dt;
        if (entity->t <= 0.0f)
        {
            entity->t = JELLY_ANM_SPEED;
            entity->frame++;
            if (entity->frame >= ARRAYSIZE(ANM_JELLY))
            {
                entity->frame = 0;
            }
        }
        render_bitmap(entity->x,entity->y,entity->palette,ANM_JELLY[entity->frame]);
    }
}
INTERNAL void kill_jelly (Entity* entity)
{
    Rect c = get_jelly_collider(entity);
    create_effect(EFX_BLOOD, c.x,c.y,c.w,c.h, JELLY_MIN_BLOOD,JELLY_MAX_BLOOD);
    create_effect(EFX_GIB, c.x,c.y,c.w,c.h, JELLY_MIN_GIB,JELLY_MAX_GIB);
    play_sound(SND_SQUEAK[random_int_range(0,ARRAYSIZE(SND_SQUEAK)-1)],0);
    gSpawner.crate_spawn_counter++;
    gSpawner.jelly_count--;
    entity->alive = false;
    if (gApp.state == APP_STATE_GAME)
    {
        if (gPlayer.current_item == ITEM_MULT)
        {
            create_effect(EFX_SCORE80, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += (JELLY_SCORE*2);
        }
        else
        {
            create_effect(EFX_SCORE40, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += JELLY_SCORE;
        }
    }
    // If the zap sound is still playing then stop it.
    if (entity->extra != -1)
    {
        stop_channel(entity->extra);
        entity->extra = -1;
    }
}
INTERNAL void collide_jelly_vs_player (Entity* entity, int px, int py)
{
    // Hurt the player if they mouse over an electric jelly.
    Rect c = get_jelly_collider(entity);
    if (entity->t2 <= -JELLY_FLASH_LEEWAY) // We have some leeway where the player won't get hurt.
    {
        if (point_vs_rect_collision(px,py, c.x,c.y,c.w,c.h))
        {
            hit_player();
        }
    }
}
INTERNAL void collide_jelly_vs_shot (Entity* entity, int sx, int sy, int sw, int sh)
{
    // Handle killing the jelly if its shot at any time.
    Rect c = get_jelly_collider(entity);
    if (rect_vs_rect_collision(sx,sy,sw,sh, c.x,c.y,c.w,c.h))
    {
        kill_jelly(entity);
    }
}

// ENT_URCHIN

#define URCHIN_SPAWN_START      8.0f
#define URCHIN_SPAWN_RATE       1.0f
#define URCHIN_START_COOLDOWN   1.5f
#define URCHIN_INCREMENT_START 10.0f
#define URCHIN_INCREMENT_RATE  10.0f
#define URCHIN_ANM_SPEED        0.2f
#define URCHIN_MAX_START 5
#define URCHIN_WIDTH    16
#define URCHIN_HEIGHT   16
#define URCHIN_SPEED    40
#define URCHIN_SCORE    20
#define URCHIN_MIN_SPAWN_X 0
#define URCHIN_MAX_SPAWN_X (SCREEN_W-URCHIN_WIDTH)
#define URCHIN_SPAWN_ANGLE (M_PI/4)
#define URCHIN_MIN_BLOOD 20
#define URCHIN_MAX_BLOOD 35
#define URCHIN_MIN_GIB   2
#define URCHIN_MAX_GIB   5

GLOBAL const Rect URCHIN_COLLIDER = { 4,4,8,8 };

INTERNAL Rect get_urchin_collider (Entity* entity)
{
    Rect collider;
    collider.x = URCHIN_COLLIDER.x + entity->x;
    collider.y = URCHIN_COLLIDER.y + entity->y;
    collider.w = URCHIN_COLLIDER.w;
    collider.h = URCHIN_COLLIDER.h;
    return collider;
}
INTERNAL void create_urchin (Entity* entity)
{
    entity->palette = PAL_URCHIN;
    entity->t = URCHIN_ANM_SPEED;
    entity->t2 = URCHIN_START_COOLDOWN;
    entity->x = random_int_range(URCHIN_MIN_SPAWN_X,URCHIN_MAX_SPAWN_X);
    entity->y = SCREEN_H;
    entity->frame = 0;
    entity->active = false;

    entity->vx = 0;
    entity->vy = -URCHIN_SPEED;
    rotate_vec2(&entity->vx, &entity->vy, random_float_range(-URCHIN_SPAWN_ANGLE,URCHIN_SPAWN_ANGLE));
}
INTERNAL void update_urchin (Entity* entity, float dt)
{
    // When urchin's spawn they have a period before they are deadly.
    if (entity->t2 > 0.0f)
    {
        entity->t2 -= dt;
    }

    // Urchin bouncing.
    if (entity->active)
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
                        Rect a = get_urchin_collider(entity);
                        Rect b = get_urchin_collider(other);

                        bool collision = false;
                        if (rect_vs_rect_collision(a.x+(entity->vx*dt),a.y,a.w,a.h, b.x,b.y,b.w,b.h)) // Horizontal.
                        {
                            entity->vx *= -1;
                            other->vx *= -1;
                            collision = true;
                        }
                        if (rect_vs_rect_collision(a.x,a.y+(entity->vy*dt),a.w,a.h, b.x,b.y,b.w,b.h)) // Vertical.
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
INTERNAL void render_urchin (Entity* entity, float dt)
{
    // Whilst urchin's arent deadly (just after spawn) they flash.
    bool visible = true;
    if (entity->t2 > 0.0f)
    {
        visible = (gApp.frame % 2 == 0);
    }

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

    if (visible)
    {
        render_bitmap(entity->x,entity->y,entity->palette,ANM_URCHIN[entity->frame]);
    }

}
INTERNAL void kill_urchin (Entity* entity)
{
    Rect c = get_urchin_collider(entity);
    create_effect(EFX_BLOOD, c.x,c.y,c.w,c.h, URCHIN_MIN_BLOOD,URCHIN_MAX_BLOOD);
    create_effect(EFX_GIB, c.x,c.y,c.w,c.h, URCHIN_MIN_GIB,URCHIN_MAX_GIB);
    play_sound(SND_SQUEAK[random_int_range(0,ARRAYSIZE(SND_SQUEAK)-1)],0);
    gSpawner.crate_spawn_counter++;
    gSpawner.urchin_count--;
    entity->alive = false;
    if (gApp.state == APP_STATE_GAME)
    {
        if (gPlayer.current_item == ITEM_MULT)
        {
            create_effect(EFX_SCORE40, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += (URCHIN_SCORE*2);
        }
        else
        {
            create_effect(EFX_SCORE20, c.x+c.w/2,c.y+c.h/2,1,1, 1,1);
            gApp.score += URCHIN_SCORE;
        }
    }
}
INTERNAL void collide_urchin_vs_player (Entity* entity, int px, int py)
{
    Rect c = get_urchin_collider(entity);
    // Cannot get hurt by an urchin whilst it isn't deadly!
    if (entity->t2 <= 0.0f)
    {
        if (point_vs_rect_collision(px,py, c.x,c.y,c.w,c.h))
        {
            hit_player();
        }
    }
}
INTERNAL void collide_urchin_vs_shot (Entity* entity, int sx, int sy, int sw, int sh)
{
    // Because urchins aren't immediately deadly the player can technically shoot them.
    // So we play a ting sound effect to make it clear they aren't hurt by bullets.
    Rect c = get_urchin_collider(entity);
    if (rect_vs_rect_collision(sx,sy,sw,sh, c.x,c.y,c.w,c.h))
    {
        play_sound(SND_TING[random_int_range(0,ARRAYSIZE(SND_TING)-1)],0);
    }
}

// ENT_BOOM

#define BOOM_EXPAND_SPEED 300
#define BOOM_MAX_RADIUS   (SCREEN_W*2)
#define BOOM_THICKNESS    20

INTERNAL void create_boom (Entity* entity)
{
    // The default position is just the current mouse location. But the caller of create_entity(ENT_BOOM)
    // can get the returned entity and assign an x and y themselves. This is what the item crate does.
    entity->x = get_mouse_x();
    entity->y = get_mouse_y();

    entity->t = 0.0f; // This value is used to mark the boom's radius.

    play_sound(SND_BOOM,0);

    flash_screen_white();
    shake_camera(4,4,0.5f);
}
INTERNAL void update_boom (Entity* entity, float dt)
{
    entity->t += BOOM_EXPAND_SPEED * dt;
    if (entity->t >= BOOM_MAX_RADIUS)
    {
        entity->alive = false;
    }

    // Kill all entities the explosion comes into contact with.
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* other = gEntity+i;
        if (other->alive)
        {
            if (other->type != ENT_BOOM)
            {
                Rect c = {0};
                switch (other->type)
                {
                    case (ENT_CRATE_LIFE):
                    case (ENT_CRATE_TIME):
                    case (ENT_CRATE_MULT):
                    case (ENT_CRATE_RAPD):
                    case (ENT_CRATE_SPRD):
                    case (ENT_CRATE_BOOM): c = get_crate_collider (other); break;
                    case (ENT_FISH      ): c = get_fish_collider  (other); break;
                    case (ENT_SQUID     ): c = get_squid_collider (other); break;
                    case (ENT_JELLY     ): c = get_jelly_collider (other); break;
                    case (ENT_URCHIN    ): c = get_urchin_collider(other); break;
                    default:
                        break;
                }

                if (circle_vs_rect_collision(entity->x,entity->y,entity->t, c.x,c.y,c.w,c.h))
                {
                    switch (other->type)
                    {
                        case (ENT_CRATE_LIFE):
                        case (ENT_CRATE_TIME):
                        case (ENT_CRATE_MULT):
                        case (ENT_CRATE_RAPD):
                        case (ENT_CRATE_SPRD):
                        case (ENT_CRATE_BOOM): kill_crate (other); break;
                        case (ENT_FISH      ): kill_fish  (other); break;
                        case (ENT_SQUID     ): kill_squid (other); break;
                        case (ENT_JELLY     ): kill_jelly (other); break;
                        case (ENT_URCHIN    ): kill_urchin(other); break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}
INTERNAL void render_boom (Entity* entity, float dt)
{
    render_circle(entity->x,entity->y, entity->t,BOOM_THICKNESS, get_palette_color(PAL_WHITE,0));
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

    // If the top fish is out of bounds then tweak to go in bounds.
    if (b->y < FISH_MIN_SPAWN_Y)
    {
        a->y += 8;
        b->y += 8;
        c->y += 8;
    }
    // If the bottom fish is out of bounds then tweak to go in bounds.
    if (c->y > FISH_MAX_SPAWN_Y)
    {
        a->y -= 8;
        b->y -= 8;
        c->y -= 8;
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

    // If the top fish is out of bounds then tweak to go in bounds.
    if (d->y < FISH_MIN_SPAWN_Y)
    {
        a->y += 16;
        b->y += 16;
        c->y += 16;
        d->y += 16;
        e->y += 16;
    }
    // If the bottom fish is out of bounds then tweak to go in bounds.
    if (e->y > FISH_MAX_SPAWN_Y)
    {
        a->y -= 16;
        b->y -= 16;
        c->y -= 16;
        d->y -= 16;
        e->y -= 16;
    }
}

// COLLISION

INTERNAL bool point_vs_rect_collision (float px, float py, float rx, float ry, float rw, float rh)
{
    return ((px >= rx) && (py >= ry) && (px < (rx+rw)) && (py < (ry+rh)));
}

INTERNAL bool rect_vs_rect_collision (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    return ((ax < (bx+bw)) && ((ax+aw) > bx) && (ay < (by+bh)) && ((ay+ah) > by));
}

INTERNAL bool circle_vs_rect_collision (float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
    int x = cx;
    int y = cy;

    if      (cx < (rx   )) x = rx;
    else if (cx > (rx+rw)) x = rx+rw;
    if      (cy < (ry   )) y = ry;
    else if (cy > (ry+rh)) y = ry+rh;

    float dx = CAST(float, cx-x);
    float dy = CAST(float, cy-y);
    float d  = sqrt((dx*dx)+(dy*dy));

    return (d <= cr);
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
            entity->t2 = 0.0f;
            entity->alive = true;

            // Specific creation logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_CRATE_LIFE):
                case (ENT_CRATE_TIME):
                case (ENT_CRATE_MULT):
                case (ENT_CRATE_RAPD):
                case (ENT_CRATE_SPRD):
                case (ENT_CRATE_BOOM): create_crate (entity); break;
                case (ENT_FISH      ): create_fish  (entity); break;
                case (ENT_SQUID     ): create_squid (entity); break;
                case (ENT_JELLY     ): create_jelly (entity); break;
                case (ENT_URCHIN    ): create_urchin(entity); break;
                case (ENT_BOOM      ): create_boom  (entity); break;
                default:
                    break;
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
                case (ENT_CRATE_LIFE):
                case (ENT_CRATE_TIME):
                case (ENT_CRATE_MULT):
                case (ENT_CRATE_RAPD):
                case (ENT_CRATE_SPRD):
                case (ENT_CRATE_BOOM): update_crate (entity, dt); break;
                case (ENT_FISH      ): update_fish  (entity, dt); break;
                case (ENT_SQUID     ): update_squid (entity, dt); break;
                case (ENT_JELLY     ): update_jelly (entity, dt); break;
                case (ENT_URCHIN    ): update_urchin(entity, dt); break;
                case (ENT_BOOM      ): update_boom  (entity, dt); break;
                default:
                    break;
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
                case (ENT_CRATE_LIFE):
                case (ENT_CRATE_TIME):
                case (ENT_CRATE_MULT):
                case (ENT_CRATE_RAPD):
                case (ENT_CRATE_SPRD):
                case (ENT_CRATE_BOOM): render_crate (entity, dt); break;
                case (ENT_FISH      ): render_fish  (entity, dt); break;
                case (ENT_SQUID     ): render_squid (entity, dt); break;
                case (ENT_JELLY     ): render_jelly (entity, dt); break;
                case (ENT_URCHIN    ): render_urchin(entity, dt); break;
                case (ENT_BOOM      ): render_boom  (entity, dt); break;
                default:
                    break;
            }
        }
    }
}

INTERNAL void clear_entity ()
{
    // Make sure to stop zapping sound on all channels it is being played on.
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            if (entity->type == ENT_JELLY)
            {
                if (entity->extra != -1)
                {
                    stop_channel(entity->extra);
                    entity->extra = 0;
                }
            }
        }
    }

    memset(gEntity, 0, sizeof(gEntity));
}

// Collision against the player is done with a point so that the player has more
// freedom to move and is less likely to get hit. Collision with shots is handled
// using a box so the player is more likely to hit fish and creates when shooting.

INTERNAL void collide_entity_vs_player (int px, int py)
{
    // Don't handle collisions if the cursor is out of bounds.
    if (!is_player_in_screen_bounds()) return;

    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific collision logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_JELLY ): collide_jelly_vs_player (entity, px,py); break;
                case (ENT_URCHIN): collide_urchin_vs_player(entity, px,py); break;
                default:
                    break;
            }
        }
    }
}

INTERNAL void collide_entity_vs_shot (int sx, int sy, int sw, int sh)
{    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific collision logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_CRATE_LIFE):
                case (ENT_CRATE_TIME):
                case (ENT_CRATE_MULT):
                case (ENT_CRATE_RAPD):
                case (ENT_CRATE_SPRD):
                case (ENT_CRATE_BOOM): collide_crate_vs_shot (entity, sx,sy,sw,sh); break;
                case (ENT_FISH      ): collide_fish_vs_shot  (entity, sx,sy,sw,sh); break;
                case (ENT_SQUID     ): collide_squid_vs_shot (entity, sx,sy,sw,sh); break;
                case (ENT_JELLY     ): collide_jelly_vs_shot (entity, sx,sy,sw,sh); break;
                case (ENT_URCHIN    ): collide_urchin_vs_shot(entity, sx,sy,sw,sh); break;
                default:
                    break;
            }
        }
    }
}

// SPAWNER

INTERNAL void create_spawner ()
{
    gSpawner.crate_spawn_counter = 0;

    gSpawner.fish_spawn_timer = FISH_SPAWN_START;

    gSpawner.jelly_spawn_timer = JELLY_SPAWN_START;
    gSpawner.jelly_count = 0;
    gSpawner.jelly_max_count = JELLY_MAX;

    gSpawner.urchin_spawn_timer = URCHIN_SPAWN_START;
    gSpawner.urchin_increment_timer = URCHIN_INCREMENT_START;
    gSpawner.urchin_count = 0;
    gSpawner.urchin_max_count = URCHIN_MAX_START;
}

INTERNAL void update_spawner (float dt)
{
    // We do not spawn anything whilst a boom effect is active.
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            if (entity->type == ENT_BOOM)
            {
                return;
            }
        }
    }

    // ENT_FISH
    gSpawner.fish_spawn_timer -= dt;
    if (gSpawner.fish_spawn_timer <= 0.0f)
    {
        // Small percentage chance to spawn a school of fish in a formation.
        gSpawner.fish_spawn_timer = FISH_SPAWN_RATE * 3;
        if (random_float() <= FISH_SPAWN_SCHOOL_CHANCE)
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
            gSpawner.fish_spawn_timer = FISH_SPAWN_RATE;
            create_entity(ENT_FISH);
        }
    }

    // We only spawn the other entity types during actual gameplay.
    if (gApp.state != APP_STATE_GAME)
    {
        return;
    }

    // ENT_CRATE
    if (gSpawner.crate_spawn_counter >= CRATE_SPAWN_COUNT)
    {
        gSpawner.crate_spawn_counter = 0;
        EntityID min_choice = ENT_CRATE_LIFE;
        EntityID max_choice = ENT_CRATE_BOOM;
        if (gPlayer.life >= MAX_LIFE) min_choice++; // Don't spawn life crates if we're at max life!
        create_entity(random_int_range(min_choice,max_choice));
    }

    // ENT_JELLY
    gSpawner.jelly_spawn_timer -= dt;
    if (gSpawner.jelly_spawn_timer <= 0.0f)
    {
        gSpawner.jelly_spawn_timer = JELLY_SPAWN_RATE;
        if (gSpawner.jelly_count < gSpawner.jelly_max_count)
        {
            gSpawner.jelly_count++;
            create_entity(ENT_JELLY);
        }
    }

    // ENT_URCHIN
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
