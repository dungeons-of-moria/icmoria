/* io.c */
/**/

#include "imoria.h"
#include <signal.h>

void convert_time(
		  unsigned long int_time,
		  quad_type *bin_time)
{
  printf("UNIMPLEMENTED convert_time\n\r");
}; 

void sleep_(unsigned long int_time) 
{
  /*{ Sleep for given time					-RAK-	}*/
  /*{ NOTE: Int_time is in seconds					}*/

  sleep((unsigned int)int_time);
};

void mini_sleep(unsigned long int_time)
{
  /*{ Sleep for short time					-DMF-	}*/

  usleep(int_time);

}; 

void init_priv_switch()
{
  /*  the hope is that imoria is sgid games or something that can write
      to the master and scores files                                    */
  games_gid = getegid();
};

void priv_switch(integer switch_val)
{
  //	{ Turns SYSPRV off if 0; on if 1;			-RAK-	}
  //	{ This is needed if image is installed with SYSPRV because	}
  //	{ user could write on system areas.  By turning the priv off	}
  //	{ system areas are secure					}

  if (switch_val) {
    setgid(games_gid);
  } else {
    setgid(getgid());
  }
  
};

void signalexit()
{
  priv_switch(0);
  msg_print("Sorry, caught a core-dump signal.");
  py.flags.dead = false;
  save_char(true);
  exit_game(0);
}

extern void d__quit();
void signalquit()
{
  priv_switch(0);
  signal(SIGINT,(void *)signalquit);
  switch (game_state) {

  case GS_GET_COMMAND:
    d__quit();
    break;

  case GS_IGNORE_CTRL_C:
    break;

  case GS_ALLOW_CTRL_C:
  case GS_LOADING:
    exit_game();
    break;
  }
}

void signalsave()
{
  priv_switch(0);
  py.flags.dead = false;
  save_char(true);
  py.flags.dead = true;
  exit_game();
}

void signalsuspend()
{
  if (game_state == GS_HELP) {
    signal(SIGTSTP,(void *)signalsuspend);
  } else {
    
    priv_switch(0);
    echo();
    nocbreak();
    
    switch (game_state) {
    case GS_GET_COMMAND:
      clear_screen();
      put_qio();
      break;
    default:
      save_screen();
      clear_screen();
      put_qio();
      break;
    }
    
#if DO_DEBUG
    fprintf(debug_file,": suspending...\n");
    fflush(debug_file);
#endif      
    
    kill(getpid(), SIGTSTP);
    signal(SIGTSTP,(void *)signalsuspend);
    
#if DO_DEBUG
    fprintf(debug_file,": ...resuming\n");
    fflush(debug_file);
#endif      
    
    cbreak();
    noecho();
    
    switch (game_state) {
    case GS_GET_COMMAND:
      clear_screen();
      draw_cave();
      break;
    default:
      clear_screen();
      put_qio();
      restore_screen();
      wrefresh(stdscr);
      break;
    }
  }
};

void no_controly()
{ 
  // { Turn off Control-Y					-RAK-	}
  /* ok, this is unix not vms, so it turns off ^C and ^Z */

  boolean  CATCH_SIGNALS = true;

#ifdef SIGINT
  signal(SIGINT,(void *)signalquit);
#endif
#ifdef SIGHUP
  signal(SIGHUP,(void *)signalsave);
#endif

  if (CATCH_SIGNALS) {
    signal(SIGTSTP,(void *)signalsuspend);
    /*signal(SIGTSTP,SIG_IGN);*/
    signal(SIGQUIT,(void *)signalexit);
    signal(SIGILL,(void *)signalexit);
    signal(SIGTRAP,(void *)signalexit);
    signal(SIGFPE,(void *)signalexit);
    signal(SIGSEGV,(void *)signalexit);
#ifdef SIGIOT
    signal(SIGIOT,(void *)signalexit);
#endif
#ifdef SIGABRT
    signal(SIGABRT,(void *)signalexit);
#endif
#ifdef SIGEMT
    signal(SIGEMT,(void *)signalexit);
#endif
#ifdef SIGBUS
    signal(SIGBUS,(void *)signalexit);
#endif
#ifdef SIGSYS
    signal(SIGSYS,(void *)signalexit);
#endif
    }
};

