#define SCORES_FILE "scores.dat"

INTERNAL int score_compare (const void* a, const void* b)
{
    U32* aa = CAST(U32*,a);
    U32* bb = CAST(U32*,b);

    if (*aa > *bb) return -1;
    if (*aa < *bb) return  1;

    return 0;
}

INTERNAL void add_highscore (U32 score)
{
    // Check if the score is at least higher than the lowest current highscore.
    // If it is then its a new highscore and itll replace the bottom one, then
    // we sort the array so that all scores are in order.

    if (score > gScores[MAX_SCORES-1])
    {
        gScores[MAX_SCORES-1] = score;
        qsort(gScores,ARRAYSIZE(gScores),sizeof(gScores[0]),score_compare);
    }
}

INTERNAL void load_scores ()
{
    // If the file is NULL there was either a problem or no scores have been saved!
    // Either way it means we won't be loading any highscores into the gScores array.
    FILE* file = fopen(SCORES_FILE, "rb");
    if (file)
    {
        fread(gScores,sizeof(gScores),1,file);
        fclose(file);
    }
}

INTERNAL void save_scores ()
{
    FILE* file = fopen(SCORES_FILE, "wb");
    if (!file)
    {
        LOGWARNING("Faild to save highscores!");
    }
    else
    {
        fwrite(gScores,sizeof(gScores),1,file);
        fclose(file);
    }
}
