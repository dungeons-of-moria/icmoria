/* blow.c */
/* chimes, horns, gems, and other stuff */

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void b__chime_and_horn_effects(integer effect, boolean *idented)
{
  /*{ Chimes...				      }*/
  
  boolean          ident;
  integer          y,x;
  integer          i3;

  ident = *idented;

  switch (effect) {
    
  case 1  :  /*{ Chime of Light }*/
    ident = light_area(char_row,char_col);
    break;
    
  case 2  :  /*{ Chime of Detect Doors/Stairs }*/
    ident = detect_sdoor();
    break;
    
  case 3  :  /*{ Chime of Detect Traps }*/
    ident = detect_trap();
    break;
    
  case 4  :  /*{ Chime of Teleportation }*/
    teleport(100);
    ident = true;
    break;
    
  case 5  :  /*{ Chime of Thunderblasts }*/
    ident = earthquake();
    break;
    
  case 6  :  /*{ Chime of Summon Monster }*/
    for (i3 = 1; i3 <= randint(4); i3++) {
      y = char_row;
      x = char_col;
      if (is_in(cave[y][x].fval, water_set)) {
	summon_water_monster(&y,&x,false);
      } else {
	summon_land_monster(&y,&x,false);
      }
    }
    ident = true;
    break;
    
  case 7 :  /*{ Chime of Disarming }*/
    ident = true;
    for (i3 = 1; i3 <= 9; i3++) {
      if (i3 != 5) {
	disarm_all(i3,char_row,char_col);
      }
    }
    break;
    
  case 8 :  /*{ Chime of Aggravation }*/
    ident = aggravate_monster(20);
    break;
    
  case 9 :  /*{ Chime of Slow Monster }*/
    ident = zap_area(0,-1,c_speed);
    break;
    
  case 10 :  /*{ Chime of Sooth Monster }*/
    ident = zap_area(0,0,c_sleep);
    break;
    
  case 11 :  /*{ Chime of Cure Light Wounds }*/
    ident = hp_player(randint(8),"a chime.");
    break;
    
  case 12 :  /*{ Chime of Changing }*/
    ident = mass_poly();
    break;
    
  case 13 :  /*{ Chime of Remove Curse }*/
    if (remove_curse()) {
      msg_print("Your equipment glows blue for a moment...");
      ident = true;
    }
    break;
    
  case 14 :  /*{ Chime of Curing }*/
    //with py.flags do;
    ident  = cure_me(&PF.blind);
    ident |= cure_me(&PF.poisoned);
    ident |= cure_me(&PF.confused);
    break;
    
  case 15 :  /*{ Chime of Dispell Evil }*/
    ident = zap_area(0x0004,60,c_hp);
    break;
    
  case 16 :  /*{ Chime of Darkness }*/
    ident = unlight_area(char_row,char_col);
    break;
    
    /*{ Horns...						      }*/
    
  case 17 :  /*{ Horn of Bubbles }*/
    msg_print("Bubbles stream forth and surround you!");
    py.flags.blind += (randint(20) +5);
    ident = unlight_area(char_row,char_col);
    break;
    
  case 18 :  /*{ Horn of Calling }*/
    for (i3 = 1; i3 <= randint(4); i3++) {
      y = char_row;
      x = char_col;
      if (is_in(cave[y][x].fval, water_set)) {
	summon_water_monster(&y,&x,false);
      } else {
	summon_land_monster(&y,&x,false);
      }
    }
    ident = true;
    break;
    
  case 19 :  /*{ Horn of Soft Sounds }*/
    ident = sleep_monsters1(char_row,char_col);
    break;
    
  case 20 :  /*{ Horn of *Blasting* }*/
    ident = destroy_area(char_row,char_col);
    break;
    
  case 21 :  /*{ Horn of Cold }*/
    msg_print("A wave of cold pours forth...");
    ident = zap_area(0x0010,60,c_hp);
    break;
    
  case 22 :  /*{ Horn of Heat }*/
    msg_print("A blast of warm air pours forth...");
    ident = zap_area(0x0020,60,c_hp);
    break;
    
  case 23 :  /*{ Horn of Gas }*/
    msg_print("Gas streams out ...!");
    ident = zap_area(0x0040,60,c_hp);
    py.flags.poisoned += randint(10);
    break;
    
  case 24 :  /*{ Horn of Recall }*/
    py.flags.word_recall = (randint(20) + 20);
    ident = true;
    break;
    
  case 25 :  /*{ Horn of *Chaos* }*/
    ident = earthquake();
    ident = unlight_area(char_row,char_col);
    for (i3 = 1; i3 <= randint(4); i3++) {
      y = char_row;
      x = char_col;
      if (is_in(cave[y][x].fval, water_set)) {
	summon_water_monster(&y,&x,false);
      } else {
	summon_land_monster(&y,&x,false);
      }
    }
    break;
    
  case 26 :  /*{ Horn of Glue }*/
    msg_print("glue pours out of the horn!");
    msg_print("all of your equipment is stuck!");
    for (i3 = Equipment_primary; i3 <= Equipment_cloak; i3++) {
      //with equipment[i3] do;
      equipment[i3].flags |= 0x80000000;
    }
    //with equipment[Equipment_secondary]. do;
    equipment[Equipment_secondary].flags |= 0x80000000;
    ident = true;
    break;
    
  case 27 :  /*{ Horn of Valhalla }*/
    msg_print("The Voice of Odin (oh brother...) smites evil!");
    ident = zap_area(0x0004,70,c_hp);
    light_area(char_row,char_col);
    hp_player(randint(20)+20,"a horn.");
    break;
    
  case 28 :  /*{ Horn of Tritons }*/
    //with py.flags do;
    ident = true;
    msg_print("All of the seas of the world still (yeah, right)!");
    msg_print("The gods of the ocean hear you..."	 );
    PF.blessed += randint(20);
    cure_me(&PF.blind);
    cure_me(&PF.poisoned);
    break;
    
  case 29 :  /*{ Horn of Fog }*/
    msg_print("Thick fog billows out...");
    ident = unlight_area(char_row,char_col);
    msg_print("The nearby creatures are confused");
    ident = zap_area(0,0,c_confuse);
    break;
    
  default:
    break;
  }

  *idented = ident;
  
};
//////////////////////////////////////////////////////////////////////
void b__misc_effects(integer effect, boolean *idented, treas_ptr item_ptr)
{
  integer          i3, i4, loss, dur;
  integer          dumy,y_dumy,x_dumy;
  integer	   y,x;
  stat_set         tstat;
  boolean          ident;
  char             dir;

  ident = *idented;

  switch (effect) {
  case 1 : /*{Silver Cross}*/
    msg_print("You feel secure...");
    protect_evil();
    ident = true;
    break;
    
  case 2 : /*{Gold Cross}*/
    ident = zap_area(0x0400,100,c_hp);
    break;
    
  case 3 : /*{Mithril Cross}*/
    x = char_row;
    y = char_col;
    ident = summon_undead(&x,&y);
    break;
    
  case 4 : /*{Cross}*/
    x = char_row;
    y = char_col;
    ident = summon_demon(&x,&y);
    break;
    
  case 5 : /*{Cross}*/
    strcat(item_ptr->data.name, "(Empty)");
    msg_print("You free a Djinni from the bottle!");
    msg_print("He grants you a wish...");
    msg_print("'I will raise one of your stats by two...'");
    msg_print("'Which stat would you like to raise?'");
    ident = (get_com("1=str 2=int 3=wis 4=dex 5=con 6=cha",&dir));
    if ((dir > '0') && (dir < '7')) {
      tstat = (integer)dir-49;
      ident = gain_stat(tstat,"X");
      ident = gain_stat(tstat," and x2, too!");
    } else {
      msg_print("Oh well, maybe next time.");
    }
    break;
    
  case 6 : /*{ Corked Bottle of Demons }*/
    msg_print("You release several demons!");
    x = char_row;
    y = char_col;
    for (i3 = 1; i3 <= 4; i3++) {
      summon_demon(&x,&y);
    }
    py.flags.paralysis += 2;
    strcat(item_ptr->data.name, "(Empty)");
    ident = true;
    break;
    
  case 7 : 
    ident = create_water(char_row,char_col);
    break;
    
  case 8 : 
    ident = destroy_water(char_row,char_col);
    break;
    
  case 9 : 
    msg_print("Many Undead appear!");
    x = char_row;
    y = char_col;
    for (i3 = 1; i3 <= 8; i3++) {
      ident = summon_undead(&x,&y);
    }
    py.flags.paralysis += 2;
    break;
    
  case 10 : 
    msg_print("Many Demons appear!");
    x = char_row;
    y = char_col;
    for (i3 = 1; i3 <= 8; i3++) {
      ident = summon_demon(&x,&y);
    }
    py.flags.paralysis += 2;
    ident = true;
    break;
    
  case 11: 
    //with py.stat do;
    for (tstat = STR; tstat <= CHR; tstat++) {
      if (restore_stat(tstat,"X")) {
	ident = true;
      }
    }
    if (hp_player(damroll("6d7"),"a statue.")) {
      ident = true;
    }
    //with py.flags do;
    if (PF.slow > 0) {
      ident = true;
      PF.slow  = 1;
    }
    if (cure_me(&PF.blind) | cure_me(&PF.poisoned) |
	cure_me(&PF.confused) | cure_me(&PF.afraid) | restore_level()) {
      ident = true;
    }
    if (ident) {
      msg_print("The statue fills you with life and all maladies are cured.");
    }
    break;
    
  case 12: 
    ident = true;
    break;
    
  case 13 :  
    redraw = true;
    wizard_light();
    for (i3 = (char_col + 1); i3 <= (char_col - 1); i3++) {
      for (i4 = (char_row + 1); i4 <= (char_row - 1); i4++) {
	if (test_light(i4,i3)) {
	  redraw = false;
	}
      }
    }
    if (redraw) {
      wizard_light();
    }
    break;
    
  case 14: 
    ident = zap_area(0,2,c_speed);
    break;
    
  case 15: 
    //with py.misc do;
    loss = 0;
    for (i3 = 1; i3 <= 6; i3++) {
      loss += (PM.money[i3]-PM.money[i3] / 2) * COIN_WEIGHT;
      PM.money[i3] /= 2;
    }
    inven_weight -= loss;
    reset_total_cash();
    msg_print("most of your money turns to dust!");
    prt_gold();
    prt_weight();
    ident = true;
    break;
    
  case 16 :  
    //with py.misc do;
    if (PM.cmana < PM.mana) {
      PM.cmana = PM.mana;
      ident = true;
      msg_print("Your feel your head clear...");
    }
    break;
    
  case 17 : 
    ident = item_petrify();
    break;
    
  case 18 : 
    strcpy(item_ptr->data.damage,"50d5");
    item_ptr->data.cost = -1;
    msg_print("Click!");
    ident = true;
    break;
    
  case 19 : 
    ident = detect_creatures(c_monster);
    break;
    
  case 20 : 
    ident = zap_area(0x0004,60,c_hp);
    break;
    
  case 21 : 
    ident = unlight_area(char_row,char_col);
    break;
    
  case 22 : 
    if (d__get_dir("Which direction?",&dur,&dumy,&y_dumy,&x_dumy)) {
      i3 = char_row;
      i4 = char_col;
      ident = fire_ball(3,dur,i3,i4,40,"Acid Ball");
    } else {
      item_ptr->data.p1++;
    }
    break;
    
  case 23 : 
    ident = detect_creatures(c_invisible);
    break;
    
  case 24 : 
    ident = ident_spell();
    break;
    
  case 25 :
    ident = light_area(char_row,char_col);
    break;
    
  case 26 : 
    for (i3 = 1; i3 <= randint(4); i3++) {
      /*{who"s the idiot who wrote these things anyhow? }*/
      y = char_row;
      x = char_col;
      if (is_in(cave[y][x].fval, water_set)) {
	summon_water_monster(&y,&x,false);
      } else {
	summon_land_monster(&y,&x,false);
      }
    }
    ident = true;
    break;
    
  case 27 : 
    if (remove_curse()) {
      msg_print("Your junk glows black for a moment...");
      ident = true;
    }
    break;
    
  case 28 : 
    ident = destroy_area(char_row,char_col);
    break;
    
  case 29 : 
    py.flags.word_recall = 10 + randint(10);
    msg_print("You feel dizzy for a moment...");
    ident = true;
    break;
    
  case 30 : 
    msg_print("The teeth flare into flames.");
    py.flags.ring_fire += 3+randint(3);
    ident = true;
    break;
    
  case 31 : 
    msg_print("The Demon teeth try to possess you...");
    lose_exp(100);
    change_rep(-10);
    ident = true;
    break;
    
  default:
    msg_print("Todd Gardiner is fat!");
    ident = true;
    break;
  }

  *idented = ident;
};
//////////////////////////////////////////////////////////////////////

