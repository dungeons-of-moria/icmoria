/* debug.c */
/**/

#include "imoria.h"

#if DO_DEBUG
FILE *debug_file = NULL;
int   call_depth = 0;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void enter(char *routine_name,char *marker)
{
  if (debug_file == NULL) {
    debug_file = (FILE *)fopen("Debug.out","w");
  }
  
  call_depth++;

  fprintf(debug_file, ":::%4d: ENTER %s | %s |\n",
	  call_depth,routine_name,marker);

//  fprintf(debug_file,":  In    %ld %ld\n",panel_row_min,panel_row_max);

  fflush(debug_file);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void leave(char *routine_name,char *marker)
{
//  fprintf(debug_file,":  Out   %ld %ld\n",panel_row_min,panel_row_max);

  fprintf(debug_file, ":::%4d: LEAVE %s | %s |\n",
	  call_depth,routine_name,marker);

  fflush(debug_file);


  call_depth--;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void return_dbg(char *routine_name,char *marker,
		char typestr, char *descript, void *valptr)
{

//  fprintf(debug_file,":  Ret   %ld %ld\n",panel_row_min,panel_row_max);

  switch (typestr) {

  case 'b':
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = %s\n",
	    call_depth,routine_name,marker,descript,
	    (*((boolean *)valptr)) == 0 ? "false" : "true");
    break;

  case 'd':
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = %ld\n",
	    call_depth,routine_name,marker,descript,*((integer *)valptr));
    break;

  case 'u':
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = %lu\n",call_depth,
	    routine_name,marker,descript,*((unsigned long *)valptr));
    break;

  case 's':
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = %s\n",
	    call_depth,routine_name,marker,descript,(char *)valptr);
    break;

  case 'y':
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = %ld\n",
    call_depth,routine_name,marker,descript,(integer)(*((bytlint *)valptr)));

  default:
    fprintf(debug_file, ":::%4d: RETUR %s | %s | %s = (unknown type %c)\n",
	    call_depth,routine_name,marker,descript,typestr);
    break;
    
  }


  fflush(debug_file);

  call_depth--;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void log_msg(char *str)
{
  fprintf(debug_file, ">            %s\n", str);
  fflush(debug_file);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
/* END FILE debug.c */
