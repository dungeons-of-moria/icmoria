/* dungeon.c */
/* Here you should find the guts of the game */

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////

/* I got rid of all the ones I think are not "real" globals... */

	integer	dir_val;      		// { For movement (running)}
//	integer	y,x,moves;      	// { For movement          }
//	integer	i1,i2,tmp1;      	// { Temporaries           }
	integer	old_chp,old_cmana;      // { Detect change         }
	real	regen_amount;         	// { Regenerate hp and mana}
	char	command;         	// { Last command          }
//	vtype	out_val,out2;        	// { For messages          }
//	vtype	tmp_str;        	// { Temporary             }
	boolean	moria_flag;      	// { Next level when true  }
	boolean	reset_flag;      	// { Do not move creatures }
	boolean	search_flag;      	// { Player is searching   }
	boolean	teleport_flag;      	// { Handle telport traps  }
	boolean	player_light;      	// { Player carrying light }
	boolean	save_msg_flag;      	// { Msg flag after INKEY  }
	ttype	s1,s2,s3,s4;		// { Summon item strings   }
	integer	i_summ_count;		// { Summon item count	   }
//	char		trash_char;
//	FILE *		f1;
//	stat_set	tstat;
//	treas_ptr	trash_ptr;

//////////////////////////////////////////////////////////////////////
void panel_bounds()
{
  /*{ Calculates current boundries				-RAK-	}*/

  panel_row_min = (trunc(panel_row*(SCREEN_HEIGHT/2)) + 1);
  panel_row_max = panel_row_min + SCREEN_HEIGHT - 1;

  panel_col_min = (trunc(panel_col*(SCREEN_WIDTH/2)) + 1);
  panel_col_max = panel_col_min + SCREEN_WIDTH - 1;

  panel_row_prt = panel_row_min - 2;
  panel_col_prt = panel_col_min - 15;

}
//////////////////////////////////////////////////////////////////////

/*  { Figure out what kind of coin is beign asked about }*/
boolean coin_stuff(char typ,    /*{ Initial of coin metal }*/
                   integer *type_num)
{
  boolean return_value;

  return_value = true;
  switch (typ) {
	case	'm' : *type_num = MITHRIL;  break;
	case	'p' : *type_num = PLATINUM; break;
	case  	'g' : *type_num = GOLD;     break;
	case	's' : *type_num = SILVER;   break;
	case	'c' : *type_num = COPPER;   break;
	case	'i' : *type_num = IRON;     break;

	default: return_value = false;      break;
  }
  return return_value;
}; /* end coin_stuff */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void get_money_type__prompt_money(vtype astr, string out_val, boolean *commas)
{
  if (*commas) {
    strcat(out_val, ", ");
  }
  strcat(out_val, astr);
  *commas = true;
}; /* end get_money_type__prompt_money */

//////////////////////////////////////////////////////////////////////

