// EFX_BUBBLE

#define BUBBLE_MAX_VELOCITY  100
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
#define BLOOD_MAX_VELOCITY  8

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

// EFX_BONE

#define BONE_MIN_LIFETIME 0.8f
#define BONE_MAX_LIFETIME 1.5f
#define BONE_ANM_SPEED    0.3f
#define BONE_VELOCITY_INC 0.5f
#define BONE_MAX_VELOCITY 10

INTERNAL void create_bone (Effect* effect)
{
    effect->t = random_float_range(BONE_MIN_LIFETIME,BONE_MAX_LIFETIME);
    effect->palette = PAL_BONE;
    effect->frame = random_int_range(0, ARRAYSIZE(ANM_BONE)-1);
    effect->vx = 10;
    effect->vy = 0;
    rotate_vec2(&effect->vx, &effect->vy, random_float_range(0,M_PI*2));
}
INTERNAL void update_bone (Effect* effect, float dt)
{
    // Slowly apply an upward force to the blood.
    effect->vy -= BONE_VELOCITY_INC;
    if (effect->vy < -BONE_MAX_VELOCITY)
    {
        effect->vy = -BONE_MAX_VELOCITY;
    }

    effect->x += effect->vx * dt;
    effect->y += effect->vy * dt;

    // Once the bone's lifetime is up then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        effect->alive = false;
    }
}

// EFX_SHOT

#define SHOT_MIN_ANM_SPEED 0.05f
#define SHOT_MAX_ANM_SPEED 0.10f

INTERNAL void create_shot (Effect* effect)
{
    effect->t = random_float_range(SHOT_MIN_ANM_SPEED,SHOT_MAX_ANM_SPEED);
    effect->palette = PAL_SHOT;
    effect->frame = 0;
}
INTERNAL void update_shot (Effect* effect, float dt)
{
    // Animate the bubble and if it is done animating then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        effect->t = random_float_range(SHOT_MIN_ANM_SPEED,SHOT_MAX_ANM_SPEED);
        effect->frame++;
        if (effect->frame >= ARRAYSIZE(ANM_SHOT))
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
                case (EFX_BONE  ): create_bone  (effect); break;
                case (EFX_SHOT  ): create_shot  (effect); break;
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
                case (EFX_BONE  ): update_bone  (effect, dt); break;
                case (EFX_SHOT  ): update_shot  (effect, dt); break;
            }
        }
    }
}

INTERNAL int effect_z_compare (const void* a, const void* b)
{
    const Effect* ea = CAST(Effect*, a);
    const Effect* eb = CAST(Effect*, b);

    if (ea->type < eb->type) return -1;
    if (ea->type > eb->type) return  1;

    return 0;
}

INTERNAL void render_effect (float dt)
{
    // Sort the effect list so they render in the correct order. The order
    // is determiend by the order of the IDs in the EffectID enumeratrion.
    qsort(gEffect, EFFECT_MAX, sizeof(Effect), effect_z_compare);

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
                case (EFX_BONE  ): clip = ANM_BONE  [effect->frame]; break;
                case (EFX_SHOT  ): clip = ANM_SHOT  [effect->frame]; break;
            }

            render_bitmap(effect->x,effect->y, effect->palette, clip);
        }
    }
}
