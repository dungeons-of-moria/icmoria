/* scrolls.c */
/**/

#include "imoria.h"
#include "dungeon.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void rs__scroll_effect(integer effect, boolean *idented, 
		       boolean *first, treas_ptr item_ptr)
{
  integer          i3,i4,randy;
  integer          y,x;
  integer          tmp[9];	/*: array [1..8] of integer;*/
  vtype            out_val,out_val2,str1,str2;
  boolean          ident, flag;
  vtype            dstr[20];

  ident = *idented;
  
  /*{ Scrolls...						}*/
  switch (effect) {
    
  case 1  :	/*{ Enchant to Hit }*/
    //with equipment[Equipment_primary]. do;
    if (equipment[Equipment_primary].tval > 0) {
      inven_temp->data = equipment[Equipment_primary];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows faintly!",out_val);
      msg_print(out_val2);
      if (enchant(&(equipment[Equipment_primary].tohit))) {
	/*{Un-curse the object}*/            /* not(Cursed_worn_bit)*/
	equipment[Equipment_primary].flags &= 0x7FFFFFFF;  
	py_bonuses(&blank_treasure,0);
      } else {
	msg_print("The enchantment fails...");
      }
      ident = true;
    }
    break;
    
  case 2  :  /*{ Enchant to Damage }*/
    //with equipment[Equipment_primary]. do;
    if (equipment[Equipment_primary].tval > 0) {
      inven_temp->data = equipment[Equipment_primary];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows faintly!",out_val);
      msg_print(out_val2);
      if (enchant(&(equipment[Equipment_primary].todam))) {
	                               /* not(Cursed_worn_bit)*/
	equipment[Equipment_primary].flags &= 0x7FFFFFFF;  
	py_bonuses(&blank_treasure,0);
      } else {
	msg_print("The enchantment fails...");
      }
      ident = true;
    }
    break;
    
  case 3  :  /*{ Enchant Armor }*/
    i3 = 0;
    i4 = 0;
    if (equipment[Equipment_armor].tval > 0) {
      i3++;
      tmp[i3] = Equipment_armor;
    }
    if (equipment[Equipment_shield].tval > 0) {
      i3++;
      tmp[i3] = Equipment_shield;
    }
    if (equipment[Equipment_cloak].tval > 0) {
      i3++;
      tmp[i3] = Equipment_cloak;
    }
    if (equipment[Equipment_gloves].tval > 0) {
      i3++;
      tmp[i3] = Equipment_gloves;
    }
    if (equipment[Equipment_helm].tval > 0) {
      i3++;
      tmp[i3] = Equipment_helm;
    }
    if (equipment[Equipment_boots].tval > 0) {
      i3++;
      tmp[i3] = Equipment_boots;
    }
    if (equipment[Equipment_belt].tval > 0) {
      i3++;
      tmp[i3] = Equipment_belt;
    }
    if (equipment[Equipment_bracers].tval > 0) {
      i3++;
      tmp[i3] = Equipment_bracers;
    }
    if (i3 > 0) {
      i4 = tmp[randint(i3)];
    }
    
    /* if something is cursed it gets priority */

    if (uand(Cursed_worn_bit,equipment[Equipment_armor].flags) != 0) {
      i4 = Equipment_armor;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_shield].flags) != 0) {
      i4 = Equipment_shield;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_cloak].flags) != 0) {
      i4 = Equipment_cloak;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_helm].flags) != 0) {
      i4 = Equipment_helm;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_gloves].flags) != 0) {
      i4 = Equipment_gloves;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_boots].flags) != 0) {
      i4 = Equipment_boots;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_belt].flags) != 0) {
      i4 = Equipment_belt;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_bracers].flags) != 0) {
      i4 = Equipment_bracers;
    }
    
    if (i4 > 0) {
      //with equipment[i4]. do;
      inven_temp->data = equipment[i4];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows faintly!",out_val);
      msg_print(out_val2);
      if (enchant(&(equipment[i4].toac))) {
	equipment[i4].flags &= 0x7FFFFFFF;  /* not(Cursed_worn_bit)*/
	py_bonuses(&blank_treasure,0);
      } else {
	msg_print("The enchantment fails...");
      }
      ident = true;
    }
    break;
    
  case 4  :	/*{ Identify }*/
    identify(&(item_ptr->data));
    msg_print("This is an identify scroll");
    msg_print(" ");
    if (ident_spell()) {
      *first = false;
    }
    break;
    
  case 5  :	/*{ Remove Curse }*/
    if (remove_curse()) {
      msg_print("You feel as if someone is watching over you.");
      ident = true;
    }
    break;
    
  case 6  :	/*{ Light }*/
    ident = light_area(char_row,char_col);
    break;
    
  case 7  :	/*{ Summon Monster }*/
    randy = randint(3);
    for (i3 = 0; i3 < randy; i3++) {
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
    
  case 8  :	/*{ Phase Door }*/
    teleport(10);
    ident = true;
    break;
    
  case 9  :	/*{ Teleport }*/
    teleport(100);
    ident = true;
    break;
    
  case 10 :  /*{ Teleport Level }*/
    dun_level += 2*randint(2) - 3;
    if (dun_level < 1) {
      dun_level = 1;
    }
    moria_flag = true;
    ident = true;
    break;
    
  case 11 :  /*{ Confuse Monster }*/
    msg_print("Your hands begin to glow.");
    py.flags.confuse_monster = true;
    ident = true;
    break;
    
  case 12 :	/*{ Magic Mapping }*/
    ident = map_area();
    break;
    
  case 13 :	/*{ Sleep Monster }*/
    ident = sleep_monsters1(char_row,char_col);
    break;
    
  case 14 :	/*{ Rune of Protection }*/
    ident = warding_glyph();
    break;
    
  case 15 :	/*{ Detect Treasure }*/
    ident = detect_item(c_treasure);
    break;
    
  case 16 :	/*{ Detect Object }*/
    ident = detect_item(c_object);
    break;
    
  case 17 :  /*{ Detect Trap }*/
    ident = detect_trap();
    break;
    
  case 18 :  /*{ Detect Secret Door }*/
    ident = detect_sdoor();
    break;
    
  case 19 :		  /*{ Mass Genocide }*/
    msg_print("This is a mass genocide scroll.");
    msg_print(" ");
    ident = mass_genocide();
    break;
    
  case 20 :	/*{ Detect Invisible }*/
    ident = detect_creatures(c_invisible);
    break;
    
  case 21 :  /*{ Aggravate Monster }*/
    ident = aggravate_monster(20);
    msg_print("There is a high pitched humming noise");
    break;
    
  case 22 :	/*{ Create Trap }*/
    ident = trap_creation();
    break;
    
  case 23 :	
    ident = td_destroy();
    break;
    
  case 24 :	/*{ Create Door }*/
    ident = door_creation();
    break;
    
  case 25 :	/*{ Recharge Item }*/
    identify(&(item_ptr->data));
    msg_print("This is a Recharge-Item scroll.");
    msg_print(" ");
    if (recharge(60)) {
      *first = false;
    }
    break;
    
  case 26 :	/*{ Genocide }*/
    msg_print("This is a genocide scroll.");
    msg_print(" ");
    ident = genocide();
    break;
    
  case 27 :	/*{ Darkness }*/
    ident = unlight_area(char_row,char_col);
    break;
    
  case 28 :	/*{ Prot vs Evil }*/
    ident = protect_evil();
    break;
    
  case 29 :	/*{ Create Food }*/
    ident = create_food(3,2,1,0,0);
    break;
    
  case 30 :	/*{ Dispell Creature }*/
    ident = zap_area(0x0008,60,c_hp);
    break;
    
  case 31 :  /*{ Blank }*/
    msg_print("That scroll appeared to be blank.");
    ident = true;
    break;
    
  case 32 :	/*{ Enchant Weapon }*/ /* 32 is the Cursed_worn_bit value */
  case 45 :	/*{ Enchant Weapon }*/
    //with equipment[Equipment_primary]. do;
    if (equipment[Equipment_primary].tval > 0) {
      inven_temp->data = equipment[Equipment_primary];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows brightly!",out_val);
      msg_print(out_val2);
      flag = false;
      randy = randint(2);
      for (i3 = 0; i3 < randy; i3++) {
	if (enchant(&(equipment[Equipment_primary].tohit))) {
	  flag = true;
	}
      }
      randy = randint(2);
      for (i3 = 0; i3 < randy; i3++) {
	if (enchant(&(equipment[Equipment_primary].todam))) {
	  flag = true;
	}
      }
      if (flag) {
                                        /* not(Cursed_worn_bit)*/
	equipment[Equipment_primary].flags &= 0x7FFFFFFF;  
	py_bonuses(&blank_treasure,0);
      } else {
	msg_print("The enchantment fails...");
      }
      ident = true;
    }
    break;
    
  case 33 :  /*{ Curse Weapon }*/
    //with equipment[Equipment_primary]. do;
    if (equipment[Equipment_primary].tval > 0) {
      inven_temp->data = equipment[Equipment_primary];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows black, then fades.",out_val);
      msg_print(out_val2);
      equipment[Equipment_primary].tohit = -randint(5) - randint(5);
      equipment[Equipment_primary].todam = -randint(5) - randint(5);
      equipment[Equipment_primary].flags = Cursed_worn_bit;
      py_bonuses(&(inven_temp->data),-1);
      ident = true;
    }
    break;
    
  case 34 :	
    if (uand(Cursed_worn_bit,equipment[Equipment_armor].flags) != 0) {
      i3 = Equipment_armor;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_shield].flags) != 0) {
      i3 = Equipment_shield;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_cloak].flags) != 0) {
      i3 = Equipment_cloak;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_helm].flags) != 0) {
      i3 = Equipment_helm;
    } else if (uand(Cursed_worn_bit,equipment[Equipment_gloves].flags) != 0) {
      i3 = Equipment_gloves;
    } else if (equipment[Equipment_armor].tval > 0) {
      i3 = Equipment_armor;
    } else if (equipment[Equipment_shield].tval > 0) {
      i3 = Equipment_shield;
    } else if (equipment[Equipment_helm].tval > 0) {
      i3 = Equipment_helm;
    } else if (equipment[Equipment_gloves].tval > 0) {
      i3 = Equipment_gloves;
    } else {
      i3 = 0;
    }
    
    if (i3 > 0) {
      //with equipment[i3]. do;
      inven_temp->data = equipment[i3];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2, "Your %s glows brightly!",out_val);
      msg_print(out_val2);
      flag = false;
      randy = randint(2);
      for (i3 = 0; i3 < randy; i3++) {
	if (enchant(&(equipment[i3].toac))) {
	  flag = true;
	}
      }
      if (flag) {
	equipment[i3].flags &= 0x7FFFFFFF;  /* not(Cursed_worn_bit)*/
	py_bonuses(&blank_treasure,0);
      } else {
	msg_print("The enchantment fails...");
      }
      ident = true;
    }
    break;
    
  case 35 :
    if ((equipment[Equipment_armor].tval > 0) && (randint(4) == 1)) {
      i3 = Equipment_armor;
    } else if ((equipment[Equipment_shield].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_shield;
    } else if ((equipment[Equipment_cloak].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_cloak;
    } else if ((equipment[Equipment_helm].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_helm;
    } else if ((equipment[Equipment_gloves].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_gloves;
    } else if ((equipment[Equipment_belt].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_belt;
    } else if ((equipment[Equipment_bracers].tval > 0) && (randint(3) == 1)) {
      i3 = Equipment_bracers;
    } else if (equipment[Equipment_armor].tval > 0) {
      i3 = Equipment_armor;
    } else if (equipment[Equipment_shield].tval > 0) {
      i3 = Equipment_shield;
    } else if (equipment[Equipment_cloak].tval > 0) {
      i3 = Equipment_cloak;
    } else if (equipment[Equipment_helm].tval > 0) {
      i3 = Equipment_helm;
    } else if (equipment[Equipment_gloves].tval > 0) {
      i3 = Equipment_gloves;
    } else if (equipment[Equipment_belt].tval > 0) {
      i3 = Equipment_belt;
    } else if (equipment[Equipment_bracers].tval > 0) {
      i3 = Equipment_bracers;
    } else {
      i3 = 0;
    }
    
    if (i3 > 0) {
      //with equipment[i3]. do;
      inven_temp->data = equipment[i3];
      objdes(out_val,inven_temp,false);
      sprintf(out_val2,"Your %s glows black, then fades.",out_val);
      msg_print(out_val2);
      equipment[i3].flags = Cursed_worn_bit;
      equipment[i3].toac = -randint(5) - randint(5);
      py_bonuses(&(inven_temp->data),-1);
      ident = true;
    }
    break;
    
  case 36 :  /*{ Summon Undead }*/
    randy = randint(3);
    for (i3 = 0; i3 < randy; i3++) {
      y = char_row;
      x = char_col;
      summon_undead(&y,&x);
    }
    ident = true;
    break;
    
  case 37 :
    ident = bless(randint(12)+6);
    break;
    
  case 38 :
    ident = bless(randint(24)+12);
    break;
    
  case 39 :	
    ident = bless(randint(48)+24);
    break;
    
  case 40 :  /*{ Word Of Recall }*/
    ident = true;
    py.flags.word_recall = 25 + randint(30);
    msg_print("The air about you becomes charged...");
    break;
    
  case 41 :
    ident = destroy_area(char_row,char_col);
    break;
    
  case 42 :	
    ident = true;
    msg_print("This is a scroll of wishing.");
    msg_print(" ");
    if (cave[char_row][char_col].tptr == 0) {
      prt("What item do you wish to summon?",1,1);
      if (get_string(str1,1,34,40)) {
	if (str1[0] >= 'a') {
	  str1[0] -= 32;
	}
	prt("What abilities should it have?",1,1);
	if (get_string(str2,1,32,40)) {
	  if (str2[0] >= 'a') {
	    str2[0] -= 32;
	  }
	  if (summon_item(char_row,char_col,str1,str2,10,true)) {
	    *first = false;
	  }
	}
      }
    } else {
      msg_print("You are standing on something.");
    }
    break;
    
  case 43 :
    msg_print("You feel your life slipping away!");
    msg_print(" ");
    strcpy(died_from, "an *EVIL SCROLL*");
    make_tomb(dstr);
    write_tomb(dstr);
    print_dead_character();
    draw_cave();
    msg_print("Huh?  What happened???");
    py.flags.confused += randint(5) + 3;
    ident = true;
    break;
    
  case 44 : /*{make munchies}*/
    ident = create_food(-1,-2,-3,-4,0);
    break;

    /* case 45 moved up to 32 */

  case 46 :	;
  case 47 :	;
  case 48 :	;
  case 49 :	;
  case 50 :	;
  case 51 :	;
  case 52 :	;
  case 53 :	;
  case 54 :	;
  case 55 :	;
  case 56 :	;
  case 57 :	;
  case 58 :	;
  case 59 :	;
  case 60 :	;
  case 61 :	;
  case 62 :	;
  default : ;
    break;
  } /* end switch */

  /*{ End of Scrolls...					}*/
  
  *idented = ident;
};
//////////////////////////////////////////////////////////////////////
void read_scroll()
{
  /*{ Scrolls for the reading				-RAK-	}*/

  unsigned long    q1,q2;
  integer          i3,i5;
  treas_ptr        i2,item_ptr;
  char             trash_char;
  boolean          redraw,ident,first;
  obj_set          stuff_to_read = {scroll1, scroll2, 0};

  first = true;
  reset_flag = true;

  if (inven_ctr > 0) {
    if (find_range(stuff_to_read,false,&i2,&i3)) {
      if (py.flags.blind > 0) {
	msg_print("You can't see to read the scroll.");
      } else if (no_light()) {
	msg_print("You have no light to read by.");
      } else if (py.flags.confused > 0) {
	msg_print("The text seems to swim about the page!");
	msg_print("You are too confused to read...");
      } else {
	redraw = false;
	if (get_item(&item_ptr,"Read which scroll?",
		     &redraw,i3,&trash_char,false,false)) {
	  //with item_ptr->data. do;
	  if (redraw) {
	    draw_cave();
	  }
	  reset_flag = false;
	  q1 = item_ptr->data.flags;
	  q2 = item_ptr->data.flags2;
	  ident = false;
	  for (;q1 > 0 || q2 > 0;) {
	    i5 = bit_pos64(&q2,&q1)+1;

	  /*
	   * It looks like scroll2 was created before flags2 was
	   * added to the treasure type, now we can fit all the
	   * potion effects into the pair of flags.
	   * 
	   * The += 31 should be 64 now, I am leaving it at 31 so
	   * that old characters do not get confused.
	   */
	    if (item_ptr->data.tval == scroll2) {
	      i5 += 31;
	    }
	    if (first) {
	      if (!(i5 == 4 || i5 == 25 || i5 == 42)) {
		/* the above are: identify, recharge item, wishing */
		msg_print("As you read the scroll it vanishes.");
		first = false;
	      }
	      
	      rs__scroll_effect(i5, &ident, &first, item_ptr);
	    }
	  }
	  
	  if (!(reset_flag)) {
	    if (ident) {
	      identify(&(item_ptr->data));
	    }
	    if (!first) {
	      desc_remain(item_ptr);
	      inven_destroy(item_ptr);
	      prt_weight();

	      if (item_ptr->data.flags != 0) {
		//with py.misc do;
		PM.exp += (item_ptr->data.level / (real)PM.lev) + .5;
		prt_experience();
	      }
	    }
	  }
	} else {
	  if (redraw) {
	    draw_cave();
	  }
	}
      }
    } else {
      msg_print("You are not carrying any scrolls.");
    }
  } else {
    msg_print("But you are not carrying anything.");
  }
};
/* END FILE  scrolls.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
