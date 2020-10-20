// EFX_BUBBLE

#define BUBBLE_MAX_VELOCITY  150
#define BUBBLE_VELOCITY_INC  10
#define BUBBLE_MIN_ANM_SPEED 0.2f
#define BUBBLE_MAX_ANM_SPEED 0.3f

INTERNAL void create_bubble (Effect* effect)
{
    effect->t = random_float_range(BUBBLE_MIN_ANM_SPEED,BUBBLE_MAX_ANM_SPEED);
    effect->palette = PAL_BUBBLE;
    effect->frame = random_int_range(0, ARRAYSIZE(ANM_BUBBLE)-2);
}
INTERNAL void update_bubble (Effect* effect, float dt)
{
    // Quickly apply an upward force to the bubbles.
    effect->vy += BUBBLE_VELOCITY_INC;
    if (effect->vy > BUBBLE_MAX_VELOCITY)
    {
        effect->vy = BUBBLE_MAX_VELOCITY;
    }

    effect->y -= effect->vy * dt;

    // Kill any bubbles that go off the top of the screen.
    if (effect->y + 8 < 0) // @Hardcoded: All effects are currently 8x8 pixels!
    {
        effect->alive = false;
    }

    // Animate the bubble and if it is done animating then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        effect->t = random_float_range(BUBBLE_MIN_ANM_SPEED,BUBBLE_MAX_ANM_SPEED);
        effect->frame++;
        if (effect->frame >= ARRAYSIZE(ANM_BUBBLE))
        {
            effect->alive = false;
        }
    }
}

// EFX_BLOOD

#define BLOOD_MIN_ANM_SPEED 0.2f
#define BLOOD_MAX_ANM_SPEED 0.6f
#define BLOOD_VELOCITY_INC  0.5f
#define BLOOD_MAX_VELOCITY  10

INTERNAL void create_blood (Effect* effect)
{
    effect->t = random_float_range(BLOOD_MIN_ANM_SPEED,BLOOD_MAX_ANM_SPEED);
    effect->palette = PAL_BLOOD;
    effect->frame = random_int_range(0, ARRAYSIZE(ANM_BLOOD)-2);
    effect->vx = 10;
    effect->vy = 0;
    rotate_vec2(&effect->vx, &effect->vy, random_float_range(0,M_PI*2));
}
INTERNAL void update_blood (Effect* effect, float dt)
{
    // Slowly apply an upward force to the blood.
    effect->vy -= BLOOD_VELOCITY_INC;
    if (effect->vy < -BLOOD_MAX_VELOCITY)
    {
        effect->vy = -BLOOD_MAX_VELOCITY;
    }

    effect->x += effect->vx * dt;
    effect->y += effect->vy * dt;

    // Animate the bubble and if it is done animating then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        effect->t = random_float_range(BLOOD_MIN_ANM_SPEED,BLOOD_MAX_ANM_SPEED);
        effect->frame++;
        if (effect->frame >= ARRAYSIZE(ANM_BLOOD))
        {
            effect->alive = false;
        }
    }
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
                case (EFX_BUBBLE): create_bubble(effect); break;
                case (EFX_BLOOD ): create_blood (effect); break;
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
            // Specific render logic for the different effect types.
            const Clip* clip = NULL;;
            switch (effect->type)
            {
                case (EFX_BUBBLE): clip = ANM_BUBBLE[effect->frame]; break;
                case (EFX_BLOOD ): clip = ANM_BLOOD [effect->frame]; break;
            }

            render_bitmap(effect->x,effect->y, effect->palette, clip);
        }
    }
}
