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

// EFFECTS

INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count)
{
    int count = random_int_range(min_count, max_count);
    for (int i=0; i<count; ++i)
    {
        for (int j=0; j<EFFECT_MAX; ++j)
        {
            Effect* effect = gEffect+j;
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
                }

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
                case (EFX_BUBBLE): update_bubble (effect, dt);
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
