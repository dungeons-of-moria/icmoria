/* mt.h  include file Mersenne Twister random number library */

/*
//
// uint32 must be an unsigned integer type capable of holding at least 32
// bits; exactly 32 should be fastest, but 64 is better on an Alpha with
// GCC at -O3 optimization so try your options and see what's best for you
//
*/
#ifndef uint32
typedef unsigned long uint32;
#endif

extern void   seedMT(uint32 seed);
/* you can seed with any uint32, but the best are odds in 0..(2^32 - 1) */

extern uint32 randomMT(void);
/* lots of lovely numbers come out of this */

extern void * saveMTstate(void *mtState);
/* if mtState is NULL then a block will be malloced and the ptr returned */

extern void * restoreMTstate(void *mtState);
/* will NOT free the space */

/* end file mt.h */
