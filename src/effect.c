// EFX_BUBBLE

#define BUBBLE_MAX_VELOCITY 150
#define BUBBLE_VELOCITY_INC 10

INTERNAL void update_bubble (Effect* effect, float dt)
{
    effect->y -= effect->vy * dt;

    effect->vy += BUBBLE_VELOCITY_INC;
    if (effect->vy > BUBBLE_MAX_VELOCITY)
    {
        effect->vy = BUBBLE_MAX_VELOCITY;
    }

    // Kill any bubbles that go off the top of the screen.
    if (effect->y + effect->clip->h < 0)
    {
        effect->alive = false;
    }
}

// EFX_BLOOD

INTERNAL void update_blood (Effect* effect, float dt)
{
    effect->x += effect->vx * dt;
    effect->y += effect->vy * dt;
}

// EFFECTS

INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count)
{
    int count = random_int_range(min_count, max_count);

    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (!effect->alive)
        {
            effect->alive = true;
            effect->type = id;

            effect->x  = random_int_range(x,x+w) - 4; // @Hardcoded: All effects are currently 8x8 pixels!
            effect->y  = random_int_range(y,y+h) - 4; // @Hardcoded: All effects are currently 8x8 pixels!
            effect->vx = 0;
            effect->vy = 0;
            effect->t  = 0.0f;

            // Specific creation logic for the different effect types.
            switch (effect->type)
            {
                // @Incomplete: Clean-up these inner switch statements as they're pretty gross. We'll
                // do this by cleaning up and improving the sprite animation system in generla first.
                case (EFX_BUBBLE):
                {
                    effect->palette = PAL_BUBBLE;
                    switch (random_int_range(0,3))
                    {
                        case (0): effect->clip = &SPR_BUBBLE_0; break;
                        case (1): effect->clip = &SPR_BUBBLE_1; break;
                        case (2): effect->clip = &SPR_BUBBLE_2; break;
                        case (3): effect->clip = &SPR_BUBBLE_3; break;
                    }
                } break;
                case (EFX_BLOOD):
                {
                    effect->palette = PAL_BLOOD;
                    effect->vx = 10;
                    effect->vy = 0;
                    rotate_vec2(&effect->vx, &effect->vy, random_float_range(0,M_PI*2));
                    switch (random_int_range(0,7))
                    {
                        case (0): effect->clip = &SPR_BLOOD_0; break;
                        case (1): effect->clip = &SPR_BLOOD_1; break;
                        case (2): effect->clip = &SPR_BLOOD_2; break;
                        case (3): effect->clip = &SPR_BLOOD_3; break;
                        case (4): effect->clip = &SPR_BLOOD_4; break;
                        case (5): effect->clip = &SPR_BLOOD_5; break;
                        case (6): effect->clip = &SPR_BLOOD_6; break;
                        case (7): effect->clip = &SPR_BLOOD_7; break;
                    }
                } break;
            }

            count--;
            if (count <= 0)
            {
                break;
            }
        }
    }
}

INTERNAL void update_effect (float dt)
{
    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            // Specific update logic for the different effect types.
            switch (effect->type)
            {
                case (EFX_BUBBLE): update_bubble(effect, dt); break;
                case (EFX_BLOOD ): update_blood (effect, dt); break;
            }
        }
    }
}

INTERNAL void render_effect (float dt)
{
    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            render_bitmap(effect->x,effect->y, effect->palette, effect->clip);
        }
    }
}
