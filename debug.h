/* debug.h */
/**/

#if DO_DEBUG

#define ENTER(rname,mark) enter((rname), (mark));
#define LEAVE(rname,mark) leave((rname), (mark));
#define RETURN(rname,mark,typestr,desc,valptr) return_dbg((rname),(mark),(typestr),(desc),(valptr));
#define MSG(str)          log_msg(str);

extern FILE *debug_file;
extern int   call_depth;

#else

#define ENTER(rname,mark) 
#define LEAVE(rname,mark) 
#define RETURN(rname,mark,typestr,desc,valptr) 
#define MSG(str)

#endif

extern void enter(char *routine_name,char *marker);
extern void leave(char *routine_name,char *marker);
extern void return_dbg(char *routine_name,char *marker,char typestr, char *descript, void *valptr);
extern void log_msg(char *str);


/* END FILE debug.h */
