/* screen.c */
/**/

#include "imoria.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void draw_cave()
{
  /*{ Draws entire screen					-RAK-	}*/
      clear_screen();
      //clear_rc(1,1);
      prt_stat_block();
      prt_map();
      prt_depth();
      prt_search();
      put_qio();
}; /* end draw_cave */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_map()
{
  /*	{ Prints the map of the dungeon				-RAK-	}*/

  integer    i1,i2,i3,i4,i5;
  integer    ypos,xpos=0,isp;
  vtype      floor_str;
  char       tmp_char;
  boolean    flag;
  
  redraw = false;		/*{ Screen has been redrawn	   }*/
  i3 = 1;			/*{ Used for erasing dirty lines   }*/
  i4 = 14;			/*{ Erasure starts in this column  }*/
  for (i1 = panel_row_min; i1 <= panel_row_max; i1++) {	
    i3++;			/*{ Increment dirty line ctr	}*/
    if (used_line[i3]) {	/*{ If line is dirty...		}*/
      erase_line(i3,i4);	/*{ erase it.			}*/
      used_line[i3] = false;	/*{ Now it's a clean line	}*/
    }
    floor_str[0] = 0;	        /*{ Floor_str is string to be printed}*/
    ypos = i1;		        /*{ Save row			}*/
    flag = false;		/*{ False until floor_str <> ''	}*/
    isp = 0;		        /*{ Number of blanks encountered}*/
    for (i2 = panel_col_min; i2 <= panel_col_max; i2++) {
      //printf ("\n\t>>(row, col) = (%d, %d)", i1,i2);  fflush(stdout);
      //with cave[i1,i2] do;
      
      /*{ Get character for location	}*/
      if (test_light(i1,i2)) {
	tmp_char = loc_symbol(i1,i2);
      } else if ((cave[i1][i2].cptr == 1) && (!find_flag)) {
	tmp_char = '@';
      } else if (cave[i1][i2].cptr > 1) {
	if (m_list[cave[i1][i2].cptr].ml) {
	  tmp_char = loc_symbol(i1,i2);
	} else {
	  tmp_char = ' ';
	}
      } else {
	tmp_char = ' ';
      }
      
      
      if (tmp_char == ' ') {
	
	if (flag) {
	  isp++;
	  if (isp > 3) {
	    print_str(floor_str,ypos,xpos);
	    flag = false;
	    isp  = 0;
	  }
	}
	
      } else {
	
	if (flag) {
	  if (isp > 0) {
	    for (i5 = 0; i5 < isp; i5++) {
	      strcat(floor_str," ");
	    }
	    isp = 0;
	  }
	  floor_str[strlen(floor_str)+1] = 0;
	  floor_str[strlen(floor_str)] = tmp_char;
	} else {
	  xpos = i2;	
	  flag = true;	
	  floor_str[1] = 0;
	  floor_str[0] = tmp_char;
	}
	
      }
    }
    if (flag) {
      print_str(floor_str,ypos,xpos);
    }
  }
  
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void prt_6_stats(stat_s_type p, stat_s_type l, byteint row, byteint col)
{ 
  if (l != NULL) {
    prt_stat_attr("STR : ",p[STR],l[STR], row  ,col);
    prt_stat_attr("INT : ",p[INT],l[INT], row+1,col);
    prt_stat_attr("WIS : ",p[WIS],l[WIS], row+2,col);
    prt_stat_attr("DEX : ",p[DEX],l[DEX], row+3,col);
    prt_stat_attr("CON : ",p[CON],l[CON], row+4,col);
    prt_stat_attr("CHR : ",p[CHR],l[CHR], row+5,col);
  } else {
    prt_stat("STR : ",p[STR],row  ,col);
    prt_stat("INT : ",p[INT],row+1,col);
    prt_stat("WIS : ",p[WIS],row+2,col);
    prt_stat("DEX : ",p[DEX],row+3,col);
    prt_stat("CON : ",p[CON],row+4,col);
    prt_stat("CHR : ",p[CHR],row+5,col);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_stat_attr(	vtype 	stat_name,
			byteint	stat,
			byteint	loss,
			integer row,
			integer column)
{
/* { Print character stat in given row, column		-RAK-	}*/

  stat_s_type out_val1;

  if (loss == 0) {
    prt_stat(stat_name, stat, row, column);
  } else {
    cnv_stat(stat, out_val1);
    put_buffer_attr(stat_name,row,column,A_DIM);
    put_buffer(out_val1,row,column+strlen(stat_name));
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_stat(	vtype 	stat_name,
		byteint	stat,
		integer row,
		integer column)
{
/* { Print character stat in given row, column		-RAK-	}*/

  stat_s_type out_val1;
  vtype       out_val2;

  cnv_stat(stat, out_val1);
  sprintf(out_val2, "%s%s", stat_name, out_val1);
  put_buffer(out_val2,row,column);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void cnv_stat( byteint stat, stat_type out_val)
{
  integer   part1,part2;
  
  sprintf(out_val, "      ");
  
  if (stat > 150) {
    part1 = 18;
    part2 = stat - 150;
    sprintf(out_val,"%2ld/%-2ld",part1,part2);
  } else {
    sprintf(out_val,"%2d   ",3+(stat div 10));
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_num(vtype header, integer num,integer row,integer column)
{
  /*	{ Print number with header at given row, column		-RAK-	}*/

  vtype out_val;

  sprintf(out_val,"%s%ld  ",header,num);
  put_buffer(out_val,row,column);

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_stat_block()
{
  /*{ Prints character-screen info                              -RAK-   }*/
  prt_field(py.misc.race,                    RACE_ROW,STAT_COLUMN);
  prt_field(py.misc.tclass,                  CLASS_ROW,STAT_COLUMN);
  prt_title();
  prt_6_stats(py.stat.c,py.stat.l,           STR_ROW,STAT_COLUMN);
  prt_num( "LEV : ",py.misc.lev,             LEVEL_ROW,STAT_COLUMN);
  prt_num( "EXP : ",py.misc.exp,             EXP_ROW,STAT_COLUMN);
  if (is_magii) {
    prt_field("MANA: ",                      MANA_ROW,STAT_COLUMN);
    prt_mana();
  }
  prt_field("HP  : ",                        HP_ROW,STAT_COLUMN);
  prt_hp();
  prt_num( "QST : ",py.misc.quests,          QUEST_ROW,STAT_COLUMN);
  prt_num( "AC  : ",py.misc.dis_ac,          AC_ROW,STAT_COLUMN);
  prt_num( "GOLD: ",py.misc.money[TOTAL_],   GOLD_ROW,STAT_COLUMN);
  prt_field("WGHT:",                         WEIGHT_ROW,STAT_COLUMN);
  prt_field("M_WT:",                         WEIGHT_ROW+1,STAT_COLUMN);
  prt_weight();
  prt_time();
  if (total_winner) {
    prt_winner();
  }
  prt_hunger(); /*{'If' statements here redundant and unnecessary, so}*/
  prt_blind();  /*{ removed per Dean's suggestion                -MAV}*/
  prt_confused();
  prt_afraid();
  prt_poisoned();
  prt_search();
  prt_rest();
  prt_quested();
  prt_light_on();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_field(vtype info, integer row, integer column)
{
  /*{ Print character info in given row, column		-RAK-	}*/
  vtype   out_val1;

  sprintf(out_val1, "%-14s", info);
  put_buffer(out_val1,row,column);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_title()
{
  /*{ Prints title of character's level			-RAK-	}*/
  prt_field(py.misc.title,TITLE_ROW,STAT_COLUMN);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_hp()
{
  /*{ Prints hit points					-DCJ-	}*/
  vtype    out_val1, out_val2;

  sprintf(out_val1,"%d/%d ",(int)(py.misc.chp),(int)(py.misc.mhp));
  if (strlen(out_val1) < 8) {
    sprintf(out_val2,"%-8s",out_val1);
    put_buffer(out_val2,HP_ROW,STAT_COLUMN+6);
  } else {
    put_buffer(out_val1,HP_ROW,STAT_COLUMN+6);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_pac()
{
  /*{ Prints current AC					-RAK-	}*/
  prt_num( "",py.misc.dis_ac,AC_ROW,STAT_COLUMN+6);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_gold()
{
  /*{ Prints current gold					-RAK-	}*/

  prt_num( "",py.misc.money[TOTAL_],GOLD_ROW,STAT_COLUMN+6);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_weight()
{
  /*{ Prints current inventory weight			-DCJ-	}*/
  prt_num("",inven_weight div 100,WEIGHT_ROW,STAT_COLUMN+6);
  prt_num("",weight_limit(),WEIGHT_ROW+1,STAT_COLUMN+6);
 
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_time()
{
  /*{ Print time of game day				-DMF-	}*/

  vtype    s1,s2,s3;
  vtype    out_val;

  sprintf(out_val,"%s %s %s",
	  time_string(py.misc.cur_age.hour,py.misc.cur_age.secs,s1),
	  day_of_week_string(py.misc.cur_age.day,2,s2),
	  place_string(py.misc.cur_age.day,s3));
  
  put_buffer(out_val, TIME_ROW, STAT_COLUMN);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_light_on()
{
  if (PF.light_on) {
    prt("         ",STATUS_ROW+1,LIGHT_ON_COLUMN);
  } else {
    put_buffer_attr("Light Off",STATUS_ROW+1,LIGHT_ON_COLUMN, A_DIM);
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_depth()
{
  /*{ Prints depth in stat area				-RAK-	}*/
  vtype    depths;
  integer  depth;

  depth = dun_level*50;
  if (depth == 0) {
    strcpy(depths, "Town level");
  } else if (depth < 10000) {
    sprintf(depths,"Depth: %ld (feet)",depth);
  } else {
    sprintf(depths,"Depth: %ld   ",depth);
  }	  

  prt(depths,STATUS_ROW,DEPTH_COLUMN);

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_hunger()
{
  /*{ Prints status of hunger				-RAK-	}*/
  if ((IS_WEAK & py.flags.status) != 0) {
    put_buffer_attr("Weak    ",STATUS_ROW,HUNGER_COLUMN,A_BOLD | A_BLINK);
  } else if ((IS_HUNGERY & py.flags.status) != 0) {
    put_buffer_attr("Hungry  ",STATUS_ROW,HUNGER_COLUMN, A_BOLD);
  } else {
    put_buffer("        ",STATUS_ROW,HUNGER_COLUMN);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_blind()
{
  /*{ Prints Blind status					-RAK-	}*/

  if ((IS_BLIND & py.flags.status) != 0) {
    put_buffer_attr("Blind  ",STATUS_ROW,BLIND_COLUMN,A_BOLD);
  } else {
    put_buffer("       ",STATUS_ROW,BLIND_COLUMN);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_confused()
{
  /*{ Prints Confusion status				-RAK-	}*/
  if ((IS_CONFUSED & py.flags.status) != 0) {
    put_buffer_attr("Confused  ",STATUS_ROW,CONFUSED_COLUMN,A_BOLD);
  } else {
    put_buffer("          ",STATUS_ROW,CONFUSED_COLUMN);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_afraid()
{
  /*{ Prints Fear status					-RAK-	}*/
  
  if ((IS_AFRAID & py.flags.status) != 0) {
    put_buffer_attr("Afraid  ",STATUS_ROW,AFRAID_COLUMN, A_BOLD);
  } else {
    put_buffer("        ",STATUS_ROW,AFRAID_COLUMN);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_poisoned()
{
  /*{ Prints Poisoned status				-RAK-	}*/

  if ((IS_POISONED & py.flags.status) != 0) {
    put_buffer_attr("Poisoned  ",STATUS_ROW,POISONED_COLUMN,A_BOLD);
  } else {
    put_buffer("          ",STATUS_ROW,POISONED_COLUMN);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_search()
{
  /*{ Prints Searching status				-RAK-	}*/

  if ((IS_SEARCHING & py.flags.status) != 0) {
    put_buffer("Searching",STATUS_ROW,SEARCHING_COLUMN);
  } else {
    put_buffer("         ",STATUS_ROW,SEARCHING_COLUMN);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_rest()
{
  /*{ Prints Resting status					-RAK-	}*/

  if ((IS_RESTING & py.flags.status) != 0) {
    put_buffer("Resting  ",STATUS_ROW,RESTING_COLUMN);
  } else {
    put_buffer("         ",STATUS_ROW,RESTING_COLUMN);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_quested()
{
  /*{ Prints Quested status					-RAD-	}*/
  if (py.flags.quested) {
    put_buffer(" Quest  ",STATUS_ROW,QUESTED_COLUMN);
  } else if (py.misc.cur_quest > 0) {
    put_buffer("  Done  ",STATUS_ROW,QUESTED_COLUMN);
  } else {
    put_buffer("        ",STATUS_ROW,QUESTED_COLUMN);
  }
};  
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_winner()
{
  /*{ Prints winner status on display			-RAK-	}*/

  put_buffer("*Winner*",WINNER_ROW,WINNER_COLUMN);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_experience()
{

  /*      with py.misc do*/
  if (py.misc.exp > player_max_exp) {
    py.misc.exp = player_max_exp;
  }
  
  if (py.misc.lev < MAX_PLAYER_LEVEL) {
    while ((player_exp[py.misc.lev]*py.misc.expfact) <= py.misc.exp) {
      gain_level();
    }
    
    if (py.misc.exp > py.misc.max_exp) {
      py.misc.max_exp = py.misc.exp;
    }
  }
  
  prt_num("",py.misc.exp,EXP_ROW,STAT_COLUMN+6);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_mana()
{
  /*{ Prints player's mana					-DCJ-	}*/
  vtype   out_val1;
  vtype   out_val2;

  sprintf(out_val1,"%d/%d",(int)(py.misc.cmana),(int)(py.misc.mana));
  if ( strlen(out_val1) < 8 ) {
    sprintf(out_val2,"%-8s",out_val1);
    put_buffer(out_val2,MANA_ROW,STAT_COLUMN+6);
  } else {
    put_buffer(out_val1,MANA_ROW,STAT_COLUMN+6);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_level()
{
  /*{ Prints level						-RAK-	}*/

  prt_num( "",py.misc.lev,LEVEL_ROW,STAT_COLUMN+6);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_a_stat(stat_set tstat)
{
  char *stat_names[STAT_SET_MAX+1] = {"STR : ",
				      "INT : ",
				      "WIS : ",
				      "DEX : ",
				      "CON : ",
				      "CHR : "};

  prt_stat_attr(stat_names[(int)tstat],
		py.stat.c[(int)tstat],
		py.stat.l[(int)tstat],
		STR_ROW+tstat,STAT_COLUMN);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  screen.c */
