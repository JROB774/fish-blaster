#ifndef SCORE_H
#define SCORE_H

#define MAX_SCORES 10
GLOBAL U32 gScores[MAX_SCORES];

INTERNAL void add_highscore (U32 score);

INTERNAL void  load_scores ();
INTERNAL void  save_scores ();
INTERNAL void reset_scores ();

#endif /* SCORE_H */
