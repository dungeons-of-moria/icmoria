/* create.c */
/* Ever want to create the perfect character? Here is where you can cheat. */

#include "imoria.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* create_character code */

byteint cc__old_stat(integer new_guy)
{
  byteint return_value;

  if (new_guy<150) {
    return_value = (squish_stat(new_guy) + 30) div 10;
  } else {
    return_value = squish_stat(new_guy) - 132;
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
byteint cc__new_stat(integer old_guy)
{
  byteint return_value;

  if (old_guy<18) {
    return_value = squish_stat(old_guy*10-30);
  } else {
    return_value = squish_stat(old_guy+132);
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
integer cc__get_min_stat (string prompt, byteint max)
{
  integer   abil = 0;
  integer   perc;
  string    tmp_str;
  string    out_str;

  if (max == 250) {
    sprintf(out_str,"Min %s (racial max 18/00) : ",prompt);
  } else if (max > 150) {
    sprintf(out_str,"Min %s (racial max 18/%d) : ",prompt,max-150);
  }  else {
    sprintf(out_str,"Min %s (racial max %d) : ",prompt,cc__old_stat(max));
  }

  prt(out_str,1,1);

  get_string(tmp_str, 1, strlen(out_str)+1, 10);
  if (pindex(tmp_str,'/') > 0) {
    /* player is asking for an 18/xx, 17/00 is left a 0 here, bummer */

    *strchr(tmp_str,'/') = ' ';
    sscanf(tmp_str,"%ld %ld",&abil,&perc);
    if (abil == 18) {
      if (perc == 0) {
	abil = 250;
      } else {
	abil = 150 + perc;
      }
    }
  } else {
    /* player is asking for something less than 18/xx */

    sscanf(tmp_str,"%ld",&abil);
    if (abil < 3) {
      abil = 3;
    } else if (abil > 18) {
      abil = 18;
    }
    
    abil = cc__new_stat(abil);
  }

  return abil;
};
//////////////////////////////////////////////////////////////////////
void cc__get_minimums(stat_s_type user, boolean *minning, stat_s_type max_r)
{
  /*	{ Get minimum stats the character wants			-DMF-	} */
  string    yes_no;

  prt("Do you wish to try for minimum statistics? ",1,1);
  if (get_string(yes_no,1,44,1)) {
    if ((yes_no[0] == 'y') || (yes_no[0] == 'Y')) {
      *minning   = true;
      user[STR] = cc__get_min_stat("STR",max_r[STR]);
      user[INT] = cc__get_min_stat("INT",max_r[INT]);
      user[WIS] = cc__get_min_stat("WIS",max_r[WIS]);
      user[DEX] = cc__get_min_stat("DEX",max_r[DEX]);
      user[CON] = cc__get_min_stat("CON",max_r[CON]);
      user[CHR] = cc__get_min_stat("CHR",max_r[CHR]);
      prt("Printing Stats...",1,1);
      prt_6_stats(user,NULL,3,65);
    }
  }
  erase_line(1,1);
//	printf("\n\nexit get_mins\n\n"); fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
integer cc__get_stat()
{
  /*	{ Generates character's stats				-JWT-	} */
  integer   i;

  i = randint(4) + randint(4) + randint(4) + 5; /* 8..17 */
  return (i-3)*10;                              /* 50..140 */
};
//////////////////////////////////////////////////////////////////////
integer cc__change_stat(integer cur_stat, integer amount)
{
  /*	{ Changes stats by given amount				-JWT-	}*/
  integer   i;

  if (amount < 0) {
    for (i = -1; i >= amount; i--) {
      cur_stat -= squish_stat(de_statp(cur_stat));
    }
  } else {
    for (i = 1; i <= amount; i++) {
      cur_stat += squish_stat(in_statp(cur_stat));
    }
  }

  return cur_stat;
};
//////////////////////////////////////////////////////////////////////
byteint cc__max_in_statp(byteint stat)
{
  if (stat < 150) {
    stat += 10;
  } else if (stat < 220) {
    stat += 25;
  } else if (stat < 240) {
    stat += 10;
  } else if (stat < 250) {
    stat += 1;
  }
  
  return stat;
};

//////////////////////////////////////////////////////////////////////
byteint cc__max_de_statp(byteint stat)
{
  if (stat < 11) {
    stat = 0;
  } else if (stat < 151) {
    stat -= 10;
  } else if (stat < 241) {
    stat -= 6;
    if (stat < 150) stat = 150;
  } else {
    stat -= 1;
  }

   return stat;
};

//////////////////////////////////////////////////////////////////////
integer cc__max_stat(integer cur_stat, integer amount)
{
  integer   i;

  if (amount < 0) {
    for (i = -1; i >= amount; i--) {
      cur_stat = cc__max_de_statp(cur_stat);
    }
  } else {
    for (i = 1; i <= amount; i++) {
      cur_stat = cc__max_in_statp(cur_stat);
    }
  }

  return cur_stat;
};

//////////////////////////////////////////////////////////////////////
boolean cc__choose_race()
{
/*	{ Allows player to select a race			-JWT-	}*/

  integer   i2, i3, i4, i5;
  char      s;
  boolean   exit_flag;
  vtype     out_val;
  boolean   return_value = false;

//	printf("enter choose_race \n"); fflush(stdout);

  i2 = 0;
  i3 = 1;
  i4 = 3;
  i5 = 22;
  clear_from(21);
  prt("Choose a race (? for Help):",21,3);
  do {
    sprintf(out_val, " %c) %s", (int)i3+96, race[i2].trace);
    put_buffer (out_val,i5,i4);
    i3++;
    i4 += 15;
    if (i4 > 70) {
      i4 = 3;
      i5++;
    }
    i2++;
  } while (i2 < MAX_RACES);

//  printf("    choose_race done with do\n"); fflush(stdout);

  py.misc.race[0] = 0;
  put_buffer("",21,30);
  exit_flag = false;

  do {
    inkey_flush(&s);
    i2 = pindex("abcdefghijklmnopqrstuvwxyz",s);
    if ((i2 <= MAX_RACES) && (i2 >= 1)) {
      py.misc.prace  = i2-1;
      strcpy(py.misc.race, race[i2-1].trace);
      exit_flag = true;
      return_value = true;
      put_buffer(py.misc.race,4,15);
    } else if (s == '?') {
      moria_help("Character Races");
      exit_flag = true;
      return_value = false;
    }
  } while (!exit_flag);

//	printf("exit choose_race \n"); fflush(stdout);

  return return_value;
}
//////////////////////////////////////////////////////////////////////
void  cc__print_try_count(int try_count)
{
  string  out_str;
  sprintf(out_str,"Try = %10d",try_count);
  put_buffer(out_str,21,60);
  put_qio();
};
//////////////////////////////////////////////////////////////////////
integer cc__next_best_stats(stat_s_type this, stat_s_type user,
			    stat_s_type best, integer best_min)
{
  integer   below_sum = 0;
  integer   below;
  int       tstat;

  for (tstat = STR; tstat <= CHR; tstat++) {
    if (this[tstat] < user[tstat]) {
	  below     = user[tstat] - this[tstat];
	  below_sum = below_sum + ((below*(below+1)) div 2);
    }
  }
  
  if (below_sum < best_min) {
    for (tstat = STR; tstat <= CHR; tstat++) {
      best[tstat] = this[tstat];
    }
    return below_sum;
  } else {
    return best_min;
  }

}
//////////////////////////////////////////////////////////////////////

boolean cc__satisfied(boolean *minning, boolean *printed_once, 
		      integer *best_min, integer *try_count,
		      stat_s_type best, stat_s_type user)
{
  /*	{ What does it take to satisfy the guy?!		-KRC-	}*/

  char      s;
  stat_set  tstat;
  boolean   return_value = false;

  if (!*minning) {
    /* figure out what the current bonuses are so the player has a clue */
    py.misc.dis_th  = tohit_adj();
    py.misc.dis_td  = todam_adj();
    py.misc.dis_tac = toac_adj();

    if (!*printed_once) {
      clear_from(21);
      put_misc1();
      put_stats();

      prt( "Press <LF> to reroll, any other key to continue:", 21, 3 );
      *printed_once = true;
    } else {
      upd_misc1();
      upd_stats();
      prt("", 21, 51 );
    } /* endif printed_once */

    inkey_flush(&s);
    return_value = (s != 10);

  } else { /* minning */

    if (!*printed_once) {
      clear_from(21);
      prt("Press any key to give up (50000 rolls max): ",21,3);
      put_qio();
      *printed_once = true;
    }

    *best_min = cc__next_best_stats(py.stat.p,user,best,*best_min);
    (*try_count)++;
    if ((*try_count % 250) == 0) {
      cc__print_try_count(*try_count);
    }

    inkey_delay(&s,0);
    if ((s != 0) || (*try_count == 50000)) {
      *minning = false;
      *printed_once = false;
      for (tstat = STR; tstat <= CHR; tstat++) {
	py.stat.p[(int)tstat] = best[(int)tstat];
	py.stat.c[(int)tstat] = best[(int)tstat];
      }
      return_value = cc__satisfied(minning, printed_once, best_min,
				   try_count, best, user);
    } else {
      return_value = (*best_min == 0);
      if (*best_min == 0) {
	put_misc1();
	put_stats();
      }
    } /* endif s || try_count */
  } /* endif minning */

  return return_value;

}; /* end cc__satisfied */

//////////////////////////////////////////////////////////////////////
void cc__get_stats()
{
/*	{ Get the statistics for this bozo			-KRC-	}*/

  stat_set  tstat;
  integer   prace;

  prace = py.misc.prace;

  for (tstat = STR; tstat <= CHR; tstat++) {
    py.stat.p[(int)tstat] = cc__change_stat(cc__get_stat(),
				       race[prace].adj[(int)tstat]);
    py.stat.c[(int)tstat] = py.stat.p[(int)tstat];
  }

  py.misc.rep        = 0;
  py.misc.srh        = race[prace].srh;
  py.misc.bth        = race[prace].bth;
  py.misc.bthb       = race[prace].bthb;
  py.misc.fos        = race[prace].fos;
  py.misc.stl        = race[prace].stl;
  py.misc.save       = race[prace].bsav;
  py.misc.hitdie     = race[prace].bhitdie;
  py.misc.lev        = 1;
  py.misc.ptodam     = todam_adj();
  py.misc.ptohit     = tohit_adj();
  py.misc.ptoac      = 0;
  py.misc.pac        = toac_adj();
  py.misc.expfact    = race[prace].b_exp;
  py.flags.see_infra = race[prace].infra;
  py.flags.swim	     = race[prace].swim;
};
//////////////////////////////////////////////////////////////////////
void cc__print_history()
{
/*	{ Will print the history of a character			-JWT-	}*/

  integer  i1;

  clear_from(14);
  put_buffer("Character Background",14,28);
  for (i1 = 0; i1 < 5; i1++) {
    put_buffer(py.misc.history[i1],i1+1+14,5);
  }
};

//////////////////////////////////////////////////////////////////////
/*
	{ Get the racial history, determines social class	-RAK-	}
	{ Assumtions:	Each race has init history beginning at 	}
	{		(race)*3 + 1					}
	{		All history parts are in ascending order	}
*/
void cc__get_history()
{

  integer    hist_ptr,cur_ptr,test_roll;
  integer    start_pos,end_pos,cur_len;
  integer    line_ctr,new_start=0,social_class;
  char       history_block[400];
  boolean    flag;

  /*	{ Get a block of history text				}*/
  hist_ptr = (py.misc.prace)*3 + 1;
  history_block[0] = 0;
  social_class = randint(4);
  cur_ptr = -1;
  
  do {
    flag = false;
    do {
      /* find the chart to use */
      cur_ptr++;
      if (background[cur_ptr].chart == hist_ptr) {
	/* found it, now pick an entry */
	test_roll = randint(100);
	while (test_roll > background[cur_ptr].roll) {
	  cur_ptr++;
	}

	strcat(history_block, background[cur_ptr].info);
	social_class += background[cur_ptr].bonus;

	if (hist_ptr > background[cur_ptr].next) { 
	  /* reset cur_ptr since next chart has alerady been passed */
	  hist_ptr = background[cur_ptr].next;
	  cur_ptr = -1;
	} else {
	  hist_ptr = background[cur_ptr].next;
	}

	flag = true;
      } /* endif chart == hist_ptr */
    } while (!flag);
  } while (hist_ptr >= 1);

  /* -------------------------------------------------- */

  /*	{ Process block of history text for pretty output	} */
  
  start_pos = 0;
  end_pos   = strlen(history_block)-1;
  line_ctr  = 0;
  flag      = false;

  while (history_block[end_pos] == ' ') {
    end_pos--;
  }
  do {
    while (history_block[start_pos] == ' ') {
      start_pos++;
    }
    cur_len = end_pos - start_pos + 1;
    if (cur_len > 70) {
      cur_len = 70;
      while (history_block[start_pos+cur_len-1] != ' ') {
	cur_len--;
      }
      new_start = start_pos + cur_len;
      while (history_block[start_pos+cur_len-1] == ' ') {
	cur_len--;
      }
    } else {
      flag = true;
    }
    strncpy(py.misc.history[line_ctr], &(history_block[start_pos]),cur_len);
    py.misc.history[line_ctr][cur_len] = 0;
    line_ctr++;
    start_pos = new_start;
  } while (!flag);
  
  for ( ; line_ctr < 5; line_ctr++) {
    py.misc.history[line_ctr][0] = 0;
  }

  /*	{ Compute social class for player			}*/
  if (social_class > 100) {
    social_class = 100;
  } else if(social_class < 1) {
    social_class = 1;
  }

  py.misc.rep = 50 - social_class;
  py.misc.sc  = social_class;

} /* end cc__get_history */

//////////////////////////////////////////////////////////////////////
boolean cc__get_sex()
{
/*	{ Gets the character's sex				-JWT-	}*/

  char      s;
  boolean   exit_flag = false;
  boolean   return_value = false;

  if (py.misc.prace == R_DRYAD) {
    strcpy(py.misc.sex,"Female");
    return_value = true;
    exit_flag = true;
    prt(py.misc.sex,5,15);
  } else {
    py.misc.sex[0] = 0;
    clear_from(21);
    prt("Choose a sex (? for Help):",21,3);
    prt("m) Male       f) Female",22,3);
    prt("",21,29);
    do {
      inkey_flush(&s);
      if (s == 'f') {
	strcpy(py.misc.sex,"Female");
	prt(py.misc.sex,5,15);
	exit_flag = true;
	return_value = true;
      } else if (s == 'm') {
	strcpy(py.misc.sex,"Male");
	prt(py.misc.sex,5,15);
	exit_flag = true;
	return_value = true;
      } else if (s == '?') {
	moria_help("Character Sex");
	exit_flag = true;
	return_value = false;
      }
    } while (!exit_flag);
  } /* endif prace */
  
  return return_value;
} /* end cc__get_sex */

//////////////////////////////////////////////////////////////////////
void cc__get_ahw()
{
  /*	{ Computes character's age, height, and weight		-JWT-	}*/

  integer   i1;

  i1 = py.misc.prace;
  py.misc.age = race[i1].b_age + randint(race[i1].m_age);

  py.misc.birth.year  = 500 + randint(50);
  py.misc.birth.month = randint(13);
  py.misc.birth.day   = randint(28);
  py.misc.birth.hour  = randint(24)-1;
  py.misc.birth.secs  = randint(400)-1;

  py.misc.cur_age.year   = py.misc.age + py.misc.birth.year;
  py.misc.cur_age.month  = py.misc.birth.month;
  py.misc.cur_age.day    = py.misc.birth.day + 1;
  if ((py.misc.cur_age.day % 7) == 0) {
    add_days(&py.misc.cur_age,2);
  }
  if ((py.misc.cur_age.day % 7) == 1) {
    add_days(&py.misc.cur_age,1);
  }
  py.misc.cur_age.hour   = 7;
  py.misc.cur_age.secs   = 300 + randint(99);

  if (characters_sex() == FEMALE) {
    py.misc.ht = randnor(race[i1].f_b_ht,race[i1].f_m_ht);
    py.misc.wt = randnor(race[i1].f_b_wt,race[i1].f_m_wt);
  } else {
    py.misc.ht = randnor(race[i1].m_b_ht,race[i1].m_m_ht);
    py.misc.wt = randnor(race[i1].m_b_wt,race[i1].m_m_wt);
  }

  py.misc.disarm = race[i1].b_dis + todis_adj();

} /* end cc__get_ahw */
//////////////////////////////////////////////////////////////////////
/*	{ Gets a character class				-JWT-	}*/
boolean cc__get_class()
{
  integer    i1, i2, i3, i4, i5;
  integer    cl[MAX_CLASS+1];
  integer    aclass;
  char       s;
  boolean    exit_flag;
  string     out_str;
  stat_set   tstat;
  boolean    return_value = false;
  
  for (i2 = 0; i2 < MAX_CLASS; i2++) {
    cl[i2] = 0;
  }
  
  i1 = py.misc.prace;
  i2 = 0;
  i3 = 0;
  i4 = 3;
  i5 = 22;
  clear_from(21);
  prt("Choose a class (? for Help):",21,3);
  
  do {
    if (race[i1].tclass & bit_array[i2+1]) {
      i3++;
      sprintf(out_str, "%c) %s", (int)i3+96, class[i2].title);
      put_buffer (out_str,i5,i4);
      cl[i3] = i2;
      i4 += 15;
      if (i4 > 70) {
	i4 = 3;
	i5++;
      }
    } // endif class bit set
    i2++;
  } while (i2 < MAX_CLASS);

  py.misc.pclass = 0;
  put_buffer("",21,31);
  exit_flag = false;

  do {
    inkey_flush(&s);
    i2 = pindex("abcdefghijklmnopqrstuvwxyz",s);
    if ((i2 <= i3) && (i2 >= 1)) {
      strcpy(py.misc.tclass, class[cl[i2]].title);
      py.misc.pclass = cl[i2];
      aclass = py.misc.pclass;
      exit_flag = true;
      return_value = true;
      clear_from(21);
      put_buffer(py.misc.tclass,6,15);
      py.misc.hitdie  += class[aclass].adj_hd;
      py.misc.mhp      = con_adj() + py.misc.hitdie;
      py.misc.chp      = py.misc.mhp;
      py.misc.bth     += class[aclass].mbth * 5 + 20;
      py.misc.bthb    += class[aclass].mbthb * 5 + 20; 
      py.misc.srh     += class[aclass].msrh;
      py.misc.disarm  += class[aclass].mdis;
      py.misc.fos     += class[aclass].mfos;
      py.misc.stl     += class[aclass].mstl;
      py.misc.save    += class[aclass].msav;
      py.misc.expfact += class[aclass].m_exp;
      strcpy(py.misc.title, player_title[aclass][0]);
      if (aclass == C_WARRIOR) {
	py.misc.mr = -10;
      } else if ((aclass == C_MAGE) || (aclass == C_PRIEST)) {
	py.misc.mr = 0;
      } else {
	py.misc.mr = -5;
      }

      /* { Adjust the stats for the class adjustment		-RAK-	}*/

      for (tstat = STR; tstat <= CHR; tstat++) {
	py.stat.p[(int)tstat] = cc__change_stat(py.stat.p[(int)tstat],
				class[py.misc.pclass].madj[(int)tstat]);
	py.stat.c[(int)tstat] = py.stat.p[(int)tstat];
      }

      py.misc.ptodam  = todam_adj();	 /*{ Real values	}*/
      py.misc.ptohit  = tohit_adj();
      py.misc.ptoac   = toac_adj();
      py.misc.pac     = 0;
      py.misc.dis_td  = py.misc.ptodam;  /*{ Displayed values	}*/
      py.misc.dis_th  = py.misc.ptohit;
      py.misc.dis_tac = py.misc.ptoac;
      py.misc.dis_ac  = py.misc.pac;

    } else if (s == '?') {
      moria_help("Character Classes");
      exit_flag = true;
      return_value = false;
    }
  } while (!exit_flag);

  return return_value;
} /* end cc__get_class */


//////////////////////////////////////////////////////////////////////

void cc__get_money()
{

  integer   tmp,i1;
  stat_set  tstat;

  tmp = 0;
  for (tstat = STR; tstat <= CHR; tstat++) {
    tmp += cc__old_stat(py.stat.c[(int)tstat]);
  }

  i1  = py.misc.sc*6 + randint(25) + 325;  //{ Social Class adj	}
  i1 -= tmp;			           //{ Stat adj		}
  i1 += cc__old_stat(py.stat.c[CHR]);	   //{ Charisma adj	}
  if (i1 < 80) { i1 = 80;}		   //{ Minimum		}
  i1 = (i1 * GOLD_VALUE) + randint(GOLD_VALUE);
  add_money(i1);
}

//////////////////////////////////////////////////////////////////////
/*
	{ Get social security number				-KRC-	}
[global,psect(create$code)] procedure get_ssn;

    [external] procedure lib$date_time	(
			%DESCR time : vtype
					); external;

    var

	account				: packed array [1..8] of char;
	time				: vtype;

    begin

      lib$date_time( time );
      get_account( account );
      py.misc.ssn := '$ < ' + account + ' > - # ' + time + ' # ' + py.misc.name;

    end;
*/
//////////////////////////////////////////////////////////////////////

void create_character()
{
  stat_s_type  best, user, max_r;
  boolean      minning = false;
  boolean      printed_once = false;
  integer      try_count = 0;
  integer      best_min = 99999999;
  stat_set     tstat;

  /*
    { This delay may be reduced, but is recomended to keep players	}
    { from continuously rolling up characters, which can be VERY	}
    { expensive CPU wise.						}
  */

  for (tstat = STR; tstat <= CHR; tstat++) {
    best[(int)tstat] = 3;
  }

  do {
    put_character();
  } while (!cc__choose_race());
	
  while (!cc__get_sex()) {
    put_character();
  }
	
  printed_once = false;
  for (tstat = STR; tstat <= CHR; tstat++) {
    max_r[(int)tstat] = cc__max_stat(140,race[py.misc.prace].adj[(int)tstat]);
  }

  cc__get_minimums(user, &minning, max_r);
  do {
    cc__get_stats();
    cc__get_history();
    cc__get_ahw();
  } while (!cc__satisfied(&minning, &printed_once, &best_min,
			  &try_count, best, user));

  cc__print_history();
  while (!cc__get_class()) {
    put_character();
    cc__print_history();
    put_misc1();
    put_stats();
  }

  py.misc.creation_time = time(NULL);  
  py.misc.save_count    = 0;  
  py.misc.claim_check   = 0;
  py.flags.light_on     = false;

  cc__get_money();
  put_stats();
  put_misc2();
  put_misc3();
  get_name();
  //	get_ssn();
  pause_exit(24,PLAYER_EXIT_PAUSE);

}; 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void put_character()
{
 /*{ Prints the following information on the screen.	-JWT-	}*/


  clear_from(1);
  prt2("Name      : ", py.misc.name,3,3);
  prt2("Race      : ", py.misc.race,4,3);
  prt2("Sex       : ", py.misc.sex,5,3);
  prt2("Class     : ", py.misc.tclass,6,3);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void put_stats()
{

/*	{ Prints the following information on the screen.	-JWT-	}*/

	  prt_6_stats(py.stat.c,NULL,3,65);
	  prt_num("+ To Hit   : ",py.misc.dis_th,10,4);
	  prt_num("+ To Damage: ",py.misc.dis_td,11,4);
	  prt_num("+ To AC    : ",py.misc.dis_tac,12,4);
	  prt_num("  Total AC : ",py.misc.dis_ac,13,4);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void upd_stats()
{

/*	{ Updates the following information on the screen. (wow)-KRC-	}*/
    stat_set  tstat;

	  for (tstat = STR; tstat <= CHR; tstat++) {
	      prt_stat( "", py.stat.c[(int)tstat], 3+tstat, 71 );
	}
	  prt_num( "", py.misc.dis_th, 10, 17 );
	  prt_num( "", py.misc.dis_td, 11, 17 );
	  prt_num( "", py.misc.dis_tac,12, 17 );
	  prt_num( "", py.misc.dis_ac, 13, 17 );
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void put_misc1()
{
/*	{ Prints age, height, weight, and SC			-JWT-	}*/

	  prt_num("Age          : ",py.misc.age	,3,40);
	  prt_num("Height       : ",py.misc.ht	,4,40);
	  prt_num("Weight       : ",py.misc.wt	,5,40);
	  prt_num("Social Class : ",py.misc.sc	,6,40);
	  prt_num("Difficulty   : ",py.misc.diffic,7,40);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void put_misc2()
{

/*	{ Prints the following information on the screen.	-JWT-	}*/

	  prt_num("Level      : ",py.misc.lev,10,31);
	  prt_num("Experience : ",py.misc.exp,11,31);
	  prt_num("Gold       : ",py.misc.money[TOTAL_],12,31);
	  prt_num("Account    : ",py.misc.account,13,31);
	  prt_num("Max Hit Points : ",py.misc.mhp,10,54);
	  prt_num("Cur Hit Points : ",(integer)(py.misc.chp),11,54);
	  prt_num("Max Mana       : ",py.misc.mana,12,54);
	  prt_num("Cur Mana       : ",(integer)(py.misc.cmana),13,54);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void put_misc3()
{

/*	{ Prints ratings on certain abilities			-RAK-	}*/
integer	 xbth,xbthb,xfos,xsrh,xstl,xdis;
integer	 xsave,xdev,xswm,xrep;
vtype	 xinfra;
vtype    tmp_str, tmp2;

  clear_from(14);

	    xbth  = py.misc.bth + py.misc.lev*BTH_LEV_ADJ + 
			py.misc.ptohit*BTH_PLUS_ADJ;
	    xbthb = py.misc.bthb + py.misc.lev*BTH_LEV_ADJ +
			py.misc.ptohit*BTH_PLUS_ADJ;
	    xfos  = 27 - py.misc.fos;
	    if (xfos < 0) {
		 xfos = 0;
		}
	    xsrh  = py.misc.srh + spell_adj(INT);
	    xstl  = py.misc.stl;
	    xdis  = py.misc.disarm + py.misc.lev + 2*todis_adj() + spell_adj(INT);
	    xsave = py.misc.save + py.misc.lev + spell_adj(WIS);
	    xdev  = py.misc.save + py.misc.lev + spell_adj(INT);
	    xswm  = py.flags.swim + 4;
	    xrep  = 6 + py.misc.rep div 25;
	    sprintf(xinfra,"%ld feet",py.flags.see_infra*10);

	prt("(Miscellaneous Abilities)",16,24);
	sprintf(tmp2, "%s%s","Fighting    : ",likert(xbth ,12,tmp_str));
	put_buffer(tmp2 ,17, 2);
	sprintf(tmp2, "%s%s","Bows/Throw  : ",likert(xbthb,12,tmp_str));
	put_buffer(tmp2 ,18, 2);
	sprintf(tmp2, "%s%s","Saving Throw: ",likert(xsave, 6,tmp_str));
	put_buffer(tmp2 ,19, 2);
	sprintf(tmp2, "%s%s","Stealth     : ",likert(xstl , 1,tmp_str));
	put_buffer(tmp2 ,17,27);
	sprintf(tmp2, "%s%s","Disarming   : ",likert(xdis , 8,tmp_str));
	put_buffer(tmp2 ,18,27);
	sprintf(tmp2, "%s%s","Magic Device: ",likert(xdev , 7,tmp_str));
	put_buffer(tmp2 ,19,27);
	sprintf(tmp2, "%s%s","Perception  : ",likert(xfos , 3,tmp_str));
	put_buffer(tmp2 ,17,52);
	sprintf(tmp2, "%s%s","Searching   : ",likert(xsrh , 6,tmp_str));
	put_buffer(tmp2 ,18,52);
	sprintf(tmp2, "%s%s","Infra-Vision: ", xinfra);
	put_buffer(tmp2, 19,52);
	sprintf(tmp2, "%s%s","Swimming    : ",likert(xswm , 1,tmp_str));
	put_buffer(tmp2 ,20,52);
	sprintf(tmp2, "%s%s","Reputation  : ",likert(xrep , 1,tmp_str));
	put_buffer(tmp2 ,20, 2);

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void upd_misc1()
{
/*{ Updates age, height, weight, and SC (amazing, huh?)	-KRC-	}*/

	  prt_num( "", py.misc.age, 3, 55 );
	  prt_num( "", py.misc.ht , 4, 55 );
	  prt_num( "", py.misc.wt , 5, 55 );
	  prt_num( "", py.misc.sc , 6, 55 );
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void display_char()
{

/*	{ Used to display the character on the screen.		-RAK-	}*/

	put_character();
	put_misc1();
	put_stats();
	put_misc2();
	put_misc3();

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void get_name()
{

/*	{ Gets a name for the character				-JWT-	}*/

 prt("Enter your player's name  [press <RETURN> when finished]",22,3);
 get_string(py.misc.name,3,15,24);
 clear_from(21);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void change_name()
{

/*	{ Chances the name of the character			-JWT-	} */

char	  c;
boolean	  flag = false;

     display_char();
do {
	prt("<c>hange character name.     <ESCAPE> to continue.",22,3);

	c = inkey();

	switch (c) {
	case 99: 
		get_name();
		break;
	case 0:
	case 3:
	case 25:
	case 26:
	case 27:
		flag = true;
		break;
	}

    } while (!flag);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void set_gem_values()
{
  integer   count;

  for (count = 1; count <= MAX_OBJECTS; count++) {
    /*with object_list[count] do*/
    if ((strstr(object_list[count].name,"Finely cut") != NULL) 
	&& (strstr(object_list[count].name,"of") != NULL)) {

      if (strstr(object_list[count].name,"Amber") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Agate") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Alexandrite") != NULL) {
	object_list[count].cost  += 5000;
      }
      // amethyst was misspelled as "amathyst".  2/15/00 JEB
      if (strstr(object_list[count].name,"Amethyst") != NULL) {
	object_list[count].cost  += 2000;
      }
      if (strstr(object_list[count].name,"Antlerite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Aquamarine") != NULL) {
	object_list[count].cost  += 6000;
      }
      if (strstr(object_list[count].name,"Argentite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Azurite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Beryl") != NULL) {
	object_list[count].cost  += 2000;
      }
      if (strstr(object_list[count].name,"Bloodstone") != NULL) {
	object_list[count].cost  += 3500;
      }
      if (strstr(object_list[count].name,"Calcite") != NULL) {
	object_list[count].cost  += 1500;
      }
      if (strstr(object_list[count].name,"Carnelian") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Coral") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Corundum") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Cryolite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Diamond") != NULL) {
	object_list[count].cost  += 35000;
      }
      if (strstr(object_list[count].name,"Diorite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Emerald") != NULL) {
	object_list[count].cost  += 20000;
      }
      if (strstr(object_list[count].name,"Flint") != NULL) {
	object_list[count].cost  += 5000;
      }
      if (strstr(object_list[count].name,"Fluorite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Gabbro") != NULL) {
	object_list[count].cost  += 5000;
      }
      if (strstr(object_list[count].name,"Garnet") != NULL) {
	object_list[count].cost  += 6500;
      }
      if (strstr(object_list[count].name,"Granite") != NULL) {
	object_list[count].cost  += 500;
      }
      if (strstr(object_list[count].name,"Gypsum") != NULL) {
	object_list[count].cost  += 3000;
      }
      if (strstr(object_list[count].name,"Hematite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Jade") != NULL) {
	object_list[count].cost  += 12000;
      }
      if (strstr(object_list[count].name,"Jasper") != NULL) {
	object_list[count].cost  += 3000;
      }
      if (strstr(object_list[count].name,"Kryptonite") != NULL) {
	object_list[count].cost  += 5000;
      }
      if (strstr(object_list[count].name,"Lapus lazuli") != NULL) {
	object_list[count].cost  += 4500;
      }
      if (strstr(object_list[count].name,"Limestone") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Malachite") != NULL) {
	object_list[count].cost  += 3000;
      }
      if (strstr(object_list[count].name,"Manganite") != NULL) {
	object_list[count].cost  += 5000;
      }
      if (strstr(object_list[count].name,"Marble") != NULL) {
	object_list[count].cost  += 5500;
      }
      if (strstr(object_list[count].name,"Mica") != NULL) {
	object_list[count].cost  += 1500;
      }
      if (strstr(object_list[count].name,"Moonstone") != NULL) {
	object_list[count].cost  += 3000;
      }
      if (strstr(object_list[count].name,"Neptunite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Obsidian") != NULL) {
	object_list[count].cost  += 2500;
      }
      if (strstr(object_list[count].name,"Onyx") != NULL) {
	object_list[count].cost  += 1500;
      }
      if (strstr(object_list[count].name,"Opal") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Pearl") != NULL) {
	object_list[count].cost  += 11500;
      }
      if (strstr(object_list[count].name,"Pyrite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Quartz") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Quartzite") != NULL) {
	object_list[count].cost  += 1500;
      }
      if (strstr(object_list[count].name,"Rhodonite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Rhyolite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Ruby") != NULL) {
	object_list[count].cost  += 14500;
      }
      if (strstr(object_list[count].name,"Sapphire") != NULL) {
	object_list[count].cost  += 14500;
      }
      if (strstr(object_list[count].name,"Sphalerite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Staurolite") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Tiger eye") != NULL) {
	object_list[count].cost  += 8500;
      }
      if (strstr(object_list[count].name,"Topaz") != NULL) {
	object_list[count].cost  += 1000;
      }
      if (strstr(object_list[count].name,"Turquoise") != NULL) {
	object_list[count].cost  += 3000;
      }
      if (strstr(object_list[count].name,"Zircon") != NULL) {
	object_list[count].cost  += 1000;
      }
    } /* end if (finely cut) */
  } /* end for */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* End FILE  create.c */
