/* prayer.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void p__prayer_effects(integer effect)
{

  /*{ Prayers...                                    }*/
  integer       i2,dir;
  integer       dumy,y_dumy,x_dumy;

  y_dumy = char_row;
  x_dumy = char_col;

  switch (effect+1) {
    
  case 1  :  /*{ Detect Evil }*/
    detect_creatures(c_evil);
    break;
    
  case 2  :  /*{ Cure Light Wounds }*/
    hp_player(damroll("3d3"),"a prayer.");
    break;
    
  case 3  :  /*{ Bless }*/
    bless(randint(12)+12);
    break;
    
  case 4  :  /*{ Remove Fear }*/
    cure_me(&py.flags.afraid);
    break;
    
  case 5  :  /*{ Call Light }*/
    light_area(char_row,char_col);
    break;
    
  case 6  :  /*{ Find Traps }*/
    detect_trap();
    break;
    
  case 7  :  /*{ Detect Doors/Stairs }*/
    detect_sdoor();
    break;
    
  case 8  :  /*{ Slow Poison }*/
    slow_poison();
    break;
    
  case 9  :  /*{ Blind Creature }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      zap_monster(dir,char_row,char_col,0,c_confuse);
    }
    break;
    
  case 10 :  /*{ Portal }*/
    teleport(py.misc.lev*3);
    break;
    
  case 11 :  /*{ Cure Medium Wounds }*/
    hp_player(damroll("4d4"),"a prayer.");
    break;
    
  case 12 :  /*{ Ray of Sanctification }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_bolt(c_good,dir,char_row,char_col,damroll("2d6"),"Purple Ray");
    }
    break;
    
  case 13 :  /*{ Heroism }*/
    py.flags.hero += randint(24) + 48;
    break;
    
  case 14 :  /*{ Sanctuary }*/
    sleep_monsters1(char_row,char_col);
    break;
    
  case 15 :  /*{ Remove Curse }*/
    for (i2 = Equipment_min; i2 <= EQUIP_MAX-1; i2++) {
      //with equipment[i2]. do;
      equipment[i2].flags &= 0x7FFFFFFF;
    }
    break;
    
  case 16 :  /*{ Resist Heat and Cold }*/
    //with py.flags do;
    PF.resist_heat += randint(10) + 10;
    PF.resist_cold += randint(10) + 10;
    break;
		  
  case 17 :  /*{ Silence }*/
    PF.temp_stealth += (randint(20) + 15);
    break;
    
  case 18 :  /*{ Resist Petrification }*/
    PF.resist_petri += (randint(15) + 10);
    break;
    
  case 19 :  /*{ Neutralize Poison }*/
    cure_me(&py.flags.poisoned);
    break;
    
  case 20 :  /*{ Cure Serious Wounds }*/
    hp_player(damroll("9d4"),"a prayer.");
    break;
    
  case 21 :  /*{ Chant }*/
    bless(24+randint(48));
    break;
    
  case 22 :  /*{ Sense Invisible }*/
    detect_inv2(randint(24)+24);
    break;
    
  case 23 :  /*{ Protection from Evil }*/
    protect_evil();
    break;
    
  case 24 :  /*{ Earthquake }*/
    earthquake();
    break;
    
  case 25 :  /*{ Create food }*/
    create_food(3,2,1,0,0);
    break;
    
  case 26 :  /*{ Sense Surroundings }*/
    map_area();
    break;
    
  case 27 :  /*{ Orb of Draining }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_ball(c_good,dir,char_row,char_col,
		damroll("3d6")+py.misc.lev,"Black Sphere");
    }
    break;
    
  case 28 :  /*{ Cure Critical Wounds }*/
    hp_player(damroll("20d4"),"a prayer.");
    break;
    
  case 29 :  /*{ Turn Undead }*/
    zap_area(0,0,c_turn);
    break;
    
  case 30 :  /*{ Prayer }*/
    py.flags.shero = 24 + randint(48);   /* XXXX not cumulitive */
    break;
    
  case 31 :  /*{ Dispell Undead }*/
    zap_area(0x0008,3*py.misc.lev,c_hp);
    break;
    
  case 32 :  /*{ Resist Paralysis }*/
    py.flags.free_time += (randint(20) + 15);
    break;
    
  case 33 :  /*{ Blade Barrier }*/
    py.flags.blade_ring += 3+randint(3);
    break;
    
  case 34 :  /*{ Dispell Evil }*/
    zap_area(0x0004,3*py.misc.lev,c_hp);
    break;
    
  case 35 :  /*{ Heal }*/
    hp_player(200,"a prayer.");
    break;
    
  case 36 :  /*{ Resist Magic }*/
    py.flags.magic_prot += 40 + randint(40);
    break;
    
  case 37 :  /*{ Holy Thunder }*/
    msg_print("KABOOM!");
    zap_area(0x0004,4+randint(4),c_thunder);
    break;
	       
  case 38 :  /*{ Glyph of Warding }*/
    warding_glyph();
    break;
    
  case 39 :  /*{ Hero's Feast }*/
    msg_print("You have a marvelous meal!");
    py.flags.foodc = PLAYER_FOOD_FULL+4000;
    prt_hunger();
    hp_player(200,"a prayer.");
    create_food(6,4,3,2,1);
    py.flags.status &= ~(IS_WEAK | IS_HUNGERY);
    prt_hunger();
    msg_print("You are full.");
    break;
		
  case 40 :  /*{ Holy Word }*/
    zap_area(0x0004,6*py.misc.lev,c_holy_word);
    cure_me(&py.flags.afraid);
    cure_me(&py.flags.poisoned);
    hp_player(1000,"a prayer.");
    break;

  default:
    break;
  }
  /*{ End of prayers...                             }*/

};
//////////////////////////////////////////////////////////////////////
void pray()
{
  /*{ Pray like HELL...                                     -RAK-   }*/

  treas_ptr     i1,item_ptr;
  integer       choice,chance,i2;
  char          trash_char;
  boolean       redraw;
  obj_set       good_book = {Prayer_Book, 0};

  reset_flag = true;
  if (py.flags.blind > 0) {
    msg_print("You can't see to read your prayer!");
  } else if (no_light()) {
    msg_print("You have no light to read by.");
  } else if (py.flags.confused > 0) {
    msg_print("You are too confused...");
  } else if (class[py.misc.pclass].pspell) {
    if (inven_ctr > 0) {
      if (find_range(good_book,false,&i1,&i2)) {
        redraw = false;
        if (get_item(&item_ptr,"Use which Holy Book?",
                     &redraw,i2,&trash_char,false,false)) {
          if (cast_spell("Recite which prayer?",item_ptr,
                         &choice,&chance,&redraw)) {
	    
            //with magic_spell[PM.pclass][choice]. do
              reset_flag = false;
	    if (randint(100) < chance) {
	      msg_print("You lost your concentration!");
	    } else {
	      p__prayer_effects(choice);
	      if (!reset_flag) {
		//with py.misc do;
		PM.exp += magic_spell[PM.pclass][choice].sexp;
		prt_experience();
		magic_spell[PM.pclass][choice].sexp = 0;
	      }
	    }
	    
	    //with py.misc do;
	    if (!reset_flag) {
	      if (magic_spell[PM.pclass][choice].smana > PM.cmana) {
		msg_print("You faint from fatigue!");
		py.flags.paralysis = 
		  randint(5*(magic_spell[PM.pclass][choice].smana-PM.cmana));
		PM.cmana = 0;
		if (randint(3) == 1) {
		  /*{XXXX does not check for sustain}*/
		  lower_stat(CON,"You have damaged your health!");
		}
	      } else {
		PM.cmana -= magic_spell[PM.pclass][choice].smana;
	      }
	      prt_mana();
	    } else {
	      if (redraw) { 
		draw_cave();
	      }
	    }
	  }
	}
      } else {
	msg_print("But you are not carrying any Holy Books!");
      }
    } else {
      msg_print("But you are not carrying any Holy Books!");
    }	
  } else {
    msg_print("Pray hard enough and your prayers may be answered.");
  }
};
/* END FILE  prayer.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
