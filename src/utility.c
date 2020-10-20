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
