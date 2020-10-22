// EFX_GIB_BLOOD

#define GIB_BLOOD_MIN_ANM_SPEED 0.05f
#define GIB_BLOOD_MAX_ANM_SPEED 0.10f

#define BLOOD_VELOCITY_INC   0.5f
#define BLOOD_START_VELOCITY 10
#define BLOOD_MAX_VELOCITY   8

INTERNAL void create_gib_blood (Effect* effect)
{
    float min = GIB_BLOOD_MIN_ANM_SPEED;
    float max = GIB_BLOOD_MAX_ANM_SPEED;

    effect->t = random_float_range(min,max);
    effect->palette = PAL_BLOOD_1;
    effect->frame = random_int_range(4, ARRAYSIZE(ANM_BLOOD)-2);
    effect->vx = 0;
    effect->vy = 0;
}
INTERNAL void update_gib_blood (Effect* effect, float dt)
{
    // Slowly apply an upward force to the blood.
    effect->vy -= BLOOD_VELOCITY_INC;
    if (effect->vy < -BLOOD_MAX_VELOCITY)
    {
        effect->vy = -BLOOD_MAX_VELOCITY;
    }

    effect->x += effect->vx * dt;
    effect->y += effect->vy * dt;

    // Animate the blood and if it is done animating then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        float min = GIB_BLOOD_MIN_ANM_SPEED;
        float max = GIB_BLOOD_MAX_ANM_SPEED;
        if (gApp.code_blood_enabled) max *= 3;

        effect->t = random_float_range(min,max);
        effect->frame++;
        if (effect->frame >= ARRAYSIZE(ANM_BLOOD))
        {
            effect->alive = false;
        }
    }
}

// EFX_BLOOD

#define BLOOD_MIN_ANM_SPEED 0.2f
#define BLOOD_MAX_ANM_SPEED 0.6f

INTERNAL void create_blood (Effect* effect)
{
    float min = BLOOD_MIN_ANM_SPEED;
    float max = BLOOD_MAX_ANM_SPEED;

    effect->t = random_float_range(min,max);
    effect->palette = PAL_BLOOD_0;
    effect->frame = random_int_range(0, ARRAYSIZE(ANM_BLOOD)-2);
    effect->vx = BLOOD_START_VELOCITY;
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

    // Animate the blood and if it is done animating then kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        float min = BLOOD_MIN_ANM_SPEED;
        float max = BLOOD_MAX_ANM_SPEED;
        if (gApp.code_blood_enabled) max *= 3;

        effect->t = random_float_range(min,max);
        effect->frame++;
        if (effect->frame >= ARRAYSIZE(ANM_BLOOD))
        {
            effect->alive = false;
        }
    }
}

// EFX_GIB

#define GIB_START_VELOCITY   90
#define GIB_VELOCITY_FALLOFF 0.10f
#define GIB_MIN_LIFETIME     0.2f
#define GIB_MAX_LIFETIME     0.3f

INTERNAL void create_gib (Effect* effect)
{
    float min = GIB_MIN_LIFETIME;
    float max = GIB_MAX_LIFETIME;
    if (gApp.code_blood_enabled) max *= 3;

    effect->t = random_float_range(min,max);
    effect->palette = PAL_BLOOD_1;
    effect->frame = random_int_range(0, ARRAYSIZE(ANM_BLOOD)-5);
    effect->vx = GIB_START_VELOCITY;
    effect->vy = 0;
    rotate_vec2(&effect->vx, &effect->vy, random_float_range(0,M_PI*2));
}
INTERNAL void update_gib (Effect* effect, float dt)
{
    // Decrease the speed of the gib.
    if (effect->vx != 0) effect->vx -= GIB_VELOCITY_FALLOFF;
    if (effect->vy != 0) effect->vy -= GIB_VELOCITY_FALLOFF;

    effect->x += effect->vx * dt;
    effect->y += effect->vy * dt;

    // Spawn blood effects randomly.
    if (random_int_range(0,100) < 60)
    {
        create_effect(EFX_GIB_BLOOD, effect->x+4,effect->y+4,1,1, 1,1);
    }

    // After the gib's counter has gone down kill it.
    effect->t -= dt;
    if (effect->t <= 0.0f)
    {
        effect->alive = false;
    }
}

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
    // Animate the shot and if it is done animating then kill it.
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

// EFX_SCORE

#define SCORE_VELOCITY 60
#define SCORE_MOVETIME 0.20f
#define SCORE_LIFETIME 0.40f

INTERNAL void create_score (Effect* effect)
{
    effect->palette = PAL_TEXT_SHADE;
    effect->t = SCORE_LIFETIME;
    effect->vx = 0.0f;
    effect->vy = -SCORE_VELOCITY;
}
INTERNAL void update_score (Effect* effect, float dt)
{
    effect->t -= dt;
    if (effect->t <= (SCORE_LIFETIME-SCORE_MOVETIME))
    {
        effect->vy = 0.0f;
        if (effect->t <= 0.0f)
        {
            effect->alive = false;
        }
    }
    if (effect->vy == 0.0f)
    {
        if ((effect->frame % 2) == 0)
        {
            effect->invis = !effect->invis;
        }
    }

    effect->y += effect->vy * dt;
    effect->frame++;
}

