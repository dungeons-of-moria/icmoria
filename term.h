/* term.h */

typedef unsigned char  int8u;

void moriaterm();
void clear_screen();
void save_screen();
void restore_screen();
void restore_term();
void bell();

/* Dungeon size parameters					*/
#define MAX_HEIGHT  66	    /* Multiple of 11; >= 22 */
#define MAX_WIDTH  198	  /* Multiple of 33; >= 66 */

#undef CTRL
#define CTRL(x)		(x & 0x1F)
#define DELETE		0x7f
#define ESCAPE	      '\033'	/* ESCAPE character -CJS-  */

/* message line location */
#define MSG_LINE  -1

#define CNIL (char *)0

/* number of messages to save in a buffer */
#define MAX_SAVE_MSG   22   /* How many messages to save -CJS- */

int  screen_change;
int  command_count;
int  eof_flag;
int  character_generated, character_saved;
int  sound_beep_flag;
int  panic_save;
FILE *highscore_fp;



/* end file term.h */
