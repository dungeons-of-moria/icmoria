/* pascal.h */
/**/

/* Things to save typing when converting pascal to c */

#define nil     0
#define false   0
#define true    1

#define div     /
#define trunc(xx)    (xx)

#define uor(xx,yy)   ((xx) | (yy))
#define uand(xx,yy)  ((xx) & (yy))
#define uxor(xx,yy)  ((xx) ^ (yy))

/* pascal routines */

extern void writeln(char *out_line);
//extern integer randint(integer maxval);
extern integer pindex(char *s1, char c1);
extern boolean is_in(integer obj, obj_set oset);
extern boolean is_vowel(char a_char);

/* END FILE  pascal.h */
