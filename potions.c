/* potions.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void q__potion_effect(integer effect, boolean *idented)
{
  integer   i4,i5;
  boolean   redraw = false;
  boolean   ident = false;

  /*{ Potions                                               }*/
  switch (effect) {
	    
  case 1  :  /*{ Gain Str }*/
    ident = gain_stat(STR,"X");
    break;
	    
  case 2  :  /*{ Lose Str }*/
    ident = lose_stat(STR,"X","X");
    break;
	    
  case 3  :  /*{ Restore Str }*/
    ident = restore_stat(STR,"X");
    break;
	    
  case 4  :  /*{ Gain Int }*/
    ident = gain_stat(INT,"X");
    break;
	    
  case 5  :  /*{ Lose Int }*/
    msg_print("This potion tastes very dull.");
    ident = lose_stat(INT,"X","X");
    break;
    
  case 6  :  /*{ Restore Int }*/
    ident = restore_stat(INT,"X");
    break;
    
  case 7  :  /*{ Gain Wis }*/
    ident = gain_stat(WIS,"X");
    break;
    
  case 8  :  /*{ Lose Wis }*/
    ident = lose_stat(WIS,"X","X");
    break;
    
  case 9  :  /*{ Restore Wis }*/
    ident = restore_stat(WIS,"X");
    break;
    
  case 10 :  /*{ Gain Chr }*/
    ident = gain_stat(CHR,"X");
    break;
    
  case 11 :  /*{ Lose Chr }*/
    ident = lose_stat(CHR,"X","X");
    break;
    
  case 12 :  /*{ Restore Chr }*/
    ident = restore_stat(CHR,"X");
    break;
    
    /*{ Cures and healing }*/
  case 13 :
    ident = hp_player(damroll("2d7"),"a potion.");
    break;
    
  case 14 :  
    ident = hp_player(damroll("4d7"),"a potion.");
    break;
    
  case 15 :  
    ident = hp_player(damroll("6d7"),"a potion.");
    break;
    
  case 16 :  
    ident = hp_player(1000,"a potion.");
    break;
    
  case 17 :  /*{ Gain Con }*/
    py.misc.mhp++;
    py.misc.chp += py.misc.mhp;
    ident = gain_stat(CON,"X");
    prt_hp();
    break;
    
  case 18 :  /*{ Gain Experience }*/
    //with py.misc do;
    i5 = (PM.exp div 2) + 10;
    if (i5 > 100000) {
      i5 = 100000;
    }
    PM.exp += i5;
    msg_print("You feel more experienced.");
    prt_experience();
    ident = true;
    break;
    
  case 19 :  /*{ Sleep }*/
    if (! (py.flags.free_act)) {
      msg_print("You fall asleep.");
      py.flags.paralysis += randint(4) + 4;
      ident = true;
    }
    break;
    
  case 20 :  /*{ Darkness }*/
    msg_print("You are covered by a veil of darkness.");
    PF.blind += randint(100) + 100;
    ident = true;
    break;
    
  case 21 :  /*{ Confusion }*/
    msg_print("Hey!  This is good stuff!  * Hick! *");
    PF.confused += randint(20) + 12;
    ident = true;
    break;
    
  case 22 :  /*{ Poison }*/
    msg_print("You feel very sick.");
    PF.poisoned += randint(15) + 10;
    ident = true;
    break;
    
  case 23 :  /*{ Haste Self }*/
    py.flags.fast += randint(25) + 15;
    ident = true;
    break;
    
  case 24 :  /*{ Slowness }*/
    py.flags.slow += randint(25) + 15;
    ident = true;
    break;
    
  case 25 :  
    ident = detect_creatures(c_monster);
    break;
    
  case 26 :  /*{ Increase Dex }*/
    ident = gain_stat(DEX,"X");
    break;
    
  case 27 :  /*{ Restore Dex }*/
    ident = restore_stat(DEX,"X");
    break;
    
  case 28 :  /*{ Restore Con }*/
    ident = restore_stat(CON,"X");
    break;
    
  case 29 :  /*{ Cure Blindness }*/
    cure_me(&py.flags.blind);
    break;
    
  case 30 :  /*{ Cure Confusion }*/
    cure_me(&py.flags.confused);
    break;
    
  case 31 :  /*{ Cure Poison }*/
    cure_me(&py.flags.poisoned);
    break;
    
  case 32 :  /*{ Learning }*/  /* 32 is the Cursed_worn_bit value */
  case 48 :  /*{ Learning }*/
    //with py.misc do;
    //with class[pclass] do;
    if (class[PM.pclass].mspell) {
      ident = learn_spell(&redraw);
      if (redraw) {
	draw_cave();
      }
    } else if (class[PM.pclass].bspell) {
      ident = learn_song(&redraw);
      if (redraw) {
	draw_cave();
      }
    } else if (class[PM.pclass].pspell) {
      ident = learn_prayer();
    } else if (class[PM.pclass].dspell) {
      ident = learn_druid();
    }
    break;
    
  case 33 :  /*{ Lose Memories }*/
    msg_print("You feel your memories fade...");
    msg_print("");
    i4 = trunc(py.misc.exp/5.0); 
    lose_exp(randint(i4)+i4);
    ident = true;
    break;
    
  case 34 :  /*{ Salt Water }*/
    //with py.flags do;
    PF.poisoned = 0;
    py.flags.status &= ~IS_POISONED;
    prt_poisoned();
    if (PF.foodc > 150) {
      PF.foodc = 150;
    }
    PF.paralysis = 4;
    msg_print("The potion makes you vomit!");
    ident = true;
    break;
    
  case 35 :  /*{ Invulnerability }*/
    py.flags.invuln += randint(10) + 10;
    ident = true;
    break;
    
  case 36 :  /*{ Heroism }*/
    py.flags.hero += randint(25) + 25;
    ident = true;
    break;
    
  case 37 :  /*{ Super-Heroism }*/
    py.flags.shero += randint(25) + 25;
    ident = true;
    break;
    
  case 38 :  /*{ Remove Fear }*/
    ident = cure_me(&py.flags.afraid);
    break;
    
  case 39 :  /*{ Restore Level }*/
    ident = restore_level();
    add_food(5000);
    py.flags.status &= ~(IS_WEAK | IS_HUNGERY);
    prt_hunger();
    break;
    
  case 40 :  /*{ Resist Heat }*/
    PF.resist_heat += randint(10) + 10;
    break;
    
  case 41 :  /*{ Resist Cold }*/
    PF.resist_cold += randint(10) + 10;
    break;
    
  case 42 :
    detect_inv2(randint(12)+12);
    break;
    
  case 43 :  /*{ Slow Poison }*/
    ident = slow_poison();
    break;
    
  case 44 :  /*{ Cure Poison }*/
    ident = cure_me(&py.flags.poisoned);
    break;
    
  case 45 :  /*{ Restore Mana }*/
    //with py.misc do;
    if (PM.cmana < PM.mana) {
      PM.cmana = PM.mana;
    }
    ident = true;
    msg_print("Your feel your head clear...");
    break;
    
  case 46 :  /*{ Infra-Vision }*/
    PF.tim_infra += 100 + randint(100);
    ident = true;
    msg_print("Your eyes begin to tingle.");
    break;
    
  case 47 :  /* cure hallucination */
    msg_print("Pretty colors!");
    PF.confused += randint(5) + 5;
    ident = cure_me(&py.flags.image);
    break;

    /* case 48 moved up to 32 */

  case 49 :  /* reduce petrification */
    if ( PF.petrification > 0 ) {
      ident = true;
      PF.petrification -= 100;
      if ( PF.petrification < 0 ) {
	PF.petrification = 0;
      }
    }
    break;

  case 50 :  ;
  case 51 :  ;
  case 52 :  ;
  case 53 :  ;
  case 54 :  ;
  case 55 :  ;
  case 56 :  ;
  case 57 :  ;
  case 58 :  ;
  case 59 :  ;
  case 60 :  ;
  case 61 :  ;
  case 62 :  ;
  default:
    break;
  } /* end switch */
  /*{ End of Potions...                                     }*/
  
  *idented = ident;
};
//////////////////////////////////////////////////////////////////////
void quaff()
{
  /*{ Potions for the quaffing                              -RAK-   }*/

  unsigned long    q1,q2;
  integer          i3,i6;
  treas_ptr        i2,item_ptr;
  char             trash_char;
  boolean          redraw,ident;
  obj_set          stuff_to_drink = {potion1, potion2, 0};

  reset_flag = true;

  if (inven_ctr > 0) {
    if (find_range(stuff_to_drink,false,&i2,&i3)) {
      redraw = false;
      if (get_item(&item_ptr,"Quaff which potion?",
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
	  i6 = bit_pos64(&q2,&q1)+1;

	  /*
	   * It looks like potion2 was created before flags2 was
	   * added to the treasure type, now we can fit all the
	   * potion effects into the pair of flags.
	   * 
	   * The += 31 should be 64 now, I am leaving it at 31 so
	   * that old characters do not get confused.
	   */
	  if (item_ptr->data.tval == potion2) {
	    i6 += 31; 
	  }

	  q__potion_effect(i6, &ident);
	} /* end for */
	
	if (ident) {
	  identify(&(item_ptr->data));
	}

	if (item_ptr->data.flags != 0) {
	  PM.exp += (item_ptr->data.level / (real)PM.lev) + .5;
	  prt_experience();
	}

	add_food(item_ptr->data.p1);
	desc_remain(item_ptr);
	inven_destroy(item_ptr);
	prt_weight();

      } else {
	if (redraw) {
	  draw_cave();
	}
      }
    } else {
      msg_print("You are not carrying any potions.");
    }
  } else {
    msg_print("But you are not carrying anything.");
  }
};

/* END FILE  potions.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
