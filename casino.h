/* casino.h */
/* common declarations for the casino modules */

extern integer    bet;
extern integer    gld;
extern integer	  tics;
extern boolean    c_closed;

extern void c__display_gold();
extern boolean c__get_response(vtype comment, integer *num);
extern void c__change_money();
extern void c__check_casino_kickout();


/* END FILE casino.h */