void controly()
{
  // { Turn on Control-Y					-RAK-	}
  /* ok, this is unix not vms, so it turns on ^C and ^Z */

};

void exit_game()
{
//	{ Immediate exit from program					}
  controly();	// { Turn control-Y back on	}
  put_qio();	// { Dump any remaining buffer	}

  /* clean up the terminal */
  echo();
  nocbreak();
  endwin();

  exit(0);      // { exit from game		}
};

void init_channel() 
{
  /* XXXX */
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void msg_record(vtype message, boolean save) 
{
  byteint  count;
  byteint  temp_ctr;
  char     ic;
  string   fixed_string;

  ENTER("msg_record","i");

  if (save) {
    record_ctr++;
    if (record_ctr > MAX_MESSAGES) {
      record_ctr = 1;
    }
    strcpy(msg_prev[record_ctr], message);
    if (strlen(msg_prev[record_ctr]) > 74) {
      msg_prev[record_ctr][74] = 0;
    }
  } else {
    /*{pre-declaration of variables}*/
    count = 0;
    temp_ctr = record_ctr;

    do {
      count++;
      /* XXXX pad, dec, what to do? */
      /*prt(pad(msg_prev[temp_ctr],' ',74) + ':' + dec(count,4,3),1,1);*/
      sprintf(fixed_string, "%02d> %s",count,msg_prev[temp_ctr]);
      //prt(msg_prev[temp_ctr],1,1);
      prt(fixed_string,1,1);
      temp_ctr--;
      if (temp_ctr < 1) {
	temp_ctr = MAX_MESSAGES;
      }
      ic = inkey();
    } while (! (  !(ic==13||ic==32||ic==86) || count==MAX_MESSAGES) );
    msg_print("End of buffer. ");
    /* XXXX another pad, what to do? */
    /*msg_print(pad('End of buffer. ',' ',80));*/
  }

  LEAVE("msg_record","i");
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void inkey_delay(char *getchar,integer delay) 
{
  /* XXXX check_input consumes the input, so we never actually get data */

  put_qio();			/*{ Dump the IO buffer		}*/

  *getchar = 0;

  if(check_input (delay)) {
    *getchar = 'a';
  }

};

void inkey_flush(char *x) 
{
      put_qio();	/*{ Dup the IO buffer	}*/
      if (!(wizard1)) {
	flush();
      }
      *x = inkey();
};

void get_message() 
{
};

void set_the_trap() 
{
};

void disable_the_trap() 
{
};

void clear_rc(integer row,integer col) 
{
  /*	{ Clears screen at given row, column				}*/
  integer i1;

  row--;  col--;

  for (i1=2; i1<=23; i1++) {
    used_line[i1] = false;
  }
  move(row, col);
  clrtobot();
//  put_buffer(cursor_erp, row, col);
  put_qio(); /* dump the clear sequence */
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean msg_print_pass_one(char *str_buff) // : varying[a] of char;
{
  boolean  return_value = false;
  integer  old_len = 0;
  char     ic = 0;

  ENTER("msg_print", "m");

  if ((msg_flag) && (!msg_terse)) {
    old_len = strlen(old_msg) + 1;
    put_buffer(" -more-",msg_line,old_len);
    do {
      ic = inkey();
    } while (ic!=10 && ic!=13 && ic!=25 && ic!=26 && ic!=27 && ic!=32);
    /* isn't this nicer: until (ord(in_char) in [3,13,25,26,27,32]) ? */
  }

  if (str_buff && str_buff[0]) {

    /* put_buffer(cursor_erl+str_buff,msg_line,msg_line);*/
    erase_line(msg_line,msg_line);
    put_buffer(str_buff,msg_line,msg_line);
    strncpy(old_msg, str_buff, sizeof(vtype));
    msg_record(str_buff,true);
    
    if (ic==3||ic==25||ic==26||ic==27) {
      return_value = true;
    } else {
      return_value = false;
    }
    
    msg_flag = true;
  } else {
    
    msg_flag = false;
    
  }
  
  RETURN("msg_print", "m", 'b',"msg", &return_value);
  return return_value;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean msg_print(char *str_buff) // : varying[a] of char;
{
  /*{ Outputs message to top line of screen				}*/

  integer   old_len;
  char      in_char   = 0;
  obj_set   big_set   = {3,10,13,25,26,27,32,0};
  obj_set   small_set = {3,25,26,27,0};
  boolean   flag;

  /*  if (str_buff && str_buff[0]) {*/

  if ((msg_flag) && (! msg_terse)) {
    old_len = strlen(old_msg) + 1;
    /*strcat(old_msg, " -more-");*/
    /*put_buffer(old_msg,msg_line,msg_line);*/
    put_buffer(" -more-",msg_line,old_len);
    do {
      in_char = inkey();
    } while (! (is_in(in_char, big_set)));
  }

  erase_line(msg_line,msg_line);
  put_buffer(str_buff,msg_line,msg_line);

  strcpy(old_msg, str_buff);
  msg_record(str_buff,true);

  msg_flag = true;

  /*
  } else {
    msg_flag = false;
  }
  */

  if (is_in(in_char,small_set)) {
    flag = true;
  } else {
    flag = false;
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean get_com(char *prompt, char *command)
{
  boolean  return_value;
  integer  com_val;

  if (strlen(prompt) > 1) {
    prt(prompt,1,1);
  }
  *command = inkey();
  com_val = (integer)*command;

  return_value = !(com_val==3 || com_val==25 || com_val==27);

  erase_line(msg_line,msg_line);
  msg_flag = false;

  return return_value;

}; /* end get_com */



//////////////////////////////////////////////////////////////////////
void print_str(char *str_buff,int row,int col)
{
  row -= panel_row_prt;/*{ Real co-ords convert to screen positions }*/
  col -= panel_col_prt;
  used_line[row] = true;
  put_buffer(str_buff,row,col);
};
//////////////////////////////////////////////////////////////////////
boolean get_yes_no(char *prompt) // : varying[a] of char;
{
  /*{ Gets response to a  Y/N question				}*/

  char      command;
  vtype     out_str;
  boolean   return_value = false;

  msg_print(" ");
  
  sprintf(out_str, "%s (Y/N) ", prompt);

  get_com(out_str,&command);
  switch (command) {
  case 'y': case 'Y' : return_value = true; break;
  }

  return return_value;
};

//////////////////////////////////////////////////////////////////////

integer get_hex_value(integer row,integer col,integer slen)
{
  integer   return_value = 0;
  vtype     tmp_str;

  if (get_string(tmp_str,row,col,slen)) {
    if (strlen(tmp_str) <= 8) {
      
      sscanf(tmp_str, "%lx", &return_value);
      
    }
  }

  return return_value;
};

void print_hex_value(integer num,integer row,integer col) 
{
  vtype    out_val;
  sprintf(out_val, "0x%08lx", num);
  prt(out_val, row, col);
};

void pause_game(integer prt_line) 
{
  pause_line(prt_line);
};

void get_paths() 
{
  //	{ Returns the image path for Moria			-RAK-	}
  //	{ Path is returned in a VARYING[80] of char			}

  char *datapath = DATA_FILE_PATH;

  // fill in the MORIA_ names;

  sprintf(MORIA_HOU,  "%s/hours.dat",         datapath);
  sprintf(MORIA_LCK,  "%s/morialock.lock",    datapath);
  sprintf(MORIA_MON,  "%s/monsters.dat",      datapath);

  sprintf(MORIA_MOR,  "%s/moria.dat",         datapath);
  sprintf(MORIA_DTH,  "%s/death.log",         datapath);
  sprintf(MORIA_MAS,  "%s/moriamas.dat",      datapath);
  sprintf(MORIA_GCST, "%s/moria_gcustom.mst", datapath);
  sprintf(MORIA_TOP,  "%s/moriatop.dat",      datapath);
  sprintf(MORIA_TRD,  "%s/moriatrd.dat",      datapath);

  /*  sprintf(MORIA_HLP,  "%s/moriahlp.hlb",      HELP_FILE_PATH); */

  sprintf(MORIA_CST,  "moria_custom.mst");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  io.c */