integer get_money_type(
		string		prompt,
		boolean		*back,
		boolean		no_check)
{
     /*	{ Prompt for what type of money to use			-DMF-	}*/

  boolean  comma_flag = false;
  boolean  test_flag  = false;
  string   out_val;
  integer  com_val;

  strncpy(out_val, prompt, sizeof(string));

  if ((py.misc.money[6] > 0) || (no_check))
    get_money_type__prompt_money("<m>ithril",out_val,&comma_flag);
  if ((py.misc.money[5] > 0) || (no_check))
    get_money_type__prompt_money("<p>latinum",out_val,&comma_flag);
  if ((py.misc.money[4] > 0) || (no_check))
    get_money_type__prompt_money("<g>old",out_val,&comma_flag);
  if ((py.misc.money[3] > 0) || (no_check))
    get_money_type__prompt_money("<s>ilver",out_val,&comma_flag);
  if ((py.misc.money[2] > 0) || (no_check))
    get_money_type__prompt_money("<c>opper",out_val,&comma_flag);
  if ((py.misc.money[1] > 0) || (no_check))
    get_money_type__prompt_money("<i>ron",out_val,&comma_flag);

  prt(out_val,1,1);
  *back = true;

  do {
    command = inkey();
    com_val = (integer)(command);
    switch (com_val) {
    case 0: case 3: case 25: case 26: case 27:
      test_flag = true;
      *back = false;
      break;
    case 109:
      test_flag = ((py.misc.money[MITHRIL] > 0) || (no_check));
      break;
    case 112:
      test_flag = ((py.misc.money[PLATINUM] > 0) || (no_check));
      break;
    case 103:
      test_flag = ((py.misc.money[GOLD] > 0) || (no_check));
      break;
    case 115:
      test_flag = ((py.misc.money[SILVER] > 0) || (no_check));
      break;
    case  99:
      test_flag = ((py.misc.money[COPPER] > 0) || (no_check));
      break;
    case 105:
      test_flag = ((py.misc.money[IRON] > 0) || (no_check));
      break;
    } /* end switch */
  } while (!test_flag);

  return com_val;

}; /* end get_money_type */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void py_bonuses(treasure_type *tobj, integer factor)
{
  /*
	{ Player bonuses					-RAK-	}
	{ When an item is worn or taken off, this re-adjusts the player }
	{ bonuses.  Factor=1 : wear; Factor=-1 : removed                }
  */

  unsigned  long   item_flags,item_flags2;
  integer          i1,old_dis_ac;
  stat_set         tstat;

  if (PF.slow_digest) {
    PF.food_digested += 1;
  }
  if (PF.regenerate) {
    PF.food_digested -= 3;
  }
  PF.see_inv     = false;
  PF.teleport    = false;
  PF.free_act    = false;
  PF.slow_digest = false;
  PF.aggravate   = false;
  for (tstat = STR; tstat <= CHR; tstat++) {
    PF.sustain[(int)tstat] = false;
  }
  PF.fire_resist = false;
  PF.hunger_item = false;
  PF.acid_resist = false;
  PF.cold_resist = false;
  PF.regenerate  = false;
  PF.lght_resist = false;
  PF.ffall       = false;
  
  if (uand(Strength_worn_bit,tobj->flags) != 0) {
    change_stat(STR,tobj->p1,factor);
    print_stat = uor(0x0001,print_stat);
  }
  if (uand(Magic_proof_worn_bit,tobj->flags2) != 0) {
    py.misc.save += (25 * factor);
  }
  if (uand(Bad_repute_worn_bit,tobj->flags2) != 0) {
    change_rep(-100*factor); /*{XXX hey!  this is bad! new variable!-ste}*/
  }
  if (uand(Disarm_worn_bit,tobj->flags2) != 0) {
    py.misc.disarm += (tobj->p1 * factor);
  }
  if (uand(Dexterity_worn_bit,tobj->flags) != 0) {
    change_stat(DEX,tobj->p1,factor);
    print_stat = uor(0x0002,print_stat);
  }
  if (uand(Constitution_worn_bit,tobj->flags) != 0) {
    change_stat(CON,tobj->p1,factor);
    print_stat = uor(0x0004,print_stat);
  }
  if (uand(Intelligence_worn_bit,tobj->flags) != 0) {
    change_stat(INT,tobj->p1,factor);
    print_stat = uor(0x0008,print_stat);
  }
  if (uand(Wisdom_worn_bit,tobj->flags) != 0) {
    change_stat(WIS,tobj->p1,factor);
    print_stat = uor(0x0010,print_stat);
  }
  if (uand(Charisma_worn_bit,tobj->flags) != 0) {
    change_stat(CHR,tobj->p1,factor);
    print_stat = uor(0x0020,print_stat);
  }
  if (uand(Searching_worn_bit,tobj->flags) != 0) {
    py.misc.srh += (tobj->p1 * factor);
    py.misc.fos -= (tobj->p1 * factor);
  }
  if (uand(Stealth_worn_bit,tobj->flags) != 0) {
    py.misc.stl += (tobj->p1 * factor) + factor;
  }
  if (uand(Speed_worn_bit,tobj->flags) != 0) {
    i1 = tobj->p1*factor;
    change_speed(-i1);
  }
  if (uand(Blindness_worn_bit,tobj->flags) != 0) {
    if (factor > 0) {
      py.flags.blind += 1000;
    }
  }
  if (uand(Timidness_worn_bit,tobj->flags) != 0) {
    if (factor > 0) {
      py.flags.afraid += 50;
    }
  }
  if (uand(Infra_Vision_worn_bit,tobj->flags) != 0) {
    py.flags.see_infra += (tobj->p1 * factor);
  }
  if (uand(Swimming_worn_bit,tobj->flags2) != 0) {
    i1 = tobj->p1*factor; /* XXX what does this do? */
  }
  if (uand(Increase_carry_worn_bit,tobj->flags2) != 0) {
    switch (tobj->p1) {
    case 1 : i1 = 500;    break;
    case 2 : i1 = 1000;   break;
    case 3 : i1 = 1750;   break;
    case 4 : i1 = 2500;   break;
    case 5 : i1 = 3500;   break;
    case 6 : i1 = 4500;   break;
    case 7 : i1 = 6000;   break;
    default:
      MSG("Increase carry worn value (p1) out of range");
      i1 = 500;
      break;
    }
    py.misc.xtr_wgt += i1 * factor;
  }

  //with py.misc do;
  old_dis_ac     = PM.dis_ac;
  PM.ptohit      = tohit_adj();   //{ Real To Hit   }
  PM.ptodam      = todam_adj();   //{ Real To Dam   }
  PM.ptoac       = toac_adj();    //{ Real To AC    }
  PM.pac         = 0;             //{ Real AC       }
  PM.dis_th      = PM.ptohit;     //{ Display To Hit        }
  PM.dis_td      = PM.ptodam;     //{ Display To Dam        }
  PM.dis_ac      = 0;             //{ Display To AC         }
  PM.dis_tac     = PM.ptoac;      //{ Display AC            }

  for (i1 = Equipment_min; i1 <= EQUIP_MAX-2; i1++) {
    //with equipment[i1] do;
    if (equipment[i1].tval > 0) {
      if (uand(Cursed_worn_bit,equipment[i1].flags) == 0) {
	PM.pac    += equipment[i1].ac;
	PM.dis_ac += equipment[i1].ac;
      }
      PM.ptohit   += equipment[i1].tohit;
      PM.ptodam   += equipment[i1].todam;
      PM.ptoac    += equipment[i1].toac;
      if (strstr(equipment[i1].name,"^") == NULL) {
	PM.dis_th  += equipment[i1].tohit;
	PM.dis_td  += equipment[i1].todam;
	PM.dis_tac += equipment[i1].toac;
      }
    }
  }
  PM.dis_ac += PM.dis_tac;

  /* { Add in temporary spell increases	}*/
  //with py.flags do;
  if (PF.invuln > 0) {
    PM.pac    += 100;
    PM.dis_ac += 100;
  }
  if (PF.blessed > 0) {
    PM.pac    += 5;
    PM.dis_ac += 5;
  }
  if (PF.detect_inv > 0) {
    PF.see_inv = true; /* does this mean that if you put on/take off stuff
			  you are going to lose magic detect_inv ? */
  }
  
  if (old_dis_ac != PM.dis_ac) {
    print_stat = uor(0x0040,print_stat);
  }
  item_flags2 = 0;
  item_flags  = 0;
  
  for (i1 = Equipment_min; i1 <= EQUIP_MAX-2; i1++) {
    //with equipment[i1] do;
    item_flags  = uor(item_flags,equipment[i1].flags);
    item_flags2 = uor(item_flags2,equipment[i1].flags2);
  }
  
  //with py.flags do;
  PF.slow_digest  = uand(  Slow_Digestion_worn_bit,   item_flags)  != 0;
  PF.aggravate    = uand(  Aggravation_worn_bit,      item_flags)  != 0;
  PF.teleport     = uand(  Teleportation_worn_bit,    item_flags)  != 0;
  PF.regenerate   = uand(  Regeneration_worn_bit,     item_flags)  != 0;
  PF.hunger_item  = uand(  Hunger_worn_bit,           item_flags2) != 0;
  PF.fire_resist  = uand(  Resist_Fire_worn_bit,      item_flags)  != 0;
  PF.acid_resist  = uand(  Resist_Acid_worn_bit,      item_flags)  != 0;
  PF.cold_resist  = uand(  Resist_Cold_worn_bit,      item_flags)  != 0;
  PF.free_act     = uand(  Free_Action_worn_bit,      item_flags)  != 0;
  PF.see_inv     |= uand(  See_Invisible_worn_bit,    item_flags)  != 0;
  PF.lght_resist  = uand(  Resist_Lightning_worn_bit, item_flags)  != 0;
  PF.ffall        = uand(  Feather_Fall_worn_bit,     item_flags)  != 0;
  
  for (i1 = Equipment_min; i1 <= EQUIP_MAX-2; i1++) {
    //with equipment[i1] do;
    if (uand(Sustain_Stat_worn_bit,equipment[i1].flags) != 0) {
      if ((equipment[i1].p1>0) && (equipment[i1].p1<7)) {
	py.flags.sustain[equipment[i1].p1-1] = true;
      }
    }
  }

  if (PF.slow_digest) {
    PF.food_digested -= 1;
  }
  if (PF.regenerate) {
    PF.food_digested += 3;
  }

}; /* end py_bonuses */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void change_stat(stat_set tstat, integer amount, integer factor)
{
  /*{ Changes stats up or down for magic items		-RAK-	}*/

  PS.m[(int)tstat] += amount*factor;
  update_stat(tstat);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void change_speed(integer num)
{
  /*
    { Changes speed of monsters relative to player		-RAK-	}
    { Note: When the player is sped up or slowed down, I simply     }
    {       change the speed of all the monsters.  This greatly     }
    {       simplified the logic...                                 }
  */
  integer   i1;

  py.flags.speed += num;

  for (i1 = muptr; i1 != 0; i1 = m_list[i1].nptr) {
    m_list[i1].cspeed += num;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void update_stat(stat_set tstat)
{
  PS.c[(int)tstat] = squish_stat(PS.p[(int)tstat] + 10*PS.m[(int)tstat] - 
				 PS.l[(int)tstat]);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void change_rep(integer amt)
{
  //with py.misc do;
  if ((amt<0) || (PM.rep+amt<=0)) { /*{bad deed or make up for sins}*/
    PM.rep += amt;
  } else {/*{ good deed that puts char into positive reputation }*/
/*{ good characters progress slowly -- past 0 it costs 2, past 20 costs 3...}*/
    if (PM.rep < 0) {	/*{ go from bad to good }*/
      amt += PM.rep;
      PM.rep = 0;
    } /*{increase goodness}*/
    PM.rep = trunc(sqrt((20+PM.rep)*(20+PM.rep)+40*amt)-20);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean panel_contains(integer y, integer x)
{
  /*{ Tests a given point to see if it is within the screen -RAK-   }*/
  /*{ boundries.                                                    }*/

  boolean   return_value = false;

  if ((y >= panel_row_min) && (y <= panel_row_max)) {
    if ((x >= panel_col_min) && (x <= panel_col_max)) {
      return_value = true;
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ml__draw_block(integer y1, integer x1, integer y2, integer x2)
{
  /*{ Given two sets of points, draw the block		}*/

  integer   i1,i2,xpos,xmax=0;
  integer   topp,bott,left,righ;
  integer   new_topp,new_bott,new_left,new_righ;
  vtype     floor_str,save_str;
  integer   floor_str_len = 0;
  char      tmp_char;
  boolean   flag;

  ENTER("ml__draw_block","m");

  /*{ From uppermost to bottom most lines player was on...  }*/
  /*{ Points are guaranteed to be on the screen (I hope...) }*/
  
//fprintf(debug_file,": draw_block: y1: %d  x1: %d  y2: %d  x2: %d\n",
//    y1,x1,y2,x2);
//fflush(debug_file);

  topp = maxmin(y1,y2,panel_row_min);
  bott = minmax(y1,y2,panel_row_max);
  left = maxmin(x1,x2,panel_col_min);
  righ = minmax(x1,x2,panel_col_max);
  
//fprintf(debug_file,": draw_block: topp: %d  bott: %d  left: %d  right: %d\n",
//      topp,bott,left,righ);
//fflush(debug_file);

  new_topp = y2 - 1;     /*{ Margins for new things to appear}*/
  new_bott = y2 + 1;
  new_left = x2 - 1;
  new_righ = x2 + 1;
  
  for (i1 = topp; i1 <= bott; i1++) {

    floor_str[0]  = 0;    /*{ Null out print string         }*/
    floor_str_len = 0;
    save_str[0]   = 0;
    xpos          = 0;

    for (i2 = left; i2 <= righ; i2++) { /*{ Leftmost to rightmost do}*/
      //with cave[i1,i2] do;
      if ((cave[i1][i2].pl) || (cave[i1][i2].fm)) {
	flag = (((i1==y1) && (i2==x1)) || ((i1==y2) && (i2==x2)));
	//flag = true;
      } else {
	flag = true;
	if (((i1 >= new_topp) && (i1 <= new_bott)) &&
	    ((i2 >= new_left) && (i2 <= new_righ))) {
	  if (cave[i1][i2].tl) {
	    if (is_in(cave[i1][i2].fval,pwall_set)) {
	      cave[i1][i2].pl = true;
	    } else if (cave[i1][i2].tptr > 0) {
	      if (is_in(t_list[cave[i1][i2].tptr].tval,light_set)) {
		if (!(cave[i1][i2].fm)) {
		  cave[i1][i2].fm = true;
		}
	      }
	    }
	  }
	}
      }
      
      if ((cave[i1][i2].pl) || (cave[i1][i2].tl) || (cave[i1][i2].fm)) {
	tmp_char = loc_symbol(i1,i2);
      } else {
	tmp_char = ' ';
      }
      
      if (py.flags.image > 0) {
	if (randint(12) == 1) {
	  tmp_char = (char)(randint(95) + 31);
	}
      }
      
      if (flag) {
	if (xpos == 0) { 
	  xpos = i2;
	}
	xmax = i2;
      }

      if (xpos > 0) {
	if (floor_str_len > 80) {
	  MSG(": ERROR draw_block floor_str_len too big\n");
	}	

	floor_str[floor_str_len++] = tmp_char;
      }
      
    } /* end for  i2 */
    
    if (floor_str_len > 80) {
      MSG(": ERROR draw_block floor_str_len too big\n");
    }	

    floor_str[floor_str_len] = 0;
//    fprintf(debug_file,":   floor before if %d: |%s|\n",i1,floor_str);
//    fflush(debug_file);
    
    if (xpos > 0) {
      i2 = i1;       /*{ Var for PRINT cannot be loop index}*/
      /*print(substr(floor_str,1,1+xmax-xpos),i2,xpos);*/

      if ( ((1+xmax-xpos+1) > 80)  ||  ((1+xmax-xpos+1) < 0) ) {
	MSG(": ERROR draw_block xmax-xpos is bad\n");
      }	

      floor_str[1+xmax-xpos+1] = 0;
      print_str(floor_str,i2,xpos);

    }
  }

  LEAVE("ml__draw_block","m");
};
//////////////////////////////////////////////////////////////////////
void ml__sub1_move_light(integer y1,integer x1,integer y2,integer x2)
{
  /*{ Normal movement                                   }*/

  integer   i1,i2;

  ENTER("ml__sub1_move_light","m");

  light_flag = true;

  for (i1 = y1-1; i1 <= y1+1; i1++) {   /*{ Turn off lamp light   }*/
    for (i2 = x1-1; i2 <= x1+1; i2++) {
      cave[i1][i2].tl = false;
    }
  }

  for (i1 = y2-1; i1 <= y2+1; i1++) {
    for (i2 = x2-1; i2 <= x2+1; i2++) {
      cave[i1][i2].tl = true;
    }
  }

  ml__draw_block(y1,x1,y2,x2);        /*{ Redraw area           }*/

  LEAVE("ml__sub1_move_light","m");
};
//////////////////////////////////////////////////////////////////////
void ml__sub2_move_light(integer y1,integer x1,integer y2,integer x2)
{
  /*{ When FIND_FLAG, light only permanent features     }*/

  integer   i1,i2,xpos;
  vtype     floor_str,save_str;
  integer   floor_str_len, save_str_len;
  char      tmp_char;
  boolean   flag;

  ENTER("ml__sub2_move_light","m");

  if (light_flag) {
    for (i1 = y1-1; i1 <= y1+1; i1++) {
      for (i2 = x1-1; i2 <= x1+1; i2++) {
	cave[i1][i2].tl = false;
      }
    }
    ml__draw_block(y1,x1,y1,x1);
    light_flag = false;
  }
  
  for (i1 = y2-1; i1 <= y2+1; i1++) {
    floor_str[0]  = 0;
    save_str[0]   = 0;
    floor_str_len = 0;
    save_str_len  = 0;
    xpos = 0;
    for (i2 = x2-1; i2 <= x2+1; i2++) {
      //with cave[i1,i2] do;
      flag = false;
      if (!((cave[i1][i2].fm) || (cave[i1][i2].pl))) {
	tmp_char = ' ';
	if (player_light) {
	  if (is_in(cave[i1][i2].fval, pwall_set)) {
	    cave[i1][i2].pl = true; /*{ Turn on perm light    }*/
	    tmp_char = loc_symbol(i1,i2);
	    flag = true;
	  } else {
	    if (cave[i1][i2].tptr > 0) {
	      if (is_in(t_list[cave[i1][i2].tptr].tval,light_set)) {
		cave[i1][i2].fm = true;  /*{ Turn on field marker  }*/
		tmp_char = loc_symbol(i1,i2);
		flag = true;
	      }
	    }
	  }
	}
      } else {
	tmp_char = loc_symbol(i1,i2);
      }
      
      if (flag) {
	if (xpos == 0) {
	  xpos = i2;
	}
	if (save_str[0] != 0) {
	  //floor_str := floor_str + save_str;
	  floor_str[floor_str_len] = 0;
	  save_str[save_str_len]   = 0;
	  strcat(floor_str, save_str);
	  floor_str_len += save_str_len;
	  save_str[0] = 0;
	  save_str_len = 0;
	}
	floor_str[floor_str_len++] = tmp_char;
      } else if (xpos > 0) {
	save_str[save_str_len++] = tmp_char;
      }
    } /* end for i2 */
    if (xpos > 0) {
      i2 = i1;
      floor_str[floor_str_len] = 0;
      print_str(floor_str,i2,xpos);
    }
  } /* end for i1 */

  LEAVE("ml__sub2_move_light","m");
};
//////////////////////////////////////////////////////////////////////
void ml__sub3_move_light(integer y1,integer x1,integer y2,integer x2)
{
  /*{ When blinded, move only the player symbol...              }*/

  integer  i1,i2;

  ENTER("ml__sub3_move_light","m");

  if (light_flag) {
    for (i1 = y1-1; i1 <= y1+1; i1++) {
      for (i2 = x1-1; i2 <= x1+1; i2++) {
	cave[i1][i2].tl = false;
      }
    }
    light_flag = false;
  }
  print(' ',y1,x1);
  print('@',y2,x2);

  LEAVE("ml__sub3_move_light","m");
};
//////////////////////////////////////////////////////////////////////
void ml__sub4_move_light(integer y1,integer x1,integer y2,integer x2)
{
  /*{ With no light, movement becomes involved...               }*/

  integer   i1,i2;
  
  ENTER("ml__sub4_move_light","m");

  light_flag = true;
  if (cave[y1][x1].tl) {
    for (i1 = y1-1; i1 <= y1+1; i1++) {
      for (i2 = x1-1; i2 <= x1+1; i2++) {
	cave[i1][i2].tl = false;
	if (test_light(i1,i2)) {
	  lite_spot(i1,i2);
	} else {
	  unlite_spot(i1,i2);
	}
      }
    }
  } else if (test_light(y1,x1)) {
    lite_spot(y1,x1);
  } else {
    unlite_spot(y1,x1);
  }
  print('@',y2,x2);

  LEAVE("ml__sub4_move_light","m");
};
//////////////////////////////////////////////////////////////////////
void move_light(integer y1,integer x1,integer y2,integer x2)
{
  /*{ Package for moving the character's light about the screen     }*/
  /*{ Three cases : Normal, Finding, and Blind              -RAK-   }*/
  /* (so what is that sub4 thing?) */

  if (py.flags.blind > 0) {
	ml__sub3_move_light(y1,x1,y2,x2);  /* blind */
  } else if (find_flag) {
	ml__sub2_move_light(y1,x1,y2,x2);  /* searching */
  } else if (!player_light) {
	ml__sub4_move_light(y1,x1,y2,x2);  /* no light */
  } else {
	ml__sub1_move_light(y1,x1,y2,x2);  /* normal */
  }
  
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void lite_spot(integer y,integer x)
{
  if (panel_contains(y,x)) {
    print(loc_symbol(y,x),y,x);
  }
};

void unlite_spot(integer y,integer x)
{
  if (panel_contains(y,x)) {
    print(' ',y,x);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void teleport(integer dis)
{
  /*{ Teleport the player to a new location                 -RAK-   }*/

  integer   y,x,i1,i2;

  ENTER("teleport", "d");

  do {
    y = randint(cur_height);
    x = randint(cur_width);
    for (;distance(y,x,char_row,char_col) > dis;) {
      y += trunc((char_row-y)/2);
      x += trunc((char_col-x)/2);
    }
  } while (!((cave[y][x].fopen) && (cave[y][x].cptr  < 2)));

  move_rec(char_row,char_col,y,x);
  for (i1 = char_row-1; i1 <= char_row+1; i1++) {
    for (i2 = char_col-1; i2 <= char_col+1; i2++) {
      //with cave[i1,i2] do;
      cave[i1][i2].tl = false;
      if (!(test_light(i1,i2))) {
	unlite_spot(i1,i2);
      }
    }
  }

  if (test_light(char_row,char_col)) {
    lite_spot(char_row,char_col);
  }

  char_row = y;
  char_col = x;
  move_char(5);
  creatures(false);
  teleport_flag = false;

  LEAVE("teleport", "d")
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean get_panel(integer y, integer x, boolean forceit)
{
  /*{ Given an row (y) and col (x), this routine detects  -RAK-     }*/
  /*{ when a move off the screen has occurred and figures new borders}*/
  /* forceit forcses the panel bounds to be recalculated (show_location). */

  integer   prow,pcol;
  boolean   return_value;

  prow = panel_row;
  pcol = panel_col;

  if (forceit || (y < panel_row_min + 2) || (y > panel_row_max - 2)) {
    prow = trunc((y - 2)/(SCREEN_HEIGHT/2));
    if (prow > max_panel_rows) {
      prow = max_panel_rows;
    } else if (prow < 0) {
      prow = 0;
    }
  }

  if (forceit || (x < panel_col_min + 3) || (x > panel_col_max - 3)) {
    pcol = trunc((x - 3)/(SCREEN_WIDTH/2));
    if (pcol > max_panel_cols) {
      pcol = max_panel_cols;
    } else if (pcol < 0) {
      pcol = 0;
    }
  }

  if ((prow != panel_row) || (pcol != panel_col) || !(cave_flag)) {
    panel_row = prow;
    panel_col = pcol;
    panel_bounds();
    cave_flag = true;
    return_value = true;
  } else {
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void move_rec(integer y1,integer x1,integer y2,integer x2)
{
  /*{ Moves creature record from one space to another       -RAK-   }*/
  /* (x1,y1) might equal (x2,y2) so use a temp var */

  byteint  i1;

  i1 = cave[y1][x1].cptr;
  cave[y1][x1].cptr = 0;
  cave[y2][x2].cptr = i1;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean find_range(obj_set item_val,boolean inner,
		   treas_ptr *first, integer *count)
{
  boolean     flag;
  treas_ptr   ptr;

  ENTER("find_range", "");

  *count = 0;
  *first = nil;
  flag   = false;

  change_all_ok_stats(false,false);

  for (ptr = inventory_list; ptr != NULL; ptr = ptr->next) {

//    fprintf(debug_file,"find: >%s<\n",ptr->data.name);
//    fprintf(debug_file,"find:     %d %d %d %d\n",
//	    (integer)ptr->data.tval, (integer)ptr->is_in, 
//	    (integer)ptr->insides, (integer)ptr->ok);

    if ((is_in(ptr->data.tval, item_val)) &&
	(!(ptr->is_in) || inner) &&
	((ptr->insides == 0) || (ptr->data.tval != bag_or_sack))) {
      if (!flag) {
	flag   = true;
	*first = ptr;
      }
      ptr->ok = true;
      (*count)++;
    }
  }

#if DO_DEBUG
  fprintf(debug_file,"find: count=%ld\n",*count);
#endif

  RETURN("find_range", "",'b',"found",&flag);
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void carry(integer y, integer x)
{
  /*{ Player is on an object.  Many things can happen BASED -RAK-   }*/
  /*{ on the TVAL of the object.  Traps are set off, money and most }*/
  /*{ objects are picked up.  Some objects, such as open doors, just}*/
  /*{ sit there...                                                  }*/
  
  treas_ptr   item_ptr;
  vtype       out_val,out2;
  char        page_char;
  char        inv_char;
  treas_ptr   tmp_ptr;
  integer     count;
  boolean     money_flag;

  money_flag = false;
  find_flag  = false;

  //with cave[y][x]. do;
  inven_temp->data = t_list[cave[y][x].tptr];

  /*{ There's GOLD in them thar hills!      }*/
  /*{ OPPS!                                 }*/

  if (is_in(t_list[cave[y][x].tptr].tval, trap_set)) {
    hit_trap(&y,&x);
  } else if (t_list[cave[y][x].tptr].tval <= valuable_metal) {

    /*{ Attempt to pick up an object.         }*/
    if (inven_check_num()) {       /*{ Too many objects?     }*/
      if (inven_check_weight()) {  /*{ Weight limit check    }*/

        /*{ Okay, pick it up      }*/
        pusht(cave[y][x].tptr);
        cave[y][x].tptr = 0;

        if (inven_temp->data.tval == valuable_metal) {
          item_ptr   = money_carry();
          money_flag = true;
        } else {
          item_ptr   = inven_carry();
        }

        prt_weight();
        objdes(out_val,item_ptr,true);

        if (money_flag) {

          page_char = '$';
          inv_char  = '$';

        } else {

          count = 0;
          tmp_ptr = inventory_list;

          if (tmp_ptr->next == item_ptr->next) {
            count = 0;
          } else {
            do {
              count++;
              tmp_ptr = tmp_ptr->next;
            } while (tmp_ptr->next != item_ptr->next);
	  }

	  if ((count div 20) > 9) {
	    page_char = '*';
	    inv_char  = '*';
	  } else {
	    page_char = ((count div 20)+49);
	    inv_char  = (count - (count div 20)*20 + 97);
	  }

        }
	sprintf(out2, "You have %s. (%c%c)",out_val,page_char,inv_char);
	msg_print(out2);
      } else {
        msg_print("You can't carry that much weight.");
      }
    } else {
      msg_print("You can't carry that many items.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer react(integer x)
{
  /*  returns 0 to 10 -- SD 2.4; */
  /*  x is average reaction for a 0 SC ugly half-troll*/
  
  integer   ans;
  
  ans = (py.stat.c[CHR]+PM.rep*2+randint(200)+randint(200)
	 +randint(200)) div 50 + x - 4;
  
  if (ans < 0) {
    ans = 0;
  } else if (ans > 10) {
    ans = 10;
  }

  return ans;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void battle_game(integer plus, vtype kb_str)
{
  integer    score,i1,time;
  vtype      out_val;

  if (get_yes_no("Do you accept their invitation?")) {
    msg_print("Good for you!");
    score = 0;
    time  = 10;

    //with py.misc do;
    for (i1 = 1; i1 <= 7; i1++) {
      if (player_test_hit(PM.bth,PM.lev,plus,20*i1,false)) {
	score++;
	time = time * 2 + 10;
      }
    }

    sprintf(out_val, "with some %s", kb_str);
    spend_time(time,out_val,false);
    
    switch (score) {
    case 1 : 
      msg_print("They ridicule your clumsy performance...");
      msg_print("'Come back when you are more experienced!!'");
      change_rep(-2);
      break;
      
    case 2 : msg_print("You do not do well..."); break;
      
    case 3 : msg_print("'Pretty good for a beginner!'"); break;
      
    case 4 : msg_print("They are quite impressed!"); break;
      
    case 5 : 
      msg_print("They are amazed by your incredible prowess!");
      change_rep(2);
      break;
      
    case 6: case 7: 
      msg_print("You handle them all with ease!");
      msg_print("'Thanks for the workout! Come back anytime!!'");
      py.misc.exp += 10;
      change_rep(5);
      break;
      
    default:
      msg_print("'Boy that was quick!! What a little wimp!'");
      msg_print("They pummel you senseless and toss you out into the street!");
      take_hit(damroll("2d4"),kb_str);
      change_rep(-5);
      py.flags.confused += 5 + randint(5);
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void brothel_game()
{
  if (get_yes_no("Do you accept?")) {
    change_rep(-3);
    //with py.misc do;
    if ((PM.disarm + PM.lev + 2 * todis_adj() + spell_adj(INT))
	> randint(100)) {
      msg_print("Good! You are invited to join the house!");
      PM.exp += 5;
      spend_time(600,"putting out for peasants",false);
    } else {
      msg_print("You fail to please your customers.");
      spend_time(400,"imitating a pine board",false);
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void guild_or_not(boolean passed)
{
  if (passed) {
    spend_time(600,"showing off your skills",false);
    msg_print("Good! You are invited to join the guild!");
    py.misc.exp += 5;
    change_rep(-3);
  } else {
    spend_time(400,"or lack thereof",false);
    msg_print("You fail to impress them.");
    if (randint(3) == 1) {
      msg_print("They think you are with the guard!");
      msg_print("You are stabbed by one of them before you escape");
      take_hit(randint(randint(16)),"Thieves Guild Member");
      prt_hp();
    }
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void thief_games()
{
  if (randint(2) == 1) {
    msg_print("The thieves invite you to prove your ability to pick locks.");
    if (get_yes_no("Do you accept?")) {
      //with py.misc do;
      guild_or_not((PM.disarm + PM.lev + 2 * todis_adj() + spell_adj(INT)) > 
		   randint(100));
    }
  } else {
    msg_print("The thieves invite you to show your stealthiness.");
    if (get_yes_no("Do you accept?")) {
      guild_or_not(py.misc.stl > randint(12));
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void kicked_out()
{
  msg_print("The owner kicks you out...");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void call_guards(vtype who)
{
  vtype  out_str;

  sprintf(out_str, "The %s call(s) for the Town Guards!", who);
  msg_print(out_str);
  monster_summon_by_name(char_row,char_col,"Town Guard",true,false);
  monster_summon_by_name(char_row,char_col,"Town Guard",true,false);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void call_wizards()
{
  msg_print("The mage calls for a Town Wizard to remove you.");
  monster_summon_by_name(char_row,char_col,"Town Wizard",true,false);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void eat_the_meal()
{
  integer   yummers,old_food;
  
  old_food = py.flags.foodc;
  
  yummers  = react(randint(8)-2);
  if ((yummers==10) && (randint(2)==1)) {
    yummers = 15;
  }
  
  spend_time(50 + 50 * yummers,"eating like a pig",false);
  
  switch (yummers) {
  case 15 : 
    msg_print("It is a sumptuous banquet, and you feel quite stuffed.");
    py.flags.foodc   = PLAYER_FOOD_MAX;
    py.flags.status &= ~(IS_WEAK | IS_HUNGERY);
    prt_hunger();
    change_rep(3);
    break;
    
  case 6: case 7: case 8: case 9: case 10 :
    msg_print("It is an ample meal, and you feel full.");
    py.flags.foodc   = PLAYER_FOOD_FULL;
    py.flags.status &= ~(IS_WEAK | IS_HUNGERY);
    prt_hunger();
    change_rep(1);
    break;
    
  default:
    if ((yummers>0) || player_saves(py.misc.lev+5*spell_adj(CON))) {
      msg_print("It was a boring meal, and you eat very little.");
      py.flags.foodc = old_food;
      prt_hunger();
    } else {
      msg_print("Yuk!  That meal was AWFUL!");
      msg_print("You throw up!");
      if (py.flags.foodc > 150) {
	py.flags.foodc = 150;
      }
      msg_print("You get food poisoning.");
      py.flags.poisoned += randint(10) + 5;
      change_rep(-2);
    }
    break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void invite_for_meal()
{
  msg_print("The occupants invite you in for a meal.");
  if (get_yes_no("Do you accept?")) {
    eat_the_meal();
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void party()
{
  msg_print("The owner invites you to join the party!");
  if (get_yes_no("Do you accept?")) {
    spend_time(400+randint(1600),"at a party",false);
    
    switch (randint(6)) {
    case 1 : 
      msg_print("Someone must have spiked the punch!");
      msg_print("Oh, your aching head!");
      py.flags.confused += 25 + randint(25);
      break;
      
    case 2 : 
      msg_print("Gee, those brownies were awfully unusual....");
      msg_print("You feel a little strange now.");
      py.flags.image += 200 + randint(100);
      break;
      
    case 3 : 
      msg_print("You smoked something strange at the party.");
      switch (randint(2)) {
      case 1 : py.flags.hero   += 25 + randint(25); break;
      case 2 : py.flags.afraid += 25 + randint(25); break;
      }
      break;
      
    case 4: case 5: case 6:
      msg_print("It is an interesting party, and you enjoy yourself.");
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void spend_the_night(vtype who)
{
  vtype   out_str;

  msg_print("The occupant(s) invite you to rest in his house.");
  if (get_yes_no("Do you accept?")) {
    sprintf(out_str, "at the home of the %s.", who);
    spend_time(1,out_str,true);
    change_rep(2);
  } else if (get_yes_no("Okay, how about staying for a meal?")) {
    eat_the_meal();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void worship()
{
  integer   preachy,i1;

  msg_print("The priest invites you to participate in the service.");

  if (get_yes_no("Do you accept?")) {
    preachy = randint(4);

    switch (preachy) {
    case 1: msg_print("You sit through a fascinating church service.");  break;
    case 2: msg_print("You sit through an interesting church service."); break;
    case 3: msg_print("You sit through a boring church service.");       break;
    case 4: msg_print("You sit through a long, boring church service."); break;
    }
    
    spend_time(100*(randint(7)+preachy*preachy),"at the Church",false);
    
    msg_print("The priest asks for donations for a new church.");
    if (get_yes_no("Will you give him some money?")) {
      //with py.misc do;
      if (PM.money[TOTAL_] > 0) {
	msg_print("Bless you, dude!");

	i1 = ((randint(12)*PM.money[TOTAL_]) div 1000 + 20)*GOLD_VALUE;
	if (i1 > PM.money[TOTAL_]*GOLD_VALUE div 2) {
	  i1 = PM.money[TOTAL_]*GOLD_VALUE div 2;
	}

	subtract_money(i1,false);
	prt_weight();
	prt_gold();

	if (i1 > 20*GOLD_VALUE) {
	  change_rep(5);
	} else {
	  change_rep((i1+5*GOLD_VALUE-1) div (5*GOLD_VALUE));
	}

      } else {
	msg_print("He says 'It is the thought that counts, my child.'");
	msg_print("Thank you for being willing to give.");
      }
    } else {
      msg_print("Syo problem, man?");
      change_rep(-5);
    }
  } else if (react(6)==0) {
    msg_print("You heathen!  Get out of my temple!");
    change_rep(-5);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void beg_food() 
{
  /*
       var      i2              : integer;
                item_ptr        : treas_ptr;
       begin
        if (find_range([food],false,item_ptr,i2)) then
          begin
            msg_print("The occupants beg you for food.");
            if get_yes_no("Will you feed them?") then
              begin
                spend_time(200,"feeding people",false);
                msg_print("How kind of you!");
                inven_destroy(item_ptr);
                change_rep(5);
                prt_weight();
              end
            else
              begin
                msg_print("What a jerk!");
                change_rep(-10);
              end;
          end
        else
          beg_money();
       end;
       */
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void beg_money() 
{
  integer   i1;
  
  msg_print("The occupants beg you for money.");
  
  if (get_yes_no("Will you give them some?")) {
    //with py.misc do;
    if (PM.money[TOTAL_] > 0) {
      msg_print("How kind of you!");
      spend_time(100,"giving handouts",false);
      i1 = ((randint(12)*PM.money[TOTAL_]) div 1000 + 20)*GOLD_VALUE;
      if (i1 > PM.money[TOTAL_]*GOLD_VALUE div 2) {
	i1 = PM.money[TOTAL_]*GOLD_VALUE div 2;
      }
      subtract_money(i1,false);
      prt_weight();
      prt_gold();
      if (i1 > 20*GOLD_VALUE) {
	change_rep(5);
      } else {
	change_rep((i1+5*GOLD_VALUE-1) div (5*GOLD_VALUE));
      }
      prt_weight();
      prt_gold();
    } else {
      msg_print("They are disappointed because you have no money.");
    }
  } else {
    msg_print("What a jerk!");
    change_rep(-10); /*{bug fixed here; used to be 10 -- MAV }*/
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean player_test_hit(integer bth,integer level,integer pth,
				integer ac, boolean was_fired)
{
  integer  i1;
  boolean  return_value;

  if (search_flag) {
    search_off();
  }
  if (py.flags.rest > 0) {
    rest_off();
  }

  i1 = bth + pth*BTH_PLUS_ADJ;

  if (was_fired) {
    i1 += (level*class[py.misc.pclass].mbthb) div 2;
  } else {
    i1 += (level*class[py.misc.pclass].mbth) div 2;
  }

  if (randint(i1) > ac) {
    return_value = true;
  } else if (randint(20) == 1) {
    /*{ hits if above ac or 1 in 20.  OOK! }*/
    return_value = true;
  } else {
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean test_hit(integer bth,integer level,integer pth,integer ac)
{
  /*{ Attacker's level and pluses, defender's AC            -RAK-   }*/

  integer  i1;
  boolean  return_value;

  if (search_flag) {
    search_off();
  }
  if (py.flags.rest > 0) {
    rest_off();
  }

  i1 = bth + level*BTH_LEV_ADJ + pth*BTH_PLUS_ADJ;

  /*{ hits if above ac or 1 in 20.  OOK! }*/
  return_value = ((randint(i1) > ac) || (randint(20) == 1));

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean minus_ac(integer typ_dam)
{
  /*{ AC gets worse                                         -RAK-   }*/
  /*{ Note: This routine affects magical AC bonuse so that stores   }*/
  /*{       can detect the damage.                                  }*/

  integer   i1,i2;
  integer   tmp[9];  /*  : array [1..8] of integer;*/
  vtype     out_str,out_val;
  boolean   return_value = false;

  i1 = 0;
  if (equipment[Equipment_armor].tval > 0) {
    i1++;
    tmp[i1] = Equipment_armor;
  }
  if (equipment[Equipment_shield].tval > 0) {
    i1++;
    tmp[i1] = Equipment_shield;
  }
  if (equipment[Equipment_cloak].tval > 0) {
    i1++;
    tmp[i1] = Equipment_cloak;
  }
  if (equipment[Equipment_gloves].tval > 0) {
    i1++;
    tmp[i1] = Equipment_gloves;
  }
  if (equipment[Equipment_helm].tval > 0) {
    i1++;
    tmp[i1] = Equipment_helm;
  }
  if (equipment[Equipment_boots].tval > 0) {
    i1++;
    tmp[i1] = Equipment_boots;
  }
  if (equipment[Equipment_belt].tval > 0) {
    i1++;
    tmp[i1] = Equipment_belt;
  }
  if (equipment[Equipment_bracers].tval > 0) {
    i1++;
    tmp[i1] = Equipment_bracers;
  }
  
  if (i1 > 0) {
    i2 = tmp[randint(i1)];
    inven_temp->data = equipment[i2];
    //with equipment[i2] do;
    if ((equipment[i2].flags & typ_dam) != 0) {
      objdes(out_val,inven_temp,false);
      sprintf(out_str,"Your %s resists damage!",out_val);
      msg_print(out_str);
      return_value = true;
    } else if ((equipment[i2].ac+equipment[i2].toac) > 0) {
      objdes(out_val,inven_temp,false);
      sprintf(out_str,"Your %s is damaged!",out_val);
      msg_print(out_str);
      equipment[i2].toac--;
      py_bonuses(&blank_treasure,0);
      return_value = true;
    }
  }  

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void fire_dam(integer dam, vtype kb_str)
{
  /*{ Burn the fool up...                                   -RAK-   }*/

  obj_set   things_that_burn = {arrow,bow_crossbow_or_sling,hafted_weapon,
				pole_arm,maul,boots,gloves_and_gauntlets,
				Cloak,soft_armor,staff,scroll1,scroll2,0};
  
  if (py.flags.fire_resist) {
    dam /= 3;
  }
  if (py.flags.resist_heat > 0) {
    dam /= 3;
  }

  take_hit(dam,kb_str);

  print_stat |= 0x0080;

  if (inven_damage(things_that_burn,3) > 0) {
    msg_print("There is smoke coming from your pack!");
    prt_weight() ;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void cold_dam(integer dam, vtype kb_str)
{
  /*{ Freeze him to death...                                -RAK-   }*/

  obj_set   things_that_freeze = {potion1, potion2, 0};

  if (py.flags.cold_resist) {
    dam /= 3;
  }
  if (py.flags.resist_cold > 0) {
    dam /= 3;
  }

  take_hit(dam,kb_str);

  print_stat |= 0x0080;

  if (inven_damage(things_that_freeze,5) > 0) {
    msg_print("Something shatters inside your pack!");
    prt_weight() ;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void light_dam(integer dam, vtype kb_str)
{
  /*{ Lightning bolt the sucker away...                     -RAK-   }*/

  if (py.flags.lght_resist) {
    dam /= 3;
  }
  if (py.flags.resist_lght > 0) {
    dam /= 3;
  }

  take_hit(dam,kb_str);

  print_stat |= 0x0080;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void acid_dam(integer dam, vtype kb_str)
{
  /*{ Throw acid on the hapless victim                      -RAK-   }*/

  integer   flag = 0;
  obj_set   things_that_dilute = {miscellaneous_object,chest,bolt,arrow,
				  bow_crossbow_or_sling,hafted_weapon,
				  pole_arm,boots,gloves_and_gauntlets,
				  Cloak,soft_armor,0};

  if (minus_ac(Resist_Acid_worn_bit)) {
    flag = 1;
  }
  if (py.flags.acid_resist) {
    flag += 2;
  }

  switch (flag) {
  case 0 : take_hit((dam / 1),kb_str);   break;
  case 1 : take_hit((dam / 2),kb_str);   break;
  case 2 : take_hit((dam / 3),kb_str);   break;
  case 3 : take_hit((dam / 4),kb_str);   break;
  }

  print_stat |= 0x00C0;

  if (inven_damage(things_that_dilute,3) > 0) {
    msg_print("There is an acrid smell coming from your pack!");
    prt_weight() ;
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void xp_loss(integer amount)
{
  /*{ Lose experience hack for lose_exp breath              -RAK-   }*/

  integer    i1,i2;
  integer    av_hp,lose_hp;
  integer    av_mn,lose_mn;
  boolean    flag;

  amount = (py.misc.exp / 100) * MON_DRAIN_LIFE;     /* passed val?  XXXX */

  //with py.misc do;
  msg_print("You feel your life draining away!");
  if (amount > PM.exp) {
    PM.exp = 0;
  } else {
    PM.exp -= amount;
  }

  for (i1=1;(integer)(player_exp[i1]*PM.expfact) <= PM.exp;) {
    i1++;
  }
  i2 = PM.lev - i1;

  for (;i2 > 0;) {
    av_hp = (PM.mhp/PM.lev);
    av_mn = (PM.mana/PM.lev);
    PM.lev--;
    i2--;
    lose_hp = randint(av_hp*2-1);
    lose_mn = randint(av_mn*2-1);
    PM.mhp  -= lose_hp;
    PM.mana -= lose_mn;
    if (PM.mhp  < 1) { PM.mhp  = 1;}
    if (PM.mana < 0) { PM.mana = 0;}
    
    //with class[PM.pclass]. do;
    if (class[PM.pclass].mspell || class[PM.pclass].pspell || 
	class[PM.pclass].dspell || class[PM.pclass].bspell || 
	class[PM.pclass].mental) {
      i1 = 32;
      flag = false;
      do {
	i1--;
	if (magic_spell[pclass][i1].learned) {
	  flag = true;
	}
      } while (!((flag) || (i1 < 2)));
      if (flag) {
	magic_spell[pclass][i1].learned = false;
	if (class[PM.pclass].mspell) {
	  msg_print("You have forgotten a magic spell!");
	} else if (class[PM.pclass].pspell) {
	  msg_print("You have forgotten a prayer!");
	} else if (class[PM.pclass].bspell) {
	  msg_print("You have forgotten a song!");
	} else {
	  msg_print("You have forgotten a discipline!");
	}
      }
    }
  }
  
  if (PM.chp   > PM.mhp)  { PM.chp   = PM.mhp;}
  if (PM.cmana > PM.mana) { PM.cmana = PM.mana;}
  
  strcpy(PM.title, player_title[PM.pclass][PM.lev]);
  
  prt_experience();
  prt_hp();
  if (is_magii) {
    prt_mana();
  }
  prt_level();
  prt_title();

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void corrode_gas(vtype kb_str)
{
  /*{ Corrode the unsuspecting person's armor               -RAK-   }*/

  obj_set   things_that_corrode = {dagger,sword,gem_helm,helm,shield,
				   hard_armor,wand,0};

  if (!(minus_ac(Resist_Acid_worn_bit))) {
    /* if nothing corrodes then take damage */
    take_hit(randint(8),kb_str);
  }

  print_stat |= 0x0040;

  if (inven_damage(things_that_corrode,5) > 0) {
    msg_print("There is an acrid smell coming from your pack.");
    prt_weight();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void poison_gas(integer dam, vtype kb_str)
{
  /*{ Poison gas the idiot...                               -RAK-   }*/

  take_hit(dam,kb_str);
  print_stat |= 0x0040;
  py.flags.poisoned += 12 + randint(dam);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean  no_light()
{
  /*{ Returns true if player has no light                   -RAK-   }*/

  boolean    return_value = false;
  
  //with cave[char_row,char_col] do;
  if (!(cave[char_row][char_col].tl)) {
    if (!(cave[char_row][char_col].pl)) {
      return_value = true;
    }
  }
  return return_value;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void rest()
{
  /*{ Resting allows a player to safely restore his hp      -RAK-   }*/

  integer    rest_num;
  vtype      rest_str;

  prt("Rest for how long? ",1,1);
  get_string(rest_str,1,20,10);
  rest_num = 0;
  sscanf(rest_str,"%ld",&rest_num);

  if (rest_num > 0) {
    if (search_flag) {
      search_off();
    }
    py.flags.rest    = rest_num;
    turn_counter    += rest_num;
    py.flags.status |= IS_RESTING;
    prt_rest();
    //with py.flags do;
    PF.food_digested--;
    msg_print("Press any key to wake up...");
    put_qio();
  } else {
    erase_line(msg_line,msg_line);
    reset_flag = true;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void water_move_player()
{
  /* I wonder what was going to go in here... */
};

boolean water_move_creature(integer num)
{
  /* Gee, another cool routine... */

  return true;
};

boolean water_move_item(integer row,integer col,integer num)
{
  /* I sense a patter about water moves... */
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean water_move()
{
  integer    i1;
  boolean    flag = false;

  /*  this was all commented out, so I left it that way XXXX 
{       for i1 := 1 to max_height do
          for i2 := 1 to max_width do
            cave[i1,i2].moved := false;
        for i1 := 1 to max_height do
          for i2 := 1 to max_width do
            begin
              if (cave[i1,i2].tptr > 0) and
                 (t_list[cave[i1,i2].tptr].tval < unseen_trap) then
                begin
                  flag := water_move_item(i1,i2,cave[i1,i2].tptr);
                end;
            end;
}
*/
  water_move_player();

  for (i1 = muptr; i1 != 0;) {
    m_list[i1].moved = false;
    i1 = m_list[i1].nptr;
  }

  for (i1 = muptr; i1 != 0;) {
    flag = water_move_creature(i1);
    i1   = m_list[i1].nptr;
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void search(integer y, integer x, integer chance)
{
  /*{ Searches for hidden things...                         -RAK-   }*/

  integer    i1,i2;
  vtype      out_val;

  //with py.flags do;
  if (PF.confused+PF.blind > 0) {
    chance = trunc(chance/10.0);
  } else if (no_light()) {
    chance = (integer)(chance/5.0);
  }

  for (i1 = (y - 1); i1 <= (y + 1); i1++) {
    for (i2 = (x - 1); i2 <= (x + 1); i2++) {
      if (in_bounds(i1,i2)) {
	if ((i1 != y) || (i2 != x)) {
	  if (randint(100) < chance) {
	    //with cave[i1][i2]. do;

	    /*{ Search for hidden objects             }*/
	    if (cave[i1][i2].tptr > 0) {
	      //with t_list[tptr] do;
	      
	      /*{ Trap on floor?                }*/
	      if (t_list[cave[i1][i2].tptr].tval == Unseen_trap) {
		sprintf(out_val, "You have found %s.",
			t_list[cave[i1][i2].tptr].name);
		msg_print(out_val);
		change_trap(i1,i2);
		find_flag = false;

		/*{ Secret door?                  }*/
	      } else if (t_list[cave[i1][i2].tptr].tval == Secret_door) {
		msg_print("You have found a secret door.");
		cave[i1][i2].fval = corr_floor2.ftval;
		change_trap(i1,i2);
		find_flag = false;

		/*{ Chest is trapped?             }*/
	      } else if (t_list[cave[i1][i2].tptr].tval == chest) {
		if (t_list[cave[i1][i2].tptr].flags > 1) {
		  if (pindex(t_list[cave[i1][i2].tptr].name,'^') > 0) {
		    known2(t_list[cave[i1][i2].tptr].name);
		    msg_print("You have discovered a trap on the chest!");
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void  lr__find_light(integer y1,integer x1,integer y2,integer x2)
{

  integer    i1,i2,i3,i4;
  obj_set    something_nifty = {1,2,17,0};

  for (i1 = y1; i1 <= y2; i1++) {
    for (i2 = x1; i2 <= x2; i2++) {
      if (is_in(cave[i1][i2].fval, something_nifty)) {
	for (i3 = i1-1; i3 <= i1+1; i3++) {
	  for (i4 = i2-1; i4 <= i2+1; i4++) {
	    cave[i3][i4].pl = true;
	  }
	}
	if (cave[i1][i2].fval == 17) {
	  cave[i1][i2].fval = 18;
	} else {
	  cave[i1][i2].fval = 2;
	}
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void light_room(integer y, integer x)
{
  /*{ Room is lit, make it appear                           -RAK-   }*/

  integer    tmp1,tmp2;
  integer    start_row,start_col;
  integer    end_row,end_col;
  integer    i1,i2;
  integer    ypos,xpos=0;
  vtype      floor_str;
  integer    floor_str_len;

  tmp1       = (integer)(SCREEN_HEIGHT/2);
  tmp2       = (integer)(SCREEN_WIDTH /2);
  start_row  = (integer)(y/tmp1)*tmp1 + 1;
  start_col  = (integer)(x/tmp2)*tmp2 + 1;
  end_row    = start_row + tmp1 - 1;
  end_col    = start_col + tmp2 - 1;

  lr__find_light(start_row,start_col,end_row,end_col);

  for (i1 = start_row; i1 <= end_row; i1++) {
    floor_str[0]  = 0;
    floor_str_len = 0;
    ypos = i1;
    for (i2 = start_col; i2 <= end_col; i2++) {
      //with cave[i1,i2] do;
      if ((cave[i1][i2].pl) || (cave[i1][i2].fm)) {
	if (floor_str_len == 0) {
	  xpos = i2;
	}
	floor_str[floor_str_len++] = loc_symbol(i1,i2);
      } else {
	if (floor_str_len > 0) {
	  floor_str[floor_str_len] = 0;
	  print_str(floor_str,ypos,xpos);
	  floor_str[0]  = 0;
	  floor_str_len = 0;
	}
      }
    }
    if (floor_str_len > 0) {
      floor_str[floor_str_len] = 0;
      print_str(floor_str,ypos,xpos);
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean pick_dir(integer dir)
{
  /*{ Picks new direction when in find mode                 -RAK-   }*/

  integer  z[3];     /*   : array [1..2] of integer;*/
  integer  i1,y,x;
  boolean  return_value;

  if ((find_flag) && (next_to4(char_row,char_col,corr_set) == 2)) {

    switch (dir) {
    case 1: case 3: case 7: case 9: 
      z[1] = rotate_dir(dir,-1);
      z[2] = rotate_dir(dir,1);
      break;
      
    case 2: case 4: case 6: case 8: 
      z[1] = rotate_dir(dir,-2);
      z[2] = rotate_dir(dir,2);
      break;
    }

    return_value = false;

    for (i1 = 1; i1 <= 2; i1++) {
      y = char_row;
      x = char_col;
      if (move_dir(z[i1],&y,&x)) {
	if (cave[y][x].fopen) {
	  return_value = true;
	  com_val = z[i1] + 48;
	}
      }
    }
  } else {
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bother(integer num)
{
  if (num > 5) {
    msg_print("Your sword screams insults at passing monsters!");
  } else {
    msg_print("Your sword loudly shouts to all nearby creatures,");
    switch (num) {
    case 1 :
      msg_print("What kinda monsters are you, mice -- or giant mice???");
      break;
    case 2 :
      msg_print("You pathetic creatures are not worth tarnishing my blade!");
      break;
    case 3 :
      msg_print("Attention all monsters:  SUPPERTIME!!!");
      break;
    case 4 :
      msg_print("Boy are we wounded!! Sure hope we don't run into a kobold!");
      break;
    case 5 :
      msg_print("Now where did I misplace my armor?  Hmmm...");
      break;
    }
  }

  if (aggravate_monster(20)) {
    msg_print("You hear the sounds of movement in the distance!");
  }
  
  msg_print(" ");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void area_affect(integer dir, integer y, integer x)
{
  /*        { Turns off Find_flag if something interesting appears  -RAK-   }*/
  /*        { BUG: Does not handle corridor/room corners, but I didn't want }*/
  /*        {      to add a lot of checking for such a minor detail         }*/

  integer    z[4];        /*: array [1..3] of integer;*/
  integer    i1,row,col;
  obj_set    fourfivesix = {4, 5, 6, 0};
  obj_set    some_hidden_stuff = {Unseen_trap,Secret_door, 0};

  if (cave[y][x].fval == 4) {
    i1 = 0;
    if (next_to4(y,x,fourfivesix) > 2) {
      find_flag = false;
    }
  }

  if ((find_flag) && (py.flags.blind < 1)) {

    switch (dir) {
    case 1: case 3: case 7: case 9:
      z[1] = rotate_dir(dir,-1);
      z[2] = dir;
      z[3] = rotate_dir(dir,1);
      break;
      
    case 2: case 4: case 6: case 8:
      z[1] = rotate_dir(dir,-2);
      z[2] = dir;
      z[3] = rotate_dir(dir,2);
      break;
    }
    
    for (i1 = 1; i1 <= 3; i1++) {
      row = y;
      col = x;
      if (move_dir(z[i1],&row,&col)) {

	//with cave[row,col] do;

	/* { Empty doorways        }*/
	if (cave[row][col].fval == 5) {
	  find_flag = false;
	}

	/*  { Objects player can see}*/
	/*  { Including doors       }*/
	if (find_flag) {
	  if (player_light) {
	    if (cave[row][col].tptr > 0) {
	      if (!(is_in(t_list[cave[row][col].tptr].tval, 
			  some_hidden_stuff))) {
		find_flag = false;
	      }
	    }
	  } else if ((cave[row][col].tl) ||
		     (cave[row][col].pl) ||
		     (cave[row][col].fm)) {
	    if (cave[row][col].tptr > 0) {
	      if (!(is_in(t_list[cave[row][col].tptr].tval, 
			  some_hidden_stuff))) {
		find_flag = false;
	      }
	    }
	  }
	}
	
	/*{ Creatures             }*/
	if (find_flag) {
	  if ((cave[row][col].tl) || (cave[row][col].pl) || (player_light)) {
	    if (cave[row][col].cptr > 1) {
	      //with m_list[cave[row][col].cptr] do;
	      if (m_list[cave[row][col].cptr].ml) {
		find_flag = false;
	      }
	    }
	  }
	}

      } /* end if move_char */
    } /* end for */
  } /* end if find and not blind */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean delete_object(integer y,integer x)
{
  /*{ Deletes object from given location                    -RAK-   }*/

  boolean    return_value = false;

  //with cave[y,x] do;
  if (t_list[cave[y][x].tptr].tval == Secret_door) {
    cave[y][x].fval = corr_floor3.ftval;
  }
  cave[y][x].fopen = true;
  pusht(cave[y][x].tptr);
  cave[y][x].tptr = 0;
  cave[y][x].fm = false;
  if (test_light(y,x)) {
    lite_spot(y,x);
    return_value = true;
  } else {
    unlite_spot(y,x);
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer mon_take_hit(integer monptr,integer dam)
{
  /*{ Decreases monsters hit points and deletes monster if needed.  }*/
  /*{ (Picking on my babies...)                             -RAK-   }*/

  real      acc_tmp;
  integer   i1 = 0;
  integer   return_value = 0;

  ENTER("mon_take_hit","d");

  //with m_list[monptr]. do;
  m_list[monptr].hp -= dam;
  m_list[monptr].csleep = 0;
  if (m_list[monptr].hp < 0) {

    monster_death(m_list[monptr].fy,m_list[monptr].fx,
		  c_list[m_list[monptr].mptr].cmove);
    
    if ((m_list[monptr].mptr == py.misc.cur_quest) && (PF.quested)) {
      PF.quested = false;
      prt_quested();
      msg_print("*** QUEST COMPLETED ***");
      msg_print("Return to the surface and report to the Arch-Wizard.");
    }
    
    //with c_list[m_list[monptr].mptr]. do;
    //with py.misc do;
    if (((c_list[m_list[monptr].mptr].cmove & 0x00004000) == 0) && 
	(c_list[m_list[monptr].mptr].mexp > 0)) {

      acc_tmp = (c_list[m_list[monptr].mptr].mexp * 
		 ((c_list[m_list[monptr].mptr].level+0.1)/(real)PM.lev));
      i1 = (integer)(acc_tmp);
      acc_exp += (acc_tmp - i1);
      if (acc_exp > 1) {
	i1++;
	acc_exp -= 1.0;
      }
      PM.exp += i1;

    } else if (c_list[m_list[monptr].mptr].mexp > 0) {

      change_rep(-c_list[m_list[monptr].mptr].mexp);
      if (py.misc.rep > -250) {
	msg_print("The townspeople look at you sadly.");
	msg_print("They shake their heads at the needless violence.");
      } else if (py.misc.rep > -1000) {
	monster_summon_by_name(char_row,char_col,"Town Guard",true,false);
	msg_print("The townspeople call for the guards!");
      } else if (py.misc.rep > -2500) {
	monster_summon_by_name(char_row,char_col,"Town Wizard",true,false);
	msg_print("A Town Wizard appears!");
      } else {
	msg_print("Your god disapproves of your recent town killing spree.");
	msg_print("Unlike the townspeople, he can do something about it.");
	msg_print(" ");
	strcpy(died_from, "The Wrath of God");
	upon_death();
      }
    }

    return_value = m_list[monptr].mptr;
    delete_monster(monptr);
    if (i1 > 0) {
      prt_experience();
    }

  } else {
    return_value = 0;
  }

  RETURN("mon_take_hit", "d", 'd',"monval", &return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void monster_death(integer y,integer x, unsigned long flags)
{
  /*{ Allocates objects upon a creatures death              -RAK-   }*/
  /*{ Oh well, another creature bites the dust...  Reward the victor}*/
  /*{ based on flags set in the main creature record                }*/

  integer   i1;

  i1 = (integer)((flags & 0x03000000) / (0x01000000));

  if (uand(flags,0x04000000) != 0) {
    if (randint(100) < 60) {
      summon_object(y,x,1,i1);
    }
  }

  if (uand(flags,0x08000000) != 0) {
    if (randint(100) < 90) {
      summon_object(y,x,1,i1);
    }
  }
  
  if (uand(flags,0x10000000) != 0) {
    summon_object(y,x,randint(2),i1);
  }
  if (uand(flags,0x20000000) != 0) {
    summon_object(y,x,damroll("2d2"),i1);
  }
  if (uand(flags,0x40000000) != 0) {
    summon_object(y,x,damroll("4d3"),i1);
  }
  if (uand(flags,0x80000000) != 0) {
    total_winner = true;
    prt_winner();
    msg_print("*** CONGRATULATIONS *** You have won the game...");
    msg_print("Use '@' when you are ready to quit.");
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void summon_object(integer y,integer x,integer num,integer typ)
{
  /*{ Creates objects nearby the coordinates given          -RAK-   }*/
  /*{ BUG: Because of the range, objects can actually be placed into}*/
  /*{      areas closed off to the player, this is rarely noticable,}*/
  /*{      and never a problem to the game.                         }*/

  integer   i1,i2,i3;

  do {
    i1 = 0;
    do {
      i2 = y - 3 + randint(5);
      i3 = x - 3 + randint(5);
      if (in_bounds(i2,i3)) {
	if (los(y,x,i2,i3)) {     /*{OOK!}*/
	  //with cave[i2][i3]. do;
	  if (is_in(cave[i2][i3].fval, floor_set)) {
	    if (cave[i2][i3].tptr == 0) {

	      switch (typ) {   /*{ Select type of object }*/
	      case 1 :  
		place_object(i2,i3);
		break;

	      case 2 :  
		place_gold(i2,i3);
		break;

	      case 3 :  
		if (randint(100) < 50) {
		  place_object(i2,i3);
		} else {
		  place_gold(i2,i3);
		}
		break;

	      default:
		break;
	      }
	      if (test_light(i2,i3)) {
		lite_spot(i2,i3);
	      }
	      i1 = 10;
	    }
	  }
	}
      }
      i1++;
    } while (!(i1 > 10));
    num--;
  } while (num != 0);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void delete_monster(integer i2)
{
  /*{ Deletes a monster entry from the level                -RAK-   }*/

  integer   i1,i3;

  ENTER("delete_monster","c");

  i3 = m_list[i2].nptr;
  if (muptr == i2) {
    muptr = i3;
  } else {
            
    for (i1 = muptr; m_list[i1].nptr != i2;) {
      i1 = m_list[i1].nptr;
    }
    m_list[i1].nptr = i3;
  }

  //with m_list[i2]. do;
  cave[m_list[i2].fy][m_list[i2].fx].cptr = 0;
  if (m_list[i2].ml) {
    //with cave[fy][fx]. do;
    if ((cave[m_list[i2].fy][m_list[i2].fx].pl) || 
	(cave[m_list[i2].fy][m_list[i2].fx].tl)) {
      lite_spot(m_list[i2].fy,m_list[i2].fx);
    } else {
      unlite_spot(m_list[i2].fy,m_list[i2].fx);
    }
  }
  pushm(i2);
  
  mon_tot_mult--;
  
  LEAVE("delete_monster","c");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean py_attack(integer y, integer x)
{
  /*{ Player attacks a (poor, defenseless) creature         -RAK-   }*/

  integer   a_cptr,a_mptr,i3,blows,tot_tohit,crit_mult;
  vtype     m_name,out_val;
  boolean   mean_jerk_flag,is_sharp;
  boolean   backstab_flag;

  obj_set   mages_suck    = {hafted_weapon,pole_arm,sword,maul,0};
  obj_set   rangers_suck  = {hafted_weapon,pole_arm,dagger,sword,0};
  obj_set   druids_suck   = {hafted_weapon,pole_arm,sword,0};
  obj_set   monks_suck    = {hafted_weapon,pole_arm,maul,0};
  obj_set   catch_this    = {sling_ammo,bolt,arrow,0};

  boolean   return_value = false;

  ENTER("py_attack","d");

  a_cptr = cave[y][x].cptr;
  a_mptr = m_list[a_cptr].mptr;

  if ((PM.pclass == C_ROGUE) && (m_list[a_cptr].csleep != 0)) {
    backstab_flag = true;
  } else {
    backstab_flag = false;
  }
  m_list[a_cptr].csleep = 0;
  
  find_monster_name(m_name,a_cptr,false);

  if (equipment[Equipment_primary].tval > 0) {   /*  { Proper weapon }*/
    blows = attack_blows(equipment[Equipment_primary].weight,&tot_tohit);
  } else {                                   /*{ Bare hands?   }*/
    if (PM.pclass == C_MONK) {
      blows = attack_blows(12000,&tot_tohit) + 1; /* a bit heavy handed... */
      tot_tohit = 0;
    } else {
      blows = 2;
      tot_tohit = -3;
    }
  }

  if (backstab_flag) {
    tot_tohit += (PM.lev / 4);
  }

  /*{ Adjust weapons for class }*/
  if (PM.pclass == C_WARRIOR) {
    tot_tohit += 1 + (PM.lev / 2);

  } else if ((PM.pclass == C_MAGE) &&
	     (is_in(equipment[Equipment_primary].tval, mages_suck))) {
    tot_tohit -= 5;

  } else if ((PM.pclass == C_RANGER) && 
	     (is_in(equipment[Equipment_primary].tval, rangers_suck))) {
    tot_tohit -= 4;

  } else if ((PM.pclass == C_DRUID) &&
	     (is_in(equipment[Equipment_primary].tval, druids_suck))) {
    tot_tohit -= 4;

  } else if ((PM.pclass == C_MONK) &&
	     (is_in(equipment[Equipment_primary].tval, monks_suck))) {
    tot_tohit -= 3;
  }

  /*{ Fix for arrows}*/
  if (is_in(equipment[Equipment_primary].tval, catch_this)) {
    blows = 1;
  }
  tot_tohit += PM.ptohit;

  /*{ stopped from killing town creatures?? }*/
  if (((c_list[a_mptr].cmove & 0x00004000)==0) || (randint(100) < -PM.rep)) {
    mean_jerk_flag = true;
  } else {
    mean_jerk_flag = get_yes_no("Are you sure you want to?");
  }

  /*{ Loop for number of blows, trying to hit the critter...        }*/
  if (mean_jerk_flag) {
    //with py.misc do;
    do {
      if (player_test_hit(PM.bth,PM.lev,tot_tohit,c_list[a_mptr].ac,false)) {
	if (backstab_flag) {
	  sprintf(out_val,"You backstab %s!",m_name);
	} else {
	  sprintf(out_val,"You hit %s.",m_name);
	}
	msg_print(out_val);
	//with equipment[Equipment_primary]. do;
	/*{ Weapon?       }*/
	if (equipment[Equipment_primary].tval > 0) {          
	  i3 = damroll(equipment[Equipment_primary].damage);
	  i3 = tot_dam(&equipment[Equipment_primary],i3,&c_list[a_mptr]);
	  is_sharp = (equipment[Equipment_primary].tval != 
		      bow_crossbow_or_sling) &&
	    ((equipment[Equipment_primary].flags2 & Sharp_worn_bit) != 0);
	  crit_mult = critical_blow(equipment[Equipment_primary].weight,
				    tot_tohit,is_sharp,false);
	  if (backstab_flag) {
	    i3 *= ((PM.lev div 7) + 1);
	  }
	  if (PM.pclass == C_WARRIOR) {
	    i3 += (PM.lev div 3);
	  }
	  i3 += (i3 + 5) * crit_mult;
	  
	} else {                        /*{ Bare hands!?  }*/
	  if (PM.pclass == C_MONK) {
	    i3 = randint((4 + 2*PM.lev) div 3);
	    crit_mult = critical_blow(12000,0,false,false);
	    if (randint(crit_mult+2) > 2) {
	      do_stun(a_cptr,-10,2);
	    }
	    i3 += (i3 + 5) * crit_mult;
	  } else {
	    i3 = damroll(bare_hands);
	    crit_mult = critical_blow(1,0,false,false);
	    i3 += (i3 + 5) * crit_mult;
	  }
	}
	
	i3 += PM.ptodam;
	if (i3 < 0) {
	  i3 = 0;
	}
	/*{ See if we done it in...                               }*/
	//with m_list[a_cptr]. do;
	if (mon_take_hit(a_cptr,i3) > 0) {
	  sprintf(out_val,"You have slain %s.",m_name);
	  msg_print(out_val);
	  blows = 0;
	  return_value = false;
	} else {
	  return_value = true;  /*{ If creature hit, but alive...}*/
	}
	
	//with equipment[Equipment_primary]. do;
	/*{ Use missiles up}*/
	if (is_in(equipment[Equipment_primary].tval, catch_this)) {
	  equipment[Equipment_primary].number--;
	  if (equipment[Equipment_primary].number <= 0) {
	    inven_weight -= equipment[Equipment_primary].weight;
	    prt_weight();
	    equip_ctr--;
	    inven_temp->data = equipment[Equipment_primary];
	    equipment[Equipment_primary] = blank_treasure;
	    py_bonuses(&(inven_temp->data),-1);
	  }
	}
      } else {
	sprintf(out_val,"You miss %s.",m_name);
	msg_print(out_val);
      }
      blows--;
    } while (blows >= 1);
  }    

  RETURN("py_attack", "",'b',"hit",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer tot_dam(treasure_type *item,integer tdam,creature_type *monster)
{
  /*{ Special damage due to magical abilities of object     -RAK-   }*/
  
  obj_set  stuff_that_goes_thump =
  {sling_ammo,bolt,arrow,lamp_or_torch,bow_crossbow_or_sling,
   hafted_weapon,pole_arm,dagger,sword,maul,flask_of_oil,0};

  unsigned long cdefense, flags, flags2;  

  cdefense = monster->cdefense;
  flags    = item->flags;
  flags2   = item->flags2;

  //with item do;
  if (is_in(item->tval, stuff_that_goes_thump)) {
    //with monster do;

    /*{ Slay Dragon   }*/
    if ((uand(cdefense,0x0001) != 0) &&
	(uand(flags,Slay_Dragon_worn_bit) != 0)) {
      tdam *= 4;

      /*{ Slay Undead   }*/
    } else if ((uand(cdefense,0x0008) != 0) &&
	       (uand(flags,Slay_Undead_worn_bit) != 0)) {
      tdam *= 3;

      /*{ Demon Bane    }*/
    } else if ((uand(cdefense,0x0400) != 0) &&
	       (uand(flags2,Slay_demon_worn_bit) != 0)) {
      tdam *= 3;

      /*{ Slay Regenerative }*/
    } else if ((uand(cdefense,0x8000) != 0) &&
	       (uand(flags2,Slay_regen_worn_bit) != 0)) {
      tdam *= 3;

      /*{ Slay Monster  }*/
    } else if ((uand(cdefense,0x0002) != 0) &&
	       (uand(flags,Slay_Monster_worn_bit) != 0)) {
      tdam *= 2;

      /*{ Slay Evil     }*/
    } else if ((uand(cdefense,0x0004) != 0) &&
	       (uand(flags,Slay_Evil_worn_bit) != 0)) {
      tdam *= 2;

      /*{ Soul Sword    }*/
    } else if ((!(uand(cdefense,0x0008) != 0)) &&
	       (uand(flags2,Soul_Sword_worn_bit) != 0)) {
      tdam *= 2;
    }
      /*{ Frost         }*/
    } else if ((uand(cdefense,0x0010) != 0) &&
	       (uand(flags,Cold_Brand_worn_bit) != 0)) {
      tdam *= 1.5;

      /*{ Fire          }*/
    } else if ((uand(cdefense,0x0020) != 0) &&
	       (uand(flags,Flame_Brand_worn_bit) != 0)) {
      tdam *= 1.5;

  }
  
  return tdam;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void get_player_move_rate()
{
  integer   cur_swim;

  //with py.flags do;
  if (is_in(cave[char_row][char_col].fval, earth_set)) {
    PF.move_rate = 4;
  } else {
    cur_swim = ((PF.swim + randint(5) - 1) / 5);

    if (cur_swim <= -2)      { PF.move_rate = 0;}
    else if (cur_swim == -1) { PF.move_rate = 1;}
    else if (cur_swim == 0)  { PF.move_rate = 2;}
    else if (cur_swim == 1)  { PF.move_rate = 4;}
    else                     { PF.move_rate = 8;}
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean xor(integer thing1, integer thing2)
{
  /* with fake boolean values you cant really do a (bool1 != bool2)
     and expect it to work.  */
  
  return !((thing1 && thing2) || (!thing1 && !thing2));
};
//////////////////////////////////////////////////////////////////////
integer movement_rate(integer cspeed,integer mon)
{
  /*{ Given speed, returns number of moves this turn.       -RAK-   }*/
  /*{ NOTE: Player must always move at least once per iteration,    }*/
  /*{       a slowed player is handled by moving monsters faster    }*/

  integer    final_rate;     /*{ final speed as integer }*/
  integer    c_rate,py_rate; /*{ rate (0,1,2,3) = (0,1/4,1/2,1)
			       _                in wrong element }*/
  integer    return_value;

  //with m_list[mon] do;
  //with c_list[mptr] do;
  //with cave[fy,fx] do;
  if (xor( is_in(cave[MY(mon)][MX(mon)].fval, earth_set), 
	   (uand(c_list[m_list[mon].mptr].cmove,0x00000010) == 0))) {
    c_rate =(integer)(uand(c_list[m_list[mon].mptr].cmove,0x00000300) div 256);
  } else {
    c_rate = 3;
  }

  if (c_rate == 3) {
    c_rate = 4;  /* I wish I knew why they did this... rounding up? */
  }

  py_rate = py.flags.move_rate;
  
  if (cspeed > 0) {
    c_rate  *= cspeed;
  } else {
    py_rate *= (2-cspeed);
  }

  final_rate = c_rate / py_rate;
  
  if (((c_rate * turn) % py_rate) < (c_rate % py_rate)) {
    final_rate++;
  }
  
  /* { if player resting, max monster move = 1 } */
  if ((final_rate > 1) && (py.flags.rest > 0)) {
    return_value = 1;
  } else {
    return_value = final_rate;
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void desc_remain(treas_ptr item_ptr)
{
  /*{ Describe amount of item remaining...                  -RAK-   }*/

  vtype   out_val,out_val2;

  inven_temp->data = item_ptr->data;

  //with inven_temp->data do;

  inven_temp->data.number--;
  objdes(out_val,inven_temp,true);
  sprintf(out_val2,"You have %s.",out_val);
  msg_print(out_val2);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void add_food(integer num)
{
  /*{ Add to the players food time                          -RAK-   }*/

  //with py.flags do;
  if (PF.foodc < 0) {
    PF.foodc = 0;
  }
  PF.foodc += num;

  if (PF.foodc > PLAYER_FOOD_FULL) {
    msg_print("You are full.");
  }

  if (PF.foodc > PLAYER_FOOD_MAX) {
    msg_print("You're getting fat from eating so much.");
    PF.foodc = PLAYER_FOOD_MAX;
    py.misc.wt += trunc(py.misc.wt*0.1);
    if (py.misc.wt > max_allowable_weight()) {
      msg_print ( "Oh no...  Now you've done it." ) ;
      death = true ;
      moria_flag = true ;
      total_winner = false ;
      strcpy(died_from,"gluttony");
    } else {
      switch (randint(3)) {
      case 1 : msg_print ("Buuurrrppppp !" ) ; break;
      case 2 : msg_print ("Remember, obesity kills." ) ; break;
      case 3 : msg_print ("Your armor doesn't seem to fit too well anymore.");
	break;
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean twall(integer y, integer x, integer t1, integer t2)
{
  /*{ Tunneling through real wall: 10,11,12                 -RAK-   }*/
  /*{ Used by TUNNEL and WALL_TO_MUD                                }*/

  obj_set  some_walls = {1,2,0};
  boolean  return_value = false;

  //with cave[y][x]. do;
  if (t1 > t2) {
    if (next_to4(y,x,some_walls) > 0) {
      cave[y][x].fval  = corr_floor2.ftval;
      cave[y][x].fopen = corr_floor2.ftopen;
    } else {
      cave[y][x].fval  = corr_floor1.ftval;
      cave[y][x].fopen = corr_floor1.ftopen;
    }
    
    if (test_light(y,x)) {
      if (panel_contains(y,x)) {
	if (cave[y][x].tptr > 0) {
	  msg_print("You have found something!");
	}
	lite_spot(y,x);
      }
    }
    
    cave[y][x].fm = false;
    cave[y][x].pl = false;
    return_value = true;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void desc_charges(treas_ptr item_ptr)
{
  /*{ Describe number of remaining charges...               -RAK-   }*/

  vtype      out_val;

  if (strstr(item_ptr->data.name,"^") == NULL) {
    sprintf(out_val,"You have %ld charges remaining.",item_ptr->data.p1);
    msg_print(out_val);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean cast_spell(vtype prompt, treas_ptr item_ptr, integer *sn, integer *sc,
		   boolean *redraw)
{
  /*{ Return spell number and failure chance                -RAK-   }*/

  unsigned long   i2, i4;
  integer         i1,i3,num;
  spl_type        aspell;
  boolean         flag = false;

  i1 = num = 0;
  i2 = item_ptr->data.flags;
  i4 = item_ptr->data.flags2;

  do {
    i3 = bit_pos64(&i4,&i2) + 1;
    /*{ Avoid the cursed bit like the plague                      -DMF-   }*/
    if (i3 > 31) {
      i3--;
    }
    if (i3 > 0) {
      i3--;
      //with magic_spell[py.misc.pclass][i3]. do;
      if ((magic_spell[py.misc.pclass][i3].slevel <= py.misc.lev)  &&
	  (magic_spell[py.misc.pclass][i3].learned)) {
	aspell[i1++].splnum = i3;
	num = i1;
      } else {
	aspell[i1++].splnum = -1;  /* leave gaps for unknown spells */
      }
    }
    
  } while (!((i2 == 0) && (i4 == 0)));
  
  if (num > 0) {
    flag = get_spell(aspell,num,sn,sc,prompt,redraw);
  }
  
  if (*redraw) {
    draw_cave();
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void d__examine_book()
{
  /*{ Examine a Book                                        -RAK-   }*/

  unsigned long  i2,i4;
  integer        i3,i5;
  treas_ptr      i1,item_ptr;
  char           trash_char;
  boolean        redraw,flag;
  char           dummy;
  vtype          out_val;
  obj_set        some_books = {Magic_Book,Prayer_Book,Instrument,Song_Book,0};

  redraw = false;

  if (!(find_range(some_books,false,&i1,&i3))) {
    msg_print("You are not carrying any books.");
  } else if (get_item(&item_ptr,"Which Book?",
		      &redraw,i3,&trash_char,false,false)) {
    flag = true;
    //with item_ptr->data. do;
    if (class[PM.pclass].mspell) {
      if (item_ptr->data.tval != Magic_Book) {
	msg_print("You do not understand the language.");
	flag = false;
      }
    } else if (class[PM.pclass].pspell) {
      if (item_ptr->data.tval != Prayer_Book) {
	msg_print("You do not understand the language.");
	flag = false;
      }
    } else if (class[PM.pclass].dspell) {
      if (item_ptr->data.tval != Instrument) {
	msg_print("You do not posses the talent.");
	flag = false;
      }
    } else if (class[PM.pclass].bspell) {
      if (item_ptr->data.tval != Song_Book) {
	msg_print("You can not read the music.");
	flag = false;
      }
    } else {
      msg_print("You do not understand the language.");
      flag = false;
    }
    
    if (flag) {
      redraw = true;
      i5 = 0;
      i2 = item_ptr->data.flags;
      i4 = item_ptr->data.flags2;
      clear_from(1);
      strcpy(out_val,"   Name                         Level  Mana   Known");
      prt(out_val,1,1);
      do {
	i3 = bit_pos64(&i4,&i2) + 1;
	if (i3 > 31) {
	  i3--;
	}
	if (i3 > 0) {
	  //with magic_spell[PM.pclass][i3]. do;
	  i3--;
	  i5++;
	  if (magic_spell[PM.pclass][i3].slevel < 99) {
	    sprintf(out_val,"%c) %-30s%2d      %2d   %s",
		    (char)(96+i5),
		    magic_spell[PM.pclass][i3].sname,
		    magic_spell[PM.pclass][i3].slevel,
		    magic_spell[PM.pclass][i3].smana,
		    magic_spell[PM.pclass][i3].learned ? "true" : "false");
	    prt(out_val,i5+1,1);
	  } else {
	    prt("",i5+1,1);
	  }
	}
      } while (! ((i2 == 0) && (i4 == 0)));
      prt("[Press any key to continue]",24,20);
      dummy = inkey();
    }
  }
    
  if (redraw) {
    draw_cave();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void d__jamdoor()
{
  /*{ Jam a closed door                                     -RAK-   }*/

  treas_ptr   i1;
  integer     y,x,i2,tmp;
  vtype       m_name;
  obj_set     pick_a_spike = {spike, 0};

  y = char_row;
  x = char_col;
  
  if (d__get_dir("Which direction?",&tmp,&tmp,&y,&x)) {
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {
      //with t_list[cave[y][x].tptr]. do;
      if (t_list[cave[y][x].tptr].tval == Closed_door) {
        if (cave[y][x].cptr == 0) {
          if (find_range(pick_a_spike,false,&i1,&i2)) {
            msg_print("You jam the door with a spike.");
            //with i1->data. do;
            if (i1->data.number > 1) {
              i1->data.number--;
            } else {
              inven_destroy(i1);
            }
            prt_weight() ;
            t_list[cave[y][x].tptr].p1 = -abs(t_list[cave[y][x].tptr].p1) - 20;
          } else {
            msg_print("But you have no spikes...");
          }                      
        } else {
          find_monster_name ( m_name, cave[y][x].cptr, true ) ;
          strcat(m_name, " is in your way!");
          msg_print( m_name ) ;
        }
      } else if (t_list[cave[y][x].tptr].tval == Open_door) {
	msg_print("The door must be closed first.");
      } else {
	msg_print("That isn't a door!");
      }
    } else {
      msg_print("That isn't a door!");
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*{ Throw an object across the dungeon...                 -RAK-   }*/
/*{ Note: Flasks of oil do fire damage                            }*/
/*{ Note: Extra damage and chance of hitting when missles are used}*/
/*{       with correct weapon.  I.E.  wield bow and throw arrow.  }*/

void to__inven_throw(treas_ptr item_ptr)
{
  inven_temp->data = item_ptr->data;
  inven_temp->data.number = 1;

  //with item_ptr->data. do;

  if ((item_ptr->data.number > 1) && (item_ptr->data.subval > 511)) {
    item_ptr->data.number--;
    inven_weight -= item_ptr->data.weight;
  } else {
    inven_destroy(item_ptr);
  }

  prt_weight();
};
//////////////////////////////////////////////////////////////////////

obj_set *to__poink(obj_set *ammo_types)
{
  ENTER("to__poink","d");

  if (equipment[Equipment_primary].tval == bow_crossbow_or_sling) {
    (*ammo_types)[1] = 0;  /* all three types have just one kind of ammo */

    switch (equipment[Equipment_primary].p1) {
    case 1 : 
      (*ammo_types)[0] = sling_ammo;
      break;

    case 2 :
    case 3 :
    case 4 : 
      (*ammo_types)[0] = arrow;
      break;
      
    case 5 :
    case 6 : 
      (*ammo_types)[0] = bolt;
      break;
    }

  } else {
    (*ammo_types)[0] = 0;
  }

  LEAVE("to__poink","d");
  return ammo_types;
};
//////////////////////////////////////////////////////////////////////
void to__facts(integer *tbth, integer *tpth, integer *tdam, integer *tdis,
	       boolean to_be_fired)
{
  integer    tmp_weight;

  //with inven_temp->data. do;

  if (inven_temp->data.weight < 1) {
    tmp_weight = 1;
  } else {
    tmp_weight = inven_temp->data.weight;
  }

  /*{ Throwing objects                      }*/

  *tdam = damroll(inven_temp->data.damage) + inven_temp->data.todam;
  *tbth = trunc(py.misc.bthb*0.75);
  *tpth = py.misc.ptohit  + inven_temp->data.tohit;
  *tdis = trunc((py.stat.c[STR]+100)*200/tmp_weight);

  if (*tdis > 10) {
    *tdis = 10;
  }

  /*{ Using Bows, slings, or crossbows      }*/

  if (to_be_fired) {     /*{ checks for correct wpn in poink }*/
    switch (equipment[Equipment_primary].p1) {
    case 1 :          /*{ Sling and Bullet  }*/
      *tdam += 2;
      *tdis  = 20;
      break;
      
    case 2 :          /*{ Short Bow and Arrow    }*/
      *tdam += 2;
      *tdis  = 25;
      break;
      
    case 3 :          /*{ Long Bow and Arrow     }*/
      *tdam += 3;
      *tdis  = 30;
      break;
      
    case 4 :          /*{ Composite Bow and Arrow}*/
      *tdam += 4;
      *tdis  = 35;
      break;
      
    case 5 :          /*{ Light Crossbow and Bolt}*/
      *tdam += 2;
      *tdis  = 25;
      break;
      
    case 6 :          /*{ Heavy Crossbow and Bolt}*/
      *tdam += 4;
      *tdis  = 35;
      break;
      
    }

    *tbth    = py.misc.bthb;
    *tpth   += equipment[Equipment_primary].tohit;
    inven_temp->data.weight  += equipment[Equipment_primary].weight + 5000;
  }
};
//////////////////////////////////////////////////////////////////////
void to__drop_throw(integer y, integer x)
{
  integer    i1,i2,i3,cur_pos;
  boolean    flag = false;
  vtype      out_val, out_val2;

  i1 = y;
  i2 = x;
  i3 = 0;

  if (randint(10) > 1) {
    do {
      if (in_bounds(i1,i2)) {
	//with cave[i1][i2]. do;
	if (cave[i1][i2].fopen) {
	  if (cave[i1][i2].tptr == 0) {
	    flag = true;
	  }
	}
      }
      if (!(flag)) {
	i1 = y + randint(3) - 2;
	i2 = x + randint(3) - 2;
	i3++;
      }
    } while (!((flag) || (i3 > 9)));
  }    
  
  if (flag) {
    popt(&cur_pos);
    cave[i1][i2].tptr = cur_pos;
    t_list[cur_pos] = inven_temp->data;
    if (test_light(i1,i2)) {
      lite_spot(i1,i2);
    }
  } else {
    objdes(out_val,inven_temp,false);
    sprintf(out_val2, "The %s disappears.", out_val);
    msg_print(out_val2);
  }
};
//////////////////////////////////////////////////////////////////////
void d__throw_object(boolean to_be_fired)
{
  integer    tbth,tpth,tdam,tdis,crit_mult;
  integer    y_dumy,x_dumy,dumy,i1;
  integer    y,x,oldy,oldx,dir,cur_dis,count;
  char       trash_char;
  boolean    redraw,flag;
  vtype      out_val, out_val2, m_name;
  treas_ptr  item_ptr,i7;
  obj_set    ammo_types = {0};

  ENTER("d__throw_object", "d");

  redraw = false;

  if (to_be_fired) {
    find_range(*to__poink(&ammo_types),false,&i7,&count);
  } else {
    count = change_all_ok_stats(true,false);
    
    for (item_ptr = inventory_list ; item_ptr != nil ; ) {
      if ((uand(item_ptr->data.flags2,Holding_bit) != 0) &&
	  (item_ptr->insides > 0)) {
	count--;
      }
      item_ptr = item_ptr->next;
    }
  }

  reset_flag = true;

  if (to_be_fired) {
    strcpy(out_val,"Fire which one?");
  } else {
    strcpy(out_val,"Hurl which item?");
  }
  
  if (count == 0) {
    if (to_be_fired) {
      msg_print("You have nothing to fire!");
    } else {
      msg_print("But you have nothing to throw.");
    }
  } else {
    if (get_item(&item_ptr,out_val,&redraw,count,&trash_char,false,false)) {
      if (redraw) {
	draw_cave();
      }
      y_dumy = char_row;
      x_dumy = char_col;
      if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	reset_flag = false;
	desc_remain(item_ptr);
	
	if (py.flags.confused > 0) {
	  msg_print("You are confused...");
	  do {
	    dir = randint(9);
	  } while (dir == 5);
	}
	
	to__inven_throw(item_ptr);
	to__facts(&tbth,&tpth,&tdam,&tdis,to_be_fired);
	
	//with inven_temp->data. do;
	flag = false;
	y = char_row;
	x = char_col;
	oldy = char_row;
	oldx = char_col;
	cur_dis = 0;
	do {
	  move_dir(dir,&y,&x);
	  cur_dis++;
	  if (test_light(oldy,oldx)) {
	    lite_spot(oldy,oldx);
	  }
	  if (cur_dis > tdis) {
	    flag = true;
	  }
	  //with cave[y][x]. do;
	  if ((cave[y][x].fopen) && (!flag)) {
	    if (cave[y][x].cptr > 1) {
	      flag = true;
	      //with m_list[cave[y][x].cptr]. do;
	      tbth -= cur_dis;
	      if (player_test_hit(tbth,PM.lev,tpth,c_list[m_list[cave[y][x].cptr].mptr].ac,to_be_fired)) {
		i1 = m_list[cave[y][x].cptr].mptr;
		objdes(out_val,inven_temp,false);
		find_monster_name ( m_name, cave[y][x].cptr, false ) ;
		sprintf(out_val2, "The %s hits %s.", out_val, m_name);
		msg_print(out_val2);
		tdam = tot_dam(&(inven_temp->data),tdam,&(c_list[i1]));
		//with inven_temp->data. do;
		crit_mult = 
		  critical_blow(inven_temp->data.weight,
				tpth,
				(equipment[Equipment_primary].flags2
				 & Sharp_worn_bit) != 0,
				to_be_fired);
		tdam += (5 + tdam) * crit_mult;
		
		if ( mon_take_hit(cave[y][x].cptr,tdam) > 0) {
		  sprintf(out_val2, "You have killed %s.", m_name);
		  msg_print(out_val2);
		}
	      } else {
		to__drop_throw(oldy,oldx);
	      }
	    } else {
	      if (panel_contains(y,x)) {
		if (test_light(y,x)) {
		  print(inven_temp->data.tchar,y,x);
		}
	      }
	    }
	  } else {
	    flag = true;
	    to__drop_throw(oldy,oldx);
	  }
	  
	  oldy = y;
	  oldx = x;
	} while (!flag);
	
      }
      
    } else {
      if (redraw) {
	draw_cave();
      }
    }
  }

  LEAVE("d__throw_object", "d");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void d__look()
{
  /*{ Look at an object, trap, or monster                   -RAK-   }*/
  /*{ Note: Looking is a free move, see where invoked...            }*/

  integer     i1,i2,y,x;
  integer     dir,dummy;
  boolean     flag = false;
  vtype       out_val, out_val2;

  y = char_row;
  x = char_col;

  if (d__get_dir("Look which direction?",&dir,&dummy,&y,&x)) {
    if (py.flags.blind < 1) {
      y = char_row;
      x = char_col;
      i1 = 0;
      do {
	move_dir(dir,&y,&x);
	//with cave[y][x]. do;
	if (cave[y][x].cptr > 1) {
	  if (m_list[cave[y][x].cptr].ml) {
	    i2 = m_list[cave[y][x].cptr].mptr;
	    if (is_vowel(c_list[i2].name[0])) {
	      sprintf(out_val,"You see an %s.",c_list[i2].name);
	    } else {
	      sprintf(out_val,"You see a %s.",c_list[i2].name);
	    }
	    msg_print(out_val);
	    flag = true;
	  }
	}

	if ((cave[y][x].tl) || (cave[y][x].pl) || (cave[y][x].fm)) {
	  if (cave[y][x].tptr > 0) {
	    if (t_list[cave[y][x].tptr].tval == Secret_door) {
	      msg_print("You see a granite wall.");
	    } else if (t_list[cave[y][x].tptr].tval != Unseen_trap) {
	      inven_temp->data = t_list[cave[y][x].tptr];
	      inven_temp->data.number = 1;
	      objdes(out_val,inven_temp,true);
	      sprintf(out_val2, "You see %s.", out_val);
	      msg_print(out_val2);
	      flag = true;
	    }
	  }
	
	  if (!cave[y][x].fopen) {
	    flag = true;
	    switch (cave[y][x].fval) {
	    case 10 : msg_print("You see a granite wall."); break;
	    case 11 : msg_print("You see some dark rock."); break;
	    case 12 : msg_print("You see a quartz vein.");  break;
	    case 15 : msg_print("You see a granite wall."); break;
	    default:  break;
	    }
	  } else {
	    switch (cave[y][x].fval) {
	    case 16: case 17: 
	      flag = true;
	      msg_print("You see some water.");
	      break;
	  default:
	    break;
	    }
	  }
	}

	i1++;
      } while (! (((! cave[y][x].fopen) || (i1 > MAX_SIGHT))));

      if (!flag) {
	msg_print("You see nothing of interest in that direction.");
      }            
    } else {
      msg_print("You can't see a damn thing!");
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* QQQQQQQ */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void d__set_coords(integer *c_row, integer *c_col)
{
  /*{ Set up the character co-ords          }*/
  if ((*c_row == -1) || (*c_col == -1)) {
    do {
      *c_row = randint(cur_height);
      *c_col = randint(cur_width);

      /*      *c_row = 8;*/
      /*      *c_col = 20;*/
    } while  (!((cave[*c_row][*c_col].fopen)	  &&
		(cave[*c_row][*c_col].cptr == 0)  &&
		(cave[*c_row][*c_col].tptr == 0)  &&
		(!(is_in(cave[*c_row][*c_col].fval,water_set)))));
  }

};
//////////////////////////////////////////////////////////////////////
void d__sun_rise_or_set()
{
  integer i1,i2;

  /*{ Sunrise and Sunset on town level	  -KRC-	}*/
  //with py.misc.cur_age do;
  if (dun_level == 0) {
    if ((PM.cur_age.hour == 6) && (PM.cur_age.secs == 0)) {
      for (i1 = 1; i1 <= cur_height; i1++) {
	for (i2 = 1; i2 <= cur_width; i2++) {
	  cave[i1][i2].pl = true;
	}
      }
      store_maint();
      draw_cave();
    } else if ((PM.cur_age.hour == 18) && (PM.cur_age.secs == 0)) {
      for (i1 = 1; i1 <= cur_height; i1++) {
	for (i2 = 1; i2 <= cur_width; i2++) {
	  if (cave[i1][i2].fval != dopen_floor.ftval) {
	    cave[i1][i2].pl = true;
	  } else {
	    cave[i1][i2].pl = false;
	  }
	}
      }
      store_maint();
      draw_cave();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__check_hours()
{
    /*{ Check for game hours                          }*/
    if (!(wizard1)) {
      if ((turn % 100) == 1) {
	if (!(check_time())) {
	  if (closing_flag > 2) {
	    if (search_flag) {
	      search_off();
	    }
	    if (py.flags.rest > 0) {
	      rest_off();
	    }
	    find_flag = false;
	    msg_print("The gates to Moria are now closed.");
	    msg_print(" ");
	    do {
	      py.flags.dead = false;
	      save_char(true);
	    } while (true);
	  } else {
	    if (search_flag) {
	      search_off();
	    }
	    if (py.flags.rest > 0) {
	      rest_off();
	    }
	    move_char(5);
	    closing_flag++;
	    msg_print("The gates to Moria are closing...");
	    msg_print("Please finish up or save your game.");
	    msg_print(" ");
	  }
	}
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__print_updated_stats()
{
    if (print_stat > 0) {
      if ((0x0001 & print_stat) != 0) {
	      prt_a_stat(STR);
      }
      if ((0x0002 & print_stat) != 0) {
	      prt_a_stat(DEX);
      }
      if ((0x0004 & print_stat) != 0) {
	      prt_a_stat(CON);
      }
      if ((0x0008 & print_stat) != 0) {
	      prt_a_stat(INT);
      }
      if ((0x0010 & print_stat) != 0) {
	      prt_a_stat(WIS);
      }
      if ((0x0020 & print_stat) != 0) {
	      prt_a_stat(CHR);
      }
      if ((0x0040 & print_stat) != 0) {
	      prt_pac();
      }
      if ((0x0100 & print_stat) != 0) {
	      prt_hp();
      }
      if ((0x0200 & print_stat) != 0) {
	      prt_title();
      }
      if ((0x0400 & print_stat) != 0) {
	      prt_level();
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__check_light_status()
{
  /*{ Check light status                            }*/
  //with equipment[Equipment_light] do;
  ENTER("d__check_light_status", "d");
  if (player_light) {
    if ((equipment[Equipment_light].p1 > 0) && PF.light_on) {
      equipment[Equipment_light].p1--;
      if (equipment[Equipment_light].p1 == 0) {
	msg_print("Your light has gone out!");
	PF.light_on = false;
	player_light = false;
	find_flag = false;
	move_light(char_row,char_col,char_row,char_col);
      } else if (equipment[Equipment_light].p1 < 40) {
	if (randint(5) == 1) {
	  if (find_flag) {
	    find_flag = false;
	    move_light(char_row,char_col,char_row,char_col);
	  }
	  msg_print("Your light is growing faint.");
	}
      }
    } else {
      PF.light_on = false;
      player_light = false;
      find_flag = false;
      move_light(char_row,char_col,char_row,char_col);
    }
  } else if ((equipment[Equipment_light].p1 > 0) && PF.light_on) {
    equipment[Equipment_light].p1--;
    player_light = true;
    move_light(char_row,char_col,char_row,char_col);
  }

  LEAVE("d__check_light_status", "d");
};
//////////////////////////////////////////////////////////////////////
void d__hunger_interrupt(char *message)
{
  msg_print(message);
  msg_flag = 0;
  rest_off(); 
}
//////////////////////////////////////////////////////////////////////
void d__check_food()
{
    /*{ Check food status             }*/
    regen_amount = PLAYER_REGEN_NORMAL;
    if ((PF.hunger_item) && (PF.foodc > (PLAYER_FOOD_ALERT + 15))) {
      PF.foodc = PLAYER_FOOD_ALERT + 15;
    }
    
    if (PF.foodc < PLAYER_FOOD_ALERT) {
      if (PF.foodc < PLAYER_FOOD_WEAK) {

	if (PF.foodc < 0) {
	  regen_amount = 0;
	} else if (PF.foodc < PLAYER_FOOD_FAINT) {
	  regen_amount = PLAYER_REGEN_FAINT;
	} else if (PF.foodc < PLAYER_FOOD_WEAK) {
	  regen_amount = PLAYER_REGEN_WEAK;
	}

	if ((IS_WEAK & PF.status) == 0) {
	  PF.status |= (IS_WEAK | IS_HUNGERY);
	  d__hunger_interrupt("You are getting weak from hunger.");
	  if (find_flag) {
	    move_char(5);
	  }
	  prt_hunger();
	  py.misc.wt -= trunc(py.misc.wt*0.015);
	  d__hunger_interrupt ( "Your clothes seem to be getting loose.");
	  if (py.misc.wt < min_allowable_weight()) {
	    msg_print ( "Oh no...  Now you've done it." ) ;
	    death = true ;
	    moria_flag = true ;
	    total_winner = false ;
	    strcpy(died_from,"starvation");
	  }
	}
	
	if (PF.foodc < 0) {
	  if (randint(5) == 1) {
	    PF.paralysis += randint(3);
	    d__hunger_interrupt("You faint from the lack of food.");
	    if (find_flag) {
	      move_char(5);
	    }
	  } else if (PF.foodc < PLAYER_FOOD_FAINT) {
	    if (randint(8) == 1) {
	      PF.paralysis += randint(5);
	      d__hunger_interrupt("You faint from the lack of food.");
	      if (find_flag) {
		move_char(5);
	      }
	    }
	  }
	} /* end if (food < 0) */

      } else {
	/* alert, but not weak */
	if ((IS_HUNGERY & PF.status) == 0) {
	  PF.status |= IS_HUNGERY;
	  d__hunger_interrupt("You are getting hungry.");
	  if (find_flag) {
	    move_char(5);
	  }
	  prt_hunger();
	}
      }


    } /* end if (food < alert) */
};
//////////////////////////////////////////////////////////////////////
void d__eat_food()
{
    /*{ Food consumtion       }*/
    /*{ Note: Speeded up characters really burn up the food!  }*/
    if (PF.speed < 0) {
      PF.foodc -= (PF.speed*PF.speed) + PF.food_digested;
    } else {
      PF.foodc -= PF.food_digested;
    }
};
//////////////////////////////////////////////////////////////////////
void d__regenerate()
{
    /*{ Regenerate            }*/
    //with py.misc do;
    if (PF.regenerate) {
      regen_amount *= 1.5;
    }
    if (PF.rest > 0){
      regen_amount *= 2;
    }
    if (py.flags.poisoned < 1) {
      if (PM.chp < PM.mhp) {
	regenhp(regen_amount);
      }
    }
    if (PM.cmana < PM.mana) {
      regenmana(regen_amount);
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_blindness()
{
    /*{ Blindness             }*/
    if (PF.blind > 0) {
      if ((IS_BLIND & PF.status) == 0) {
	PF.status |= IS_BLIND;
	prt_map();
	prt_blind();
	if (search_flag) {
	  search_off();
	}
      }
      PF.blind--;
      if (PF.blind == 0) {
	PF.status &= ~IS_BLIND;
	prt_blind();
	prt_map();
	msg_print("The veil of darkness lifts.");
	if (find_flag) {
	  move_char(5);
	}
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_confusion()
{
    /*{ Confusion             }*/
    if (PF.confused > 0) {
      if ((IS_CONFUSED & PF.status) == 0) {
	PF.status |= IS_CONFUSED;
	prt_confused();
      }
      PF.confused--;
      if (PF.confused == 0) {
	PF.status &= ~IS_CONFUSED;
	prt_confused();
	msg_print("You feel less confused now.");
	if (find_flag) {
	  move_char(5);
	}
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_resist_lightning()
{
    /*{ Resist Lightning }*/
    if (PF.resist_lght > 0) {
      PF.resist_lght--;
    }

};
//////////////////////////////////////////////////////////////////////
void d__update_monster_protect()
{
    /*{ Protection from Monsters }*/
    if (PF.protmon > 0) {
      PF.protmon--;
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_fire_ring()
{
    /*{ Ring of Fire }*/
    if (PF.ring_fire > 0) {
      msg_print("Flames arise!");
      explode(c_fire,char_row,char_col,20+randint(20),"Ring of Fire");
      PF.ring_fire--;
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_frost_ring()
{
    
    /*{ Ring of Frost }*/
    if (PF.ring_ice > 0) {
      explode(c_cold,char_row,char_col,10+randint(20),"Ring of Frost");
      PF.ring_ice--;
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_blade_barrier()
{

    /*{ Blade Barrier }*/
    if (PF.blade_ring > 0) {
      explode(c_null,char_row,char_col,12+randint(py.misc.lev),
	      "Blade Barrier");
      PF.blade_ring--;
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_magic_protect()
{
    /*{ Magic protection }*/
    if (PF.magic_prot > 0) {
      if ((IS_MAGIC_PROTECED & PF.status) == 0) {
	PF.status |= IS_MAGIC_PROTECED;
	py.misc.save += 25;
      }
      PF.magic_prot--;
      if (PF.magic_prot == 0) {
	py.misc.save -= 25;
	PF.status &= ~IS_MAGIC_PROTECED;
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_resist_petrfy()
{
    /*{Timed resist Petrification}*/
    if (PF.resist_petri > 0) {
      PF.resist_petri--;
    }    

};
//////////////////////////////////////////////////////////////////////
void d__update_stealth()
{
    /*{ Timed Stealth    }*/
    if (PF.temp_stealth > 0) {
      if ((IS_STEALTHY & PF.status) == 0) {
	PF.status |= IS_STEALTHY;
	py.misc.stl += 3;
      }
      PF.temp_stealth--;
      if (PF.temp_stealth == 0) {
	PF.status &= ~IS_STEALTHY;
	py.misc.stl -= 3;
	msg_print("The monsters can once again detect you with ease.");
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_resist_charm()
{
  /*{ Resist Charm }*/
  if (PF.free_time > 0) {
    if ((IS_CHARM_PROOF & PF.status) == 0) {
      PF.status |= IS_CHARM_PROOF;
      PF.free_time--;
      if (PF.free_time == 0) {
	PF.status &= ~IS_CHARM_PROOF;
	if (find_flag) {
	  move_char(5);
	}
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_hoarse()
{
    /*{ Hoarse                }*/
    if (PF.hoarse > 0) {
      PF.hoarse--;
      if (PF.hoarse == 0) {
	msg_print("You feel your voice returning.");
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_fear()
{
    /*{ Afraid                }*/
    if (PF.afraid > 0) {
      if ((IS_AFRAID & PF.status) == 0) {
	if ((PF.shero+PF.hero) > 0) {
	  PF.afraid = 0;
	} else {
	  PF.status |= IS_AFRAID;
	  prt_afraid();
	}
      } else if ((PF.shero+PF.hero) > 0) {
	PF.afraid = 1;
      }

      PF.afraid--;
      if (PF.afraid == 0) {
	PF.status &= ~IS_AFRAID;
	prt_afraid();
	msg_print("You feel bolder now.");
	if (find_flag) {
	  move_char(5);
	}
      }
    }
    if (PF.afraid < 0) {
      PF.afraid = 0; /* fix when getting hit with fear while shero or hero */
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_poison()
{
    /*{ Poisoned              }*/
  if (PF.poisoned > 0) {
    if ((IS_POISONED & PF.status) == 0) {
      PF.status |= IS_POISONED;
      prt_poisoned();
    }
    PF.poisoned--;
    if (PF.poisoned == 0) {
      PF.status &= ~IS_POISONED;
      prt_poisoned();
      msg_print("You feel better.");
      if (find_flag) {
	move_char(5);
      }
    } else {
      switch (con_adj()) {
      case -4:	   take_hit(4,"poison"); break;
      case -3: case -2:  take_hit(3,"poison"); break;
      case -1:           take_hit(2,"poison"); break;
      case  0:	   take_hit(1,"poison"); break;
      case  1: case 2: case 3: 
	if ((turn % 2) == 0) {
	  take_hit(1,"poison");
	}
	break;
      case  4: case 5:
	if ((turn % 3) == 0) {
	  take_hit(1,"poison");
	}
	break;
      case  6: 
	if ((turn % 4) == 0) {
	  take_hit(1,"poison.");
	}
	break;
      } /* end switch */
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_fast()
{

    /*{ Fast                  }*/
    if (PF.fast > 0) {
      if ((IS_HASTED & PF.status) == 0) {
	PF.status |= IS_HASTED;
	msg_print("You feel yourself moving faster.");
	change_speed(-1);
	if (find_flag) {
	  move_char(5);
	}
      }
      PF.fast--;
      if (PF.fast == 0) {
	PF.status &= ~IS_HASTED;
	msg_print("You feel yourself slow down.");
	change_speed(+1);
	if (find_flag) {
	  move_char(5);
	}
      }
    }

};
//////////////////////////////////////////////////////////////////////
void d__update_slow()
{

    /*{ Slow                  }*/
    if (PF.slow > 0) {
      if ((IS_SLOW & PF.status) == 0) {
	PF.status |= IS_SLOW;
	msg_print("You feel yourself moving slower.");
	change_speed(+1);
	if (find_flag) {
	  move_char(5);
	}
      }
      PF.slow--;
      if (PF.slow == 0) {
	PF.status &= ~IS_SLOW;
	msg_print("You feel yourself speed up.");
	change_speed(-1);
	if (find_flag) {
	  move_char(5);
	}
      }
    }
};
//////////////////////////////////////////////////////////////////////
void d__update_resting()
{
  /*{ Resting is over?      }*/
  if (PF.rest > 0) {
    /*{ Hibernate every 20 iterations so that process does  }*/
    /*{ not eat up system...                                }*/
    /*{ NOTE: Remove comments for VMS version 4.0 or greater}*/
    /*{       INKEY_DELAY takes care of hibernation for     }*/
    /*{       VMS 3.7 or less                               }*/
    if ((PF.rest % 20) == 1) {
      usleep(500);
      if ((equipment[Equipment_primary].flags2 & Soul_Sword_worn_bit) != 0) {
	bother(randint(10));
	PF.rest = 1;
      }
    }
    PF.rest--;
    /*{ Test for any key being hit to abort rest.  Also,    }*/
    /*{ this will do a PUT_QIO which updates the screen...  }*/
    /*{ One more side benifit; since inkey_delay hibernates }*/
    /*{ small amount before executing, this makes resting   }*/
    /*{ less CPU intensive...                               }*/
    inkey_delay(&command,0);
    /*if (want_trap) { dump_ast_mess; XXXX}*/
    if (PF.rest == 0) {               /*{ Resting over          }*/
      rest_off();
    } else if (command != 0) {  /*{ Resting aborted       }*/
      rest_off();
    }
  }  
};
//////////////////////////////////////////////////////////////////////
void d__update_hallucinate()
{
  /*{ Hallucinating?  (Random characters appear!)}*/
  if (PF.image > 0) {
    PF.image--;
    if (PF.image == 0) {
      draw_cave();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_petrify()
{
  /*{  Petrification wears off slowly  } */
  if ( (turn % 100) == 0) {
    //with py.flags do;
    if (PF.petrification > 100) {
      PF.petrification--;
    }
  }


  /* not sure what this did, but it was commented out... */
  /*
  if (PF.speed > 0) and (paral_init = speed_paral) then
		   paralysis := paralysis + paral_init + 1;
  */
  
  /*{ Paralysis             }*/
  if (PF.paralysis > 0) {
    PF.paralysis--;
    if (PF.rest > 0) {
      rest_off();
    }
    if ((search_flag) && (PF.paralysis > PF.paral_init)) {
      search_off();
    }
  }

  /* hey look!  more commented out code!! */
  /*
  if (speed > 0) and (speed_flag) then
		   begin
		   speed_flag := false;
  speed_paral := paral_init;
  end
    else if (speed_paral > 1) then
				speed_paral := speed_paral - 1
				else
				  begin
				    speed_paral := 0;
  speed_flag := true;
  end;
  */    
};
//////////////////////////////////////////////////////////////////////
void d__update_evil_protect()
{
  /*{ Protection from evil counter}*/
  if (PF.protevil > 0) {
    PF.protevil--;
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_invulnerable()
{
  /*{ Invulnerability        }*/
  if (PF.invuln > 0) {
    if ((IS_INVULNERABLE & PF.status) == 0) {
      PF.status |= IS_INVULNERABLE;
      if (find_flag) {
	move_char(5);
      }
      msg_print("Your skin turns into steel!");
      py.misc.pac    += 100;
      py.misc.dis_ac += 100;
      prt_pac();
    }
    PF.invuln--;
    if (PF.invuln == 0) {
      PF.status &= ~IS_INVULNERABLE;
      if (find_flag) {
	move_char(5);
      }
      msg_print("Your skin returns to normal...");
      py.misc.pac    -= 100;
      py.misc.dis_ac -= 100;
      prt_pac();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_heroism()
{
  /*{ Heroism       }*/
  if (PF.hero > 0) {
    if ((IS_HERO & PF.status) == 0) {
      PF.status |= IS_HERO;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.mhp  += 10;
      PM.chp  += 10;
      PM.bth  += 12;
      PM.bthb += 12;
      msg_print("You feel like a HERO!");
      prt_hp();
    }
    PF.hero--;
    if (PF.hero == 0) {
      PF.status &= ~IS_HERO;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.mhp  -= 10;
      if (PM.chp > PM.mhp) {
	PM.chp = PM.mhp;
      }
      PM.bth  -= 12;
      PM.bthb -= 12;
      msg_print("The heroism wears off.");
      prt_hp();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_super_heroism()
{
  /*{ Super Heroism }*/
  if (PF.shero > 0) {
    if ((IS_SUPER_HERO & PF.status) == 0) {
      PF.status |= IS_SUPER_HERO;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.mhp  += 20;
      PM.chp  += 20;
      PM.bth  += 24;
      PM.bthb += 24;
      msg_print("You feel like a SUPER HERO!");
      prt_hp();
    }
    PF.shero--;
    if (PF.shero == 0) {
      PF.status &= ~IS_SUPER_HERO;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.mhp  -= 20;
      if (PM.chp > PM.mhp) {
	PM.chp = PM.mhp;
      }
      PM.bth -= 24;
      PM.bthb -= 24;
      msg_print("The super heroism wears off.");
      prt_hp();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_blessed()
{
  /*{ Blessed       }*/
  if (PF.blessed > 0) {
    if ((IS_BLESSED & PF.status) == 0) {
      PF.status |= IS_BLESSED;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.bth    += 5;
      PM.bthb   += 5;
      PM.pac    += 5;
      PM.dis_ac += 5;
      msg_print("You feel righteous!");
      prt_hp();
      prt_pac();
    }
    PF.blessed--;
    if (PF.blessed == 0) {
      PF.status &= ~IS_BLESSED;
      if (find_flag) {
	move_char(5);
      }
      //with py.misc do;
      PM.bth    -= 5;
      PM.bthb   -= 5;
      PM.pac    -= 5;
      PM.dis_ac -= 5;
      msg_print("The prayer has expired.");
      prt_hp();
      prt_pac();
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_resist_heat()
{
  /*{ Resist Heat   }*/
  if (PF.resist_heat > 0) {
    PF.resist_heat--;
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_resist_cold()
{
  /*{ Resist Cold   }*/
  if (PF.resist_cold > 0) {
    PF.resist_cold--;
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_detect_invisible()
{
  /*{ Detect Invisible      }*/
  if (PF.detect_inv > 0) {
    if ((IS_ABLE_TO_SEE_INVIS & PF.status) == 0) {
      PF.status  |= IS_ABLE_TO_SEE_INVIS;
      PF.see_inv  = true;
    }
    PF.detect_inv--;
    if (PF.detect_inv == 0) {
      PF.status &= ~IS_ABLE_TO_SEE_INVIS;
      PF.see_inv = false;
      py_bonuses(&blank_treasure,0);
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_infra_vision()
{
  /*{ Timed infra-vision    }*/
  if (PF.tim_infra > 0) {
    if ((IS_ABLE_TO_SEE_HEAT & PF.status) == 0) {
      PF.status |= IS_ABLE_TO_SEE_HEAT;
      PF.see_infra++;
    }
    PF.tim_infra--;
    if (PF.tim_infra == 0) {
      PF.status &= ~IS_ABLE_TO_SEE_HEAT;
      PF.see_infra--;
      msg_print("Your eyes stop tingling.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_word_of_recall()
{
  /*{ Word-of-Recall  Note: Word-of-Recall is a delayed action      }*/
  if (PF.word_recall > 0) {
    if (PF.word_recall == 1) {
      if (dun_level > 0) {
	msg_print("You feel yourself yanked upwards!");
	dun_level = 0;
      } else if (py.misc.max_lev > 0) {
	msg_print("You feel yourself yanked downwards!");
	dun_level = py.misc.max_lev;
      }
      moria_flag = true;
      PF.paralysis++;
      PF.word_recall = 0;
    } else {
      PF.word_recall--;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__update_hit_points()
{
  /*{ Check hit points for adjusting...                     }*/
  //with py.misc do;
  ENTER("d__update_hit_points", "d")

  if (!(find_flag)) {
    if (py.flags.rest < 1) {
      if (old_chp != (integer)(PM.chp)) {
	if (PM.chp > PM.mhp) {
	  PM.chp = PM.mhp;
	}
	prt_hp();
	old_chp = trunc(PM.chp);
      }
      if (old_cmana != trunc(PM.cmana)) {
	if (PM.cmana > PM.mana) {
	  PM.cmana = PM.mana;
	}
	if (is_magii) {
	  prt_mana();
	}
	old_cmana = trunc(PM.cmana);
      }
    }
  }
  LEAVE("d__update_hit_points", "d")
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean d__get_dir(vtype prompt, integer *dir, integer *com_val,
		   integer *y, integer *x)
{
  /*{ Prompts for a direction                               -RAK-   }*/

  vtype      temp_prompt, prompt2;
  char       command;
  boolean    flag = false;
  boolean    return_value = false;

  sprintf(temp_prompt, "(1 2 3 4 6 7 8 9) %s", prompt);
  prompt2[0] = 0;
  
  do {
    if (get_com(prompt2,&command)) {
      *com_val = (integer)command;
      *dir = *com_val - 48;
      
      /*{ Note that '5' is not a valid direction        }*/
      if ((*dir > 0) && (*dir < 10) && (*dir != 5)) {
	move_dir(*dir,y,x);
	flag = true;
	return_value = true;
      } else {
	strcpy(prompt2, temp_prompt);
      }
    } else {
      reset_flag = true;
      return_value = false;
      flag = true;
    }
    
  } while (!flag);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
void d__quit()
{ 
  /* this can be called from signalquit in io.c */

  char    command;

  flush();
  if (get_com("Enter 'Q' to quit (and kill this character)",&command)) {
    switch (command) {
    case 'q':
    case 'Q':
      if (total_winner) {
	moria_flag = true;
	death      = true;
      } else {
	if (is_from_file) {
	  unlink(finam);
	}
	clear_from(1);
	exit_game();
      }
      break;
    default:
      break;
    }
  }

  erase_line(1,1);
  put_qio();
};
//////////////////////////////////////////////////////////////////////
void d__go_up()
{
  /*{ Go up one level                                       -RAK-   }*/
  /*{ Or several, with a steep staircase                    -DMF-   }*/

  //with cave[char_row][char_col]. do;
  if (cave[char_row][char_col].tptr > 0) {
    if (t_list[cave[char_row][char_col].tptr].tval == Up_staircase) {
      dun_level--;
      moria_flag = true;
      msg_print("You enter a maze of up staircases.");
      msg_print("You pass through a one-way door.");
    } else if (t_list[cave[char_row][char_col].tptr].tval == 
	       Up_steep_staircase) {
      dun_level -= randint(3) + 1;
      if (dun_level < 0) {
	dun_level = 0;
      }
      moria_flag = true;
      msg_print("You enter a long maze of up staircases.");
      msg_print("You pass through a one-way door.");
    } else {
      msg_print("I see no up staircase here.");
    }
  } else {
    msg_print("I see no up staircase here.");
  }
};
//////////////////////////////////////////////////////////////////////
void d__go_down()
{
  /*{ Go down one level                                     -RAK-   }*/
  /*{ Or several, with a steep staircase                    -DMF-   }*/

  //with cave[char_row][char_col]. do;
  if (cave[char_row][char_col].tptr > 0) {
    if (t_list[cave[char_row][char_col].tptr].tval == Down_staircase) {
      dun_level++;
      moria_flag = true;
      msg_print("You enter a maze of down staircases.");
      msg_print("You pass through a one-way door.");
    } else if (t_list[cave[char_row][char_col].tptr].tval == 
	       Down_steep_staircase) {
      dun_level += randint(3) + 1;
      moria_flag = true;
      msg_print("You enter a long maze of down staircases.");
      msg_print("You pass through a one-way door.");
    } else {
      msg_print("I see no down staircase here.");
    }
  } else {
    msg_print("I see no down staircase here.");
  }
};
//////////////////////////////////////////////////////////////////////
void d__bash()
{
  /*{ Bash open a door or chest                             -RAK-   }*/
  /*{ Note: Affected by strength and weight of character            }*/

  integer    y,x,tmp;
  integer    old_ptodam,old_ptohit,old_bth;

  y = char_row;
  x = char_col;

  if (d__get_dir("Which direction?",&tmp,&tmp,&y,&x)) {
    //with cave[y][x]. do;
    if (cave[y][x].cptr > 1) {
      if (py.flags.afraid > 0) {
	msg_print("You are afraid!");
      } else {
	/*{ Save old values of attacking  }*/
	inven_temp->data = equipment[Equipment_primary];
	old_ptohit = py.misc.ptohit;
	old_ptodam = py.misc.ptodam;
	old_bth    = py.misc.bth;
	/*{ Use these values              }*/
	equipment[Equipment_primary] = blank_treasure;
	//with equipment[Equipment_primary]. do;
	strcpy(equipment[Equipment_primary].damage,
	       equipment[Equipment_shield].damage);
	equipment[Equipment_primary].weight = (py.stat.c[STR] + 20) * 100;
	equipment[Equipment_primary].tval   = 1;
	
	//with py do;
	PM.bth    = trunc(((PS.c[STR]+20) div 5+PM.wt)/6.0);
	PM.ptohit = 0;
	PM.ptodam = trunc(PM.wt/75.0) + 1;
	
	if (py_attack(y,x)) {
	  do_stun(cave[y][x].cptr,-10,2);
	}
	
	/*{ Restore old values            }*/
	equipment[Equipment_primary] = inven_temp->data;
	py.misc.ptohit = old_ptohit;
	py.misc.ptodam = old_ptodam;
	py.misc.bth    = old_bth;
	if (randint(300) > py.stat.c[DEX]) {
	  msg_print("You are off-balance.");
	  py.flags.paralysis = randint(3);
	}
      }	
    }  else if (cave[y][x].tptr > 0) {
      //with t_list[cave[y][x].tptr]. do;
      if (t_list[cave[y][x].tptr].tval == Closed_door) {
	//with py do;
	if (test_hit(PM.wt+(PS.c[STR]*PS.c[STR]) div 500,0,0,
		     abs(t_list[cave[y][x].tptr].p1)+150)){
	  msg_print("You smash into the door! The door crashes open!");
	  t_list[cave[y][x].tptr] = door_list[DL_OPEN];
	  t_list[cave[y][x].tptr].p1 = 1;
	  cave[y][x].fopen = true;
	  lite_spot(y,x);
	} else {
	  msg_print("You smash into the door! The door holds firm.");
	  py.flags.paralysis = 2;
	}

      } else if (t_list[cave[y][x].tptr].tval == chest) {

	if (randint(10) == 1) {
	  msg_print("You have destroyed the chest...");
	  msg_print("and its contents!");
	  strcpy(t_list[cave[y][x].tptr].name,"& ruined chest");
	  t_list[cave[y][x].tptr].flags = 0;
	} else if (uand(0x00000001,t_list[cave[y][x].tptr].flags) != 0) {
	  if (randint(10) == 1) {
	    /* just "unlocks", traps are still in place */
	    msg_print("The lock breaks open!");
	    t_list[cave[y][x].tptr].flags &= 0xFFFFFFFE;
	  }
	}

      } else {
	msg_print("I do not see anything you can bash there.");
      }
    } else {
      msg_print("I do not see anything you can bash there.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__chest_trap(integer y, integer x)
{
  /*{ Chests have traps too...                              -RAK-   }*/
  /*{ Note: Chest traps are based on the FLAGS value                }*/

  integer         i1,i2,i3;
  unsigned long	  flags;

  flags = t_list[cave[y][x].tptr].flags;

  //with t_list[cave[y][x].tptr]. do;

  if (uand(0x00000010,flags) != 0) {
    msg_print("A small needle has pricked you!");
    if (lose_stat(STR,"","You are unaffected.")) {
      take_hit(damroll("1d4"),"a poison needle");
      print_stat |= 0x0001;
      msg_print("You feel weakened!");
    }          
  }
  
  if (uand(0x00000020,flags) != 0) {
    msg_print("A small needle has pricked you!");
    take_hit(damroll("1d6"),"a poison needle.");
    py.flags.poisoned += 10 + randint(20);
  }
  
  if (uand(0x00000040,flags) != 0) {
    msg_print("A puff of yellow gas surrounds you!");
    if (py.flags.free_act) {
      msg_print("You are unaffected.");
    } else {
      msg_print("You choke and pass out.");
      py.flags.paralysis = 10 + randint(20);
    }
  }
  
  if (uand(0x00000080,flags) != 0) {
    msg_print("There is a sudden explosion!");
    delete_object(y,x);
    take_hit(damroll("5d8"),"an exploding chest");
  }
  
  if (uand(0x00000100,flags) != 0) {
    for (i1 = 0; i1 < 3; i1++) {
      i2 = y;
      i3 = x;
      if (is_in(cave[i2][i3].fval, water_set)) {
	summon_water_monster(&i2,&i3,false);
      } else {
	summon_land_monster(&i2,&i3,false);
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__openobject()
{
  /*{ Opens a closed door or closed chest...                -RAK-   }*/

  integer    y,x,tmp;
  boolean    flag;
  char      *tmpc;

  y = char_row;
  x = char_col;

  if (d__get_dir("Which direction?",&tmp,&tmp,&y,&x)) {
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {

      /*{ Closed door           }*/
      if (t_list[cave[y][x].tptr].tval == Closed_door) {
	//with t_list[cave[y][x].tptr]. do;
	if (t_list[cave[y][x].tptr].p1 > 0) {  /*{ It's locked...        }*/
	  //with py.misc do;
	  tmp = PM.disarm + PM.lev + 2*todis_adj() + spell_adj(INT);
	  
	  if (py.flags.confused > 0) {
	    msg_print("You are too confused to pick the lock.");
	  } else if ((tmp-t_list[cave[y][x].tptr].p1) > randint(100)) {
	    msg_print("You have picked the lock.");
	    py.misc.exp++;
	    prt_experience();
	    t_list[cave[y][x].tptr].p1 = 0;
	  } else {
	    msg_print("You failed to pick the lock.");
	  }
	  
	} else if (t_list[cave[y][x].tptr].p1 < 0) {     /*{ It's stuck    }*/
	  msg_print("It appears to be stuck.");
	}
	
	if (t_list[cave[y][x].tptr].p1 == 0) {
	  t_list[cave[y][x].tptr] = door_list[DL_OPEN];
	  cave[y][x].fopen = true;
	  lite_spot(y,x);
	}
	
      } else if (t_list[cave[y][x].tptr].tval == chest) {
	/*{ Open a closed chest...                }*/
	//with py.misc do;
	tmp = PM.disarm + PM.lev + 2*todis_adj() + spell_adj(INT);
	
	//with t_list[tptr] do;
	flag = false;
	if (uand(0x00000001,t_list[cave[y][x].tptr].flags) != 0) {
	  if (py.flags.confused > 0) {
	    msg_print("You are too confused to pick the lock.");
	  } else if ((tmp-(2*t_list[cave[y][x].tptr].level)) > randint(100)) {
	    msg_print("You have picked the lock.");
	    flag = true;
	    py.misc.exp += t_list[cave[y][x].tptr].level;
	    prt_experience();
	  } else {
	    msg_print("You failed to pick the lock.");
	  }
	} else {
	  flag = true;
	}
	
	if (flag) {
	  t_list[cave[y][x].tptr].flags &= 0xFFFFFFFE;
	  tmpc = strstr(t_list[cave[y][x].tptr].name," (");
	  if (tmpc != NULL) {
	    *tmpc = 0;
	  }
	  strcat(t_list[cave[y][x].tptr].name," (Empty)");
	  known2(t_list[cave[y][x].tptr].name);
	  t_list[cave[y][x].tptr].cost = 0;
	}
	
	flag = false;

	/*{ Was chest still trapped?  (Snicker)   }*/
	if (uand(0x00000001,t_list[cave[y][x].tptr].flags) == 0) {
	  d__chest_trap(y,x);
	  if (cave[y][x].tptr > 0) {
	    flag = true;
	  }
	}
	
	/*{ Chest treasure is allocted as if a creature   }*/
	/*{ had been killed...                            }*/
	
	if (flag) {
	  monster_death(y,x,t_list[cave[y][x].tptr].flags);
	  t_list[cave[y][x].tptr].flags = 0;
	}
	
      } else {
	msg_print("I do not see anything you can open there.");
      }
    } else {
      msg_print("I do not see anything you can open there.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__closeobject()
{
  /*{ Closes an open door...                                -RAK-   }*/

  integer    y,x,tmp;
  vtype      m_name;

  y = char_row;
  x = char_col;

  if (d__get_dir("Which direction?",&tmp,&tmp,&y,&x)) {
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {
      if (t_list[cave[y][x].tptr].tval == Open_door) {
	if (cave[y][x].cptr == 0) {
	  if (t_list[cave[y][x].tptr].p1 == 0) {
	    t_list[cave[y][x].tptr] = door_list[1];
	    cave[y][x].fopen = false;
	    lite_spot(y,x);
	  } else {
	    msg_print("The door appears to be broken.");
	  }
	} else {
	  find_monster_name ( m_name, cave[y][x].cptr, true ) ;
	  strcat(m_name, " is in your way!");
	  msg_print(m_name);
	}
      } else {
	msg_print("I do not see anything you can close there.");
      }
    } else {
      msg_print("I do not see anything you can close there.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__disarm_trap()
{
  /*{ Disarms a trap                                        -RAK-   }*/
  
  integer   y,x,i1,tdir;
  integer   tot,t1,t2,t3,t4,t5;
  char     *tmpc;
  
  y = char_row;
  x = char_col;
  
  if (d__get_dir("Which direction?",&tdir,&i1,&y,&x)) {
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {
      t1 = py.misc.disarm; /*{ Ability to disarm     }*/
      t2 = py.misc.lev;    /*{ Level adjustment      }*/
      t3 = 2*todis_adj();  /*{ Dexterity adjustment  }*/
      t4 = spell_adj(INT); /*{ Intelligence adjustment}*/
      tot = t1 + t2 + t3 + t4;
      
      if (py.flags.blind > 0) {
	tot /= 5;
      } else if (no_light()) {
	tot /= 2;
      }
      
      if (py.flags.confused > 0) {
	tot /= 3;
      }
      
      i1 = t_list[cave[y][x].tptr].tval;
      t5 = t_list[cave[y][x].tptr].level;
      
      if (i1 == Seen_trap) {           /* { Floor trap    } */
	//with t_list[cave[y][x].tptr]. do;
	if ((tot - t5) > randint(100)) {
	  msg_print("You have disarmed the trap.");
	  py.misc.exp += t_list[cave[y][x].tptr].p1;
	  cave[y][x].fm = false;
	  pusht(cave[y][x].tptr);
	  cave[y][x].tptr = 0;
	  move_char(tdir);
	  lite_spot(y,x);
	  prt_experience();
	} else if (randint(tot) > 5) {
	  msg_print("You failed to disarm the trap.");
	} else {
	  msg_print("You set the trap off!");
	  move_char(tdir);
	}
      } else if (i1 == 2) {         /*{ Chest trap    }*/
	//with t_list[cave[y][x].tptr]. do;
	if (strstr(t_list[cave[y][x].tptr].name,"^") != NULL) {
	  msg_print("I don't see a trap...");
	} else if (uand(0x000001F0,t_list[cave[y][x].tptr].flags) != 0) {
	  if ((tot - t5) > randint(100)) {
	    t_list[cave[y][x].tptr].flags &= 0xFFFFFE0F;
	    tmpc = strstr(t_list[cave[y][x].tptr].name," (");
	    if (tmpc != NULL) {
	      *tmpc = 0;
	    }
	    if (uand(0x00000001,t_list[cave[y][x].tptr].flags) != 0) {
	      strcat(t_list[cave[y][x].tptr].name," (Locked)");
	    } else {
	      strcat(t_list[cave[y][x].tptr].name, " (Disarmed)");
	    }
	    msg_print("You have disarmed the chest.");
	    known2(t_list[cave[y][x].tptr].name);
	    py.misc.exp += t5;
	    prt_experience();
	  } else if (randint(tot) > 5) {
	    msg_print("You failed to disarm the chest.");
	  } else {
	    msg_print("You set a trap off!");
	    known2(t_list[cave[y][x].tptr].name);
	    d__chest_trap(y,x);
	  }
	} else {
	  msg_print("The chest was not trapped.");
	}
      } else {
	msg_print("I do not see anything to disarm there.");
      }
    } else {
      msg_print("I do not see anything to disarm there.");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__refill_lamp()
{
  /*{ Refill the players lamp                               -RAK-   }*/

  integer    i2,i3;
  treas_ptr  i1;
  obj_set    this_be_oil = {flask_of_oil, 0};

  i3 = equipment[Equipment_light].subval;

  if ((i3 > 0) && (i3 < 10)) {
    if (find_range(this_be_oil,false,&i1,&i2)) {
      msg_print("Your lamp is full.");
      //with equipment[Equipment_light]. do;
      equipment[Equipment_light].p1 += i1->data.p1;
      if (equipment[Equipment_light].p1 > OBJ_LAMP_MAX) { 
	equipment[Equipment_light].p1 = OBJ_LAMP_MAX;
      }
      desc_remain(i1);
      inven_destroy(i1);
      prt_weight();
    } else {
            msg_print("You have no oil.");
    }
  } else {
    msg_print("But you are not using a lamp.");
  }
};
//////////////////////////////////////////////////////////////////////
void d__tunnel()
{
  /*{ Tunnels through rubble and walls                      -RAK-   }*/
  /*{ Must take into account: secret doors, special tools           }*/

  integer   y,x,i1,tabil;

  y = char_row;
  x = char_col;

  if (d__get_dir("Which direction?",&i1,&i1,&y,&x)) {
    //with cave[y][x]. do;

    /*{ Compute the digging ability of player; based on       }*/
    /*{ strength, and type of tool used                       }*/
    tabil = (py.stat.c[STR] + 20) div 5;
    if (equipment[Equipment_primary].tval > 0) {
      //with equipment[Equipment_primary] do;
      if (uand(Tunneling_worn_bit,equipment[Equipment_primary].flags) != 0) {
	tabil += 25 + equipment[Equipment_primary].p1*50;
      }
    }
    
    /*{ Regular walls; Granite, magma intrusion, quartz vein  }*/
    /*{ Don't forget the boundry walls, made of titanium (255)}*/
    switch (cave[y][x].fval) {
    case 10 : 
      i1 = randint(1200) + 80;
      if (twall(y,x,tabil,i1)) {
	msg_print("You have finished the tunnel.");
      } else {
	msg_print("You tunnel into the granite wall.");
      }
      break;
      
    case 11 : 
      i1 = randint(600) + 10;
      if (twall(y,x,tabil,i1)) {
	msg_print("You have finished the tunnel.");
      } else {
	msg_print("You tunnel into the magma intrusion.");
      }
      break;
      
    case 12 : 
      i1 = randint(400) + 10;
      if (twall(y,x,tabil,i1)) {
	msg_print("You have finished the tunnel.");
      } else {
	msg_print("You tunnel into the quartz vein.");
      }
      break;

    case 15 : 
      msg_print("This seems to be permanent rock.");
      break;
      
    case 16 : 
      msg_print("You can't tunnel through water!");
      break;

    default:
      /*{ Is there an object in the way?  (Rubble and secret doors)}*/
      if (cave[y][x].tptr > 0) {

	/*{ Rubble...     }*/
	if (t_list[cave[y][x].tptr].tval == Rubble) {
	  if (tabil > randint(180)) {
	    pusht(cave[y][x].tptr);
	    cave[y][x].tptr = 0;
	    cave[y][x].fm = false;
	    cave[y][x].fopen = true;
	    msg_print("You have removed the rubble.");
	    if (randint(10) == 1) {
	      place_object(y,x);
	      if (test_light(y,x)) {
		msg_print("You have found something!");
	      }
	    }
	    lite_spot(y,x);
	  } else {
	    msg_print("You dig in the rubble...");
	  }
	  
	  /*{ Secret doors...}*/
	} else if (t_list[cave[y][x].tptr].tval == Secret_door) {
	  msg_print("You tunnel into the granite wall.");
	  search(char_row,char_col,py.misc.srh);
	} else {
	  msg_print("You can't tunnel through that.");
	}                    
      } else {
	msg_print("Tunnel through what?  Empty air???");
      }
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void d__drop()
{
  /*{ Drop an object being carried                          -RAK-   }*/
  /*{ Note: Only one object per floor spot...                       }*/

  treas_ptr   com_ptr;
  boolean     redraw;
  char        trash_char;
  vtype       out_val,out_val2;
  integer     temp;
  integer     count;

  reset_flag = true;

  //with py.misc do;
  temp = (PM.money[6]+PM.money[5]+PM.money[4]+
	  PM.money[3]+PM.money[2]+PM.money[1]);

  if ((inven_ctr > 0) || (temp > 0)) {
    count = change_all_ok_stats(true,false);
    com_ptr = inventory_list;
    for (;com_ptr != nil;) {
      if ((com_ptr->data.tval == bag_or_sack) && (com_ptr->insides != 0)) {
	com_ptr->ok = false;
	count--;
      }
      com_ptr = com_ptr->next;
    }

    /*{Someone said that it always redraws when drop}*/
    redraw = false;  

    if (get_item(&com_ptr,"Which one? ",&redraw,count,&trash_char,true,false)){
      if (redraw) { 
	draw_cave();
      }
      //with cave[char_row][char_col]. do;
      if (cave[char_row][char_col].tptr > 0) {
	msg_print("There is something there already.");
      } else {
	if (trash_char == '$') {
	  inven_drop(com_ptr,char_row,char_col,true);
	} else {
	  inven_drop(com_ptr,char_row,char_col,false);
	}
	prt_weight();
	objdes(out_val,inven_temp,true);
	sprintf(out_val2,"Dropped %s.",out_val);
	msg_print(out_val2);
	reset_flag = false;
      }
    } else if (redraw) {
      draw_cave();
    }          
  } else {
    msg_print("You are not carrying anything.");
  }
};
//////////////////////////////////////////////////////////////////////
void view_old_mess()
{
  vtype         tmp_str;
  boolean       done = false;
  message_ptr   ptr;
  string        out_mess;
  string        tmp2;

  ptr = old_message;

  for ( ; (ptr != nil) && !done ; ) {
    strncpy(out_mess, ptr->data, sizeof(string));
    if (strlen(out_mess) > 71) {
      for ( ; strlen(out_mess) > 71 ; ) {
	strncpy(tmp_str, out_mess, 71);
	strcpy(tmp2, &(out_mess[71]));
	strcpy(out_mess, tmp2);
	done = msg_print(tmp_str);
      }
    }
    done = msg_print(out_mess);
    ptr  = ptr->next;
  }
};
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void d__execute_command(integer *com_val)
{
  /*{ Commands are executed in following case statement             }*/
  /*{ The following keys are used for commands:                     }*/
  /*{ ^A ^B ^C ^D ^E ^F ^G ^H ^I ^J ^K ^L ^M ^N ^O ^P ^R ^T ^U ^V   }*/
  /*{ ^W ^X ^Y ^Z ^3 ^_ $ ! + . / 1 2 3 4 5 6 7 8 9 < > ? @         }*/
  /*{ A B C D E F G H I J K L M N   P Q R S T U V W X      ]        }*/
  /*{ a b c d e f   h i j   l m   o p q r s t u v w x      |        }*/

  integer       i1;
  integer       y,x;
  stat_set      tstat;
  treas_ptr	trash_ptr;
  vtype         out_val, out2, tmp_str;
  
  ENTER("d__execute_command", "d");

#if DO_DEBUG
  fprintf(debug_file,": command: %ld\n",*com_val); fflush(debug_file);
#endif

//      sprintf(out_val, "Key = %d", (int)(*com_val));
//      msg_print(out_val);
//      msg_print( "" );

  switch (*com_val) {

  //case   1  :    /* ^A = Cure all     W1 */
  //case   2  :    /* ^B = objects      W1 */
  //case   4  :    /* ^D = up/down      W1 */
  //case   5  :    /* ^E = wizchar      W2 */
  //case   6  :    /* ^F = genocide     W2 */
  //case   7  :    /* ^G = treasure     W2 */
  //case   8  :    /* ^H = wizhelp      W1 */
  //case   9  :    /* ^I = identify     W1 */
  //case  10  :    /* ^J = gain exp     W2 */
  //case  11  :    /* ^K = summon       W2 */
  //case  12  :    /* ^L = wizlight     W1 */
  //case  14  :    /* ^N = mon map      W1 */
  //case  15  :    /* ^O = summon       W2 */
  //case  20  :    /* ^T = teleport     W1 */
  //case  22  :    /* ^V = restore      W1 */
  //case  21  :    /* ^U = summon       W2 */
  //case  23  :    /* ^W = create       W2 */
  //case  24  :    /* ^X = ed score     W2 */
  //case  27  :    /* ^3 = store maint  W2 */
  //case  31  :    /* ^_                W1 */

  case   3:        /* ^C            */
    /* signalquit in io.c handles this one, it calls d__quit */

  case   0:        /* \0            */
  case   '@':      /* duh           */
  //case  25:      /* ^Y = exit     */  changed to be save
    d__quit();
    reset_flag = true;
    break;

  case  13:        /*  {^M = repeat  }*/
    msg_print(old_msg);
    reset_flag = true;
    break;

  case  16:        /*{^P = password}*/
    enter_wizard_mode(true);
    reset_flag = true;
    break;

  case  18:        /* ^R = redraw */
    draw_cave();
    reset_flag = true;
    break;

  case  25:        /* ^Y = (was exit) */
    if (total_winner) {
      msg_print("You are a Total Winner, your character must be retired...");
      msg_print("Use '@' when you are ready to quit.");
    } else {
      if (search_flag) {
	search_off();
      }
      py.flags.dead = false;
      save_char(false);
      py.flags.dead = true;
    }
    reset_flag = true;
    break;

  case  26:   /*{^Z = suspend  (we never get this, look at signalsuspend) }*/
      reset_flag = true;
      break;

  case  36:   /* {$  = Shell   }*/
  case '!':
    if (0) {
      shell_out();
    } else {
      msg_print("Use ^Z instead");
    }
    reset_flag = true;
    break;

  case  43:   /*{+ = lvl help }*/
    sprintf(out_val,"Character Classes Experience %4.2f",py.misc.expfact);
    moria_help(out_val);
    draw_cave();
    reset_flag = true;
    break;
    
  case  46:        /*{. = run / find     }*/
    y = char_row;
    x = char_col;
    if (d__get_dir("Which direction?",&dir_val,com_val,&y,&x)) {
      if (PF.image == 0) {
	find_flag = true;
	move_char(dir_val);
      } else {
	/* make them walk it off */
	switch (randint(25)) {
	case 1:       msg_print("It touches you.");    break;
	case 2:       msg_print("It hits you.");       break;
	case 3:       msg_print("It bites you.");      break;
	case 4:       msg_print("It crawles on you."); break;
	case 5:       msg_print("You miss it.");       break;
	case 6:       msg_print("You hit it.");        break;
	case 7:       msg_print("You have slain it."); break;
	default:      move_char(dir_val);              break;
	}      
      }
    }
    break;

  case  47:  /*{/ = identify }*/
    ident_char();
    reset_flag = true;
    break;

  case  49:        /*{ Move dir 1    }*/
  case  50:        /*{ Move dir 2    }*/
  case  51:        /*{ Move dir 3    }*/
  case  52:        /*{ Move dir 4    }*/
  case  54:        /*{ Move dir 6    }*/
  case  55:        /*{ Move dir 7    }*/
  case  56:        /*{ Move dir 8    }*/
  case  57:        /*{ Move dir 9    }*/
    move_char(*com_val - 48);
    break;

  case  53:        /*{ Rest one turn }*/
    move_char(*com_val - 48);
    usleep(10);
    flush();
    break;

  case  60:        /*{ < = go up    }*/
    d__go_up();
    break;

  case  62:        /*{ > = go down  }*/
    d__go_down();
    break;

  case  63:        /*{? = help     }*/
    help();
    reset_flag = true;
    break;

  case  65:       /*{A = age, Hours}*/
    msg_print(show_char_age(out_val));
    sprintf(out_val,"You have been playing for %s",show_play_time(tmp_str));
    msg_print(out_val);
    reset_flag = true;
    break;

  case  66:       /*{B = bash     } */
    d__bash();
    break;

  case  67:    /*{C = character}*/
    if (get_com("Print to file? (Y/N)",&command)) {
      switch (command) {
      case 'y': case 'Y':
	file_character();
	break;
      case 'n': case 'N':
	change_name();
	draw_cave();
	break;
      default:
	break;
      }
    }
    reset_flag = true;
    break;

  case  68:      /*{D = disarm   }*/
    d__disarm_trap();
    break;

  case  69:    /* {E = eat      }*/
    eat();
    break;

  case  70:       /* {F = refill   }*/
    d__refill_lamp();
    break;

  case  71:    /*{G = Game date}*/
    sprintf(out_val, "The date is %s",
	    full_date_string(py.misc.cur_age,out2));
    msg_print(out_val);
    reset_flag = true;
    break;

  case  72:    /*{H = moria hlp}*/
    moria_help("");
    draw_cave();
    reset_flag = true;
    break;

  case  73:       /*{I = Selected inv}*/
    reset_flag = true;
    if (inven_command('I',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case 74:       /* J = Screen Map */
    screen_map();
    break;

  case  75:   /* {K = Know Quest  }*/
    if (py.flags.quested) {
      sprintf(out_val, "Current quest is to kill a %s",
	      c_list[py.misc.cur_quest].name);
      msg_print(out_val);
    } else {
      msg_print("No quest currently.");
    }
    reset_flag = true;
    break;
    
  case  76:    /* {L = location }*/
    show_location();
    reset_flag = true;
    break;
    

  case  77:       /*{M = money }*/
    reset_flag = true;
    if (inven_command('M',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case  78:   /* {N = name mstr}*/
    mon_name();
    reset_flag = true;
    break;

  case  79:      /*{O = Old Mess }*/
    msg_print("Try 'V'.");
//  view_old_mess();                       
    reset_flag = true;
    break;
    
  case  80:     /*{P = print map}*/
    if ((py.flags.blind > 0) || (no_light())) {
      msg_print("You can't see to draw a map.");
    } else {
      print_map();
    }
    reset_flag = true;
    break;
    
  case  81:    /* {Q = toggle more}*/
    msg_terse =  !msg_terse;
    if (msg_terse) {
      msg_print("Question '-More-' toggled off");
      msg_terse = true;           /* try to only use true and false */
    } else {
      msg_print("Question '-More-' toggled on");
      msg_terse = false;
    }
    reset_flag = true;
    break;
    
  case  82:    /*  {R = rest     }*/
    rest();
    break;

    case  83:                  /*{S = srh mode }*/
      if (search_flag) {
	search_off();
	reset_flag = true;
      } else if (py.flags.blind > 0) {
	msg_print("You are incapable of searching while blind.");
      } else {
	search_on();
	reset_flag = true;
      }
      break;

  case  84:       /*  {T = tunnel   }*/
    d__tunnel();
    break;

  case  85:       /*  {U = use instr}*/
    blow();
    break;

  case  86:       /*  {V = preVious messages}*/
    msg_record("",false);         
    reset_flag = true;
    break;

  case  87:    /* {W = what time}*/
    sprintf(out_val, "The current time is %s",show_current_time(out2));
    msg_print(out_val);
    reset_flag = true;
    break;

  case  88:    /* {X = toggle light source }*/
    reset_flag = true;
    if (equipment[Equipment_light].tval > 0) {
      if (equipment[Equipment_light].p1 > 0) {
	if (PF.light_on) {
	  sprintf(out_val, "Light Off.  %ld turns left.",
		  equipment[Equipment_light].p1);
	  PF.light_on    = false;
	  player_light   = false;
	} else {
	  sprintf(out_val, "Light On.  %ld turns left.",
		  equipment[Equipment_light].p1);
	  PF.light_on    = true;
	  player_light   = true;
	}
	prt_light_on();
	msg_print(out_val);
	move_light(char_row,char_col,char_row,char_col);
      } else {
	msg_print("Your light has gone out!");
      }
    } else {
      msg_print("You are not carrying a light.");
    }
    break;

  case  93:   /* {] = armr help}*/
    moria_help("Adventuring Armor_Class Armor_List");
    draw_cave();
    reset_flag = true;
    break;

  case  97:   /* {a = aim      }*/
    aim_wand();
    break;

  case  98:  /*  {b = browse   }*/
    d__examine_book();
    break;

  case  99:       /* {c = close    }*/
    d__closeobject();
    break;

  case 100:       /* {d = drop     }*/
    d__drop();
    break;

  case 101:       /*  {e = equipment }*/
    reset_flag = true;
    if (inven_command('e',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case 102:      /*  {f = fire } */
    d__throw_object(true);
    break;
    
  case 104:     /*  {h = hurlx }*/
    d__throw_object(false);
    break;

  case 105:       /*{i = inventory }*/
    reset_flag = true;
    if (inven_command('i',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case 106:    /*  {j = jam      }*/
    d__jamdoor();
    break;

  case 108:    /*{l = look     }*/
    d__look();
    reset_flag = true;
    break;

  case 109:    /* m = magick, monk, music */
    if (class[py.misc.pclass].mspell) {
      cast();             /* {m = magick   } */
    } else if (class[py.misc.pclass].mental) {
      discipline();       /* {m = monk? :) } */
    } else {
      sing();             /* {m = music    } */
    }
    break;

  case 111:    /* {o = open     } */
    d__openobject();
    break;

  case 112:    /* p = pray, play */
    if (class[py.misc.pclass].pspell) {
      pray();          /*{p = pray     }*/
    } else {
      play();          /*{p = play     }*/
    }
    break;

  case 113:     /*  {q = quaff    }*/
    quaff();
    break;

  case 114:     /* {r = read     }*/
    read_scroll();
    break;

  case 115:     /* {s = search   }*/
    if (py.flags.blind > 0) {
      msg_print("You are incapable of searching while blind.");
    } else {
      search(char_row,char_col,py.misc.srh);
    }
    break;

  case 116:       /*{t = unwear }*/
    reset_flag = true;
    if (inven_command('t',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case 117:   /*{u = use staff}*/
    use_staff();
    break;

  case 118:   /*  {v = version  }*/
    reset_flag = true;
    game_version();
    break;

  case 119:       /*{ w = wear     }*/
    reset_flag = true;
    if (inven_command('w',&trash_ptr,"")) {
      draw_cave();
    } else {
      prt_weight();
    }
    break;

  case 120:       /*{x = exchange }*/
    reset_flag = true;
    if (inven_command('x',&trash_ptr,"")) {
      draw_cave();
    }
    break;

  case 124:    /*{| = wpn help }*/
    moria_help("Adventuring Weapons Weapon_List");
    draw_cave();
    reset_flag = true;
    break;

  case 'g':
    if (PM.cur_age.hour > 10) {
      PM.cur_age.hour = 5;
      PM.cur_age.secs = 290;
    } else {
      PM.cur_age.hour = 17;
      PM.cur_age.secs = 290;
    }
    reset_flag = true;
    break;

  default:
    reset_flag = true;

    if (!wizard1) {  /* begin wizard commands */
      prt("Type '?' for help...",1,1);
    } else {

      switch (*com_val) {
	/* wizard commands */

      case   1:  /*    {^A = Cure all} */
	//with py.flags do;
	hp_player(1000,"cheating");
	PM.cmana = PM.mana;
	if (is_magii) {
	  prt_mana();
	}
	remove_curse();
	cure_me(&(PF.blind)); cure_me(&(PF.hoarse)); cure_me(&(PF.afraid));
	cure_me(&(PF.poisoned)); cure_me(&(PF.confused));

	for (tstat = STR; tstat <= CHR; tstat++) {
	  restore_stat(tstat,"");
	}

	if (PF.slow > 1) {
	  PF.slow = 1;
	}
	if (PF.image > 1) {
	  PF.image = 1;
	}
	break;

      case   2  :     /*{^B = objects }*/
	print_objects();
	break;

      case   4  :    /*{^D = up/down }*/
	prt("Go to which level (0 -1200) ? ",1,1);
	if (get_string(tmp_str,1,31,10)) {
	  i1 = -1;
	  sscanf(tmp_str,"%ld",&i1);
	  if (i1 > -1 || !strcmp(tmp_str, "*")) {
	    dun_level = i1;
	    if (dun_level > 1200) {
	      dun_level = 1200;
	    } else if (dun_level < 0) {
	      dun_level = py.misc.max_lev;
	    }
	    moria_flag = true;
	  } else {
	    erase_line(msg_line,msg_line);
	  }
	} else {
	  erase_line(msg_line,msg_line);
	}
	break;

      case   8:    /*  {^H = wizhelp }*/
	wizard_help();
	break;

      case   9:    /* {^I = identify}*/
	msg_print("Poof!  Your items are all identifed!!!");
	for (trash_ptr = inventory_list ; trash_ptr != NULL;) {
	  identify(&(trash_ptr->data));
	  known2(trash_ptr->data.name);
	  trash_ptr = trash_ptr->next;
	}
	break;

      case  12:    /*  {^L = wizlight}*/
	wizard_light();
	break;

      case  14:    /*{^N = mon map }*/
	print_monsters();
	break;

      case  20:    /*{^T = teleport}*/
	teleport(100);
	break;

      case  22:    /*{^V = restore }*/
	/* this nukes the current character so I disabled it, use -U or -R */

	//restore_char("",false,false);  
	break;

      case  31:    /* ^_  Can you say security through obscurity? */
	if (wizard1 && search_flag && PM.cheated) {
	  py.misc.cheated = false;
	  msg_print("Cheat flag turned off.");
	}
	break;

      default:
	if (!wizard2) {
	  prt("Type '?' for help...",1,1);
	} else {

	  switch (*com_val) {
	    /* wizard2 commands */

	  case   5:    /* {^E = wizchar }  */
	    change_character();                       
	    break;

	  case   6:  /*   {^F = genocide} */
	    mass_genocide();
	    break;

	  case   7:  /*{^G = treasure}*/
	    alloc_object(floor_set,5,25);
	    prt_map();
	    break;

	  case  10:  /*{^J = gain exp}*/
	    if (py.misc.exp == 0) {
	      py.misc.exp = 1;
	    } else {
	      py.misc.exp *= 2;
	    }
	    prt_experience();
	    break;

	  case  11:    /* {^K = summon  }*/
	    y = char_row;
	    x = char_col;
	    if (is_in(cave[y][x].fval, water_set)) {
	      summon_water_monster(&y,&x,true);
	    } else {
	      summon_land_monster(&y,&x,true);
	    }
	    creatures(false);
	    break;

	  case  15:    /*{^O = summon  }*/
	    monster_summon_by_name(char_row,char_col,"",false,true);
	    creatures(false);
	    break;

	  case  21:    /*   {^U = summon  }*/
	    if (cave[char_row][char_col].tptr == 0) {
	      summon_item(char_row,char_col,"","",0,false);
	    } else {
	      msg_print("You are standing on something!");
	    }
	    break;

	  case  23:   /* {^W = create  }*/
	    if (cave[char_row][char_col].tptr == 0) {
	      wizard_create();
	    } else {
	      msg_print("You are standing on something!");
	    }
	    break;

	  case  24 :   /* {^X = ed score}*/
	    edit_score_file();
	    break;

	  case  27:    /* ^3  Run store_maint */
	    store_maint();
	    msg_print("Stores updated.");
	    break;

	  default:
	    prt("Type '?' for help...",1,1);
	    break;
	  }

	}
      }
    }
    break;

  }/* end com_val switch */

  LEAVE("d__execute_command", "d");
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/*ZZZZZZZZZZZZZZZ*/
void dungeon()
{
  ENTER("dungeon", "d")

  s1[0] = 0;  s2[0] = 0;  s3[0] = 0;  s4[0] = 0;
  
  cur_inven    = inventory_list;
  i_summ_count = 0;

  /*{ Check light status for setup          }*/
  if ((equipment[Equipment_light].p1 > 0) && PF.light_on) {
    player_light = true;
  } else {
    player_light = false;
  }

  /*{ Check for a maximum level             }*/
  if (dun_level > py.misc.max_lev) {
    py.misc.max_lev = dun_level;
  }

  d__set_coords(&char_row, &char_col);

  /*{ Reset flags and initialize variables  }*/
  moria_flag    = false;
  cave_flag     = false;
  find_flag     = false;
  search_flag   = false;
  teleport_flag = false;
  mon_tot_mult  = 0;
  cave[char_row][char_col].cptr = 1;
  old_chp   = trunc(py.misc.chp);
  old_cmana = trunc(py.misc.cmana);

  /*{ Light up the area around character    }*/
  move_char(5);

  /*{ Light, but do not move critters       }*/
  creatures(false);

  /*{ Print the depth                       }*/
  prt_depth();

  /*{ Loop until dead, or new level 		}*/
  do {
    /*{ Check for the AST's			-DMF-	}*/
    /*if (want_trap) then dump_ast_mess; XXXX*/
    
    /*{ Increment turn counter			}*/
    turn++;
    
    if ((PF.speed > 0) || ((turn % (abs(PF.speed) + 1)) == 0)) {
      water_move();
      adv_time(true);	/*{ Increment game time }*/
    }
    
    d__sun_rise_or_set();
    
    /*{ Check for kickout				}*/
    check_kickout_time(turn,10);
    
    d__check_hours();
    
    /*{ Check for random wandering monster generation 		}*/
    if (randint(MAX_MALLOC_CHANCE) == 1) {
      alloc_land_monster(floor_set,1,MAX_SIGHT,false,false);
    }    

    /*{ Screen may need updating, used mostly for stats}*/
    d__print_updated_stats();

    d__check_light_status();

    /*{ Update counters and messages			}*/
    //with py.flags do;
    
    d__check_food();
    d__eat_food();
    d__regenerate();
    d__update_blindness();
    d__update_confusion();
    d__update_resist_lightning();
    d__update_monster_protect();
    d__update_fire_ring();
    d__update_frost_ring();
    d__update_blade_barrier();
    d__update_magic_protect();
    d__update_resist_petrfy();
    d__update_stealth();
    d__update_resist_charm();
    d__update_hoarse();
    d__update_fear();
    d__update_poison();
    d__update_fast();
    d__update_slow();
    d__update_resting();
    d__update_hallucinate();
    d__update_petrify();
    d__update_evil_protect();
    d__update_invulnerable();
    d__update_heroism();
    d__update_super_heroism();
    d__update_blessed();
    d__update_resist_heat();
    d__update_resist_cold();
    d__update_detect_invisible();
    d__update_infra_vision();
    d__update_word_of_recall();
    d__update_hit_points();

    if ((py.flags.paralysis < 1) &&        /*{ Accept a command?     }*/
	(py.flags.rest < 1) &&
	(!(death))) {

      /*{ Accept a command and execute it                               }*/
      do {
	print_stat = 0;
	reset_flag = false;
	turn_counter++;
	if (turn_counter > 4000000) {
	  turn_counter = 100000;
	}
	
	/*{ Random teleportation  }*/
	if (py.flags.teleport) {
	  if (randint(100) == 1) {
	    find_flag = false;
	    teleport(40);
	  }
	}
	
	if (!find_flag) {
          print_null(char_row, char_col);
	  save_msg_flag = msg_flag;
	  game_state = GS_GET_COMMAND;
	  command = inkey();
	  game_state = GS_IGNORE_CTRL_C;
	  if (save_msg_flag) {
	    erase_line(msg_line,msg_line);
	  }
	  com_val = (integer)command;
	}
	
	d__execute_command(&com_val);
	
      } while (!(!reset_flag || moria_flag)); /* end command do */
      
    } /* end if (able to do something) */
    /*{ Teleport?                     }*/
    if (teleport_flag) {
      teleport(100);
    }
    
    /*{ Move the creatures            }*/
    if (!moria_flag) {
      creatures(true);
    }
    
    /*{ Exit when moria_flag is set   }*/
  } while (!moria_flag);
  
  if (search_flag) {
    search_off(); /*{ Fixed "SLOW" bug; 06-11-86 RAK     }*/
  } 

  LEAVE("dungeon", "d")
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  dungeon.c */
