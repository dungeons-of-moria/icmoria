/* imoria.c */
/* include flies for all the modlues */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <math.h>
#include <time.h>
#include <unistd.h>         /* for ftruncate, usleep */

/*#include <sgtty.h>*/
#include "mtwist/mtwist.h"  /* random number generation */
#include "patchlevel.h"
#include "configure.h"
#include "constants.h"
#include "types.h"
#include "pascal.h"
#include "routines.h"
#include "term.h"

#include "debug.h"

// moria.c includes the actual declarations of all the variables,
// everything else just includes the extern stuff

#ifdef __MORIA_C__
#include "values.h"
#else
#include "variables.h"
#endif

#define PSPELL(xx) (magic_spell[py.misc.pclass][(xx)])
#define PCLASS     (class[py.misc.pclass])
#define PM         (py.misc)
#define PS         (py.stat)
#define PF         (py.flags)

#define print_null(r,c) (move_cursor((r)-panel_row_prt-1,(c)-panel_col_prt-1))

/* END FILE  imoria.h */
