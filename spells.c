/* spells.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void lower_stat(stat_set tstat, vtype msg1)
{


  //with py.stat do;
  PS.l[(int)tstat] += de_statp(PS.p[(int)tstat] - PS.l[(int)tstat]);
  update_stat(tstat);
  if (strcmp(msg1, "X") == 0) {
    switch (tstat) {
    case STR : msg_print("You feel very sick.");        break;
    case INT : msg_print("You become very dizzy.");     break;
    case WIS : msg_print("You feel very naive.");       break;
    case DEX : msg_print("You feel very sore.");        break;
    case CON : msg_print("You feel very sick.");        break;
    case CHR : msg_print("Your skin starts to itch.");  break;
    }
  } else if (strlen(msg1) != 0) {
    msg_print(msg1);
  }
  prt_a_stat(tstat);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean lose_stat(stat_set tstat, vtype msg1, vtype msg2)
{
  boolean  return_value = true;

  if (!(PF.sustain[(int)tstat])) {
    lower_stat(tstat,msg1);
  } else {
    if (strcmp(msg2,"X") == 0) {
      switch (tstat) {
      case STR : 
	msg_print("You feel sick for a moment, then it passes.");     break;
      case INT : 
	msg_print("You become dizzy for a moment, then it passes.");  break;
      case WIS : 
	msg_print("You feel naive for a moment, then it passes.");    break;
      case DEX : 
	msg_print("You feel sore for a moment, then it passes.");     break;
      case CON : 
	msg_print("You feel sick for a moment, then it passes.");     break;
      case CHR : 
	msg_print("Your skin starts to itch, but feels better now."); break;
      }
    } else if (strlen(msg2) != 0) {
      msg_print(msg2);
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean restore_stat(stat_set tstat,vtype msg1)
{
  /*{stat adjusted by magic worn only}*/
  boolean  return_value = true;

  //with py.stat do;
  if (PS.l[(int)tstat] > 0) {
    PS.l[(int)tstat] = 0;
    update_stat(tstat);
    if (strcmp(msg1, "X") == 0) {
      switch (tstat) {
      case STR : msg_print("You feel warm all over.");         break;
      case INT : msg_print("You have a warm feeling.");        break;
      case WIS : msg_print("You feel your wisdom returning."); break;
      case DEX : msg_print("You feel less clumsy.");           break;
      case CON : msg_print("You feel your health returning."); break;
      case CHR : msg_print("You feel your looks returning.");  break;
      }
    } else if (strlen(msg1) != 0) {
      msg_print(msg1);
    }
    prt_a_stat(tstat);
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean cure_me(integer *what_flag)
{
  boolean   return_value = false;

  /*{py.flags:  confused, blind, poisoned, hoarse, afraid, image}*/

  if (*what_flag > 1) {
    *what_flag   = 1;
    return_value = true;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean remove_curse()
{
  /*{ Removes curses from items in inventory		-RAK-	}*/

  integer   i1;
  boolean   return_value = false;

  for (i1 = Equipment_primary; i1 <= Equipment_cloak; i1++) {
    //with equipment[i1] do;
    if ((0x80000000 & equipment[i1].flags) != 0) {
      equipment[i1].flags &= 0x7FFFFFFF;
      py_bonuses(&blank_treasure,0);
      return_value = true;
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean hp_player(integer num, vtype kind)
{
  /*{ Change players hit points in some manner		-RAK-	}*/

  boolean   return_value = false;

  //with py.misc do;
  if (num < 0) {
    take_hit(num,kind);
    if (PM.chp < 0) {
      msg_print("You feel your life slipping away!");
      msg_print(" ");
    }
    return_value = true;
  } else if (PM.chp < PM.mhp) {
    PM.chp += num;
    if (PM.chp > PM.mhp) {
      PM.chp = PM.mhp;
    }
    prt_hp();
    
    switch ((integer)(num/5)) {
    case 0: 
      msg_print("You feel a little better.");   break;
    case 1: case 2:
      msg_print("You feel better.");            break;
    case 3: case 4: case 5: case 6:
      msg_print("You feel much better.");       break;
    default:
      msg_print("You feel very good.");         break;
    }
    return_value = true;
  }

      return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean aggravate_monster(integer dis_affect)
{
  /*{ Get all the monsters on the level pissed off...	-RAK-	}*/

  /* I'm guessing that this was supposed to only return true if any
     monsters were in range of the effect, it used to always be true */

  integer    i1;
  boolean    return_value = false;

  i1 = muptr;
  do {
    //with m_list[i1] do;
    m_list[i1].csleep = 0;
    if (m_list[i1].cdis <= dis_affect) {
      if (m_list[i1].cspeed < 2) {
	m_list[i1].cspeed++;
      }
      return_value = true;
    }
    i1 = m_list[i1].nptr;
  } while (i1 != 0);

  return return_value;
}; 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean explode(integer typ,integer y,integer x,integer dam_hp,ctype descrip)
{
  integer   i1,i2;
  integer   dam,max_dis,thit,tkill;
  integer   weapon_type,harm_type;
  obj_set   *destroy;
  vtype	    out_val;
  boolean   return_value = true;

  thit	  = 0;
  tkill	  = 0;
  max_dis = 2;

  get_flags(typ,&weapon_type,&harm_type,&destroy);
  
  for (i1 = y-max_dis; i1 <= y+max_dis; i1++) {
    for (i2 = x-max_dis; i2 <= x+max_dis; i2++) {
      if (in_bounds(i1,i2)) {
        if (distance(y,x,i1,i2) <= max_dis) {
          if (los(y,x,i1,i2)) {               /*{ FIXED BUG V4.5        }*/
            //with cave[i1,i2] do;
	    if (cave[i1][i2].tptr > 0) {
	      if (is_in(t_list[cave[i1][i2].tptr].tval, *destroy)) {
		delete_object(i1,i2);
	      }
	    }
	    if (cave[i1][i2].fopen) {
	      if (panel_contains(i1,i2)) {
		print('*',i1,i2);
		put_qio();
		usleep(DRAW_BALL_DELAY);
	      }
	      if (cave[i1][i2].cptr > 1) {
		//with m_list[cave[i1][i2].cptr] do;
		//with c_list[m_list[cave[i1][i2].cptr].mptr] do;
		dam = dam_hp;
		
		if ((typ == c_illusion) || (typ == c_joke)) {
		  if (mon_save(cave[i1][i2].cptr,0,c_sc_mental)) {
		    if (typ == c_illusion) {
		      dam = 0;
		    } else {
		      dam = dam / 4;
		    }
		  } else {
		    m_list[cave[i1][i2].cptr].confused = true;
		  }
		}
		
		if ((harm_type & 
		     c_list[m_list[cave[i1][i2].cptr].mptr].cdefense) != 0) {
		  dam *= 2;
		} else if ((weapon_type & 
			    c_list[m_list[cave[i1][i2].cptr].mptr].spells) != 0) {
		  dam /= 4;
		}
		
		dam = (integer)(dam/(distance(i1,i2,y,x)+1));
		  
		if ((dam > 0) && (! mon_resists(cave[i1][i2].cptr))) {
		  thit++;
		  if (mon_take_hit(cave[i1][i2].cptr,dam) > 0) {
		    tkill++;
		  } else {
		    if (panel_contains(i1,i2)) {
		      print(c_list[m_list[cave[i1][i2].cptr].mptr].cchar,
			    i1,i2);
		      m_list[cave[i1][i2].cptr].ml = true;
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    } /* end for */
  } /* end for */
  
  
  for (i1 = y-max_dis; i1 <= y+max_dis; i1++) {
    for (i2 = x-max_dis; i2 <= x+max_dis; i2++) {
      if (in_bounds(i1,i2)) {
        if (panel_contains(i1,i2)) {
	  
          if (distance(y,x,i1,i2) <= max_dis) {
            //with cave[i1,i2] do;
	    if (test_light(i1,i2)) {
	      lite_spot(i1,i2);
	    } else if (cave[i1][i2].cptr == 1) {
	      lite_spot(i1,i2);
	    } else if (cave[i1][i2].cptr > 1) {
	      if (m_list[cave[i1][i2].cptr].ml) {
		lite_spot(i1,i2);
	      } else {
		unlite_spot(i1,i2);
	      }
	    } else {
	      unlite_spot(i1,i2);
	    }
          }
	}
      }
    }
  }	  
  /*{ End  explosion...                     }*/
  if (thit == 1) {
    sprintf(out_val, "The %s envelopes a creature!",descrip);
    msg_print(out_val);
  } else if (thit > 1) {
    sprintf(out_val, "The %s envelopes several creatures!",descrip);
    msg_print(out_val);
  }
  
  if (tkill == 1) {
    if (typ == c_joke) {
      msg_print("There is a scream of side-splitting laughter!");
    } else {
      msg_print("There is a scream of agony!");
    }
  } else if (tkill > 1) {
    if (typ == c_joke) {
      msg_print("There are several screams of agonized laughter!");
    } else {
      msg_print("There are several screams of agony!");
    }
  }
  /*{ End ball hitting...                   }*/
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void get_flags (integer typ,integer *weapon_type,integer *harm_type,
		obj_set **destroy)
{
  /*{ Return flags for given type area affect               -RAK-   }*/

  switch (typ) {
  case c_lightning: /* {1} */
    *weapon_type = 0x00080000;
    *harm_type   = 0x0100;
    *destroy     = &destroyed_by_lightning;
    break;

  case c_gas  :  /* {2}*/
    *weapon_type = 0x00100000;
    *harm_type   = 0x0040;
    *destroy     = &null_obj_set;
    break;

  case c_acid  :  /* {3}*/
    *weapon_type = 0x00200000;
    *harm_type   = 0x0080;
    *destroy     = &destroyed_by_acid;
    break;

  case c_cold  :  /* {4}*/
    *weapon_type = 0x00400000;
    *harm_type   = 0x0010;
    *destroy     = &destroyed_by_cold;
    break;

  case c_fire  :  /* {5}*/
    *weapon_type = 0x00800000;
    *harm_type   = 0x0020;
    *destroy     = &destroyed_by_fire;
    break;

  case c_good  :  /* {6}*/
    *weapon_type = 0x00000000;
    *harm_type   = 0x0004;
    *destroy     = &null_obj_set;
    break;

  case c_evil  :  /* {7}*/
    *weapon_type = 0x00000000;
    *harm_type   = 0x0000;
    *destroy     = &null_obj_set;
    break;

  case c_petrify  :  /* {8}*/
    *weapon_type = 0x00000000;
    *harm_type   = 0x0000;
    *destroy     = &destroyed_by_petrify;
    break;

  case c_sunray :  /* {9}*/
    *weapon_type = 0x00000000;
    *harm_type   = 0x0108;
    *destroy     = &destroyed_by_sunray;
    break;

  default:
    *weapon_type = 0;
    *harm_type   = 0;
    *destroy     = &null_obj_set;
    break;

  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean mon_save(integer a_cptr,integer bonus,integer spell_class)
{
  integer   mon_level;
  boolean   return_value;

  mon_level = c_list[m_list[a_cptr].mptr].level;

  //with m_list[a_cptr] do;
  //with c_list[mptr] do;
  if ((0x1000 & c_list[m_list[a_cptr].mptr].cdefense) != 0) {
    switch (spell_class) {
    case c_sc_hold : 
      bonus += 4;
      break;

    case c_sc_mental :
      bonus += 20;
      break;

    default:
      break;
    }
  }

  return_value = ((20+mon_level+randint(mon_level)+5*bonus) >
		  (randint(80)+randint(py.misc.lev)));
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean mon_resists(byteint a_cptr)
{
  integer    res_chance;
  integer    delta_lev;
  vtype      out_val;
  boolean    return_value;

  //with m_list[a_cptr] do;
  //with c_list[m_list[a_cptr].mptr] do;

  res_chance = c_list[m_list[a_cptr].mptr].mr;

  delta_lev = (py.misc.lev + py.misc.mr);
  if (delta_lev < 0) {
    delta_lev = 0;
  }

  res_chance -= (5 * delta_lev);
  if (res_chance < 0) {
    res_chance = 0;
  }

  if (res_chance >= randint(100)) {
    sprintf(out_val, "The %s is protected by a mysterious force.", 
	    c_list[m_list[a_cptr].mptr].name); 
    msg_print(out_val);
    return_value = true;
  } else {
    return_value = false;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean do_stun(byteint a_cptr, integer save_bonus, integer time)
{
  integer   held;
  vtype     m_name,out_val;
  boolean   return_value = false; /* was true, but that seemed odd */

  //with m_list[a_cptr]. do;
  find_monster_name(m_name,a_cptr,true);
  if (! mon_resists(a_cptr)) {
    if (! mon_save(a_cptr,save_bonus,c_sc_hold)) {
      return_value = true;
      sprintf(out_val, "%s appears stunned!",m_name);
      msg_print(out_val);
      held = m_list[a_cptr].stunned + 1 + randint(time);
      if (held > 24) {
	m_list[a_cptr].stunned = 24;
      } else {
	m_list[a_cptr].stunned = held;
      }
    }
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean teleport_away(integer monptr,integer dis)
{
  /*{ Move the creature record to a new location            -RAK-   }*/

  integer   yn,xn,ctr;
  boolean   return_value = false;

  //with m_list[monptr] do;
  if (!mon_resists(monptr)) {
    ctr = 0;
    do {
      do {
	yn = m_list[monptr].fy + (randint(2*dis+1) - (dis + 1));
	xn = m_list[monptr].fx + (randint(2*dis+1) - (dis + 1));
      } while (!in_bounds(yn,xn));
      ctr++;
      if (ctr > 9) {
	ctr = 0;
	dis += 5;
      }
    } while (!((cave[yn][xn].fopen) && (cave[yn][xn].cptr == 0)));
    
    move_rec(m_list[monptr].fy,m_list[monptr].fx,yn,xn);
    if (test_light(m_list[monptr].fy,m_list[monptr].fx)) {
      lite_spot(m_list[monptr].fy,m_list[monptr].fx);
    }
    m_list[monptr].fy = yn;
    m_list[monptr].fx = xn;
    m_list[monptr].ml = false;
    return_value = true;
  }    

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean teleport_to(integer ny,integer nx)
{
  /*{ Teleport player to spell casting creature             -RAK-   }*/

  integer   dis,ctr,y,x,i1,i2;
  boolean   return_value = true;

  dis = 1;
  ctr = 0;
  do {
    do {
      y = ny + (randint(2*dis+1) - (dis + 1));
      x = nx + (randint(2*dis+1) - (dis + 1));
    } while (!in_bounds(y,x));
    ctr++;
    if (ctr > 9) {
      ctr = 0;
      dis++;
    }
  } while (!((cave[y][x].fopen) && (cave[y][x].cptr < 2)));
  
  move_rec(char_row,char_col,y,x);
  for (i1 = char_row-1; i1 <= char_row+1; i1++) {
    for (i2 = char_col-1; i2 <= char_col+1; i2++) {
      //with cave[i1][i2]. do;
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
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean breath(integer typ,integer y,integer x,integer dam_hp,vtype ddesc)
{
  /*{ Breath weapon works like a fire_ball, but affects the player. }*/
  /*{ Note the area affect....                              -RAK-   }*/

  integer    i1,i2;
  integer    dam,max_dis;
  integer    weapon_type,harm_type;
  obj_set    *destroy;
  boolean    return_value = true;

  max_dis = 2;
  get_flags(typ,&weapon_type,&harm_type,&destroy);
  for (i1 = y-2; i1 <= y+2; i1++) {
    for (i2 = x-2; i2 <= x+2; i2++) {
      if (in_bounds(i1,i2)) {
	if (distance(y,x,i1,i2) <= max_dis) {
	  //with cave[i1][i2]. do;
	  if (cave[i1][i2].tptr > 0) {
	    if (is_in(t_list[cave[i1][i2].tptr].tval, *destroy)) {
	      delete_object(i1,i2);
	    }
	  }
	  if (fopen) {
	    if (panel_contains(i1,i2)) {
	      print('*',i1,i2);
	      put_qio();
	      usleep(DRAW_BOLT_DELAY);
	    }
	    if (cave[i1][i2].cptr > 1) {
	      //with m_list[cave[i1][i2].cptr]. do;
	      //with c_list[m_list[cave[i1][i2].cptr].mptr]. do;
	      dam = dam_hp;
	      if (uand(harm_type,
		       c_list[m_list[cave[i1][i2].cptr].mptr].cdefense) != 0) {
		dam *= 2;
	      } else if (uand(weapon_type,
		      c_list[m_list[cave[i1][i2].cptr].mptr].spells) != 0) {
		dam = trunc(dam/4.0);
	      }
	      dam = (integer)(dam/(distance(i1,i2,y,x)+1));
	      if (!mon_resists(cave[i1][i2].cptr)) {
		m_list[cave[i1][i2].cptr].hp -= dam;
	      }
	      m_list[cave[i1][i2].cptr].csleep = 0;
	      if (m_list[cave[i1][i2].cptr].hp < 0) {
		monster_death(m_list[cave[i1][i2].cptr].fy,
			      m_list[cave[i1][i2].cptr].fx,
			      c_list[m_list[cave[i1][i2].cptr].mptr].cmove);
		delete_monster(cave[i1][i2].cptr);
	      }
	    } else if (cave[i1][i2].cptr == 1) {
	      dam = trunc(dam_hp/(distance(i1,i2,y,x)+1));
	      switch (typ) {
	      case c_lightning :     light_dam(dam,ddesc);   break;
	      case c_gas       :     poison_gas(dam,ddesc);  break;
	      case c_acid      :     acid_dam(dam,ddesc);    break;
	      case c_cold      :     cold_dam(dam,ddesc);    break;
	      case c_fire      :     fire_dam(dam,ddesc);    break;
	      case c_evil      :     xp_loss(dam);           break;
	      }
	    }
	  }
	}
      }
    }
  }

  for (i1 = (y - 2); i1 <= (y + 2); i1++) {
    for (i2 = (x - 2); i2 <= (x + 2); i2++) {
      if (in_bounds(i1,i2)) {
	if (panel_contains(i1,i2)) {
	  if (distance(y,x,i1,i2) <= max_dis) {
	    //with cave[i1][i2]. do;
	    if (test_light(i1,i2)) {
	      lite_spot(i1,i2);
	    } else if (cave[i1][i2].cptr == 1) {
	      lite_spot(i1,i2);
	    } else if (cave[i1][i2].cptr > 1) {
	      if (m_list[cave[i1][i2].cptr].ml) {
		lite_spot(i1,i2);
	      } else {
		unlite_spot(i1,i2);
	      }
	    } else {
	      unlite_spot(i1,i2);
	    }
	  }
	}
      }
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void lose_exp(integer amount)
{
  /*{ Lose experience                                       -RAK-   }*/

  integer    i1,i2;
  integer    av_hp,lose_hp;
  integer    av_mn,lose_mn;
  boolean    flag;

  //with py.misc do;
  if (amount > PM.exp) {
    PM.exp = 0;
  } else {
    PM.exp -= amount;
  }

  for (i1=1; trunc(player_exp[i1]*PM.expfact) <= PM.exp; i1++) {
  }

  for (i2 = PM.lev - i1; i2 > 0;) {
    i2--;
    PM.lev--;
    av_hp    = trunc(PM.mhp/PM.lev);
    av_mn    = trunc(PM.mana/PM.lev);
    lose_hp  = randint(av_hp*2-1);
    lose_mn  = randint(av_mn*2-1);
    PM.mhp  -= lose_hp;
    PM.mana -= lose_mn;
    if (PM.mhp  < 1) { PM.mhp  = 1; }
    if (PM.mana < 0) { PM.mana = 0; }

    //with class[pclass] do;
    if ((class[PM.pclass].mspell) || (class[PM.pclass].pspell) || 
	(class[PM.pclass].dspell) || (class[PM.pclass].bspell)) {
      i1   = 32;
      flag = false;

      do {
	i1--;
	if (magic_spell[PM.pclass][i1].learned) {
	  flag = true;
	}
      } while (!((flag) || (i1 < 2)));
      
      if (flag) {
	magic_spell[PM.pclass][i1].learned = false;
	if (class[PM.pclass].mspell) {
	  msg_print("You have forgotten a magic spell!");
	} else if (class[PM.pclass].pspell) {
	  msg_print("You have forgotten a prayer!");
	} else {
	  msg_print("You have forgotten a song!");
	}
      }
    }
  } /* end for */

  if (PM.chp   > PM.mhp)  { PM.chp   = PM.mhp; }
  if (PM.cmana > PM.mana) { PM.cmana = PM.mana;}
  strcpy(PM.title, player_title[PM.pclass][PM.lev]);
  prt_experience();
  prt_hp();
  if (is_magii) { prt_mana(); }
  prt_level();
  prt_title();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean gain_stat(stat_set tstat, vtype msg1)
{
  integer     i1;
  boolean     return_value = true;

  i1 = in_statp(PS.p[(int)tstat] - PS.l[(int)tstat]);
  if (i1 < PS.l[(int)tstat]) {
    PS.l[(int)tstat] -= i1;
  } else {
    PS.l[(int)tstat] = 0;
    PS.p[(int)tstat] = squish_stat(PS.p[(int)tstat] - PS.l[(int)tstat] + i1);
  }
  update_stat(tstat);

  if (strcmp(msg1,"X") == 0) {
    switch (tstat) {
    case STR : msg_print("Wow!  What bulging muscles!");            break;
    case INT : msg_print("Aren't you brilliant!");                  break;
    case WIS : msg_print("You suddenly have a profound thought!");  break;
    case DEX : msg_print("You feel more limber!");                  break;
    case CON : msg_print("You feel tingly for a moment.");          break;
    case CHR : msg_print("Gee ain't you cute!");                    break;
    }
  } else if (strlen(msg1) != 0) {
    msg_print(msg1);
  }

  prt_a_stat(tstat);

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean slow_poison()
{
  /*{ Slow Poison                                           -RAK-   }*/
  boolean     return_value = false;

  if (PF.poisoned > 0) {
    PF.poisoned /= 2;
    if (PF.poisoned < 1) {
      PF.poisoned = 1;
    }
    return_value = true;
    msg_print("The effects of the poison has been reduced.");
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean restore_level()
{
  /*{ Restores any drained experience                       -RAK-   }*/

  byteint   max_level = 1;
  boolean   return_value = false;

  while ((integer)(player_exp[max_level]*py.misc.expfact) <= py.misc.max_exp) {
    max_level++;
  }

  if (randint(100) > (max_level - py.misc.lev)*2.25) {

    //with py.misc do;
    return_value = true;
    msg_print("You feel your life energies returning...");
    for (;PM.exp < PM.max_exp;) {
      PM.exp = PM.max_exp;
      prt_experience();
    }

  } else {
    msg_print("The restoring fails!");
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_creatures(integer typ)
{
  /*{ Display evil creatures on current panel               -RAK-   }*/

  integer    i1;
  boolean    flag,found;

  flag = false;
  i1 = muptr;

  do {
    //with m_list[i1]. do;
    if (panel_contains(m_list[i1].fy,m_list[i1].fx)) {

      switch (typ) {
      case c_evil : 
	found = uand(0x0004,c_list[m_list[i1].mptr].cdefense)!=0;  break;
      case c_monster : 
	found = uand(0x10000,c_list[m_list[i1].mptr].cmove)==0;    break;
      case c_invisible : 
	found = uand(0x10000,c_list[m_list[i1].mptr].cmove)!=0;    break;
      default : 
	MSG("Unknown typ in detect_creatures");
	found = false; 	break;
      }
      
      if (found) {
	m_list[i1].ml = true;
	print(c_list[m_list[i1].mptr].cchar,m_list[i1].fy,m_list[i1].fx);
	flag = true;
      }
    }      

    i1 = m_list[i1].nptr;
      
  } while (i1 != 0);
    
  if (flag) {
    switch (typ) {
    case c_evil : 
      msg_print("You sense the presence of evil!"); break;
    case c_monster : 
      msg_print("You sense the presence of monsters!"); break;
    case c_invisible : 
      msg_print("You sense the presence of invisible creatures!"); break;
    }
    msg_print(" ");
    msg_flag = false;
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_inv2(integer amount)
{
  /*{ Detect Invisible for period of time                   -RAK-   }*/

  PF.detect_inv += amount;
  
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_item(integer typ)
{
  /*{ Detect treasure or object on current panel    -Steven-}*/

  integer    i1,i2;
  boolean    show_it;
  obj_set    treasures = {valuable_jewelry,valuable_gems,valuable_metal,0};
  boolean    flag = false;


  for (i1 = panel_row_min; i1 <= panel_row_max; i1++) {
    for (i2 = panel_col_min; i2 <= panel_col_max; i2++) {
      //with cave[i1][i2]. do;
      if (cave[i1][i2].tptr > 0) {
	if (typ == c_treasure) {
	  show_it = is_in(t_list[cave[i1][i2].tptr].tval, treasures);
	} else {
	  show_it = (t_list[cave[i1][i2].tptr].tval < valuable_metal);
	}
	
	if (show_it) {
	  if (!(test_light(i1,i2))) {
	    lite_spot(i1,i2);
	    cave[i1][i2].tl = true;
	    flag = true;
	  }
	}

      }
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_trap()
{
  /*{ Locates and displays traps on current panel           -RAK-   }*/

  integer    i1,i2;
  boolean    flag = false;

  for (i1 = panel_row_min; i1 <= panel_row_max; i1++) {
    for (i2 = panel_col_min; i2 <= panel_col_max; i2++) {
      //with cave[i1][i2]. do;
      if (cave[i1][i2].tptr > 0) {
	if (t_list[cave[i1][i2].tptr].tval == Unseen_trap) {
	  change_trap(i1,i2);
	  cave[i1][i2].fm = true;
	  flag = true;
	} else if (t_list[cave[i1][i2].tptr].tval == chest) {
	  //with t_list[tptr] do;
	  known2(t_list[cave[i1][i2].tptr].name);
	}
      }
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_sdoor()
{
  /*{ Locates and displays all secret doors on current panel -RAK-  }*/

  integer    i1,i2;
  obj_set    pick_a_stair = {Up_staircase,Down_staircase,
			     up_steep_staircase,down_steep_staircase,0};

  boolean    flag = false;

  for (i1 = panel_row_min; i1 <= panel_row_max; i1++) {
    for (i2 = panel_col_min; i2 <= panel_col_max; i2++) {
      //with cave[i1][i2]. do;
      if (cave[i1][i2].tptr > 0) {
	/*{ Secret doors  }*/
	if (t_list[cave[i1][i2].tptr].tval == Secret_door) {
	  cave[i1][i2].fval = corr_floor3.ftval;
	  change_trap(i1,i2);
	  cave[i1][i2].fm = true;
	  flag = true;

		    /*{ Staircases    }*/
	} else if (is_in(t_list[cave[i1][i2].tptr].tval, pick_a_stair)) {
	  if (!(cave[i1][i2].fm)) {
	    cave[i1][i2].fm = true;
	    lite_spot(i1,i2);
	    flag = true;
	  }
	}
      }
    }
  }
  
  return flag;		    
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean enchant(worlint *pluses)
{
  /*{ Enchants a plus onto an item...                       -RAK-   }*/

  integer    chance = 0;
  boolean    return_value = false;

  if (*pluses > 0) {
    switch (*pluses) {
    case 1  :  chance = 040; break;
    case 2  :  chance = 100; break;
    case 3  :  chance = 200; break;
    case 4  :  chance = 400; break;
    case 5  :  chance = 600; break;
    case 6  :  chance = 700; break;
    case 7  :  chance = 800; break;
    case 8  :  chance = 900; break;
    case 9  :  chance = 950; break;
    default :  chance = 995; break;
    }
  }

  if (randint(1000) > chance) {
    (*pluses)++;
    return_value = true;
  }

  return return_value;
 };
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean bless(integer amount)
{
  /*{ Bless                                                 -RAK-   }*/

  PF.blessed += amount;

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean ident_spell()
{
  /*{ Identify an object                                    -RAK-   }*/

  treas_ptr    item_ptr;
  vtype        out_val;
  char         trash_char;
  treas_ptr    ptr;
  integer      count = 0;
  boolean      redraw = false;
  boolean      return_value = false;

  /*  change_all_ok_stats(true,true);*/

  /* only show things that need to be identified */
  change_all_ok_stats(false,false);
  for (ptr = inventory_list; ptr != NULL; ptr = ptr->next) {
    if (strstr(ptr->data.name,"^") || strstr(ptr->data.name,"|")) {
      ptr->ok = true;
      count++;
    }
  }

  if (count == 0) {
    msg_print("All your items are already identified.");
    msg_print("");
  } else {

    if (get_item(&item_ptr,"Item you wish identified?",
		 &redraw,count,&trash_char,false,false)) {
      //with item_ptr->data. do;
      identify(&(item_ptr->data));
      known2(item_ptr->data.name);
      objdes(out_val,item_ptr,true);
      msg_print(out_val);
      
      return_value = true;
    }
  }  
  if (redraw) {
    //msg_print("");
    draw_cave();
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean light_area(integer y,integer x)
{
  /*{ Light an area: 1.  If corridor then light immediate area -RAK-}*/
  /*{                2.  If room then light entire room.            }*/

  integer    i1,i2;
  obj_set    room_floors = {1,2,17,18,0};
  boolean    flag = true;

  msg_print("You are surrounded by a white light.");
  
  if (is_in(cave[y][x].fval, room_floors) && (dun_level > 0)) {
    light_room(y,x);
  } else {
    for (i1 = y-1; i1 <= y+1; i1++) {
      for (i2 = x-1; i2 <= x+1; i2++) {
	if (in_bounds(i1,i2)) {
	  if (!test_light(i1,i2)) {
	    lite_spot(i1,i2);
	  }
	  cave[i1][i2].pl = true;
	}
      }
    }
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean unlight_area(integer y,integer x)
{
  /*{ Darken an area, opposite of light area                -RAK-   }*/

  integer   i1,i2,i3,tmp1,tmp2,ov_len;
  integer   start_row,start_col;
  integer   end_row,end_col;
  vtype     out_val;
  obj_set   room_floors = {1,2,17,18,0};
  obj_set   four_five_six = {4,5,6,0};
  boolean   flag = false;

  if (is_in(cave[y][x].fval,room_floors) && (dun_level > 0)) {
    tmp1 = trunc(SCREEN_HEIGHT/2);
    tmp2 = trunc(SCREEN_WIDTH /2);
    start_row = trunc(y/tmp1)*tmp1 + 1;
    start_col = trunc(x/tmp2)*tmp2 + 1;
    end_row = start_row + tmp1 - 1;
    end_col = start_col + tmp2 - 1;
    for (i1 = start_row; i1 <= end_row; i1++) {
      out_val[0] = 0; ov_len = 0;
      i3 = 0;
      for (i2 = start_col; i2 <= end_col; i2++) {
	//with cave[i1][i2];
	if (is_in(cave[i1][i2].fval, room_floors)) {
	  cave[i1][i2].pl = false;
	  cave[i1][i2].fval = 1;
	  if (!(test_light(i1,i2))) {
	    if (i3 == 0) {
	      i3 = i2;
	    }
	    out_val[ov_len++] = ' ';
	  } else if (i3 > 0) {
	    flag = true;
	    out_val[ov_len] = 0;
	    print_str(out_val,i1,i3);
	    out_val[0] = 0; ov_len = 0;
	    i3 = 0;
	  }
	} else if (i3 > 0) {
	  flag = true;
	  out_val[ov_len] = 0;
	  print_str(out_val,i1,i3);
	  out_val[0] = 0;
	  i3 = 0;
	}
      } /* end for i2 */

      if (i3 > 0) {
	flag = true;
	out_val[ov_len] = 0;
	print_str(out_val,i1,i3);
      }
    } /* end for i1 */

  } else {

    for (i1 = y-1; i1 <= y+1; i1++) {
      for (i2 = x-1; i2 <= x+1; i2++) {
	if (in_bounds(i1,i2)) {
	  //with cave[i1][i2]. do;
	  if (is_in(cave[i1][i2].fval, four_five_six)) {
	    if (cave[i1][i2].pl) {
	      cave[i1][i2].pl = false;
	      flag = true;
	    }
	  }
	}
      }
    }

    if (flag) {
      msg_print("Darkness surrounds you...");
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean map_area()
{
  /*{ Map the current area plus some                        -RAK-   }*/

  integer    i1,i2,i3,i4,i5,i6,i7,i8;

  i1 = panel_row_min - randint(10);
  i2 = panel_row_max + randint(10);
  i3 = panel_col_min - randint(20);
  i4 = panel_col_max + randint(20);

  for (i5 = i1; i5 <= i2; i5++) {
    for (i6 = i3; i6 <= i4; i6++) {
      if (in_bounds(i5,i6)) {
	if (is_in(cave[i5][i6].fval, floor_set)) {
	  for (i7 = i5-1; i7 <= i5+1; i7++) {
	    for (i8 = i6-1; i8 <= i6+1; i8++) {
	      //with cave[i7][i8]. do;
	      if (is_in(cave[i7][i8].fval, pwall_set)) {
		cave[i7][i8].pl = true;
	      } else if (cave[i7][i8].tptr > 0) {
		if (is_in(t_list[cave[i7][i8].tptr].tval, light_set)) {
		  cave[i7][i8].fm = true;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  prt_map();

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean sleep_monsters1(integer y,integer x)
{
  /*{ Sleep creatures adjacent to player                    -RAK-   }*/

  integer    i1,i2;
  vtype      out_val;
  boolean    flag = false;

  for (i1 = y-1; i1 <= y+1; i1++) {
    for (i2 = x-1; i2 <= x+1; i2++) {
      //with cave[i1][i2]. do;
      if (cave[i1][i2].cptr > 1) {
	//with m_list[cave[i1][i2].cptr]. do;
	//with c_list[m_list[cave[i1][i2].cptr].mptr]. do;
	if (!mon_resists(cave[i1][i2].cptr)) {
	  flag = true;
	  if (mon_save(cave[i1][i2].cptr,0,c_sc_mental)) {
	    sprintf(out_val,"The %s is unaffected.",
		    c_list[m_list[cave[i1][i2].cptr].mptr].name);
	  } else {
	    sprintf(out_val,"The %s falls asleep.",
		    c_list[m_list[cave[i1][i2].cptr].mptr].name);
	    m_list[cave[i1][i2].cptr].csleep = 500;
	  }
	  msg_print(out_val);
	}
      }
    }
  }
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean trap_creation()
{
  /*{ Surround the fool with traps (chuckle)                -RAK-   }*/

  integer   i1,i2;

  for (i1 = char_row-1; i1 <= char_row+1; i1++) {
    for (i2 = char_col-1; i2 <= char_col+1; i2++) {
      //with cave[i1][i2]. do;
      if (is_in(cave[i1][i2].fval, floor_set)) {
	if (cave[i1][i2].tptr > 0) {
	  delete_object(i1,i2);
	}
	place_trap(i1,i2,1,randint(MAX_TRAPA));
      }
    }
  }
  
   return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean door_creation()
{
  /*{ Surround the player with doors...                     -RAK-   }*/

  integer    i1,i2,i3;
  boolean    flag = true;

  for (i1 = char_row-1; i1 <= char_row+1; i1++) {
    for (i2 = char_col-1; i2 <= char_col+1; i2++) {
      if ((i1 != char_row) || (i2 != char_col)) {
	//with cave[i1][i2]. do;
	if (is_in(cave[i1][i2].fval, floor_set)) {
	  popt(&i3);
	  if (cave[i1][i2].tptr > 0) {
	    delete_object(i1,i2);
	  }
	  cave[i1][i2].fopen = false;
	  cave[i1][i2].tptr = i3;
	  t_list[i3] = door_list[2];
	  if (test_light(i1,i2)) {
	    lite_spot(i1,i2);
	  }
	}
      }
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean td_destroy()
{
  /*{ Destroys any adjacent door(s)/trap(s)                 -RAK-   }*/

  integer    i1,i2;
  obj_set    pick_a_door = {Unseen_trap,Seen_trap,Open_door,
			    Closed_door,Secret_door,0};

  boolean    flag = false;

  for (i1 = char_row-1; i1 <= char_row+1; i1++) {
    for (i2 = char_col-1; i2 <= char_col+1; i2++) {
      //with cave[i1][i2]. do;
      if (cave[i1][i2].tptr > 0) {
	if (is_in(t_list[cave[i1][i2].tptr].tval,pick_a_door)) {
	  if (delete_object(i1,i2)) {
	    flag = true;
	  }
	}
      }
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean warding_glyph()
{
  /*{ Leave a glyph of warding... Creatures will not pass over! -RAK-}*/

  integer    i1;

  //with cave[char_row][char_col]. do;
  if (cave[char_row][char_col].tptr == 0) {
    popt(&i1);
    cave[char_row][char_col].tptr = i1;
    t_list[i1] = scare_monster;
  }

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean protect_evil()
{
  /*{ Evil creatures don't like this...                     -RAK-   }*/

  PF.protevil += randint(25) + 3*py.misc.lev;

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean recharge(integer num)
{
  /*{ Recharge a wand, staff, or rod.  Sometimes the item breaks. -RAK-}*/

  treas_ptr   item_ptr;
  boolean     redraw = false;
  char        trash_char;
  obj_set     batteries_not_included = {staff,rod,wand,chime,horn,0};
  boolean     return_value = false;
  
  change_all_ok_stats(true,true);
  if (get_item(&item_ptr,"Recharge which item?",
	       &redraw,inven_ctr,&trash_char,false,false)) {
    //with item_ptr->data. do;
    if (is_in(item_ptr->data.tval,batteries_not_included)) {
      if (randint(8) == 1) {
	return_value = true;
	msg_print("There is a bright flash of light...");
	inven_destroy(item_ptr);
      } else {
	return_value = true;
	num = num / (item_ptr->data.level+2);
	item_ptr->data.p1 += 2 + randint(num);
	if (strstr(item_ptr->data.name,"^") != NULL) {
	  insert_str(item_ptr->data.name," (%P1","^ (%P1");
	}
      }
    } else if ((item_ptr->data.tval == lamp_or_torch) &&
	       (item_ptr->data.subval == 17)) {
      if (randint(50) == 1) {
	return_value = true;
	msg_print("There is a bright flash of light...");
	inven_destroy(item_ptr);
      } else {
	return_value = true;
	num *= 100;
	item_ptr->data.p1 += num / 3 +randint(num);
	if (strstr(item_ptr->data.name,"^") != NULL) {
	  insert_str(item_ptr->data.name," (%P1","^ (%P1");
	}
      }
    } else if ((item_ptr->data.tval == lamp_or_torch) &&
	       (item_ptr->data.subval == 15)) {
      if (randint(15)==1) {
	return_value = true;
	msg_print("There is a bright flash of light...");
	inven_destroy(item_ptr);
      } else {
	return_value = true;
	num *= 80;
	item_ptr->data.p1 += num / 3 +randint(num);
	if (strstr(item_ptr->data.name,"^") != NULL) {
	  insert_str(item_ptr->data.name," (%P1","^ (%P1");
	}
      }
    } 
  }
  
  if (redraw) {
    /*msg_print(" ");*/
    draw_cave();
  } else {
    prt_weight();
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean mass_genocide()
{
  /*{ Delete all creatures within MAX_SIGHT distance        -RAK-   }*/
  /*{ NOTE : Winning creatures cannot be genocided                  }*/

  integer    i1,i2;
  boolean    flag = false;

  i1 = muptr;  /* what happens if there are no monsters in the world? */
  do {
    //with m_list[i1]. do;
    //with c_list[m_list[i1].mptr]. do;
    i2 = m_list[i1].nptr;
    if (m_list[i1].cdis <= MAX_SIGHT) {
      if ((uand(c_list[m_list[i1].mptr].cmove,0x80000000) == 0) && 
	  (!mon_resists(i1))) {
	delete_monster(i1);
	flag = true;
      }
    }      
    i1 = i2;
  } while (i1 != 0);
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean genocide()
{
  /*{ Delete all creatures of a given type from level.      -RAK-   }*/
  /*{ This does not keep creatures of type from appearing later.    }*/

  integer    i1,i2;
  char       typ;
  vtype      out_val;
  boolean    flag = true;

  i1 = muptr;  /* what happens if there are no monsters in the world? */

  if (get_com("Which type of creature do wish exterminated?",&typ)) {
    do {
      //with m_list[i1]. do;
      //with c_list[m_list[i1].mptr]. do;
      i2 = m_list[i1].nptr;
      if (typ == c_list[m_list[i1].mptr].cchar) {
	if ((uand(c_list[m_list[i1].mptr].cmove,0x80000000) == 0) && 
	    (!mon_resists(i1))) {
	  delete_monster(i1);
	} else {
	  sprintf(out_val,"The %s is unaffected.",
		  c_list[m_list[i1].mptr].name);
	  msg_print(out_val);
	}
	i1 = i2;
      }
    } while (i1 != 0);
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean create_food(integer t0,integer t1,integer t2,integer t3,integer t4)
{
  /*{ Create food for the player.           -RAK-   }*/

  integer    i1,i2,i3,this_one,dist;

  for (i1 = char_row-2; i1 <= char_row+2; i1++) {
    for (i2 = char_col-2; i2 <= char_col+2; i2++) {
      //with cave[i1][i2]. do;
      dist = abs(i1-char_row)+abs(i2-char_col);
      if ((abs(i1-char_row)==2) || (abs(i2-char_col)==2)) {
	dist++;
      }

      if ((cave[i1][i2].fopen) && (cave[i1][i2].tptr==0) && (dist<5)) {

	  switch (dist) {
	  case 0 : this_one = t0;break;     /*{food types dep. on dist = }*/
	  case 1 : this_one = t1;break;          /*{X434X}*/
	  case 2 : this_one = t2;break;          /*{42124}*/
	  case 3 : this_one = t3;break;          /*{31013}*/
	  case 4 : this_one = t4;break;          /*{42124}*/
	  default: this_one =  0;break;          /*{X434X}*/
	  }	  

	  if (this_one != 0) {
	    place_object(i1,i2);
	    if (this_one < 0) {      /*{junk food.}*/
	      i3 = store_choice[10][(STORE_CHOICES*(-this_one)
				     +randint(STORE_CHOICES)+3) div 5 - 1];
	      t_list[cave[i1][i2].tptr] = inventory_init[i3];
	    } else {        /*{good food}*/
	      t_list[cave[i1][i2].tptr] = yums[this_one];
	    }
	    if (test_light(i1,i2)) {
	      lite_spot(i1,i2);
	    }
	  }
      }
    }
  }

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean za__did_it_work(integer monptr, integer cflag, integer dmge,
			integer typ)
{
  boolean    hmm = false;

  //with m_list[i1]. do;
  //with c_list[m_list[i1].mptr]. do;

  switch (typ) {
  case c_confuse:
  case c_sleep:
  case c_joke: 
    hmm =  !mon_save(monptr,0,c_sc_mental);
    break;

  case c_hold:
  case c_thunder : 
    hmm = do_stun(monptr,0,dmge);
    break;

  case c_speed : 
    hmm = ! mon_save(c_list[m_list[monptr].mptr].level,0,c_null) || (dmge>0);
    break;

  case c_turn : 
    hmm = uand(c_list[m_list[monptr].mptr].cdefense,0x0008) != 0;
    break;

  case c_drain : 
    hmm = uand(c_list[m_list[monptr].mptr].cdefense,0x0008) == 0;
    break;

  case c_hp : 
    hmm = uand(c_list[m_list[monptr].mptr].cdefense,cflag) != 0;
    break;

  default:
    hmm = true;
    break;
  }

  return  hmm && (!mon_resists(monptr));
};
//////////////////////////////////////////////////////////////////////
void za__yes_it_did(integer monptr, integer cflag, integer dmge,
		    integer typ, integer i1)
{
  vtype      out_val;

  //with m_list[i1]. do;

  switch (typ) {
  case c_confuse:
  case c_turn:
    sprintf(out_val, "The %s runs frantically!",c_list[m_list[monptr].mptr].name);
    msg_print(out_val);
    m_list[monptr].confused = true;
    break;

  case c_sleep : 
    m_list[monptr].csleep = 500;
    break;

  case c_speed :
    m_list[monptr].cspeed += dmge;
    m_list[monptr].csleep = 0;
    break;

  case c_hold : ; /*{done in do_stun in did_it_work already}*/
    break;

  case c_thunder : 
    m_list[monptr].confused = true;
    break;

  case c_hp:
  case c_joke:
  case c_drain:
  case c_holy_word : 
    //with c_list[m_list[i1].mptr]. do;
    if (mon_take_hit(monptr,randint(dmge))>0) {
      if (typ == c_joke) {
	sprintf(out_val, "The %s dies laughing!",c_list[m_list[monptr].mptr].name);
      } else {
	sprintf(out_val, "The %s dissolves!",c_list[m_list[monptr].mptr].name);
      }
      msg_print(out_val);
    } else {
      if (typ == c_joke) {
	sprintf(out_val, "The %s chuckles.",c_list[m_list[monptr].mptr].name);
	msg_print(out_val);
	m_list[monptr].confused = true;
      } else {
	sprintf(out_val, "The %s shudders.",c_list[m_list[monptr].mptr].name);
	msg_print(out_val);
	if (typ == c_holy_word) {
	  if (do_stun(monptr,-4,4+randint(4))) {
	    m_list[monptr].confused = true;
	  }
	}
      }
    }
    break;
  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
boolean za__no_it_didnt(integer monptr, integer cflag, integer dmge,
			integer typ)
{
  vtype      out_val;
  obj_set    some_stuff = {c_confuse,c_sleep,c_speed,c_hold,c_joke,0};
  boolean    flag = false;

  //with m_list[i1]. do;
  if (is_in(typ, some_stuff)) {
    flag = true;
    if (typ == c_joke) {
      sprintf(out_val,"The %s appears offended...",
	      c_list[m_list[monptr].mptr].name);
      msg_print(out_val);
      if (mon_take_hit(monptr,randint(dmge) div 4) > 0) {
	msg_print("and dies from disgust!!!");
      }
    } else {
      sprintf(out_val,"The %s is unaffected...",
	      c_list[m_list[monptr].mptr].name);
      msg_print(out_val);
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean zap_area(integer cflag,integer dmge,integer typ)
{
  integer    i1,m_next;
  boolean    flag = false;

  i1 = muptr;  /* what if no monsters? */

  do {
    m_next = m_list[i1].nptr;
    //with m_list[i1]. do;
    //with c_list[m_list[i1].mptr] do;
    if (m_list[i1].ml) {
      if (za__did_it_work(i1,cflag,dmge,typ)) {
	za__yes_it_did(i1,cflag,dmge,typ,i1);
	flag = true;
      } else if (za__no_it_didnt(i1,cflag,dmge,typ)) {
	flag = true;
      }
    }
    i1 = m_next;
  } while (i1 != 0);

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void da__replace_spot(integer y,integer x,integer typ)
{
  //with cave[y][x]. do;
  switch (typ) {
  case 1: case 2: case 3:
    cave[y][x].fval  = corr_floor1.ftval;
    cave[y][x].fopen = corr_floor1.ftopen;
    break;

  case 4: case 7: case 10:
    cave[y][x].fval  = rock_wall1.ftval;
    cave[y][x].fopen = rock_wall1.ftopen;
    break;

  case 5: case 8: case 11:
    cave[y][x].fval  = rock_wall2.ftval;
    cave[y][x].fopen = rock_wall2.ftopen;
    break;

  case 6: case 9: case 12:
    cave[y][x].fval  = rock_wall3.ftval;
    cave[y][x].fopen = rock_wall3.ftopen;
    break;

  }

  cave[y][x].pl = false;
  cave[y][x].fm = false;

  if (cave[y][x].tptr > 0) {
    delete_object(y,x);
  }

  if ((cave[y][x].cptr > 1) && (! mon_resists(cave[y][x].cptr))) {
    delete_monster(cave[y][x].cptr);
  }

};
//////////////////////////////////////////////////////////////////////
boolean destroy_area(integer y,integer x)
{
  /*{ The spell of destruction...                           -RAK-   }*/
  /*{ NOTE : Winning creatures that are deleted will be considered  }*/
  /*{        as teleporting to another level.  This will NOT win the}*/
  /*{        game...                                                }*/
  
  integer    i1,i2,i3;
  
  if (dun_level > 0) {
    for (i1 = (y-15); i1 <= (y+15); i1++) {
      for (i2 = (x-15); i2 <= (x+15); i2++) {
	if (in_bounds(i1,i2)) {
	  if (cave[i1][i2].fval != 15) {
	    i3 = distance(i1,i2,y,x);
	    if (i3 < 13) {
	      da__replace_spot(i1,i2,randint(6));
	    } else if (i3 < 16) {
	      da__replace_spot(i1,i2,randint(9));
	    }
	  }
	}
      }
    }
  }    

  msg_print("There is a searing blast of light!");
  py.flags.blind += 10 + randint(10);
  
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean earthquake ()
{
  /*{ This is a fun one.  In a given block, pick some walls and     }*/
  /*{ turn them into open spots.  Pick some open spots and turn     }*/
  /*{ them into walls.  An "Earthquake" effect...           -RAK-   }*/

  integer    i1,i2;
  obj_set    one_or_two = {1,2,0};

  for (i1 = char_row-8; i1 <= char_row+8; i1++) {
    for (i2 = char_col-8; i2 <= char_col+8; i2++) {
      if ((i1 != char_row) || (i2 != char_col)) {
	if (in_bounds(i1,i2)) {
	  if (randint(8) == 1) {
	    //with cave[i1][i2]. do;
	    if (cave[i1][i2].tptr > 0) {
	      delete_object(i1,i2);
	    }
	    if ((cave[i1][i2].cptr > 1) && (! mon_resists(cave[i1][i2].cptr))){
	      mon_take_hit(cave[i1][i2].cptr,damroll("2d8"));
	    }
	    if (is_in(cave[i1][i2].fval, wall_set)) {
	      if (next_to4(i1,i2,one_or_two) > 0) {
		cave[i1][i2].fval  = corr_floor2.ftval;
		cave[i1][i2].fopen = corr_floor2.ftopen;
	      } else {
		cave[i1][i2].fval  = corr_floor1.ftval;
		cave[i1][i2].fopen = corr_floor1.ftopen;
	      }
	      if (test_light(i1,i2)) {
		unlite_spot(i1,i2);
	      }
	      cave[i1][i2].pl = false;
	      cave[i1][i2].fm = false;
	      if (cave[i1][i2].tl) {
		lite_spot(i1,i2);
	      }
	    } else if (is_in(cave[i1][i2].fval, floor_set)) {
	      
	      switch (randint(10)) {
	      case 1: case 2: case 3: case 4: case 5: 
		cave[i1][i2].fval  = rock_wall3.ftval;
		cave[i1][i2].fopen = rock_wall3.ftopen;
		break;
		
	      case 6: case 7: case 8: 
		cave[i1][i2].fval  = rock_wall2.ftval;
		cave[i1][i2].fopen = rock_wall2.ftopen;
		break;
		
	      case 9: case 10: 
		cave[i1][i2].fval  = rock_wall1.ftval;
		cave[i1][i2].fopen = rock_wall1.ftopen;
	      }

	      cave[i1][i2].fm = false;
	    }
	    
	    if (test_light(i1,i2)) {
	      lite_spot(i1,i2);
	    }

	  }
	}
      }
    }
  }
  
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean mass_poly()
{
  /*{ Polymorph any creature that player can see...         -RAK-   }*/
  /*{ NOTE: cannot polymorph a winning creature (BALROG)            }*/

  integer   i1,i2,y,x;
  boolean   flag = false;

  i1 = muptr;

  do {
    //with m_list[i1]. do;
    i2 = m_list[i1].nptr;
    if (m_list[i1].cdis < MAX_SIGHT) {
      //with c_list[m_list[i1].mptr]. do;
      if ((uand(c_list[m_list[i1].mptr].cdefense,0x80000000) == 0) &&
	  (! mon_resists(i1))) {
	y = m_list[i1].fy;
	x = m_list[i1].fx;
	delete_monster(i1);
	place_monster(y,x,randint(m_level[MAX_MONS_LEVEL])
		      + m_level[0],false);
	flag = true;
      }
    }

    i1 = i2;
  } while (i1 != 0);  /* what if no monsters? */
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean light_line(integer dir, integer y, integer x, integer power)
{
  /*{ Leave a line of light in given dir, blue light can sometimes  }*/
  /*{ hurt creatures...                                     -RAK-   }*/

  integer  i1,i2;
  vtype    out_val;

  ENTER("light_line", "s");

  for ( ; cave[y][x].fopen ; ) {
    //with cave[y][x]. do;
    
    if (panel_contains(y,x)) {
      
      if (!((cave[y][x].tl) || (cave[y][x].pl))) {
	if (cave[y][x].fval == 2) {
	  light_room(y,x);
	} else {
	  lite_spot(y,x);
	}
      }
      
      if (cave[y][x].cptr > 1) {
	//with m_list[cave[y][x].cptr]. do;
	//with c_list[m_list[cave[y][x].cptr].mptr]. do;
	if (!mon_resists(cave[y][x].cptr)) {
	  if (0x0100 & c_list[m_list[cave[y][x].cptr].mptr].cdefense) {
	    
	    sprintf(out_val, "The %s wails out in pain!", 
		    c_list[m_list[cave[y][x].cptr].mptr].name);

	    msg_print(out_val);
	    i1 = 0;
	    for (i2 = 1; i2 <= power; i2++) {
	      i1 += mon_take_hit(cave[y][x].cptr,damroll("2d8"));
	    }
	    if (i1 > 0) {
	      sprintf(out_val, "The %s dies in a fit of agony.", 
		      c_list[m_list[cave[y][x].cptr].mptr].name);

	      msg_print(out_val);
	    }
	  }
	}
      }
    }
    cave[y][x].pl = true;
    
    move_dir(dir,&y,&x);
  }
  
  LEAVE("light_line", "s");
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean starlite(integer y, integer x)
{
  /*{ Light line in all directions                          -RAK-   }*/

  integer   i1;

  for (i1 = 1; i1 <= 9; i1++) {
    if (i1 != 5) {
      light_line(i1,y,x,2);
    }
  }

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean fb__ill_joke(integer a_cptr,integer typ,integer dam,
		     char *str,char *str2)
{
  integer    i2;
  vtype      out_val;
  
  find_monster_name(str,a_cptr,false);
  find_monster_name(str2,a_cptr,true);
  //with m_list[a_cptr] do;
  //with c_list[mptr] do;
  if (! mon_save(a_cptr,0,c_sc_mental)) {
    m_list[a_cptr].confused = true;
    i2 = mon_take_hit(a_cptr,dam);
    if (typ==c_illusion) {
      sprintf(out_val, "%s seems to believe the illusion...", str2);
      msg_print(out_val);
      if (i2>0) {
	msg_print("and dies from fright!");
      } else {
	msg_print("and appears quite shaken.");
      }
    } else {
      
      sprintf(out_val, "The punch line strikes %str...", str);
      msg_print(out_val);
      if (i2>0) {
	msg_print("who dies in a fit of laughter!");
      } else{
	msg_print("who becomes weak from laughter!");
      }
    }
    
  } else {
    if (typ==c_illusion) {
      sprintf(out_val, "%s is unaffected.", str2);
      msg_print(out_val);
    } else {
      find_monster_name(str,a_cptr,true);
      sprintf(out_val, "%s appears offended....", str);
      msg_print(out_val);
      if (mon_take_hit(a_cptr,dam div 4)>0) {
	msg_print("but your joke still knocks it dead!");
      }
    }
  }
  
  return true;
};
//////////////////////////////////////////////////////////////////////
boolean fire_bolt(integer typ,integer dir,integer y,integer x,integer dam,
		  ctype bolt_typ)
{
  /*{ Shoot a bolt in a given direction                     -RAK-   }*/

  integer    dist,cptr,mptr;
  integer    weapon_type,harm_type;
  obj_set    *dummy;
  vtype      str,str2,out_val;
  
  get_flags(typ,&weapon_type,&harm_type,&dummy);
  dist = 0;
  if (bolt_to_creature(dir,&y,&x,&dist,OBJ_BOLT_RANGE,true)) {
    //with cave[y][x]. do;
    if ((typ == c_illusion) || (typ == c_joke)) {
      fb__ill_joke(cave[y][x].cptr,typ,dam,str,str2);
    } else {
      //with m_list[cave[y][x].cptr]. do;
      //with c_list[m_list[cave[y][x].cptr].mptr]. do;
      cptr = cave[y][x].cptr;
      mptr = m_list[cptr].mptr;
      if (! mon_resists(cptr)) {
	
	find_monster_name(str,cptr,false);
	find_monster_name(str2,cptr,true);
	sprintf(out_val, "The %s strikes %s.", bolt_typ, str);
	msg_print(out_val);
	if (uand(harm_type,c_list[mptr].cdefense) != 0) {
	  dam *= 2;
	} else if (uand(weapon_type,c_list[mptr].spells) != 0) {
	  dam /= 4;
	}
	if (mon_take_hit(cptr,dam) > 0) {
	  sprintf(out_val, "%s dies in a fit of agony.", str2);
	  msg_print(out_val);
	} else {
	  if (panel_contains(y,x)) {
	    print(c_list[mptr].cchar,y,x);
	    m_list[cptr].ml = true;
	  }
	}
      }
    }
  }
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean fire_ball(integer typ,integer dir,integer y,integer x,integer dam_hp,
		  ctype descrip)
{
  /*{ Shoot a ball in a given direction.  Note that balls have an   }*/
  /*{ area affect....                                       -RAK-   }*/

  integer   dist = 0;
  boolean   flag = false;

  if (bolt_to_creature(dir,&y,&x,&dist,OBJ_BOLT_RANGE,true)) {
    explode(typ,y,x,dam_hp,descrip);
  } else if (dist <= OBJ_BOLT_RANGE) {
    move_dir(10-dir,&y,&x);
    flag = explode(typ,y,x,dam_hp,descrip);
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean wall_to_mud(integer dir,integer y,integer x)
{
  /*{ Turn stone to mud, delete wall....                    -RAK-   }*/

  integer   i1, cptr, mptr;
  vtype     out_val, out_val2;
  boolean   flag = false;
  boolean   return_value = false;
  
  do {
    move_dir(dir,&y,&x);
    //with cave[y][x]. do;
    if (in_bounds(y,x)) {
      if (is_in(cave[y][x].fval, wall_set)) {
	flag = true;
	twall(y,x,1,0);
	if (test_light(y,x)) {
	  msg_print("The wall turns into mud.");
	  return_value = true;
	} else {
	  unlite_spot(y,x);
	}
      } else if ((cave[y][x].tptr > 0) && (!(cave[y][x].fopen))) {  
	/* kills doors? */
	flag = true;
	if (panel_contains(y,x)) {
	  if (test_light(y,x)) {
	    inven_temp->data = t_list[cave[y][x].tptr];
	    objdes(out_val,inven_temp,false);
	    sprintf(out_val2, "The %s turns into mud.",out_val);
	    msg_print(out_val2);
	    return_value = true;
	  }
	}
	delete_object(y,x);
      }
      
      cptr = cave[y][x].cptr;
      mptr = m_list[cptr].mptr;
      if (cptr > 1) {

	if (uand(0x0200,c_list[m_list[cave[y][x].cptr].mptr].cdefense) != 0) {
	  i1 = mon_take_hit(cptr,100);
	  flag = true;
	  if (m_list[cptr].ml) {
	    if (i1 > 0) {
	      sprintf(out_val, "The %s dies in a fit of agony.",
		      c_list[mptr].name);
	    } else {
	      sprintf(out_val, "The %s wails out in pain!.",
		      c_list[mptr].name);
	    }
	    msg_print(out_val);
	  }
	}
      }
    } else {
      flag = true;
    }
  } while (!flag);
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean td_destroy2(integer dir,integer y,integer x)
{
  /*{ Destroy all traps and doors in a given direction      -RAK-   }*/

  obj_set    thump_stuff = {chest,Unseen_trap,Seen_trap,
			    Closed_door,Secret_door,0};
  boolean    flag = false;

  do {
    move_dir(dir,&y,&x);
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {
      //with t_list[cave[y][x].tptr]. do;
      if (is_in(t_list[cave[y][x].tptr].tval, thump_stuff)) {
	if (delete_object(y,x)) {
	  msg_print("There is a bright flash of light!");
	  cave[y][x].fopen = true;
	  flag = true;
	}
      }
    }
  } while (cave[y][x].fopen);
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean poly_monster(integer dir,integer y,integer x)
{
  /*{ Polymorph a monster                                   -RAK-   }*/
  /*{ NOTE: cannot polymorph a winning creature (BALROG)            }*/

  vtype      out_val;
  integer    cptr;
  integer    dist = 0;
  boolean    flag = false;
  boolean    return_value = false;

  do {
    if (bolt_to_creature(dir,&y,&x,&dist,OBJ_BOLT_RANGE,false)) {
      //with cave[y][x]. do;
      cptr = cave[y][x].cptr;
      if (! mon_save(cptr,0,c_null)) {
	if (! mon_resists(cptr)) {
	  flag = true;
	  delete_monster(cptr);
	  place_monster(y,x,
                        randint(m_level[MAX_MONS_LEVEL])+ m_level[0],false);
	  if (panel_contains(y,x)) {
	    if (test_light(y,x)) {
	      return_value = true;
	    }
	  }
	}
      } else {
	sprintf(out_val, "The %s is unaffected.",
		c_list[m_list[cptr].mptr].name);
        msg_print(out_val);
      }
    } else {
      flag = true;
    }
  } while (!flag);

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean build_wall(integer dir,integer y,integer x)
{
  /*{ Create a wall...                                      -RAK-   }*/

  integer    i1 = 0;
  boolean    flag = false;

  move_dir(dir,&y,&x);
  for (; (cave[y][x].fopen) && (i1 < 10) ; ) {
    //with cave[y,x] do;
    if (cave[y][x].tptr > 0) {
      delete_object(y,x);
    }
    if ((cave[y][x].cptr > 1) && (! mon_resists(cave[y][x].cptr))) {
      mon_take_hit(cave[y][x].cptr,damroll("2d8"));
    }
    cave[y][x].fval  = rock_wall2.ftval;
    cave[y][x].fopen = rock_wall2.ftopen;
    cave[y][x].fm = false;
    if (test_light(y,x)) {
      lite_spot(y,x);
    }
    i1++;
    flag = true;
    move_dir(dir,&y,&x);
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean clone_monster(integer dir,integer y,integer x)
{
  /*{ Replicate a creature                                  -RAK-   }*/

  boolean    flag = false;

  if (move_to_creature(dir,&y,&x)) {
    //with cave[y][x]. do;
    if (! mon_resists(cave[y][x].cptr)) {
      multiply_monster(y,x,m_list[cave[y][x].cptr].mptr,false);
      if (panel_contains(y,x)) {
	if (m_list[cave[y][x].cptr].ml) {
	  flag = true;
	}
      }
    }
  }
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean disarm_all(integer dir,integer y,integer x)
{
  /*{ Disarms all traps/chests in a given direction         -RAK-   }*/

  integer    oldy,oldx,tval;
  char      *achar;
  boolean    flag = false;

  do {
    //with cave[y][x]. do;
    if (cave[y][x].tptr > 0) {
      //with t_list[tptr] do;
      tval = t_list[cave[y][x].tptr].tval;
      if ((tval == Unseen_trap) || (tval == Seen_trap)) {
	if (delete_object(y,x)) {
	  flag = true;
	}
      } else if (tval == Closed_door) {
	t_list[cave[y][x].tptr].p1 = 0;
      } else if (tval == Secret_door) {
	cave[y][x].fval = corr_floor3.ftval;
	change_trap(y,x);
	cave[y][x].fm = true;
	flag = true;
      } else if (tval == chest) {
	if (t_list[cave[y][x].tptr].flags > 0) {
	  msg_print("Click!");
	  t_list[cave[y][x].tptr].flags = 0;
	  flag = true;
	  achar = strstr(t_list[cave[y][x].tptr].name," (");
	  if (achar != NULL) {
	    *achar = 0;
	  }
	  strcat(t_list[cave[y][x].tptr].name," (Unlocked)");
	  known2(t_list[cave[y][x].tptr].name);
	}
      }
    }
    oldy = y;
    oldx = x;
    move_dir(dir,&y,&x);
  } while ((cave[oldy][oldx].fopen));
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean teleport_monster(integer dir,integer y,integer x)
{
  /*{ Teleport all creatures in a given direction away      -RAK-   }*/

  boolean   flag = false;

  for ( ; move_to_creature(dir,&y,&x) ; ) {
    teleport_away(cave[y][x].cptr,MAX_SIGHT);
    flag = true;
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean zm__did_it_work(integer zaptype, integer cptr,
			integer aux )
{
  boolean flag;

  //with cave[y,x] do;
  //with c_list[m_list[cptr].mptr] do;

  switch (zaptype) {
  case c_sleep: case c_confuse: 
    flag = ((! mon_save(cptr,0,c_sc_mental))
	    && (! mon_resists(cptr)));
    break;
    
  case c_drain: 
    flag = ((uand(c_list[m_list[cptr].mptr].cdefense,0x0008)==0)
	    && (! mon_resists(cptr)));
    break;
    
  case c_speed :
    flag = ((! mon_save(cptr,0,c_null))
	    && (! mon_resists(cptr))) || (aux>0);
    break;

  case c_hold : 
    flag = ((! mon_save(cptr,0,c_hold))
	    && (! mon_resists(cptr)));
    break;
    
  default:
    flag = true;
    break;
  }
  return flag;
};

//////////////////////////////////////////////////////////////////////
void zm__yes_it_did(integer zaptype, integer cptr, integer aux, 
		    char *str1, char *str2)
{
  integer   i1;
  vtype     out_val;

  //with cave[y,x] do;
  //with m_list[cptr]. do;

  switch (zaptype) {
  case c_probe :
    sprintf(out_val, "The mysterious ray strikes %s.", str2);
    msg_print(out_val);
    msg_print("A voice booms down from above!  It says..");
    sprintf(out_val, "This monster has %d hp left.", m_list[cptr].hp);
    msg_print(out_val);
    break;

  case c_sleep :
    m_list[cptr].csleep = 500;
    sprintf(out_val, "%s falls asleep.", str1);
    msg_print(out_val);
    break;

  case c_confuse : 
    m_list[cptr].confused = true;
    m_list[cptr].csleep = 0;
    sprintf(out_val, "%s appears confused.",str1);
    msg_print(out_val);
    break;

  case c_hp: case c_drain :
    if (mon_take_hit(cptr,aux) > 0) {
      sprintf(out_val, "%s dies in a fit of agony.", str1);
    } else {
      sprintf(out_val, "%s screams in agony.", str1);
    }
    msg_print(out_val);
    break;

  case c_speed : 
    m_list[cptr].cspeed += aux;
    m_list[cptr].csleep = 0;
    break;

  case c_hold : 
    sprintf(out_val, "%s appears frozen!", str1);
    msg_print(out_val);
    i1 = m_list[cptr].stunned + aux;
    if (i1>31) {
      i1 = 31;
    }
    m_list[cptr].stunned = i1;
    break;

  default:
    break;
  }

};
//////////////////////////////////////////////////////////////////////
boolean zm__no_it_didnt(integer zaptype, char *str1)
{
  /*{ returns true for item idents }*/

  obj_set   things_you_can_know = {c_illusion,c_sleep,c_confuse,0};
  vtype     out_val;
  boolean   flag;

  flag = is_in(zaptype, things_you_can_know);

  if (zaptype != c_drain) {
    sprintf(out_val, "%s is unaffected.", str1);
    msg_print(out_val);
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean zap_monster(integer dir,integer y,integer x,integer aux,
			integer zaptype)
{
  /*{ Contains all aimed spell effects that stop at first victim.  New
    spell effects should be put into constants.inc.  Aux is used for damage
    or speed change if used.}*/

  integer    cptr,mptr;
  vtype      str1,str2;
  boolean    flag = false;
  
  if (move_to_creature(dir,&y,&x)) {
    //with cave[y][x]. do;
    //with m_list[cave[y][x].cptr]. do;
    //with c_list[m_list[cave[y][x].cptr].mptr] do;

    cptr = cave[y][x].cptr;
    mptr = m_list[cptr].mptr;
    
    find_monster_name(str1,cptr,true);
    find_monster_name(str2,cptr,false);
    if (zm__did_it_work(zaptype,cptr,aux)) {
      zm__yes_it_did(zaptype,cptr,aux,str1,str2);
      flag = true;
    } else if (zm__no_it_didnt(zaptype,str1)) {
      flag = true;
    }
  }
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean bolt_to_creature(integer dir, integer *y, integer *x, integer *dist, 
			 integer max_dist, boolean visable)
{
  boolean   hit_creature = false;
  integer   oldx,oldy;
  
  do {
    oldy = *y;
    oldx = *x;
    move_dir(dir,y,x);
    (*dist)++;
    if (visable) {       /*{ erase bolt }*/
      if (test_light(oldy,oldx)) {
	lite_spot(oldy,oldx);
      } else {
	unlite_spot(oldy,oldx);
      }
    }
    if (*dist <= max_dist) {
      //with cave[y][x]. do;
      if (cave[*y][*x].fopen) {
	hit_creature = (cave[*y][*x].cptr > 1);
	if (visable && (! hit_creature)) {
	  if (panel_contains(*y,*x)) {
	    print('*',*y,*x);   /*{ draw bolt }*/
	    put_qio();
	    usleep(DRAW_BOLT_DELAY);
	  }
	}
      }
    }
  } while (!((! cave[*y][*x].fopen) || (*dist > max_dist) || hit_creature));
  
  return hit_creature;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean move_to_creature(integer dir, integer *y, integer *x)
{
  boolean    hit_creature;
  integer    dist = 0;

  hit_creature = bolt_to_creature(dir,y,x,&dist,999,false);
  return hit_creature;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean am_i_dumb()
{
  return py.misc.lev < randint(randint(50));
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean lore_spell()
{
  /*{ Give name for most items in inventory                         -Cap'n- }*/

  treas_ptr   thingy;

  thingy = inventory_list;
  for ( ; thingy != NULL ; ) {
    if (! am_i_dumb()) {
      identify(&(thingy->data));
      known2(thingy->data.name);
    }
    thingy = thingy->next;
  }
  
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_curse()
{
  char         trash_char;
  treas_ptr    item_ptr;
  boolean      redraw;
  boolean      flag = false;

  redraw = false;
  change_all_ok_stats(true,true);
  if (get_item(&item_ptr,"Item you wish to examine?",
	       &redraw,inven_ctr,&trash_char,false,false)) {
    //with item_ptr->data. do;
    if (uand(Cursed_worn_bit,item_ptr->data.flags) != 0) {
      item_ptr->data.flags2 |= Known_cursed_bit;
      msg_print("The item is cursed!");
    } else {
      msg_print("This item is not cursed.");
    }
      flag = true;
  }
  if (redraw) {
    msg_print(" ");
    draw_cave();
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean detect_magic()
{
  /*{ Determine whether an item is magical or not           -Cap'n- }*/

  treas_ptr    item_ptr;
  char         trash_char;
  boolean      redraw,dumb,dumber;
  boolean      flag = false;

  redraw = false;
  dumb = am_i_dumb();
  dumber = dumb && am_i_dumb();
  change_all_ok_stats(true,true);
  if (get_item(&item_ptr,"Item you wish to examine?",
	       &redraw,inven_ctr,&trash_char,false,false)) {
    //with item_ptr->data. do;
    if ((((item_ptr->data.flags!=0) || (item_ptr->data.tohit>0) || 
	  (item_ptr->data.todam>0) || (item_ptr->data.toac>0)) && ! dumb) || 
	dumber) {
      msg_print("The item seems magical!");
    } else {
      msg_print("The item does not seem to be magical...");
    }
    flag = true;
  }
  if (redraw) {
    msg_print(" ");
    draw_cave();
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean create_water(integer y, integer x)
{
/*{ Creates an area of water on open floor                -Cap'n- }*/
  /* XXXX no code existed */

  return true;
};
//////////////////////////////////////////////////////////////////////

boolean destroy_water(integer y, integer x)
{
  /*{ Makes an area of water into open floor                -Cap'n- }*/
  /* XXXX no code existed */

  return true;
};
//////////////////////////////////////////////////////////////////////

boolean item_petrify()
{
/*{ Does a petrify attack on the fool that used the bad item -Capn-}*/
  /* XXXX no code existed */
  return  true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean creeping_doom(integer dir,integer y,integer x,integer dam_hp,
		      integer range, ctype ddesc)
{
  /*{ Creeping doom type spells, a missile, but with a set range    }*/

  integer    dist;
  integer    cptr, mptr;
  vtype      out_val;

  dist = 0;

  if (bolt_to_creature(dir,&y,&x,&dist,range,true)) {
    //with cave[y,x] do;
    //with m_list[cptr] do;
    //with c_list[mptr] do;
    cptr = cave[y][x].cptr;
    mptr = m_list[cptr].mptr;

    if (! mon_resists(cptr)) {
      sprintf(out_val, "The %s hits the %s.",ddesc, c_list[mptr].name);
      msg_print(out_val);
      if (mon_take_hit(cptr,dam_hp) > 0) {
	sprintf(out_val, "The %s dies in a fit of agony.", c_list[mptr].name);
	msg_print(out_val);
      } else {
	if (panel_contains(y,x)) {
	  print(c_list[mptr].cchar,y,x);
	  m_list[cptr].ml = true;
	}
      }
    }
  }
  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean fire_line(integer typ,integer dir,integer y,integer x,integer dam_hp,
		  ctype descrip)
{
  /*{ Fire a spell that affects a line of monsters                  }*/

  integer    dist;
  integer    weapon_type,harm_type;
  integer    cptr, mptr;
  obj_set   *dummy;
  vtype      out_val;

  get_flags(typ,&weapon_type,&harm_type,&dummy);
  dist = 0;
  for ( ; bolt_to_creature(dir,&y,&x,&dist,OBJ_BOLT_RANGE,true) ; ) {
    //with cave[y,x] do;
    //with m_list[cptr] do;
    //with c_list[mptr] do;
    cptr = cave[y][x].cptr;
    mptr = m_list[cptr].mptr;
    if (! mon_resists(cptr)) {
      sprintf(out_val, "The %s strikes the %s.",descrip, c_list[mptr].name);
      msg_print(out_val);
      if (uand(harm_type,c_list[mptr].cdefense) != 0) {
                  dam_hp *= 2;
      } else if (uand(weapon_type,c_list[mptr].spells) != 0) {
                  dam_hp /= 4;
      }

      if (mon_take_hit(cptr,dam_hp) > 0) {
	sprintf(out_val, "The %s dies in a fit of agony.", c_list[mptr].name);
	msg_print(out_val);
      } else {
	if (panel_contains(y,x)) {
	  print(c_list[mptr].cchar,y,x);
	  m_list[cptr].ml = true;
	}
      }
    }
  }

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/* END FILE  spells.c */
