/* death.c */
/**/

#include "imoria.h"
#include "master.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ud__get_username(char *name)
{
  user_name(name);
};
//////////////////////////////////////////////////////////////////////
static char* ud__fill_str(char *centered_str, char *in_str)
{
  /*{ Centers a string within a 31 character string		-JWT-	}*/

  register int i, j;

  i = strlen(in_str);
  j = 15 - i/2;
  (void) sprintf (centered_str, "%*s%s%*s", j, "", in_str, 31 - i - j, "");
  return centered_str;

};
//////////////////////////////////////////////////////////////////////
void dprint(vtype str, integer row)
{
/*{ Prints a line to the screen efficiently		-RAK-	}*/

  integer   i1,i2,nblanks,xpos,slen;
  vtype     prt_str;

  prt_str[0]  = 0;
  nblanks     = 0;
  xpos        = 0;

  slen = strlen(str);
  for (i1 = 0; i1 < slen; i1++) {
//printf("\tdo a char: %d >%c<\n",i1,str[i1]); fflush(stdout);
    if (str[i1] == ' ') {
      if (xpos > 0) {
	nblanks++;
	if (nblanks > 5) {
	  nblanks = 0;
	  put_buffer(prt_str,row,xpos);
	  prt_str[0] = 0;
	  xpos = 0;
	}
      }
    } else {
      if (xpos == 0) {
	xpos = i1;
      }
      if (nblanks > 0) {
	for (i2 = 0; i2 < nblanks; i2++) {
	  strcat(prt_str, " ");
	}
	nblanks = 0;
      }
      strncat(prt_str, &(str[i1]), 1);
    }
  } /* end for */

  if (xpos > 0) {
//    printf("doing final put_buffer >%s<\n",prt_str); fflush(stdout);
    put_buffer(prt_str,row,xpos);
  }
}
//////////////////////////////////////////////////////////////////////
void ud__kingly()
{
  /*{ Change the player into a King!			-RAK-	}*/
  /*	{ Change the character attributes...		}*/

  dun_level = 0;
  strcpy(died_from, "Ripe Old Age");

  if ( characters_sex() == MALE ) {
    strcpy(PM.title, "Magnificent");
    strcat(PM.tclass, " King");
  } else {
    strcpy(PM.title, "Beautiful");
    strcat(PM.tclass, " Queen");
  }

  PM.lev      +=  MAX_PLAYER_LEVEL;
  PM.account  +=  250000;
  PM.max_exp  +=  5000000;
  PM.exp       =  PM.max_exp;

  /*{ Let the player know that he did good...	}*/
  clear_from(1);
  dprint("                                  #",2);
  dprint("                                #####",3);
  dprint("                                  #",4);
  dprint("                            ,,,  $$$  ,,,",5);
  dprint("                        ,,=$   \"$$$$$\"   $=,,",6);
  dprint("                      ,$$        $$$        $$,",7);
  dprint("                      *>         <*>         <*",8);
  dprint("                      $$         $$$         $$",9);
  dprint("                      \"$$        $$$        $$\"",10);
  dprint("                       \"$$       $$$       $$\"",11);
  dprint("                        *#########*#########*",12);
  dprint("                        *#########*#########*",13);
  dprint("                          Veni, Vidi, Vici!",16);
  dprint("                     I came, I saw, I conquered!",17);
  dprint("                      All Hail the Mighty King!",18);
  flush();
  pause_game(24);
  
};
//////////////////////////////////////////////////////////////////////
void ud__print_tomb(vtype dstr[])
{
  /*{ Prints the gravestone of the character		-RAK-	}*/
  vtype    user, temp;
  FILE    *f1;
	
  if (py.misc.lev > 10) {
    ud__get_username(user);
    user[12] = 0;
    f1 = priv_fopen(MORIA_DTH,"r+");
    if (f1 != NULL) {
      fseek(f1,0,SEEK_END);
      if (py.misc.cheated) {
	fprintf(f1, "*%-12s %1d %2d %2d %2d %4ld %4d %s\n",
		user, py.misc.diffic, py.misc.prace,
		py.misc.pclass, py.misc.lev, dun_level,
		py.misc.max_lev, died_from);
	fprintf(f1,"%50s %s\n", "",show_current_time(temp));
      } else {
	fprintf(f1, " %-12s %1d %2d %2d %2d %4ld %4d %s\n",
		user, py.misc.diffic, py.misc.prace,
		py.misc.pclass, py.misc.lev, dun_level,
		py.misc.max_lev, died_from);
	fprintf(f1,"%50s %s\n", "",show_current_time(temp));
	//fprintf(f1," %44s %s\n", py.misc.ssn, show_current_time(temp));
      }
      fclose(f1);
    } /* endif f1 != NULL */
  } /* endif level > 10 */
  make_tomb(dstr);
  write_tomb(dstr);
};
//////////////////////////////////////////////////////////////////////
integer total_points()
{
  /*	{ Calculates the total number of points earned		-JWT-	}*/
  /*	{ The formula was changed to reflect the difficulty of low exp.  */
  /*      modifier classes like warriors			-Cap'n- }*/
  integer  return_value;

  if (PM.expfact == 0) {
    return_value = PM.max_exp + 100*PM.max_lev;
  } else {
    return_value = trunc(PM.max_exp / PM.expfact) + 100*PM.max_lev;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
void upon_death()
{
  /*{ Handles the gravestone and top-twenty routines	-RAK-	}*/
GDBM_FILE    f2;
master_key   mkey;

  vtype     dstr[20];

  /*{ What happens upon dying...				-RAK-	}*/

  if (!master_file_open(&f2)) {
    msg_print("ERROR opening file MASTER.  Contact your local wizard.");
    //msg_print("Status = "+itos(status(f1)));
    msg_print(" ");
  } else {
    mkey.creation_time = PM.creation_time;
    master_file_delete(f2, &mkey);
    master_file_close(&f2);
  }

  /*  I think this says "delete the file"...
      open(f1,file_name:=finam,record_length:=1024,history:=old,
      disposition:=delete,error:=continue);
      */
  unlink(finam);

  if (total_winner) {
    ud__kingly();
  }
  ud__print_tomb(dstr);
  print_dead_character();
  top_twenty(max_score);
  exit_game();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void print_dead_character()
{
  /*{ Allow the bozo to print out his dead character...	-KRC-	}*/
  char  command;

  if (get_com("Print character sheet to file? (Y/N)",&command)) {
    if (command == 'y' || command == 'Y') {
      file_character();
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void top_twenty(integer this_many)
{
  /*{ Enters a players name on the top twenty list		-JWT-	}*/

  string    list[MAX_HIGH_SCORES+2];
  integer   players_line = 0;
  integer   i1,i2,i3,i4;
  int       n1;
  vtype     o1,s1;
  FILE     *f1;
  boolean   flag;
  char      ch;

  if (py.misc.cheated) {
    exit_game();
  }
  clear_screen();

  if (!read_top_scores(&f1, MORIA_TOP, list, MAX_HIGH_SCORES, &n1, s1)) {
    prt(s1,2,1);
    prt("",3,1);
  } else {

    i3   = total_points();
    flag = false;
    
    if (i3 == 0) {
      i1 = n1;
    } else {
      for (i1=1; (i1 <= n1) && !flag ; ) {   /* XXXX check for corruption */
	sscanf(&(list[i1][13]),"%ld",&i4);
	if (i4 < i3) {
	  flag = true;
	} else {
	  i1++;
	}
      }
    }

    if ((i3 > 0) && ((flag) || (n1 == 0) || (n1 < MAX_HIGH_SCORES))) {

      for (i2 = MAX_HIGH_SCORES-1; i2 >= i1 ; i2--) {
	strcpy(list[i2+1], list[i2]);
      }
      
      user_name(o1);
      
      format_top_score(list[i1], o1, i3, PM.diffic, PM.name,
		       PM.lev, PM.race, PM.tclass);
      
      if (n1 < MAX_HIGH_SCORES) {
	n1++;
      }

      max_score    = n1;
      players_line = i1;
      flag         = false;

      write_top_scores(&f1, list, n1);

    } else { 
      /* did not get a high score */
      max_score = 20;
    }      
    
    if (!close_top_scores(&f1)) {
      prt("Error unlocking score file.",2,1);
      prt("",3,1);
    }
    
    put_buffer("Username     Points  Diff    Character name    Level  Race         Class",1,1);
    put_buffer("____________ ________ _ ________________________ __ __________ ________________",2,1);
    
    i2 = 3;
    if (max_score > n1) {
      max_score = n1;
    }
    
    if (this_many > 0) {
      if (this_many > MAX_HIGH_SCORES) {
	max_score = MAX_HIGH_SCORES;
      } else {
	max_score = this_many;
      }
    }
    for (i1 = 1; i1 <= max_score; i1++) {
      /*insert_str(list[i1],chr(7),''); XXXX  why? */
      if (i1 == players_line) {
	put_buffer_attr(list[i1],i2,1, A_REVERSE);
      } else {
	put_buffer(list[i1],i2,1);
      }
      if ((i1 != 1) && ((i1 % 20) == 0) && (i1 != max_score)) {
	prt("[Press any key to continue, or <Control>-Z to exit]",
	    24,1);
	ch = inkey();
	switch (ch) {
	case 3: case 25: case 26:
	  erase_line(24,1);
	  put_buffer(" ",23,13);
	  exit_game();
	  break;
	}
	clear_rc(3,1);
	i2 = 2;
      }
      i2++;
    } /* end for */
    
    erase_line(23,1);
    put_qio();
    
  } /* end if read_top_scores */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sc(char *out_str, char *in_str)
{
  strcpy(out_str, in_str);
}
//////////////////////////////////////////////////////////////////////
void si(char *out_str, char *part1, char *part2, char *part3)
{
  sprintf(out_str, "%s%s%s", part1, part2, part3);
}
//////////////////////////////////////////////////////////////////////
static void date(day)
char *day;
{
  register char *tmp;
  long clockvar;

  clockvar = time((long *) 0);
  tmp = (char *)ctime(&clockvar);
  tmp[10] = '\0';
  (void) strcpy(day, tmp);
}
//////////////////////////////////////////////////////////////////////
void make_tomb(vtype dd[])
{ 
  vtype    str1,str2,str3,str4,str5,str6,str7,str8;
  vtype    temp;
  integer  i1;
  char     day[11];

  date(day);
  ud__fill_str(str1, py.misc.name);
  ud__fill_str(str2, py.misc.title);
  ud__fill_str(str3, py.misc.tclass);
  sprintf(temp,"Level : %d",py.misc.lev);
  ud__fill_str(str4, temp);
  sprintf(temp,"%ld Exp",py.misc.exp);
  ud__fill_str(str5, temp);
  sprintf(temp,"%ld Au",(py.misc.account+py.misc.money[TOTAL_]));
  ud__fill_str(str6, temp);
  sprintf(temp,"Died on Level : %ld",dun_level);
  ud__fill_str(str7, temp);
  sprintf(temp,"%s.",died_from);
  ud__fill_str(str8, temp);

sc(dd[ 0]," ");
sc(dd[ 1],"               _______________________");
sc(dd[ 2],"              /                       \\         ___");
sc(dd[ 3],"             /                         \\ ___   /   \\      ___");
sc(dd[ 4],"            /            RIP            \\   \\  :   :     /   \\");
sc(dd[ 5],"           /                             \\  : _;,,,;_    :   :");
si(dd[ 6],"          /",str1,                       "\\,;_          _;,,,;_");
sc(dd[ 7],"         |               the               |   ___");
si(dd[ 8],"         | ",str2,                       " |  /   \\");
sc(dd[ 9],"         |                                 |  :   :");
si(dd[10],"         | ",str3,                       " | _;,,,;_   ____");
si(dd[11],"         | ",str4,                       " |          /    \\");
si(dd[12],"         | ",str5,                       " |          :    :");
si(dd[13],"         | ",str6,                       " |          :    :");
si(dd[14],"         | ",str7,                       " |         _;,,,,;_");
sc(dd[15],"         |            killed by            |");
si(dd[16],"         | ",str8,                       " |");
si(dd[17],"         |           ",day,   "            |");
sc(dd[18],"        *|   *     *     *    *   *     *  | *");
sc(dd[19]," _______)/\\\\_)_/___(\\/___(//_\\)/_\\//__\\\\(/_|_)_______");

  clear_from(1);
  for (i1 = 0; i1 < 20; i1++) {
    //printf(">%s<\n",dd[i1]);
    dprint(dd[i1],i1+1);
  }
  flush();


};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void write_tomb(vtype dstr[])
{ 
  vtype    out_str;
  vtype    fnam;
  char     command;
  FILE    *f1;
  integer  i1;
  boolean  flag;

  if (get_com("Print to file? (Y/N)",&command)) {
    if (command == 'y' || command == 'Y') {
      prt("Enter Filename:",1,1);
      flag = false;
      do {
	if (get_string(fnam,1,17,60)) {
	  if (strlen(fnam) == 0) {
	    strcpy(fnam, "MORIACHR.DIE");
	  }
	  f1 = (FILE *)fopen(fnam, "w");
	  if (f1 == NULL) {
	    sprintf(out_str, "Error creating> %s",fnam);
	    prt(out_str,2,1);
	  } else {
	    flag = true;
	    for (i1 = 0; i1 < 20; i1++) {
	      fprintf(f1,"%s\n",dstr[i1]);
	    }
	  }
	  fclose(f1);
	} else {
	  flag = true;
	}
      } while (!flag);
    } /* end if command == Y */
  } /* end if get command */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  death.c */
