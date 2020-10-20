INTERNAL void rotate_vec2 (float* x, float* y, float rad)
{
    assert(x && y);

    float nx = (*x) * cos(rad) - (*y) * sin(rad);
    float ny = (*x) * sin(rad) + (*y) * cos(rad);

    *x = nx;
    *y = ny;
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