void blow()
{
  unsigned long   i1;
  integer         i3,chance,i5;
  treas_ptr       i2,item_ptr;
  char            trash_char;
  boolean         redraw,ident;
  obj_set         things_that_blow = {misc_usable,valuable_gems,chime,horn,0};

  reset_flag = true;
  if (inven_ctr > 0) {
    if (find_range(things_that_blow,false,&i2,&i3)) {
      redraw = false;
      if (get_item(&item_ptr,"Use which item?",
		   &redraw,i3,&trash_char,false,false)) {
	//with item_ptr->data. do;
	if (redraw) {
	  draw_cave();
	}
	reset_flag = false;
	//with py.misc do;
	chance = PM.save + PM.lev + bard_adj() - item_ptr->data.level - 5;
	if (((py.flags.confused + py.flags.afraid) > 0) &&
	    ((item_ptr->data.tval == chime) || 
	     (item_ptr->data.tval == horn))) {
	  msg_print("you can not use the instrument...");
	} else if (item_ptr->data.p1 > 0) {
	  if (chance < 0) {
	    chance = 0;
	  }
	  if ((randint(chance) < USE_DEVICE) &&
	      ((item_ptr->data.tval != misc_usable) && 
	       (item_ptr->data.subval != 24))) {
	    msg_print("You failed to use the item properly.");
	  } else {
	    i1 = item_ptr->data.flags;
	    ident = false;
	    item_ptr->data.p1--;
	    for ( ; i1 > 0 ; ) {
	      i5 = bit_pos(&i1) + 1;
	      if ((item_ptr->data.tval == chime) || 
		  (item_ptr->data.tval == horn)) {
		b__chime_and_horn_effects(i5, &ident);
	      } else {
		b__misc_effects(i5, &ident, item_ptr);
	      }
	    }
	    
	    if (ident) {
	      identify(&(inven_temp->data));
	    }
	    if (inven_temp->data.flags != 0) {
	      //with py.misc do;
	      PM.exp += (item_ptr->data.level / (real)PM.lev) + .5;
	      prt_experience();
	    }
	    desc_charges(item_ptr);
	  }
	} /* end if p1 */
      } else {
	if (redraw) {
	  draw_cave();
	}
      }
    } else {
      msg_print("You are not carrying anything to use...");
    }
  } else {
    msg_print("But you are not carrying anything.");
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  blow.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

