/* horse.h */
/* declarations for the horse racing module in the casino */

#define    RACES_PER_DAY    10
#define    MAX_HORSE_NAMES  55

typedef    integer    h_stat[MAX_HORSE_NAMES+1];
typedef    vtype      h_name[MAX_HORSE_NAMES+1];
typedef    boolean    h_bool[MAX_HORSE_NAMES+1];
typedef    real       statr[11];

extern void hr__game_horse();

/* END FILE horse.h */
