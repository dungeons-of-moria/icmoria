/* slots.h */
/* declarations for the slot machine module in the casino */

#define xS_JACKPOT 1
#define xS_CHERRY  2
#define xS_ORANGE  3
#define xS_BELL    4
#define xS_BAR     5

#define S_JACKPOT 0
#define S_CHERRY  1 
#define S_ORANGE  2
#define S_BELL    3
#define S_BAR     4

typedef   integer   stype;   /* stype = (jackpot,cherry,orange,bell,bar);*/
typedef   stype     slot[4]; /* slot  = array[1..3] of stype;*/

extern void sm__game_slots();

/* END FILE slots.h */
