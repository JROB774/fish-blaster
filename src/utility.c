INTERNAL void rotate_vec2 (float* x, float* y, float rad)
{
    assert(x && y);

    float nx = (*x) * cos(rad) - (*y) * sin(rad);
    float ny = (*x) * sin(rad) + (*y) * cos(rad);

    *x = nx;
    *y = ny;
}

INTERNAL float lerp (float a, float b, float t)
{
    return a + t * (b - a);
}

INTERNAL float round_to_multiple (float num, float multiple)
{
    if (multiple == 0) return num;

    float rem = fmod(FABSF(num), multiple);
    if (rem == 0) return num;

    if (num < 0) return -(FABSF(num) - rem);
    else return num + multiple - rem;
}

INTERNAL void seed_random ()
{
    srand(CAST(unsigned int, time(NULL)));
}

INTERNAL int random_int ()
{
    return rand();
}
INTERNAL int random_int_range (int min, int max)
{
    return ((rand() % ((max+1)-min)) + min);
}

INTERNAL float random_float ()
{
    return (CAST(float,rand()) / CAST(float,RAND_MAX));
}
INTERNAL float random_float_range (float min, float max)
{
    return (min + CAST(float,rand()) / CAST(float,RAND_MAX/(max-min)));
}