// EFFECTS

INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count)
{
    int count = random_int_range(min_count, max_count);

    // If the effect is any of the gore types and the BLOOD code has been
    // enabled then we want to multiply the number of  particles spawned.
    if (gApp.code_blood_enabled)
    {
        if (id == EFX_GIB_BLOOD || id == EFX_BLOOD || id == EFX_GIB)
        {
            count *= 3;
        }
    }

    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (!effect->alive)
        {
            effect->alive = true;
            effect->type = id;
            effect->invis = false;

            int fw = 0;
            int fh = 0;

            switch (effect->type)
            {
                case (EFX_BLOOD    ): fw =  8, fh = 8; break;
                case (EFX_GIB_BLOOD): fw =  8, fh = 8; break;
                case (EFX_GIB      ): fw =  8, fh = 8; break;
                case (EFX_BUBBLE   ): fw =  8, fh = 8; break;
                case (EFX_SHOT     ): fw =  8, fh = 8; break;
                case (EFX_SCORE10  ):
                case (EFX_SCORE20  ):
                case (EFX_SCORE40  ):
                case (EFX_SCORE80  ): fw = 16, fh = 8; break;
            }

            effect->x  = random_int_range(x,x+w) - (fw/2);
            effect->y  = random_int_range(y,y+h) - (fh/2);
            effect->vx = 0;
            effect->vy = 0;
            effect->t  = 0.0f;

            // Specific creation logic for the different effect types.
            switch (effect->type)
            {
                case (EFX_BLOOD    ): create_blood    (effect); break;
                case (EFX_GIB_BLOOD): create_gib_blood(effect); break;
                case (EFX_GIB      ): create_gib      (effect); break;
                case (EFX_BUBBLE   ): create_bubble   (effect); break;
                case (EFX_SHOT     ): create_shot     (effect); break;
                case (EFX_SCORE10  ):
                case (EFX_SCORE20  ):
                case (EFX_SCORE40  ):
                case (EFX_SCORE80  ): create_score    (effect); break;
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
                case (EFX_BLOOD    ): update_blood     (effect, dt); break;
                case (EFX_GIB_BLOOD): update_gib_blood (effect, dt); break;
                case (EFX_GIB      ): update_gib       (effect, dt); break;
                case (EFX_BUBBLE   ): update_bubble    (effect, dt); break;
                case (EFX_SHOT     ): update_shot      (effect, dt); break;
                case (EFX_SCORE10  ):
                case (EFX_SCORE20  ):
                case (EFX_SCORE40  ):
                case (EFX_SCORE80  ): update_score     (effect, dt); break;
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

INTERNAL void render_effect_lo (float dt)
{
    // Sort the effect list so they render in the correct order. The order
    // is determiend by the order of the IDs in the EffectID enumeration.
    qsort(gEffect, EFFECT_MAX, sizeof(Effect), effect_z_compare);

    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            // We only want to render certain effect types on the low pass.
            if (effect->type != EFX_SCORE10 &&
                effect->type != EFX_SCORE20 &&
                effect->type != EFX_SCORE40 &&
                effect->type != EFX_SCORE80)
            {
                // Specific render logic for the different effect types.
                const Clip* clip = NULL;;
                switch (effect->type)
                {
                    case (EFX_BLOOD    ): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_GIB_BLOOD): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_GIB      ): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_BUBBLE   ): clip = ANM_BUBBLE[effect->frame]; break;
                    case (EFX_SHOT     ): clip = ANM_SHOT  [effect->frame]; break;
                }

                if (!effect->invis)
                {
                    render_bitmap(effect->x,effect->y, effect->palette, clip);
                }
            }
        }
    }
}

INTERNAL void render_effect_hi (float dt)
{
    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            // We only want to render certain effect types on the high pass.
            if (effect->type == EFX_SCORE10 ||
                effect->type == EFX_SCORE20 ||
                effect->type == EFX_SCORE40 ||
                effect->type == EFX_SCORE80)
            {
                // Specific render logic for the different effect types.
                const Clip* clip = NULL;;
                switch (effect->type)
                {
                    case (EFX_SCORE10  ): clip = &SPR_SCORE10; break;
                    case (EFX_SCORE20  ): clip = &SPR_SCORE20; break;
                    case (EFX_SCORE40  ): clip = &SPR_SCORE40; break;
                    case (EFX_SCORE80  ): clip = &SPR_SCORE80; break;
                }

                if (!effect->invis)
                {
                    render_bitmap(effect->x,effect->y, effect->palette, clip);
                }
            }
        }
    }
}
