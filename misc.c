/* misc.c */
/**/

#include "imoria.h"
#include <time.h>
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean check_kickout()
{
  /*{ Check to see if everyone should be kicked out of the game,	}*/
  /*{ by attempting to open the kick-out file.		-DMF-	        }*/

  boolean   return_value = false;
  FILE     *kick;

  kick = priv_fopen(MORIA_LCK,"r");

  if (kick != NULL) {
    fclose(kick);
    return_value = true;
  }

  return return_value;
}; /* end check_kickout */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sort_objects()
{
//	{ Order the treasure list by level			-RAK-	}
  integer        i1, i2, i3, gap;
  treasure_type  tmp;

  gap = MAX_OBJECTS / 2;
  while (gap > 0) {
    for (i1=gap+1; i1<=MAX_OBJECTS; i1++) {
      i2 = i1 - gap;
      while (i2 > 0) {
	i3 = i2 + gap;
	if (object_list[i2].level > object_list[i3].level) {
	  tmp = object_list[i2];
	  object_list[i2] = object_list[i3];
	  object_list[i3] = tmp;
	} else {
	  i2 = 0;
	}
	i2 = i2 - gap;
      } /* end while i2 */
    } /* end for i1 */
    gap = gap / 2;
  } /* end while gap */

//  Verify that the sort worked
//    i2 = 0;
//    for (i1=1; i1<=MAX_OBJECTS; i1++) {
//      if (i2 > object_list[i1].level) {
//	printf("Error: sort_objects failed\n");
//	exit_game();
//      }
//      i2 = object_list[i1].level;
//    }
}; /* end sort_objects */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void init_m_level()
{
//	{ Initializes M_LEVEL array for use with PLACE_MONSTER	-RAK-	}
  int   i1 = 1;
  int   i2 = 0;
  int   i3 = MAX_CREATURES - WIN_MON_TOT;

  do {
    m_level[i2] = 0;
    while ((i1 <= i3) && (c_list[i1].level == i2)) {
      m_level[i2]++;
      i1++;
    }
    i2++;
  } while (i2 <= MAX_MONS_LEVEL);

  for (i1 = 2; i1 <= MAX_MONS_LEVEL; i1++) {
    m_level[i1] += m_level[i1-1];
  }
  
//  for (i1 = 0; i1 < MAX_MONS_LEVEL+1; i1++) {
//    printf ("\n m_level[%d] : %d",i1,m_level[i1]);  fflush(stdout);
//  }

}; /* end init_m_level */ 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void init_t_level()
{
//	{ Initializes T_LEVEL array for use with PLACE_OBJECT	-RAK-	}
  int   i1 = 1;
  int   i2 = 0;

  do {
    while ((i1 <= MAX_OBJECTS) && (object_list[i1].level == i2)) {
      t_level[i2] = t_level[i2] + 1;  // number of treasures with this level
      i1++;
    }
    i2++;
  } while (!((i2 > MAX_OBJ_LEVEL) || (i1 > MAX_OBJECTS)));

  for (i1 = 1; i1 <= MAX_OBJ_LEVEL; i1++) {
    t_level[i1] += t_level[i1-1];
  }

}; /* end init_t_level */ 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void price_adjust()
{
  integer  i1;
  for (i1=1; i1<= MAX_OBJECTS; i1++) {
    object_list[i1].cost = trunc(object_list[i1].cost*COST_ADJ + 0.99);
  }

  for (i1=1; i1<= INVEN_INIT_MAX; i1++) {
    inventory_init[i1].cost = trunc(inventory_init[i1].cost*COST_ADJ + 0.99);
  }
}; /* end cost_adj */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void item_weight_adjust()
{
  integer  i1;
  for (i1=1; i1<= MAX_OBJECTS; i1++) {
    object_list[i1].weight *= WEIGHT_ADJ;
  }

  for (i1=1; i1<= INVEN_INIT_MAX; i1++) {
    inventory_init[i1].weight *= WEIGHT_ADJ;
  }
}; /* end item_weight_adjust */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean check_time()
{
  /*{ Check the day-time strings to see if open             -RAK-   }*/

  /* the file hours.dat is where days gets filled in from     */
  /* default hours are defined in values.h, in the days array */

  struct tm  now;
  integer    cur_time;
  integer    day, hour;
  boolean    return_value = false;

  /*{ Returns the day number; 1=Sunday...7=Saturday         -RAK-   }*/
  /*{ Returns the hour number; 0=midnight...23=11 PM        -RAK-   }*/

  cur_time = time(NULL);
  now      = *localtime(&cur_time);
  day      = now.tm_wday;
  hour     = now.tm_hour;

  if (days[day][hour+4] == 'X') {
    return_value = true;
  }

  return return_value;
}; /* end check_time */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean already_playing()
{
 /* XXXX check already playing */
  return false;
}; /* end already_playing */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char * cost_str(integer amt, string result)
{
  /*{ Return string describing how much the amount is worth	-DMF-	}*/
  integer    amtd9 = amt div 9;

  if (amtd9 >= MITHRIL_VALUE) {
    sprintf(result,"%ld mithril",((amt+MITHRIL_VALUE-1) div MITHRIL_VALUE));
  } else if (amtd9 >= PLATINUM_VALUE) {
    sprintf(result,"%ld platinum",((amt+PLATINUM_VALUE-1) div PLATINUM_VALUE));
  } else if (amtd9 >= GOLD_VALUE) {
    sprintf(result,"%ld gold",((amt+GOLD_VALUE-1) div GOLD_VALUE));
  } else if (amtd9 >= SILVER_VALUE) {
    sprintf(result,"%ld silver",((amt+SILVER_VALUE-1) div SILVER_VALUE));
  } else if (amtd9 >= COPPER_VALUE) {
    sprintf(result,"%ld copper",((amt+COPPER_VALUE-1) div COPPER_VALUE));
  } else {
    sprintf(result,"%ld iron",amt);
  }

  return result;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void reset_total_cash()
{
  /*{ recomputes cash totals for player and bank }*/

  integer  i1;

  py.misc.money[TOTAL_] = 0;
  for (i1 = IRON; i1 <= MITHRIL; i1++) {
    py.misc.money[TOTAL_] += py.misc.money[i1]*coin_value[i1];
  }
  py.misc.money[TOTAL_] = py.misc.money[TOTAL_] div GOLD_VALUE;

  bank[TOTAL_] = 0;
  for (i1 = GOLD; i1 <= MITHRIL; i1++) {
    bank[TOTAL_] += bank[i1] * coin_value[i1];
  }
  bank[TOTAL_] = bank[TOTAL_] div GOLD_VALUE;

};
//////////////////////////////////////////////////////////////////////
integer weight_limit()
{
  /*	{ Computes current weight limit				-RAK-	}*/
  integer   weight_cap;

	weight_cap =(py.stat.c[STR]+30)*PLAYER_WEIGHT_CAP + py.misc.wt;
	if (weight_cap > 3000) {
	  weight_cap = 3000;
	}
	weight_cap += py.misc.xtr_wgt;

	return weight_cap;
};
//////////////////////////////////////////////////////////////////////
void adv_time(boolean flag)
{
  /*{ Advance the game clock by one 'second'		-DMF-	}*/

  //with py.misc.cur_age do;
  PM.cur_age.secs++;

  if (PM.cur_age.secs > 399) {
    PM.cur_age.hour++;
    PM.cur_age.secs = 0;
    if (PM.cur_age.hour == 24) {
      PM.cur_age.day++;
      PM.cur_age.hour = 0;
      if (PM.cur_age.day == 29) {
	PM.cur_age.month++;
	PM.cur_age.day = 1;
	if (PM.cur_age.month == 14) {
	  PM.cur_age.month = 1;
	  PM.cur_age.year++;
	}
      }
    }
  }
  
  if ((flag) && ((PM.cur_age.secs % 100) == 0)) {
    prt_hp();
    if (is_magii) {
      prt_mana();
    }
    prt_time();
  }
  
};
//////////////////////////////////////////////////////////////////////
void check_kickout_time(integer num,integer check)
{
  /*{ Check for kicking people out of the game              -DMF-   }*/

  if ((num % check) == 1) {
    if (check_kickout()) {
      find_flag = false;
      msg_print("A new version of IMORIA is being installed.");
      msg_print("After your character is saved, wait a few minutes,");
      msg_print("And then try to run the game.");
      msg_print("");
      do {
	py.flags.dead = false;
	save_char(true);
      } while (true);
    }
  }
};

//////////////////////////////////////////////////////////////////////
char get_loc_symbol(integer y, integer x)
{
  /* check lights and stuff before calling loc_symbol */

  char     tmp_char = ' ';

  if (test_light(y,x)) {
    tmp_char = loc_symbol(y,x);
  } else if ((cave[y][x].cptr == 1) && (!find_flag)) {
    tmp_char = '@';
  } else if (cave[y][x].cptr > 1) {
    if (m_list[cave[y][x].cptr].ml) {
      tmp_char = loc_symbol(y,x);
    } else {
      tmp_char = ' ';
    }
  } else {
    tmp_char = ' ';
  }

  return tmp_char;
};
//////////////////////////////////////////////////////////////////////
char loc_symbol(integer y,integer x)
{
  char     sym;
  byteint  cptr, tptr, fval;
  
  wordint  mptr;
  
  cptr = cave[y][x].cptr;
  fval = cave[y][x].fval;
  tptr = cave[y][x].tptr;
  
  //with cave[y,x] do;
  if ((cptr == 1) && (!find_flag)) {
    sym = '@';
  } else if (py.flags.blind > 0) {
    sym = ' ';
  } else {
    
    
    if (cptr > 1) {
      
      //with m_list[cptr] do;
      mptr = m_list[cptr].mptr;
      if ((m_list[cptr].ml) &&
	  (!is_in(fval,water_set) ||
	   (is_in(fval,water_set) &&
	    ((uand(c_list[mptr].cmove,0x00800000) != 0) ||
	     (distance(char_row,char_col,y,x) <= 5)))) &&
	  ((uand(c_list[mptr].cmove,0x00010000) == 0) || (py.flags.see_inv))) {
	sym = c_list[mptr].cchar;
      } else if (tptr > 0) {
	sym = t_list[tptr].tchar;
      } else if (is_in(fval,earth_set)) { /* 0, 3, 8 and 9 were here too */
	sym = '.';
      } else if (is_in(fval,pwall_set)) {
	sym = '#';
      } else if (is_in(fval,water_set)) {
	sym = '`' + 0x80;
      } else {
	/* unknown terrain type */
	sym = '.' + 0x80;
      }      

    } else if (tptr > 0) {
      
      if (is_in(fval,water_set)) {
	if (is_in(t_list[tptr].tval, float_set) ||
	    ((distance(char_row,char_col,y,x) <= 5) &&
	     (los(char_row,char_col,y,x)))) {
	  sym = t_list[tptr].tchar;
	} else {
	  sym = '`' + 0x80;
	}
      } else {
	sym = t_list[tptr].tchar;
      }
      
    } else if (is_in(fval,earth_set)) { /* 0, 3, 8 and 9 were here too */
      sym = '.';
    } else if (is_in(fval,pwall_set)) {
      sym = '#';
    } else if (is_in(fval,water_set)) {
      sym = '`' + 0x80;
    } else {
      /* unknown terrain type */
      sym = '.' + 0x80;
    }      
  }
  
#if DO_DEBUG
  if ((((int)sym & 0x7F) < 32) || (((int)sym & 0x7F) > 126)) {
    fprintf(debug_file,": ERROR in loc_sym: (%ld, %ld) = %ld   cptr=%ld tptr=%ld fval=%ld\n",
	    x,y,(integer)sym,(integer)cptr,(integer)tptr,(integer)fval);
    fflush(debug_file);
  }
#endif

  return sym;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
byteint squish_stat(integer this)
{
	if (this > 250) {
	  return 250;
	} else if (this < 0) {
          return 0;
	} else {
	  return this;
	}
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
byteint de_statp(byteint stat)
{
/*	{ Decreases a stat by one randomized level		-RAK-	}*/

  byteint   duh;
  byteint   return_value;

  if (stat < 11) {
    return_value = stat;
  } else if (stat < 151) {
    return_value = 10;
  } else if (stat < 241) {
    duh = randint(10) + 5;
    if (stat - duh < 150) {
      duh = stat - 150;
    }
    return_value = duh;
  } else {
    return_value = randint(3);
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
byteint in_statp(byteint stat)
{
    /*	{ Increases a stat by one randomized level		-RAK-	}*/
  
  byteint return_value;

  if (stat < 150) {
    return_value = 10;
  } else if (stat < 220) {
    return_value = randint(25);
  } else if (stat < 240) {
    return_value = randint(10);
  } else if (stat < 250) {
    return_value = 1;
  } else {
    return_value = 0;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer todam_adj()
{
    /*	{ Returns a character's adjustment to damage		-JWT-	}*/
  integer   return_value;
  integer   str;

  str = py.stat.c[STR];

  if      (str <	10) {
    return_value = -2;
  } else if (str <	20) {
    return_value = -1;
  } else if (str < 130) {
    return_value =  0;
  } else if (str < 140) {
    return_value =  1;
  } else if (str < 150) {
    return_value =  2;
  } else if (str < 226) {
    return_value =  3;
  } else if (str < 241) {
    return_value =  4;
  } else if (str < 249) {
    return_value =  5;
  } else {
    return_value =  6;
  }
  
  return return_value;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer todis_adj()
{
  /*	{ Returns a character's adjustment to disarm		-RAK-	}*/
  integer   return_value;
  integer   dex;

  dex = py.stat.c[DEX];

  if      (dex < 10) {
    return_value =  -8;
  } else if (dex < 20) {
    return_value =  -6;
  } else if (dex < 30) {
    return_value =  -4;
  } else if (dex < 40) {
    return_value =  -2;
  } else if (dex < 50) {
    return_value =  -1;
  } else if (dex < 100) {
    return_value =   0;
  } else if (dex < 130) {
    return_value =   1;
  } else if (dex < 150) {
    return_value =   2;
  } else if (dex < 191) {
    return_value =   4;
  } else if (dex < 226) {
    return_value =   5;
  } else if (dex < 249) {
    return_value =   6;
  } else {
    return_value =   8;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer con_adj()
{
  /*	{ Returns a character's adjustment to hit points	-JWT-	}*/

  integer   con;
  integer   return_value;

  con = py.stat.c[CON];

  if      (con <	10) {
    return_value = -4;
  } else if (con <	20) {
    return_value = -3;
  } else if (con <	30) {
    return_value = -2;
  } else if (con <	40) {
    return_value = -1;
  } else if (con < 140) {
    return_value =  0;
  } else if (con < 150) {
    return_value =  1;
  } else if (con < 226) {
    return_value =  2;
  } else if (con < 299) {
    return_value =  3;
  } else {
    return_value =  4;
  }

  return return_value;
 };
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
real chr_adj()
{
  /*{ Adjustment for charisma				-RAK-	}*/
  /*{ Percent decrease or increase in price of goods		}*/

  real      return_value;

  //with py.stat do;
  if (PS.c[CHR] > 249) {
    return_value = -0.10;
  } else if (PS.c[CHR] > 239) {
    return_value = -0.08;
  } else if (PS.c[CHR] > 219) { 
    return_value = -0.06;
  } else if (PS.c[CHR] > 199) { 
    return_value = -0.04;
  } else if (PS.c[CHR] > 150) { 
    return_value = -0.02;
  } else if (PS.c[CHR] >= 100) { 
    return_value = 0.15 - (PS.c[CHR] div 10)/100;
  } else {
    return_value = 0.25 - (PS.c[CHR] div 10)/50;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer tohit_adj()
{
  /*	{ Returns a character's adjustment to hit.		-JWT-	}*/

  integer   total;
  integer   dex, str;
    
  dex = py.stat.c[DEX];
  str = py.stat.c[STR];

  if	(dex <  10) {
    total = -3;
  } else if (dex <  30) {
    total = -2;
  } else if (dex <  50) {
    total = -1;
  } else if (dex < 130) {
    total =  0;
  } else if (dex < 140) {
    total =  1;
  } else if (dex < 150) {
    total =  2;
  } else if (dex < 201) {
    total =  3;
  } else if (dex < 250) {
    total =  4;
  } else {
    total =  5;
  }

  if	(str <  10) {
    total = total - 3;
  } else if (str <  20) {
    total = total - 2;
  } else if (str <  40) {
    total = total - 1;
  } else if (str < 150) {
    total = total + 0;
  } else if (str < 226) {
    total = total + 1;
  } else if (str < 241) {
    total = total + 2;
  } else if (str < 249) {
    total = total + 3;
  } else {
    total = total + 4;
  }

  return total;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer toac_adj()
{
/*	{ Returns a character's adjustment to armor class	-JWT-	}*/

  integer   dex;
  integer   return_value;

  dex = py.stat.c[DEX];

  if      (dex <	10) {
    return_value =  -4;
  } else if (dex <	20) {
    return_value =  -3;
  } else if (dex <	30) {
    return_value =  -2;
  } else if (dex <	40) {
    return_value =  -1;
  } else if (dex < 120) {
    return_value =   0;
  } else if (dex < 150) {
    return_value =   1;
  } else if (dex < 191) {
    return_value =   2;
  } else if (dex < 226) {
    return_value =   3;
  } else if (dex < 249) {
    return_value =   4;
  } else {
    return_value =   5;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
byteint  characters_sex()
{
  /*	{ Determine character's sex				-DCJ-	}*/
  /*	  characters_sex := trunc((index(sex_type,py.misc.sex)+5)/6) ;  */

  return ((py.misc.sex[0] == 'F') ? FEMALE : MALE);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void add_days(game_time_type	*ti,integer d)
{
  /*	{ Add days to the current date				-DMF-	}*/

  //	  ti->day++;
  //	  ti->month += (ti->day-1) div 28;
  //	  ti->day    = ((ti->day-1) % 28) + 1;
  //	  ti->year  += (ti->month-1) div 13;
  //	  ti->month  = ((ti->month-1) % 13) + 1;

  // 10/26/00 -- JEB:
  // DMF's code works great (if a little strangely) if you only ever add 1 day,
  // which this function did.  notice that the above code ignores the 'd'
  // parameter, which in turn means that no matter how long of a stay you buy
  // in the inn, you really only get 1 day.  i thought about just putting a loop
  // around the above code to iterate 'd' times, but that's lame so here's some
  // more robust code that simply calculates the day, month, and year increments
  // for any value of 'd'.  note that the above code implies that the year is
  // 364 days long (13 months of 28 days each), which i've kept:

  byteint yrs, mos;
  yrs = (int)(d/364);  // yrs = how many years you get from 'd' days
  d -= 364*yrs;        // d = however many days are left over...
  mos = (int)(d/28);   // mos = how many months you get from the remaining days
  d -= 28*mos;         // d = however many days are left over...
  ti->day   += d;      // add the remaining days, months, and years
  ti->month += mos;
  ti->year  += yrs;
  if(ti->day   > 28) { ti->month++; ti->day   %= 28; }   // fix any overflows
  if(ti->month > 13) { ti->year++;  ti->month %= 13; }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void am__add_munny(integer *amount, integer *to_bank, integer wl, 
		   integer type_num)
{
  integer   trans,w_max;
  integer   coin_num;

  coin_num = py.misc.money[type_num];
  trans = *amount div coin_value[type_num];
  w_max = (wl*100-inven_weight) div COIN_WEIGHT;
  if (w_max < - coin_num) {
    w_max = - coin_num;
  }
  if (w_max < trans) {
    *to_bank += (trans - w_max) * coin_value[type_num];
    trans     = w_max;
  }
  inven_weight += COIN_WEIGHT * trans;
  PM.money[type_num] = coin_num + trans;
  *amount = *amount % coin_value[type_num];
};
//////////////////////////////////////////////////////////////////////
void add_money(integer amount)
{
  /*	{ Add money in the lightest possible amounts.		-DMF-/DY}*/

  integer   to_bank,wl,i1;
  string    out_val, out2;
  integer   type_num;

  to_bank = 0;
  wl = weight_limit();
  //with py.misc do;

  for (type_num = MITHRIL; type_num >= IRON; type_num--) {
    am__add_munny(&amount, &to_bank, wl, type_num);
  }

  reset_total_cash();

  if (to_bank > 0) {
    sprintf(out_val,"You cannot carry %s of the money",cost_str(to_bank,out2));
    msg_print(out_val);
    if (get_yes_no("Do you wish to send a page to the bank with the excess money?")) {
      i1 = (((95 * to_bank) div 100) div GOLD_VALUE);
      if (i1 < 5) {
	msg_print("The page cannot be moved by such paltry sums of gold.");
      } else {
	if (randint(mugging_chance) == 1) {
	  msg_print("The page is mugged!");
	  sprintf(out_val, "The %s is lost!", cost_str(to_bank,out2));
	  msg_print(out_val);
	} else {
	  bank[GOLD] += i1;
	  py.misc.account += i1;
	  bank[TOTAL_] = ((bank[MITHRIL]*coin_value[MITHRIL]+
			   bank[PLATINUM]*coin_value[PLATINUM]) div 
			  GOLD_VALUE + bank[GOLD]);
	  sprintf(out_val,
		  "The page deposits %ld gold at the bank for you.",i1);
	  msg_print(out_val);
	}
      }
    } else {
      msg_print("You cannot carry the change, so it is lost.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
treas_ptr money_carry()
{
  /*{ Pick up some money	-DMF-	}*/

  //with py.misc do;
  //with inven_temp^.data do;

  PM.money[inven_temp->data.level] += inven_temp->data.number;
  reset_total_cash();
  inven_weight += inven_temp->data.number * inven_temp->data.weight;
  
  prt_gold();
  prt_weight();
  
  return inven_temp;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean sm__sub_munny(integer *amt, integer *wt, integer type_num)
{
  integer   trans,coin_num;
  boolean   return_value;

  coin_num = py.misc.money[type_num];
  trans = (*amt+coin_value[type_num]-1) div coin_value[type_num];
  if (coin_num < trans) {
    trans = coin_num;
  }
  (*wt)  += COIN_WEIGHT*trans;
  py.misc.money[type_num] = coin_num - trans;
  (*amt) -= trans*coin_value[type_num];

  return_value = (amt > 0);
  return  return_value;
};
//////////////////////////////////////////////////////////////////////
void subtract_money(integer amount, boolean make_change)
{
  /*{ Give money to store, but can give back change	 -DMF-/DY}*/

  integer    amt,wt;
  integer    type_num;

  amt      = amount;
  wt       = 0;
  type_num = 1;

  for (;sm__sub_munny(&amt,&wt,type_num) && (type_num < MITHRIL); type_num++) {
  }

  inven_weight -= wt;
  reset_total_cash();

  if (make_change) {
    add_money(-amt);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void char_inven_init()
{
  /*	{ Init players with some belongings			-RAK-	}*/

  integer    i1,i2;

//	printf("\nENTER char_inven_init\n\n"); fflush(stdout);

	inventory_list = nil;
	for (i1 = Equipment_min; i1 < EQUIP_MAX; i1++) {
	  equipment[i1].tval = 0;
	}

	for (i1 = 0; i1 < 5; i1++) {
//	  printf("    char_inven_init A%d\n",i1); fflush(stdout);

	    i2 = player_init[py.misc.pclass][i1];
	    inven_temp->data = inventory_init[i2];
	    inven_carry();

//	  printf("    char_inven_init %d (done)\n",i1); fflush(stdout);
	}
//	printf("\nEXIT char_inven_init\n\n"); fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer spell_adj(stat_set attr)
{
  integer  statval;
  integer  return_value;

  statval = py.stat.c[(int)attr];

  if (statval >249) {
    return_value = 7;
  } else if (statval > 239) {
    return_value = 6;
  } else if (statval > 219) {
    return_value = 5;
  } else if (statval > 199) {
    return_value = 4;
  } else if (statval > 149) {
    return_value = 3;
  } else if (statval > 109) {
    return_value = 2;
  } else if (statval >  39) {
    return_value = 1;
  } else {
    return_value = 0;
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer bard_adj()
{
  return (spell_adj(CHR) +spell_adj(DEX) + 1) div 2;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer druid_adj()
{
  return (spell_adj(CHR) + spell_adj(WIS) + 1) div 2;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer monk_adj()
{
  return (spell_adj(INT) + spell_adj(WIS) + 1) div 2;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean learn_spell(boolean *redraw)
{
  /*{ Learn some magic spells (Mage)			-RAK-	}*/

  unsigned long   i2,i4;
  integer         i1,i3,sn,sc;
  integer         new_spells;
  unsigned long   spell_flag,spell_flag2;
  spl_type        spell;
  treas_ptr       curse;

  boolean         return_value = false;

  i1 = 0;
  spell_flag  = 0;
  spell_flag2 = 0;
  curse = inventory_list;
  new_spells = num_new_spells(spell_adj(INT));

  while (curse != nil) {
    if (curse->data.tval == Magic_Book) {
      spell_flag  |= curse->data.flags;
      spell_flag2 |= curse->data.flags2;
    }
    curse = curse->next;
  }
  
  while ((new_spells > 0) && ((spell_flag > 0) || (spell_flag2 > 0))) {
    i1 = 0;
    i2 = spell_flag;
    i4 = spell_flag2;
    
    do {
      i3 = bit_pos64(&i4,&i2);
      if (i3 > 31) {
	i3--;
      }
      if (PSPELL(i3).slevel <= py.misc.lev) {
	if (!(PSPELL(i3).learned)) {
	  spell[i1++].splnum = i3;
	}
      }
    } while ((i2 != 0) || (i4 != 0));

    if (i1 > 0) {
      print_new_spells(spell,i1,redraw);
      if (get_spell(spell,i1,&sn,&sc,"Learn which spell?",redraw)) {
//	printf("\n\n>>> Got spell %d<<<\n\n", sn);fflush(stdout);
	PSPELL(sn).learned = true;
	return_value = true;
	if (py.misc.mana == 0) {
	  py.misc.mana   = 1;
	  py.misc.cmana  = 1;
	}
      } else {
	new_spells = 0;
      }
    } else {
      new_spells = 0;
    }
    new_spells--;
  } /* end while new_spells > 0 */

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean learn_prayer()
{
  /*{ Learn some prayers (Priest)				-RAK-	}*/
  integer        i1,i2,i3,i4,new_spell;
  integer        test_array[32];	    /*	: array [1..32] of integer;*/
  unsigned long  spell_flag,spell_flag2;
  treas_ptr      curse;
  boolean        return_value = false;

  i1 = 0;
  spell_flag  = 0;
  spell_flag2 = 0;
  curse = inventory_list;
  while (curse != nil) {
    if (curse->data.tval == Prayer_Book) {
      spell_flag  |= curse->data.flags;
      spell_flag2 |= curse->data.flags2;
    }
    curse = curse->next;
  }

  i1 = 0;
  while ((spell_flag > 0) || (spell_flag2 > 0)) {
    i2 = bit_pos64(&spell_flag2,&spell_flag);
    if (i2 > 31) {
      i2--;
    }
    if (PSPELL(i2).slevel <= py.misc.lev) {
      if (!PSPELL(i2).learned) {
	test_array[i1++] = i2;
      }
    }
  }

  i2 = num_new_spells(spell_adj(WIS));
  new_spell = 0;

  while ((i1 > 0) && (i2 > 0)) {
    i3 = randint(i1)-1;
    PSPELL(test_array[i3]).learned = true;
    new_spell++;

    for (i4 = i3; i4 < i1; i4++) {
      test_array[i4] = test_array[i4+1];
    }

    i1--;	/*{ One less spell to learn	}*/
    i2--;	/*{ Learned one			}*/
  }

  if (new_spell > 0) {
    if (new_spell > 1) {
      msg_print("You learned new prayers!");
    } else {
      msg_print("You learned a new prayer!");
    }
    if (py.misc.exp == 0) {
      msg_print(" ");
    }
    if (py.misc.mana == 0) {
      py.misc.mana  = 1;
      py.misc.cmana = 1;
    }
    return_value = true;
    
  } else { 
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean learn_discipline()
{
  /*{ Learn some disciplines (Monk)				-RAK-	}*/

  integer        i1,i2,i3,i4,new_spell;
  integer        test_array[32];	    /*	: array [1..32] of integer;*/
  unsigned long  spell_flag,spell_flag2;
  boolean        return_value = false;

//  printf("\n\n  ^^^ENTER learn_discip^^^\n\n");fflush(stdout);

  i1 = 0;
  spell_flag  = 0x00003FFF;
  spell_flag2 = 0x00000000;

  while ((spell_flag > 0) || (spell_flag2 > 0)) {
    i2 = bit_pos64(&spell_flag2,&spell_flag);
    if (i2 > 31) {
      i2--;
    }
    if (PSPELL(i2).slevel <= py.misc.lev) {
      if (!PSPELL(i2).learned) {
	i1++;
	test_array[i1] = i2;
      }
    }
  }
  
  i2 = num_new_spells(spell_adj(WIS));
  new_spell = 0;
  
  while ((i1 > 0) && (i2 > 0)) {
    i3 = randint(i1)-1;
    PSPELL(test_array[i3]).learned = true;
    new_spell++;
    
    for (i4 = i3; i4 < i1; i4++) {
      test_array[i4] = test_array[i4+1];
    }
    
    i1--;	/*{ One less spell to learn	}*/
    i2--;	/*{ Learned one			}*/
  }
  if (new_spell > 0) {
    if (new_spell > 1) {
      msg_print("You learned new disciplines!");
    } else {
      msg_print("You learned a new discipline!");
    }
    if (py.misc.exp == 0) {
      msg_print(" ");
    }
    if (py.misc.mana == 0) {
      py.misc.mana  = 1;
      py.misc.cmana = 1;
    }
    return_value = true;
    
  } else { 
    return_value = false;
  }
    
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean learn_song(boolean *redraw)
{
  /*{ Learn some magic songs (Bard)			-Cap'n-	}*/

  unsigned long   i2,i4;
  integer         i1,i3,sn,sc;
  integer         new_spells;
  unsigned long   spell_flag,spell_flag2;
  spl_type        spell;
  treas_ptr       curse;

  boolean         return_value = false;

  i1 = 0;
  spell_flag  = 0;
  spell_flag2 = 0;
  curse = inventory_list;
  new_spells = num_new_spells(bard_adj());

  while (curse != nil) {
    if (curse->data.tval == Song_Book) {
      spell_flag  |= curse->data.flags;
      spell_flag2 |= curse->data.flags2;
    }
    curse = curse->next;
  }
  
  while ((new_spells > 0) && ((spell_flag > 0) || (spell_flag2 > 0))) {
    i1 = 0;
    i2 = spell_flag;
    i4 = spell_flag2;
    
    do {
      i3 = bit_pos64(&i4,&i2);
      if (i3 > 31) {
	i3--;
      }
      if (PSPELL(i3).slevel <= py.misc.lev) {
	if (!(PSPELL(i3).learned)) {
	  spell[i1++].splnum = i3;
	}
      }
    } while ((i2 != 0) || (i4 != 0));

    if (i1 > 0) {
      print_new_spells(spell,i1,redraw);
      if (get_spell(spell,i1,&sn,&sc,"Learn which spell?",redraw)) {
	PSPELL(sn).learned = true;
	return_value = true;
	if (py.misc.mana == 0) {
	  py.misc.mana   = 1;
	  py.misc.cmana  = 1;
	}
      } else {
	new_spells = 0;
      }
    } else {
      new_spells = 0;
    }
    new_spells--;
  } /* end while new_spells > 0 */

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean learn_druid()
{
  /*{ Learn some druid spells (Druid)			-Cap'n-	}*/

  integer        i1,i2,i3,i4,new_spell;
  integer        test_array[32];	    /*	: array [1..32] of integer;*/
  unsigned long  spell_flag,spell_flag2;
  treas_ptr      curse;
  boolean        return_value = false;

  i1 = 0;
  spell_flag  = 0;
  spell_flag2 = 0;
  curse = inventory_list;
  while (curse != nil) {
    if (curse->data.tval == Instrument) {
      spell_flag  |= curse->data.flags;
      spell_flag2 |= curse->data.flags2;
    }
    curse = curse->next;
  }

  i1 = 0;
  while ((spell_flag > 0) || (spell_flag2 > 0)) {
    i2 = bit_pos64(&spell_flag2,&spell_flag);
      if (i2 > 31) {
	i2--;
      }
    if (PSPELL(i2).slevel <= py.misc.lev) {
      if (!PSPELL(i2).learned) {
	test_array[i1++] = i2;
      }
    }
  }

  i2 = num_new_spells(druid_adj());
  new_spell = 0;

  while ((i1 > 0) && (i2 > 0)) {
    i3 = randint(i1)-1;
    PSPELL(test_array[i3]).learned = true;
    new_spell++;

    for (i4 = i3; i4 < i1; i4++) {
      test_array[i4] = test_array[i4+1];
    }

    i1--;	/*{ One less spell to learn	}*/
    i2--;	/*{ Learned one			}*/
  }

  if (new_spell > 0) {
    if (new_spell > 1) {
      msg_print("You learned new songs!");
    } else {
      msg_print("You learned a new song!");
    }
    if (py.misc.exp == 0) {
      msg_print(" ");
    }
    if (py.misc.mana == 0) {
      py.misc.mana  = 1;
      py.misc.cmana = 1;
    }
    return_value = true;
    
  } else { 
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gain_mana(integer amount)
{
  /*{ Gain some mana if you know at least one spell 	-RAK-	}*/

  integer   i1,new_mana;
  boolean   knows_spell = false;

  for (i1 = 0; i1 < MAX_SPELLS; i1++) {
    if (PSPELL(i1).learned) {
      knows_spell = true;
    }
  }

  if (knows_spell) {
    if (py.misc.lev | 1) {

      switch (amount) {
      case 0:                    new_mana = 0;          break;
      case 1:  case 2:  case 3:  new_mana = 1;          break;
      case 4:  case 5:           new_mana = 2;          break;
      case 6:                    new_mana = 3;          break;
      case 7:                    new_mana = 4;          break;
      default:                   new_mana = 0;          break;
      }

    } else {
      
      switch (amount) {
      case 0:                    new_mana = 0;          break;
      case 1:  case 2:           new_mana = 1;          break;
      case 3:  case 4:           new_mana = 2;          break;
      case 5:  case 6:           new_mana = 3;          break;
      case 7:                    new_mana = 4;          break;
      default:                   new_mana = 0;          break;
      }

    }
    py.misc.mana  += new_mana;
    py.misc.cmana += new_mana;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void print_new_spells(spl_type	spell,
		      integer	num,
		      boolean	*redraw)
{

  /*{ Print list of spells					-RAK-	}*/

  integer   i1;
  vtype     out_val;

  *redraw = true;
  clear_from(1);
  prt("   Name                          Level  Mana  %Failure",2,1);
  for (i1 = 0; i1 < num; i1++) {
    /*with magic_spell[py.misc.pclass,spell[i1].splnum] do*/
    if (i1 < 23) {
      if (spell[i1].splnum == -1) {
	out_val[0] = 0; /* leave gaps for unknown spells */
      } else {
	spell_chance(&(spell[i1]));
	sprintf(out_val,"%c) %-30s %3d    %3d      %2ld",
		97+(int)i1,
		PSPELL(spell[i1].splnum).sname,
		PSPELL(spell[i1].splnum).slevel,
		PSPELL(spell[i1].splnum).smana,
		spell[i1].splchn);
      }
      prt(out_val,3+i1,1);
    }
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean get_spell(spl_type spell, integer num,
		  integer *sn,integer *sc, vtype prompt,
		  boolean *redraw)
{
   /*{ Returns spell pointer					-RAK-	}*/


  boolean    flag = true;
  char       choice;
  vtype      out_val1;

  *sn = -1;
  
  sprintf(out_val1,"(Spells a-%c, *,<space>=List, <ESCAPE>=exit) %s",
	  (int)num+96,prompt);

  while (((*sn < 0) || (*sn >= num)) && (flag)) {
    *sn = -1;

    prt(out_val1,1,1);
    choice = inkey();
    switch (choice) {
    case 0: case 3: case 25: case 26: case 27:
      flag = false;
      /*{ reset_flag := true;}*/
      break;
      
    case 42: case 32:
      print_new_spells(spell,num,redraw);
      break;

    default:
      *sn = choice - 97;
      if (spell[*sn].splnum < 0) {
	*sn = -1;
      }
      break;
    }
  } /* end while */

  msg_flag = false;
  if (flag) {
    spell_chance(&(spell[*sn]));
    *sc = spell[*sn].splchn;
    *sn = spell[*sn].splnum;
  }

//  printf("\n\n  ^^^EXIT get_spell %d^^^\n\n", *sn);fflush(stdout);
  return flag;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer num_new_spells(integer smarts)
{
  integer   return_value;

  switch (smarts) {
  case 1:  case 2:  case 3:    return_value = 1;                break;
  case 4:  case 5:             return_value = randint(2);       break;
  case 6:                      return_value = randint(3);       break;
  case 7:                      return_value = randint(2)+1;     break;
  default:                     return_value = 0;                break;
  }

  return return_value;
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void spell_chance(spl_rec *spell) {
/*	{ Returns spell chance of failure for spell		-RAK-	}*/

  /*	with magic_spell[py.misc.pclass,spell.splnum] do*/
  /*	  with spell do                                 */

  spell->splchn = PSPELL(spell->splnum).sfail -
    3*(py.misc.lev - PSPELL(spell->splnum).slevel);

  if (class[py.misc.pclass].mspell) {
    spell->splchn -= 3*(spell_adj(INT)-1);
  } else if (class[py.misc.pclass].bspell) {
    spell->splchn -= 3*(bard_adj()-1);
  } else if (class[py.misc.pclass].dspell) {
    spell->splchn -= 3*(druid_adj()-1);
  } else {
    spell->splchn -= 3*(spell_adj(WIS)-1);    
  }

  if (PSPELL(spell->splnum).smana > py.misc.cmana) {
    spell->splchn += 5* (int)(PSPELL(spell->splnum).smana-
			      py.misc.cmana);
  }

  if (spell->splchn > 95) {
    spell->splchn = 95;
  } else if (spell->splchn < 5) {
    spell->splchn = 5;
  }

}; /* end spell_chance */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer bit_pos(unsigned long *test) {
/* Returns position of first set bit			-RAK-	*/
/*     and clears that bit */

  register int i;
  register unsigned long mask = 0x1;

  for (i = 0; i < sizeof(*test)*8; i++) {
    if (*test & mask) {
      *test &= ~mask;
      return(i);
    }
    mask <<= 1;
  }

  /* no one bits found */
  return(-1);
};

//////////////////////////////////////////////////////////////////////

integer bit_pos64(unsigned long *high, unsigned long *low)
{
  /*!	This is the 64-bit version of bit_pos */

  integer    pos;

  pos = bit_pos(low);
  if (pos == -1) {
    pos = bit_pos(high);
    if (pos != -1) {
      pos += 32;
    }
  }

  return pos;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void insert_str(char *object_str, char *mtc_str, char *insert_str)
{
  char * s1;
  char ending[80];

  s1 = strstr(object_str, mtc_str);
  if (s1 != NULL) {
    strcpy(ending, s1+strlen(mtc_str));
    strcpy(s1, insert_str);
    strcat(s1, ending);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void insert_num(char *object_str,char *mtc_str, 
		integer number, boolean show_sign)
{
  vtype     numstr;

  sprintf(numstr, "%s%ld", (((number>0) && show_sign) ? "+" : ""), number);
//  strcat(object_str, " ");
  
  insert_str(object_str, mtc_str, numstr);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void set_difficulty(integer diff)
{

  /*	{ Set up the variables that depend upon the difficulty of
	game chosen.						-DMF-	}*/

  switch (diff) {
  case 1 : 				/*{ Brain-dead	}*/
    dun_str_mc	= 75;
    dun_str_qc	= 30;
    dun_unusual	= 250;
    obj_great	= 50;
    treas_room_alloc= 3;
    treas_any_alloc	= 1;
    treas_gold_alloc= 4;
    obj_std_adj	= 0.9;
    obj_std_min	= 3;
    obj_town_level	= 5;
    obj_base_magic	= 6;
    obj_base_max	= 70;
    obj_div_special	= 20;
    obj_div_cursed	= 1.2;
    mon_nasty	= 150;
    mon_mult_adj	= 15;
    break;
  case 2 :				/*{ Easy		}*/
    dun_str_mc	= 85;
    dun_str_qc	= 45;
    dun_unusual	= 175;
    obj_great	= 42;
    treas_room_alloc= 5;
    treas_any_alloc	= 1;
    treas_gold_alloc= 3;
    treas_gold_alloc= 3;
    obj_std_adj	= 1.05;
    obj_std_min	= 5;
    obj_town_level	= 6;	
    obj_base_magic	= 9;
    obj_base_max	= 85;
    obj_div_special	= 15;
    obj_div_cursed	= 1.2;
    mon_nasty	= 100;
    mon_mult_adj	= 10;
    break;
  case 3 :				/*{ Normal	}*/
  default:
    dun_str_mc	= 95;
    dun_str_qc	= 55;
    dun_unusual	= 100;
    obj_great	= 30;
    treas_room_alloc= 7;
    treas_any_alloc	= 2;
    treas_gold_alloc= 2;
    obj_std_adj	= 1.25;
    obj_std_min	= 7;
    obj_town_level	= 7;
    obj_base_magic	= 12;
    obj_base_max	= 100;
    obj_div_special	= 11;
    obj_div_cursed	= 1.2;
    mon_nasty	= 50;
    mon_mult_adj	= 7;
    break;
  case 4 :				/*{ Hard		}*/
    dun_str_mc	= 97;
    dun_str_qc	= 75;
    dun_unusual	= 60;
    obj_great	= 24;
    treas_room_alloc= 12;
    treas_any_alloc	= 3;
    treas_gold_alloc= 2;
    obj_std_adj	= 1.5;
    obj_std_min	= 10;
    obj_town_level	= 10;
    obj_base_magic	= 15;
    obj_base_max	= 115;
    obj_div_special	= 8;
    obj_div_cursed	= 1.2;
    mon_nasty	= 20;
    mon_mult_adj	= 5;
    break;
  case 5 : 				/*{ Ultra-hard	}*/
    dun_str_mc	= 99;
    dun_str_qc	= 85;
    dun_unusual	= 30;
    obj_great	= 15;
    treas_room_alloc= 15;
    treas_any_alloc	= 4;
    treas_gold_alloc= 1;
    obj_std_adj	= 2.0;
    obj_std_min	= 15;
    obj_town_level	= 15;
    obj_base_magic	= 18;
    obj_base_max	= 130;
    obj_div_special	= 6;
    obj_div_cursed	= 1.2;
    mon_nasty	= 5;
    mon_mult_adj	= 3;
    break;
  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char * day_of_week_string(integer day,integer wid,string  result)
{
  /*{ Return first X characters of day of week		-DMF-	}*/
  switch (day % 7) {
  case 0 : strcpy(result, "Saturday  "); break;
  case 1 : strcpy(result, "Sunday    "); break;
  case 2 : strcpy(result, "Monday    "); break;
  case 3 : strcpy(result, "Tuesday   "); break;
  case 4 : strcpy(result, "Wednesday "); break;
  case 5 : strcpy(result, "Thursday  "); break;
  case 6 : strcpy(result, "Friday    "); break;
  }
  if (strlen(result) > wid) {
    result[wid] = 0;
  }

  return result;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char * month_string(integer mon, string result)
{
  /*{ Return the name of a numbered month			-DMF-	}*/
  switch (mon) {
  case 1  : strcpy(result,"January");        break;
  case 2  : strcpy(result,"February");       break;
  case 3  : strcpy(result,"March");          break;
  case 4  : strcpy(result,"April");          break;
  case 5  : strcpy(result,"May");            break;
  case 6  : strcpy(result,"June");           break;
  case 7  : strcpy(result,"July");           break;
  case 8  : strcpy(result,"August");         break;
  case 9  : strcpy(result,"September");      break;
  case 10 : strcpy(result,"October");        break;
  case 11 : strcpy(result,"November");       break;
  case 12 : strcpy(result,"December");       break;
  default:
  case 13 : strcpy(result,"Moria");          break;
  }
  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char * time_string(integer hour, integer sec, string  result)
{
  /*{ Return the time in the format HH:MM			-DMF-	}*/

  integer  min;

  min = (sec * 0.15);
  sprintf(result,"%02ld:%02ld",hour,min);
  //insert_str(result," ","0");
  //insert_str(result," ","0");
  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char * place_string(integer num, string result)
{
  /*{ Return the ending to a number string (1st, 2nd, etc)	-DMF-	}*/
  switch (num) {
  case 1 : sprintf(result,"%ldst",num); break;
  case 2 : sprintf(result,"%ldnd",num); break;
  case 3 : sprintf(result,"%ldrd",num); break;
  default:
    if (num < 20) {
      sprintf(result,"%ldth",num);
    } else {
      switch (num % 10) {
      case 1 : sprintf(result,"%ldst",num); break;
      case 2 : sprintf(result,"%ldnd",num); break;
      case 3 : sprintf(result,"%ldrd",num); break;
      default: sprintf(result,"%ldth",num); break;
      } 
    } /* end second switch */
    break;
  } /* end first switch */

  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gain_level()
{
  /*{ Increases hit points and level			-RAK-	}*/

  integer    nhp,dif_exp,need_exp;
  boolean    redraw;
  vtype      out_val;

  nhp = get_hitdie();
  py.misc.mhp += nhp;
  py.misc.chp += nhp;
  if (py.misc.mhp < 1) {
    py.misc.mhp = 1;
    py.misc.chp = 1;
  }
  py.misc.lev++;
  need_exp = trunc(player_exp[py.misc.lev]*py.misc.expfact);
  if (py.misc.exp > need_exp) {
    dif_exp = py.misc.exp - need_exp;
    py.misc.exp = need_exp + (dif_exp div 2);
  }
  strcpy(py.misc.title, player_title[py.misc.pclass][py.misc.lev]);
  sprintf(out_val,"Welcome to level %d.",py.misc.lev);
  msg_print(out_val);
  msg_print(" ");
  msg_flag = false;
  prt_hp();
  prt_level();
  prt_title();
  if (PCLASS.mspell) {
    redraw = false;
    learn_spell(&redraw);
    if (redraw) {
      draw_cave();
    }
    gain_mana(spell_adj(INT));
    prt_mana();
  } else if (PCLASS.dspell) {
    learn_druid();
    gain_mana(druid_adj());
    prt_mana();
  } else if (PCLASS.bspell) {
    redraw = false;
    learn_song(&redraw);
    if (redraw) {
      draw_cave();
    }
    gain_mana(bard_adj());
    prt_mana();
  } else if (PCLASS.pspell) {
    learn_prayer();
    gain_mana(spell_adj(WIS));
    prt_mana();
  } else if (PCLASS.mental) {
    learn_discipline();
    gain_mana(monk_adj());
    prt_mana();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer get_hitdie()
{
  /*{ Calculates hit points for each level that is gained.	-RAK-	}*/
  integer    return_value;

  return_value = randint(py.misc.hitdie) + con_adj();

  return return_value;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean in_bounds(integer y,integer x)
{
  /*{ Checks a co-ordinate for in bounds status		-RAK-	}*/
  boolean    return_value;

  if ((y > 1) && (y < cur_height) &&
      (x > 1) && (x < cur_width)) {
    return_value = true;
  } else {
    return_value= false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
wordint  max_allowable_weight()
{
  wordint  return_value;

  if (characters_sex() == FEMALE) {
    return_value = race[py.misc.prace].f_b_wt +
      4*race[py.misc.prace].f_m_wt ;
  } else {
    return_value = race[py.misc.prace].m_b_wt +
      4*race[py.misc.prace].m_m_wt ;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
wordint  min_allowable_weight()
{
  wordint  return_value;

  if (characters_sex() == FEMALE) {
    return_value = race[py.misc.prace].f_b_wt -
      4*race[py.misc.prace].f_m_wt ;
  } else {
    return_value = race[py.misc.prace].m_b_wt -
      4*race[py.misc.prace].m_m_wt ;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean move_dir(integer dir, integer *y, integer *x) /* was move */
{
  /*{ Given direction 'dir', returns new row, column location -RAK- }*/
  
  integer    new_row,new_col;
  boolean    return_value = false;
  
  new_row = *y + dy_of[dir];
  new_col = *x + dx_of[dir];
  if ((new_row >= 1) && (new_row <= cur_height)) {
    if ((new_col >= 1) && (new_col <= cur_width)) {
      *y = new_row;
      *x = new_col;
      return_value = true;
    }
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void popm(integer *x)
{
  /*{ Returns a pointer to next free space			-RAK-	}*/

  if (mfptr < 1) {
    compact_monsters();

    validate_monsters();
  }

  *x = mfptr;
  mfptr = m_list[*x].nptr;
  m_list[*x].nptr = 0;

};

void pushm(integer x)
{
  /*{ Pushs a record back onto free space list		-RAK-	}*/

      m_list[x] = blank_monster;
      m_list[x].nptr = mfptr;
      mfptr = x;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void report_mlist_error(const char *err_msg, int error_node, int prev_node)
{
  (void)error_node;
  (void)prev_node;

  msg_print((char *)err_msg);
}

void validate_monsters()
{
  boolean	used_list[MAX_MALLOC+1];
  boolean       free_list[MAX_MALLOC+1];
  boolean       busted;
  int i1,i2;

  for (i1 = 0; i1 <= MAX_MALLOC; i1++ ) {
    used_list[i1] = false;
    free_list[i1] = false;
  }
  
  busted = false;
  for (i2=0,i1 = muptr ; i1 ; i2=i1, i1 = m_list[i1].nptr) {
    if ( used_list[i1] ) {
      /*
       * there is a loop in the monster list!
       */
      report_mlist_error("Internal Error: m_list has a loop!",
			 i1,i2);
      busted = true;
    } else {
      used_list[i1] = true;
    }
  }

  if ( busted ) {
    if ( i2 ) {
      m_list[i2].nptr = 0;
    } else {
      muptr = 0;
    }
  }


  busted = false;
  for (i2=0,i1 = mfptr ; i1 ; i2=i1, i1 = m_list[i1].nptr) {
    if ( free_list[i1] ) {
      /*
       * there is a loop in the free list
       */
      report_mlist_error("Internal Error: m_list has a free list loop!",
			 i1,i2);
      busted = true;
      break;
    } else {
      free_list[i1] = true;
    }

    if ( used_list[i1] ) {
      /*
       * the monster list and free list overlap!
       */
      report_mlist_error("Internal Error: m_list lists overlap!",
			 i1,i2);
      busted = true;
      break;
    } 
  }

  if ( busted ) {
    if ( i2 ) {
      m_list[i2].nptr = 0;
    } else {
      mfptr = 0;
    }
  }

}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void compact_monsters()
{
  integer   i1,i2,i3,ctr,cur_dis;
  boolean   delete_1,delete_any;

  validate_monsters();
  
  cur_dis = 66;
  delete_any = false;
  
  do {
    i1 = muptr;
    i2 = 0;
    do {
      delete_1 = false;
      i3 = m_list[i1].nptr;
      //with m_list[i1] do;
      if (cur_dis < m_list[i1].cdis) {
	if (randint(3) == 1) {
	  if (i2 == 0) {
	    muptr = i3;
	  } else {
	    m_list[i2].nptr = i3;
	  }
	  cave[m_list[i1].fy][m_list[i1].fx].cptr = 0;
	  m_list[i1]       =  blank_monster;
	  m_list[i1].nptr  =  mfptr;
	  mfptr            =  i1;
	  delete_1         =  true;
	  delete_any       =  true;
	  ctr++;
	}
      }
      if (!(delete_1)) {
	i2 = i1;
      }
      i1 = i3;
    } while (i1 != 0);
    if (!(delete_any)) {
      cur_dis -= 6;
      if ( cur_dis < 0 ) {
	cur_dis = 0; /* not really needed but it makes debugging happier */
      }
    }
  } while (!delete_any);
  
  if (cur_dis < 66) {
    prt_map();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void popt(integer *x)
{
  /*{ Gives pointer to next free space			-RAK-	}*/

  if (tcptr < 1) {
    compact_objects();
  }

  *x    = tcptr;
  tcptr = t_list[*x].p1;
};

void pusht(integer x) 
{
  /*{ Pushs a record back onto free space list		-RAK-	}*/

  t_list[x]    = blank_treasure;
  t_list[x].p1 = tcptr;
  tcptr = x;

}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void compact_objects()
{
  /*{ If too many objects on floor level, delete some of them-RAK-	}*/

  integer   i1,i2,ctr,cur_dis;
  boolean   flag;
  obj_set   fragile_stuff = {1,6,9,0}; /* open pit, loose rock, loose rock */

  ctr     = 0;
  cur_dis = 66;
  
  do {
    for (i1 = 1; i1 <= cur_height; i1++) {
      for (i2 = 1; i2 <= cur_width; i2++) {
	//with cave[i1][i2]. do;
	if (cave[i1][i2].tptr > 0) {
	  if (distance(i1,i2,char_row,char_col) > cur_dis) {
	    flag = false;
	    //with t_list[cave[i1][i2].tptr] do;
	    switch (t_list[cave[i1][i2].tptr].tval) {
	    case Seen_trap : 
	      if (is_in(t_list[cave[i1][i2].tptr].subval, fragile_stuff)) {
		flag = true;
	      } else if (randint(4) == 1) {
		flag = true;
	      }
	      break;
	      
	    case rubble : 
	      flag = true;
	      break;
	      
	    case open_door: 
	    case closed_door:
	      if (randint(8) == 1) {
		flag = true;
	      }
	      break;
	      
	    case up_staircase:
	    case down_staircase:
	    case up_steep_staircase:
	    case down_steep_staircase:
	    case entrance_to_store : 
	      break;
	      
	    default:
	      if (randint(8) == 1) {
		flag = true;
	      }
	      break;
	    }
	    
	    if (flag) {
	      cave[i1][i2].fopen = true;
	      t_list[cave[i1][i2].tptr] = blank_treasure;
	      t_list[cave[i1][i2].tptr].p1 = tcptr;
	      tcptr = cave[i1][i2].tptr;
	      cave[i1][i2].tptr = 0;
	      ctr++;
	    }
	  }
          if (ctr == 0) {
	    cur_dis -= 6;
	  }
	}
      }
    }
  } while (ctr <= 0);
	
  if (cur_dis < 66) {
    prt_map();
  }
  
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean test_light(integer y,integer x)
{
/*{ Tests a spot for light or field mark status		-RAK-	}*/
  return ((cave[y][x].pl) || (cave[y][x].fm) || (cave[y][x].tl));
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer distance(integer y1,integer x1,integer y2,integer x2)
{
  register int dy, dx;

/*
	;	Distance returned is only an approximation based on :
	;
	;	dy = abs(y1-y2)
	;	dx = abs(x1-x2)
	;
	;	distance =  2*(dy+dx) - MIN(dy,dx)
	;		    ----------------------
	;			      2
*/
  dy = y1 - y2;
  if (dy < 0)
    dy = -dy;
  dx = x1 - x2;
  if (dx < 0)
    dx = -dx;

  return ((((dy + dx) << 1) - (dy > dx ? dx : dy)) >> 1);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean los(integer y1,integer x1,integer y2,integer x2)
{
  /*{ Returns true if no obstructions between two given points -RAK-}*/

  integer    ty,tx,stepy,stepx,p1,p2;
  real       slp,tmp;
  boolean    flag = true;

//  ENTER("los", "m")
//  fprintf(debug_file,"los: (%d, %d) to (%d, %d)\n",x1,y1,x2,y2);

  ty = (y1 - y2);
  tx = (x1 - x2);

//  fprintf(debug_file,"los: ty = %d   tx = %d\n",ty,tx);
  
  if ((ty != 0) || (tx != 0)) {
    if (ty < 0) {
      stepy = -1;
    } else {
      stepy = 1;
    }
    if (tx < 0) {
      stepx = -1;
    } else {
      stepx = 1;
    }
    if (ty == 0) {

//      fprintf(debug_file,"los: ty==0  stepx = %d\n",stepx);
      do {
	x2 += stepx;
//	fprintf(debug_file,"los: checking y=[%d]  x=[%d]  %d\n",
//		y2,x2,cave[y2][x2].fopen);
	flag = cave[y2][x2].fopen;
      } while (!((x1 == x2) || !flag));

    } else if (tx == 0) {

//      fprintf(debug_file,"los: tx==0  stepy = %d\n",stepy);
      do {
	y2 += stepy;
//	fprintf(debug_file,"los: checking y=[%d]  x=[%d]  %d\n",
//		y2,x2,cave[y2][x2].fopen);
	flag = cave[y2][x2].fopen;
      } while (!((y1 == y2) || !flag));

    } else if (abs(ty) > abs(tx)) {

//      fprintf(debug_file,"los: ty>tx  stepy = %d\n",stepy);
      slp = ((real)abs(tx)/(real)abs(ty))*stepx;
      tmp = x2;
      do {
	y2  += stepy;
	tmp += slp;
	p1 = tmp - 0.1 + .5;
	p2 = tmp + 0.1 + .5;
//	fprintf(debug_file,"los: checking y=[%d]  x=[%d]  x=[%d]  %d : %d\n",
//		y2,p1,p2,cave[y2][p1].fopen,cave[y2][p2].fopen);
	if (! ((cave[y2][p1].fopen) || (cave[y2][p2].fopen))) {
//	  fprintf(debug_file,"los: setting ty>tx false\n");
	  flag = false;
	}
      } while (!((y1 == y2) || !flag));

    } else {

//      fprintf(debug_file,"los: tx>ty  stepx = %d\n",stepx);
      slp = ((real)abs(ty)/(real)abs(tx))*stepy;
      tmp = y2;
      do {
	x2  += stepx;
	tmp += slp;
	p1 = tmp - 0.1 + .5;
	p2 = tmp + 0.1 + .5;
//	fprintf(debug_file,"los: checking y=[%d]  y=[%d]  x=[%d]  %d : %d\n",
//		p1,p2,x2,cave[p1][x2].fopen,cave[p2][x2].fopen);
	if (! ((cave[p1][x2].fopen) || (cave[p2][x2].fopen))) {
//	  fprintf(debug_file,"los: setting tx>ty false\n");
	  flag = false;
	}
      } while (!((x1 == x2) || !flag));

    }
  }
  
//  RETURN("los", "m", 'b', "have los: ",&flag);
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tlink()
{
  /*{ Link all free space in treasure list together			}*/

  integer   i1;

  for (i1 = 0; i1 <= MAX_TALLOC; i1++) {
    t_list[i1]     = blank_treasure;
    t_list[i1].p1  = i1 - 1;
  }

  tcptr = MAX_TALLOC;

};

void mlink()
{
  /*{ Link all free space in monster list together			}*/

  integer   i1;
  
  for (i1 = 0; i1 <= MAX_MALLOC; i1++) {
    m_list[i1]       = blank_monster;
    m_list[i1].nptr  = i1 - 1;
  }
  
  m_list[2].nptr = 0;  /* extra space saved for the win creatures? */
  muptr = 0;
  mfptr = MAX_MALLOC;
  
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_gold(integer y,integer x)
{
  /*{ Places a treasure (Gold or Gems) at given row, column -RAK-	}*/

  integer    cur_pos,i1;

  popt(&cur_pos);
  i1 = (2+randint(dun_level+4)+randint(dun_level+4)) div 4;
  if (randint(obj_great) == 1) {
    i1 += randint(dun_level);
  }
  if (i1 > MAX_GOLD) {
    i1 = MAX_GOLD + 1 - randint(randint(3));
  }

  cave[y][x].tptr = cur_pos;
  t_list[cur_pos] = gold_list[i1-1];
  
  if (t_list[cur_pos].tval == valuable_metal) {
    t_list[cur_pos].number = randint(t_list[cur_pos].number) +
                             t_list[cur_pos].number div 2;
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_a_staircase(integer y,integer x,integer typ)
{
  /*{ Place an up staircase at given y,x			-RAK-	}*/
  
  integer   cur_pos;
  
  if (cave[y][x].tptr != 0) {
    pusht(cave[y][x].tptr);
    cave[y][x].tptr  = 0;
    cave[y][x].fopen = true;
  }
  
  popt(&cur_pos);
  cave[y][x].tptr = cur_pos;
  switch (typ) {
  case up_staircase :         t_list[cur_pos] = up_stair;    break;
  case down_staircase :       t_list[cur_pos] = down_stair;  break;
  case up_steep_staircase :   t_list[cur_pos] = up_steep;    break;
  case down_steep_staircase : t_list[cur_pos] = down_steep;  break;
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_stairs(integer typ,integer num,integer walls)
{
  /*{ Places a staircase 1=up, 2=down			-RAK-	}*/

  integer   i1,i2,y1,x1,y2,x2;
  boolean   flag;
  
  for (i1 = 0; i1 < num; i1++) {
    flag = false;
    do {
      i2 = 0;
      do {
	y1 = randint(cur_height - 12);
	x1 = randint(cur_width  - 12);
	y2 = y1 + 12;
	x2 = x1 + 12;
	do {
	  do {
	    if (is_in(cave[y1][x1].fval, open_dry_floors)) {
	      if (cave[y1][x1].tptr == 0) {
		if (next_to4(y1,x1,wall_set) >= walls) {
		  flag = true;
		  place_a_staircase(y1,x1,typ);
		}
	      }
	    }
	    x1++;
	  } while (!((x1 == x2) || (flag)));
	  x1 = x2 - 12;
	  y1++;
	} while (!((y1 == y2) || (flag)));
	i2++;
      } while (!((flag) || (i2 > 30)));
      walls--;
    } while (!flag);
  } /* end for */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer get_obj_num(integer level, integer tries)
{
  /*{ Returns the array number of a random object		-RAK-	}*/

  /* pick an object tries times, keep the one with the highest level */
  /* here are the comments from umoria:                              */

      /* This code has been added to make it slightly more likely to get the
	 higher level objects.	Originally a uniform distribution over all
	 objects less than or equal to the dungeon level.  This distribution
	 makes a level n objects occur approx 2/n% of the time on level n,
	 and 1/2n are 0th level. */

  integer   i1, i2, i3, i4;

  if (level > MAX_OBJ_LEVEL) {
    level = MAX_OBJ_LEVEL;
  }

  if (randint(obj_great) == 1) {
    level = MAX_OBJ_LEVEL;
  }

  i4 = 1;
  i3 = i2 = 0;
  do {
    if (level == 0) {
      i1 = randint(t_level[0]); 
      if (object_list[i1].level >= i3) {
	i4 = i1;
	i3 = object_list[i1].level;
      }
    } else {
      i1 = randint(t_level[level]);
      if (object_list[i1].level >= i3) {
	i4 = i1;
	i3 = object_list[i1].level;
      }
    }
  } while (++i2 < tries);

  return i4;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void place_object(integer y,integer x)
{
  /*{ Places an object at given row, column co-ordinate	-RAK-	}*/
  integer   cur_pos;

  popt(&cur_pos);
  cave[y][x].tptr = cur_pos;
  t_list[cur_pos] = object_list[get_obj_num(dun_level,PLACE_OBJECT_TRIES)];
  magic_treasure(cur_pos,dun_level,false);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer next_to4(integer y,integer x, obj_set group_set)
{
  /*{ Checks points north, south, east, and west for a type -RAK-	}*/

  integer i1;

  i1 = 0;

  if (y > 1) {
    if (is_in(cave[y-1][x].fval, group_set)) {
      i1++;
    }
  }
  
  if (y < cur_height) {
    if (is_in(cave[y+1][x].fval, group_set)) {
      i1++;
    }
  }

  if (x > 1) {
    if (is_in(cave[y][x-1].fval, group_set)) {
      i1++;
    }
  }

  if (x < cur_width) {
    if (is_in(cave[y][x+1].fval, group_set)) {
      i1++;
    }
  }

  return i1;
};

integer next_to8(integer y,integer x,obj_set group_set)
{
  /*{ Checks all adjacent spots for elements		-RAK-	*/

  integer   i1,i2,i3;
  
  i1 = 0;
  for (i2 = (y - 1); i2 <= (y + 1); i2++) {
    for (i3 = (x - 1); i3 <= (x + 1); i3++) {
      if (in_bounds(i2,i3)) {
	if (is_in(cave[i2][i3].fval, group_set)) {
	  i1++;
	}
      }
    }
  }

  return i1;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer max_hp(dtype hp_str)
{
  /*{ Gives Max hit points					-RAK-	}*/

  integer   i1,num=0,die=0;
  dtype     hp_copy;
  integer   return_value;

  strcpy(hp_copy, hp_str);

  for (i1 = 0; i1 < strlen(hp_copy); i1++) {
    if (hp_copy[i1] == 'd') {
	  hp_copy[i1] = ' ';
    }
  }

  sscanf(hp_copy,"%ld %ld",&num,&die);
  return_value = num*die;

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer damroll(dtype dice)
{
  /*{ Converts input string into a dice roll		-RAK-	}*/
  /*{	Normal input string will look like '2d6', '3d8'... etc. }*/

  integer   i1;
  dtype     dice_copy;
  integer   num = 0, sides = 0;
  integer   return_value = 0;

  ENTER("damroll", "m")

  strcpy(dice_copy, dice);

  for (i1 = 0; i1 < strlen(dice_copy); i1++) {
    if (dice_copy[i1] == 'd') {
	  dice_copy[i1] = ' ';
    }
  }

  sscanf(dice_copy,"%ld %ld",&num,&sides);
  return_value = rand_rep(num,sides);
  
  RETURN("damroll", "m", 'd',"damage",&return_value)
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define max(xx,yy) (((xx)>(yy)) ? (xx) : (yy))
#define min(xx,yy) (((xx)<(yy)) ? (xx) : (yy))

integer maxmin(integer x, integer y, integer z)
{
  /* return max( min(x,y) - 1, z ) */
  integer  i1;
  
  i1 = min(x,y) - 1;
  return max(i1, z);
};

integer minmax(integer x, integer y, integer z)
{
  /* return min( max(x,y) + 1, z ) */
  integer  i1;

  i1 = max(x,y) + 1;
  return min(i1, z);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean summon_land_monster(integer *y, integer *x,boolean slp)
{
  /*{ Places land creature adjacent to given location	-RAK-	}*/

  integer   i1,i2,i3,i4,i5,count;
  boolean   flag;
  boolean   return_value = false;

  i1 = 0;
  i5 = dun_level + MON_SUMMON_ADJ;
  
  do {
    i2 = *y - 2 + randint(3);
    i3 = *x - 2 + randint(3);
    if (in_bounds(i2,i3)) {
      //with cave[i2][i3]. do;
      if (is_in(cave[i2][i3].fval, earth_set)) {
	if (cave[i2][i3].cptr == 0) {
	  if (cave[i2][i3].fopen) {
	    flag  = false;
	    count = 0;
	    do {
	      if (i5 > MAX_MONS_LEVEL) {
		i4 = MAX_MONS_LEVEL;
	      } else {
		i4 = i5;
	      }
	      if (dun_level == 0) {
		i4 = randint(m_level[0]);
	      } else {
		i4 = randint(m_level[i4]) + m_level[0];
	      }
	      if (i4 > MAX_CREATURES) {
		i4 = MAX_CREATURES;
	      }
	      if (((c_list[i4].cmove &  0x00008000) == 0) &&
		  (((c_list[i4].cmove & 0x00000010) == 0) ||
		   ((c_list[i4].cmove & 0x00000040) == 0) ||
		   ((c_list[i4].cmove & 0x00800000) != 0))) {
		place_monster(i2,i3,i4,slp);
		return_value = true;
		flag = true;
	      }
	      count++;
	    } while (!((flag) || (count > 10)));
	    i1  = 9;
	    *y  = i2;
	    *x  = i3;
	  }
	}
      }
    }
    i1++;
  } while (i1 <= 9);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean summon_water_monster(integer *y, integer *x,boolean slp)
{
  /*{ Places water creature adjacent to given location  -DMF-   }*/

  integer   i1,i2,i3,i4,i5,count;
  boolean   flag;
  boolean   return_value = false;

  i1 = 0;
  i5 = dun_level + MON_SUMMON_ADJ;
  
  do {
    i2 = *y - 2 + randint(3);
    i3 = *x - 2 + randint(3);
    if (in_bounds(i2,i3)) {
      //with cave[i2][i3]. do;
      if (is_in(cave[i2][i3].fval, water_set)) {
	if (cave[i2][i3].cptr == 0) {
	  if (cave[i2][i3].fopen) {
	    flag  = false;
	    count = 0;
	    do {
	      if (i5 > MAX_MONS_LEVEL) {
		i4 = MAX_MONS_LEVEL;
	      } else {
		i4 = i5;
	      }
	      if (dun_level == 0) {
		i4 = randint(m_level[0]);
	      } else {
		i4 = randint(m_level[i4]) + m_level[0];
	      }
	      if (i4 > MAX_CREATURES) {
		i4 = MAX_CREATURES;
	      }
	      if ((((c_list[i4].cmove & 0x00008000) == 0) &&
		   (((c_list[i4].cmove & 0x00000010) != 0) ||
		    ((c_list[i4].cmove & 0x00000040) == 0) ||
		    ((c_list[i4].cmove & 0x00800000) != 0)))) {
		place_monster(i2,i3,i4,slp);
		return_value = true;
		flag = true;
	      }
	      count++;
	    } while (!((flag) || (count > 10)));
       	    i1  = 9;
	    *y  = i2;
	    *x  = i3;
	  }
	}
      }
    }
    i1++;
  } while (i1 <= 9);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean summon_undead(integer *y,integer *x)
{
  /*{ Places undead adjacent to given location          -RAK-   }*/

  integer   i1,i2,i3,i4,i5,ctr;
  obj_set   undead_set = {1,2,4,5,0};
  boolean   return_value = false;
  
  i1 = 0;
  i4 = m_level[MAX_MONS_LEVEL] + m_level[0];
  
  do {
    i5  = randint(i4);
    ctr = 0;
    do {
      if ((c_list[i5].cdefense & 0x0008) != 0) {
	ctr = 20;
	i4  = 0;
      } else {
	i5++;
	if (i5 > i4) {
	  ctr = 20;
	} else {
	  ctr++;
	}
      }
    } while (ctr <= 19);
  } while (i4 != 0);
  
  do {
    i2 = *y - 2 + randint(3);
    i3 = *x - 2 + randint(3);
    if (in_bounds(i2,i3)) {
      //with cave[i2,i3] do;
      if (is_in(cave[i2][i3].fval , undead_set)) {
	if ((cave[i2][i3].cptr == 0) && (cave[i2][i3].fopen)) {
	  place_monster(i2,i3,i5,false);
	  return_value = true;
	  i1 = 9;
	  *y = i2;
	  *x = i3;
	}
      }
    }
    i1++;
  } while (i1 <= 9);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean summon_demon(integer *y,integer *x)
{
  /*{ Places demon adjacent to given location           -RAK-   }*/

  integer   i1,i2,i3,i4,i5,ctr;
  obj_set   demon_set = {1,2,4,5,0};
  boolean   return_value = false;

  i1 = 0;
  i4 = m_level[MAX_MONS_LEVEL] + m_level[0];
  do {
    i5  = randint(i4);
    ctr = 0;
    do {
      /*{        Check monsters for demon }*/
      if ((c_list[i5].cdefense & 0x0400) != 0) {
	ctr = 20;
	i4  = 0;
      } else {
	i5++;
	if (i5 > i4) {
	  ctr = 20;
	} else {
	  ctr++;
	}
      }
    } while (ctr <= 19);
  } while (i4 != 0);
  
  do {
    do {
      i2 = *y - 2 + randint(3);
      i3 = *x - 2 + randint(3);
    } while (!((i2 != *y) || (i3 != *x)));
    if (in_bounds(i2,i3)) {
      //with cave[i2,i3] do;
      if (is_in(cave[i2][i3].fval, demon_set)) {
	if ((cave[i2][i3].cptr == 0) && (cave[i2][i3].fopen)) {
	  place_monster(i2,i3,i5,false);
	  return_value = true;
	  i1 = 9;
	  *y = i2;
	  *x = i3;
	}
      }
    }
    i1++;
  } while (i1 <= 9);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean summon_breed(integer *y,integer *x)
{
  /*{ Places breeding monster adjacent to given location }*/

  integer   i1,i2,i3,i4,i5,ctr;
//  obj_set   breed_set = {1,2,4,5,0};
  boolean   return_value = false;

  i1 = 0;
  do {
    i2 = *y - 2 + randint(3);
    i3 = *x - 2 + randint(3);
    if (in_bounds(i2,i3)) {
      //with cave[i2,i3] do;
      if ((is_in(cave[i2][i3].fval, earth_set)) ||
	  (is_in(cave[i2][i3].fval, water_set))) {
	if ((cave[i2][i3].cptr == 0) && (cave[i2][i3].fopen)) {
	  i4 = m_level[MAX_MONS_LEVEL] + m_level[0];
	  do {
	    i5  = randint(i4);
	    ctr = 0;
	    do {
	      if (((c_list[i5].cmove & 0x00200000) != 0) &&
		  (((is_in(cave[i2][i3].fval, earth_set)) &&
		    (((c_list[i5].cmove & 0x00000010) == 0) ||
		    ((c_list[i5].cmove & 0x00000040) == 0) ||
		    ((c_list[i5].cmove & 0x00800000) != 0))) ||
		   ((is_in(cave[i2][i3].fval, water_set)) &&
		    (((c_list[i5].cmove & 0x00000010) != 0) ||
		    ((c_list[i5].cmove & 0x00000040) == 0) ||
		    ((c_list[i5].cmove & 0x00800000) != 0))))) {
		ctr = 20;
		i4  = 0;
	      } else {
		i5++;
		if (i5 > i4) {
		  ctr = 20;
		} else {
		  ctr++;
		}
	      }
	    } while (ctr <= 19);
	  } while (i4 != 0);
	  place_monster(i2,i3,i5,false);
	  return_value = true;
	  i1 = 9;
	  *y = i2;
	  *x = i3;
	}
      }
    }
    i1++;
  } while (i1 <= 9);

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean player_saves(integer adjust)
{
  /*{ Saving throws for player character... 		-RAK-	}*/
  boolean  return_value;
  return_value = (randint(100) <= py.misc.save + adjust) && (randint(20) != 1);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean player_spell_saves()
{
  boolean  return_value;
  return_value = player_saves(py.misc.lev + 5 * spell_adj(WIS));
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sp__takey_munny(integer coin_value, integer *bank_assets,
		 integer *to_bank, integer *from_bank)
{
  integer   trans;

  trans = (*to_bank * GOLD_VALUE) / coin_value;
  if (*bank_assets < trans) { 
    trans = *bank_assets;
  }
  *bank_assets     -= trans;
  *from_bank       += (trans * coin_value) / GOLD_VALUE;
  *to_bank         -= (trans * coin_value) / GOLD_VALUE;
  py.misc.account  -= (trans * coin_value) / GOLD_VALUE;
};
//////////////////////////////////////////////////////////////////////
boolean send_page(integer to_bank)
{
  /*{ Send a page to the bank to fetch money		-DMF-	}*/

  boolean   back;
  integer   from_bank;
  string    out_val;

  back = false;
  if (get_yes_no("Do you wish to send a page to the bank for money?")) {
    from_bank = 0;
    if (py.misc.account < to_bank) {
      msg_print("The page returns and says that your balance is too low.");
    } else if (bank[TOTAL_] < to_bank) {
      msg_print("The page returns and says that the bank is out of money.");
    } else {
      sp__takey_munny(coin_value[MITHRIL],&bank[MITHRIL],&to_bank,&from_bank);
      sp__takey_munny(coin_value[PLATINUM],&bank[PLATINUM],&to_bank,&from_bank);
      sp__takey_munny(GOLD_VALUE,&bank[GOLD],&to_bank,&from_bank);
      if (randint(mugging_chance) == 1) {
	msg_print("The page was mugged while returning from the bank!");
	sprintf(out_val,"You have lost %ld gold pieces!",from_bank);
	msg_print(out_val);
      } else {
	sprintf(out_val,"The page returns with %ld gold pieces.",from_bank);
	msg_print(out_val);
	subtract_money(py.misc.money[TOTAL_] * GOLD_VALUE,false);
	back = true;
      }
    }
    msg_print(" ");
  } else {
    msg_print("You cannot buy that with the money you are carrying.");
  }
  
  return back;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void time_diff(game_time_type a,game_time_type b,game_time_type *c)
{
  /*{ Return the difference of two time records             -DMF-   }*/

  if (a.secs < b.secs) {
    a.secs = a.secs + 400;
    a.hour = a.hour - 1;
  }
  c->secs = a.secs - b.secs;

  if (a.hour < b.hour) {
    a.hour = a.hour + 24;
    a.day  = a.day - 1;
  }
  c->hour = a.hour - b.hour;

  if (a.day < b.day) {
    a.day   = a.day + 28;
    a.month = a.month - 1;
  }
  c->day = a.day - b.day;

  if (a.month < b.month) {
    a.month = a.month + 13;
    a.year  = a.year - 1;
  }
  c->month = a.month - b.month;

  c->year = a.year - b.year;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *show_char_age(string result)
{
  /*{ Return string for the age of the character            -DMF-   }*/

  game_time_type   dif;
  vtype            out_val;

  time_diff(py.misc.cur_age,py.misc.birth,&dif);

  sprintf(result,"You are %ld years, %d months, %d days, and %s hours old.",
	 dif.year, dif.month, dif.day, time_string(dif.hour,dif.secs,out_val));

  if (dif.year == 1) {
    insert_str(result,"years","year");
  }
  if (dif.month == 1) {
    insert_str(result,"months","month");
  }
  if (dif.day == 1) {
    insert_str(result,"days","day");
  }

  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *play_time(time_type *t, char *result)
{
  /*{ Return string for how long character has been playing -DMF-   }*/

  sprintf(result, "%d day%s and %d:%02d:%02d hours.",
	  t->days, ((t->days == 1) ? "" : "s"), t->hours,
	  t->minutes, t->seconds);

  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/*
        { Add two time_types together                           -DMF-   }
[global,psect(misc5$code)] procedure add_play_time(
                var res : time_type;
                add     : time_type);
      begin
        with res do
          begin
            days := days + add.days;
            hours := hours + add.hours;
            minutes := minutes + add.minutes;
            seconds := seconds + add.seconds;
            hundredths := hundredths + add.hundredths;
            if hundredths > 100 then
              begin
                hundredths := hundredths - 100;
                seconds := seconds + 1;
              end;
            if seconds > 60 then
              begin
                seconds := seconds - 60;
                minutes := minutes + 1;
              end;
            if minutes > 60 then
              begin
                minutes := minutes - 60;
                hours := hours + 1;
              end;
            if hours > 24 then
              begin
                hours :=  hours - 24;
                days := days + 1;
              end;
          end;
      end;
*/
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
time_type *convert_seconds_to_time(time_t seconds,time_type *tim)
{
  tim->years       = 0;
  tim->months      = 0;
  tim->hundredths  = 0;

  tim->days        = seconds / (60 * 60 * 24);
  seconds         -= tim->days * (60 * 60 * 24);

  tim->hours       = seconds / (60 * 60);
  seconds         -= tim->hours * (60 * 60);

  tim->minutes     = seconds / (60);
  seconds         -= tim->minutes * (60);

  tim->seconds     = seconds;

  return tim;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
time_t convert_time_to_seconds(time_type *tim)
{
  time_t t;

  t  = (tim->days * (60*60*24) + tim->hours * (60*60) + 
	tim->minutes * (60) + tim->seconds);

  return t;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *show_play_time(string result)
{
  /*{ Return string for amount of play time                 -DMF-   }*/

  time_type   tim;

//  strcpy(result, "way to long.");
    
  play_time(convert_seconds_to_time(time(NULL) - start_time + 
				    convert_time_to_seconds(&py.misc.play_tm),
				    &tim), result);

  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *full_date_string(game_time_type time, string result)
{
  /*{ Return string with entire date/time                   -DMF-   }*/
  
  string  out1,out2,out3,out4;
  char   *pos;

  day_of_week_string(time.day,10,out1);
  pos = strstr(out1," ");
  if (pos != NULL) {
    *pos = 0;
  }
  //with time do;
  sprintf(result,"%s, %s the %s, %s",out1,month_string(time.month,out2),
	  place_string(time.day,out3),time_string(time.hour,time.secs,out4));

  return result;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *show_current_time(char *result)
{
  /*{ Return current time in the game                       -DMF-   }*/
  /* Tue Jul 07 00:05:40 1998 */

  //quad_type    current_time;
  //vtype        out;

  time_t         cur_time;
  struct tm      split_time;

  cur_time   = time(NULL);
  split_time = *localtime(&cur_time);
  strftime(result, 70, "%c", &split_time);

  return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer rotate_dir(integer dir,integer rot)
{
  integer return_value;

//  ENTER("rotate_dir", "m")
  if (dir == 5) {
    return_value = 5;
  } else {
    for ( ; rot < 0 ; rot += 8); /* make sure the mod is a positive number */
    return_value = key_of[(oct_of[dir] + rot) % 8];
  }

//  RETURN("rotate_dir", "m", 'd',"dir",&return_value)
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer attack_blows(integer weight, integer *wtohit)
{
  /*{ Weapon weight VS strength and dexterity               -RAK-   }*/

  integer   max_wield,adj_weight,blows,lev_skill;
  integer   a_dex;

  blows   = 1;
  *wtohit = 0;

  //with py.stat do;
  max_wield = weight_limit() / 10;           
  if (max_wield < (weight div 100)) {
    /*{ make to-hit drop off gradually instead of being so abrupt  -DCJ-}*/
    *wtohit = max_wield - (weight div 100);
  } else {
    a_dex = PS.c[DEX];

    if      (a_dex <  70) { blows = 3; }
    else if (a_dex < 150) { blows = 4; }
    else if (a_dex < 151) { blows = 5; }
    else if (a_dex < 200) { blows = 6; }
    else if (a_dex < 220) { blows = 7; }
    else if (a_dex < 240) { blows = 8; }
    else if (a_dex < 250) { blows = 10;}
    else                  { blows = 12;}

    lev_skill = class[py.misc.pclass].mbth*(py.misc.lev+10);

    /*{warriors 100-500, paladin 80-400, priest 60-300, mage 40-200}*/
    blows = trunc(0.8 + (real)blows/3.0 + (real)lev_skill/350.0);

    /*{usually 3 for 18+ dex, 5 max except 6 for high level warriors}*/
    adj_weight = (integer)((real)PS.c[STR]/(real)(weight div 100)*2.5);

    if (adj_weight < 1)      { blows = 1;           }
    else if (adj_weight < 2) { blows = (blows/3.00);}
    else if (adj_weight <3)  { blows = (blows/2.50);}
    else if (adj_weight <5)  { blows = (blows/2.00);}
    else if (adj_weight <10) { blows = (blows/1.66);}
    else                     { blows = (blows/1.50);}

  }

  return blows;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer critical_blow(integer weight,integer plus,boolean cs_sharp,
		      boolean is_fired)
{
  /*{ Critical hits, Nasty way to die...                    -RAK-   }*/

  integer   randomthing, py_crit;
  integer   return_value = 0;

  weight = weight div 100;
  if (cs_sharp) {
    weight += 600;
  }

  /*{ Weight of weapon, pluses to hit, and character level all      }*/
  /*{ contribute to the chance of a critical                        }*/

  //with py.misc do;
  if (is_fired) {
    py_crit = class[pclass].mbthb;
  } else {
    py_crit = class[pclass].mbth;

    if (pclass == C_MONK) {  /*{ monks are crit specialists }*/
      py_crit *= 2;
    }
  }

  if (randint(5000) <= (weight+6*plus+py_crit*(PM.lev+10))) {
    randomthing = randint(300 + randint(weight));
    if (randomthing <= 150) {
      return_value = 1;
      msg_print("It was a good hit! (x2 damage)");
    } else if (randomthing <= 250) {
      return_value = 2;
      msg_print("It was an excellent hit! (x3 damage)");
    } else if (randomthing <= 375) {
      return_value = 3;
      msg_print("It was a superb hit! (x4 damage)");
    } else if (randomthing <= 550) {
      return_value = 4;
      msg_print("It was a *GREAT* hit! (x5 damage)");
    } else if (randomthing < 700) {
      return_value = 6;
      msg_print("It was an *INCREDIBLE* hit! (x7 damage)");
    } else if (randomthing < 875) {
      return_value = 9;
      msg_print("It was an *AMAZING* hit! (x10 damage)");
    } else {
      return_value = 14;
      msg_print("It was a **PERFECT** hit! (x15 damage)");
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void find_monster_name(vtype m_name,integer ptr,boolean	begin_sentence)
{
  integer   i2;

  i2 = m_list[ptr].mptr;

  /*{ Does the player know what he's fighting?      }*/
  if (((uand(0x10000,c_list[i2].cmove) != 0) &&
       (!(py.flags.see_inv))) || (py.flags.blind > 0) ||
      (!(m_list[ptr].ml))) {
    if (begin_sentence) {
      strcpy(m_name, "It");
    } else {
      strcpy(m_name, "it");
    }
  } else {
    if (begin_sentence) {
      sprintf(m_name, "The %s", c_list[i2].name);
    } else {
      sprintf(m_name, "the %s", c_list[i2].name);
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer get_hexdecant(integer dy,integer dx)
{
  /*{ Returns hexdecant of dy,dx                    }*/
  /*{ 0,1 = ea 2,3 = ne, 4,5 = n ... 14,15 = se     }*/

  integer    ay,ax;
  integer    hexdecant;
  integer    return_value;

  ENTER("get_hexdecant", "m")

  ay = abs(dy); 
  ax = abs(dx);

  if (ay*2.41421 < ax)         { hexdecant = 1; }
  else if (ay < ax)            { hexdecant = 2; }
  else if (ay/2.41421 < ax)    { hexdecant = 3; }
  else                         { hexdecant = 4; }

  if (dx < 0)                  { hexdecant = 9 - hexdecant; }

  if (dy > 0)                  { return_value = (17 - hexdecant) % 16; }
  else                         { return_value = hexdecant; }

  RETURN("get_hexdecant", "m",'d',"dir",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void petrify(integer amt)
{
  PF.petrification += randint(amt);

  if (PF.petrification < 100) {
    msg_print("You feel your joints stiffening.");
  } else if (PF.petrification < 150) {
    msg_print("Your feet are beginning to feel heavy.");
  } else if (PF.petrification < 200) {
    msg_print("Your knees are no longer able to bend.");
  } else if (PF.petrification < 250) {
    msg_print("Your legs feel like blocks of stone.");
  } else if (PF.petrification < 300) {
    msg_print("You are finding it difficult to breathe.");
  } else {
    msg_print("You have turned to stone.");
    strcpy(died_from,"petrification");
    upon_death();
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_win_monster()
{
  /*{ Places a monster at given location                    -RAK-   }*/

  integer   cur_pos;
  integer   y,x;

  if (!total_winner) {
    popm(&cur_pos);
    //with m_list[cur_pos] do;
    do {
      y = randint(cur_height-2)+1;
      x = randint(cur_width-2)+1;
    } while (!((is_in(cave[y][x].fval, open_dry_floors))     &&
	       (cave[y][x].cptr == 0)             &&
	       (cave[y][x].tptr == 0)             &&
	       (distance(y,x,char_row,char_col) > MAX_SIGHT)));
    
    m_list[cur_pos].fy = y;
    m_list[cur_pos].fx = x;

    m_list[cur_pos].mptr  = (randint(WIN_MON_TOT) + 
			     m_level[MAX_MONS_LEVEL] + m_level[0]);
    m_list[cur_pos].nptr  = muptr;
    muptr = cur_pos;

    if (uand(c_list[m_list[cur_pos].mptr].cdefense,0x4000) != 0) {
      m_list[cur_pos].hp = max_hp(c_list[m_list[cur_pos].mptr].hd);
    } else {
      m_list[cur_pos].hp = damroll(c_list[m_list[cur_pos].mptr].hd);
    }

    m_list[cur_pos].cdis    = distance(char_row,char_col,y,x);
    m_list[cur_pos].cspeed  = c_list[m_list[cur_pos].mptr].speed + PF.speed;
    m_list[cur_pos].stunned = 0;
    m_list[cur_pos].csleep  = 0;
    cave[y][x].cptr = cur_pos;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void alloc_object(obj_set alloc_set, integer typ, integer num)
{
  /*{ Allocates an object for tunnels and rooms             -RAK-   }*/

  integer   i1,i2,i3;

  for (i3 = 1; i3 <= num; i3++) {
    do {
      i1 = randint(cur_height);
      i2 = randint(cur_width);
    } while (!((is_in(cave[i1][i2].fval, alloc_set)) &&
	       (cave[i1][i2].tptr == 0)));

    switch (typ) {
    case 1 : place_trap(i1,i2,1,randint(MAX_TRAPA));   break;
    case 2 : place_trap(i1,i2,2,randint(MAX_TRAPB));   break;
    case 3 : place_rubble(i1,i2);                      break;
    case 4 : place_gold(i1,i2);                        break;
    case 5 : place_object(i1,i2);                      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_open_door(integer y,integer x)
{
  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr   =  cur_pos;
  t_list[cur_pos]   =  door_list[0];
  cave[y][x].fval   =  corr_floor3.ftval;
  cave[y][x].fopen  =  true;

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_broken_door(integer y,integer x)
{
  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr    =  cur_pos;
  t_list[cur_pos]    =  door_list[0];
  cave[y][x].fval    =  corr_floor3.ftval;
  cave[y][x].fopen   =  true;
  t_list[cur_pos].p1 =  1;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_closed_door(integer y,integer x)
{
  integer   cur_pos;

   popt(&cur_pos);

  cave[y][x].tptr   =  cur_pos;
  t_list[cur_pos]   =  door_list[1];
  cave[y][x].fval   =  corr_floor3.ftval;
  cave[y][x].fopen  =  false;

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_locked_door(integer y,integer x)
{
  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr    =  cur_pos;
  t_list[cur_pos]    =  door_list[1];
  cave[y][x].fval    =  corr_floor3.ftval;
  cave[y][x].fopen   =  false;
  t_list[cur_pos].p1 =  randint(10) + 10;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_stuck_door(integer y,integer x)
{
  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr    =  cur_pos;
  t_list[cur_pos]    =  door_list[1];
  cave[y][x].fval    =  corr_floor3.ftval;
  cave[y][x].fopen   =  false;
  t_list[cur_pos].p1 = -randint(10) - 10;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_secret_door(integer y,integer x)
{
  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr   =  cur_pos;
  t_list[cur_pos]   =  door_list[2];
  cave[y][x].fval   =  corr_floor4.ftval;
  cave[y][x].fopen  =  false;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_door(integer y,integer x)
{

  switch (randint(3)) {
  case 1 : 
    switch (randint(4)) {
    case 1  : place_broken_door(y,x);   break;
    default : place_open_door(y,x);     break;
    }
    break;
    
  case 2 : 
    switch (randint(12)) {
    case 1 : case 2 : place_locked_door(y,x);  break;
    case 3          : place_stuck_door(y,x);   break;
    default         : place_closed_door(y,x);  break;
    }
    break;
    
  case 3 : 
    place_secret_door(y,x);
    break;
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void random_object(integer y,integer x,integer num) 
{
  /*{ Creates objects nearby the coordinates given          -RAK-   }*/

  integer    i1,i2,i3;

  do {
    i1 = 0;
    do {
      i2 = y - 3 + randint(5);
      i3 = x - 4 + randint(7);

      //with cave[i2][i3]. do;
      if (is_in(cave[i2][i3].fval, floor_set)) {
	if (cave[i2][i3].tptr == 0) {
	  if (randint(100) < 75) {
	    place_object(i2,i3);
	  } else {
	    place_gold(i2,i3);
	  }
	  i1 = 9;
	}
      }
      i1++;
    } while (i1 <= 10);
    num--;
  } while (num != 0);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *likert(integer x, integer y, btype out_str)
{
  /*{ Returns a rating of x depending on y                  -JWT-   }*/

  if ((int)(x/y) < -3) {
    strcpy(out_str, "Very Bad");
  } else {
    switch ((int)(x/y)) {
    case -3: case -2: case -1: strcpy(out_str, "Very Bad");  break;
    case  0: case  1:          strcpy(out_str, "Bad");       break;
    case  2:                   strcpy(out_str, "Poor");      break;
    case  3: case  4:          strcpy(out_str, "Fair");      break;
    case  5:                   strcpy(out_str, "Good");      break;
    case  6:                   strcpy(out_str, "Very Good"); break;
    case  7: case  8:          strcpy(out_str, "Superb");    break;
    default:                   strcpy(out_str, "Excellent"); break;
    }
  }

  return out_str;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/* END FILE  misc.c */
