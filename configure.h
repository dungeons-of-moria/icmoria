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

  WARNING:  Be sure to copy monsters.dat to this directory!  The game
            will attempt to create the other files the first time that
	    it is run.
	    
*/
#define DATA_FILE_PATH "/home/kertes/icmoria"

/*
  Keep this one under 80 characters, it points to the help program...
  You may have to edit mhelp.pl to set the path to perl.
*/
#define HELP_FILE_PATH DATA_FILE_PATH "/mhelp.pl"

/*
  The curses I have on my system defines attr_get() and attrset(attr)
  differently than the one that a lot of people using Red Hat have.
  
  I don't know what to use to get and set the attributes with the
  newer library so change this to 1 if you want to try the nifty
  attr changes I added.  Otherwise you can leave it set to 0 and
  always wonder what the game could be like...

  This is only used in put_buffer_attr (term.c).  If you get it working
  with a curses that it does not currently work with please let me know!
*/
#define USE_CURSES_ATTRS 1

/* END FILE  configure.h */
