/* configure.h*/
/* include file for fun configurable stuff */

/* 
   My system has des_crypt.h, some do not.  Set to 0 to disable des encryption
   in favor of xor against random numbers.  It turns out that it was happier
   to xor everything against the random numbers even when DES was going to be
   used, so turning on des is probably major overkill but I do not mind.
*/
#define ENABLE_DES    0

/* 
   If USE_MTWIST is 0 then rand() will be used rather than the
   Mersenne Twister library.
*/
#define USE_MTWIST    1

/*
  Keep the path under 60 characters or it will overflow some variables.

  This is where hours.dat, moria.dat, monsters.dat, moria_gcustom.mst,
  moriamas.dat, moriatop.dat, moriatrd.dat, death.log
*/
#define DATA_FILE_PATH "/home/kertes/icmoria"

/*
  Keep this one under 80 characters, it points to the help program...
  You may have to edit mhelp.pl to set the path to perl.
*/
#define HELP_FILE_PATH DATA_FILE_PATH "/mhelp.pl"

/*
  
*/


/* END FILE  configure.h */