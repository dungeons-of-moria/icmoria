/* magic.c */
/**/

#include "imoria.h"
#include "dungeon.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void c__mage_spell_effects(integer effect)
{
    /*{ Spells...                                     }*/

    integer      i2,dir;
    integer      dumy,y_dumy,x_dumy;

    y_dumy = char_row;
    x_dumy = char_col;

    switch (effect+1) {

    case 1  :  /*{ Magic Missile }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    fire_bolt(0,dir,char_row,char_col,
		      damroll("2d6")+1,"Magic Missile");
	}
	break;

    case 2  :  /*{ Detect Monster }*/
	detect_creatures(c_monster);
	break;

    case 3  :  /*{ Phase Door }*/
	teleport(10);
	break;

    case 4  :  /*{ Light }*/
	light_area(char_row,char_col);
	break;

    case 5  :  /*{ Cure Light }*/
	hp_player(damroll("4d4"),"a magic spell.");
	break;

    case 6  :  /*{ Find Hidden Traps/Door }*/
	detect_sdoor();
	detect_trap();
	break;

    case 7  :  /*{ Stinking Cloud }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    fire_ball(c_gas,dir,char_row,char_col,9,"Stinking Cloud");
	}
	break;

    case 8  :  /*{ Confuse Monster }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    zap_monster(dir,char_row,char_col,0,c_confuse);
	}
	break;

    case 9 :  /*{ Lightning Bolt }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    fire_bolt(c_lightning,dir,char_row,char_col,
		      damroll("3d8")+1,"Lightning Bolt");
	}
	break;

    case 10 :  /*{ Trap/Door Destruction }*/
	td_destroy();
	break;

    case 11 :  /*{ Sleep I }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  zap_monster(dir,char_row,char_col,0,c_sleep);
	}
	break;

    case 12 :  /*{ Cure Poison }*/
	cure_me(&(py.flags.poisoned));
	break;

    case 13 :  /*{ Shadow Door }*/
	teleport(py.misc.lev*5);
	break;

    case 14 :  /*{ Remove Curse }*/
	for (i2 = Equipment_min ; i2 <=  EQUIP_MAX-1 ; i2++) {
	    //with equipment[i2] do;
	    equipment[i2].flags &= 0x7FFFFFFF;
	}
	break;

    case 15 :  /*{ Frost Bolt }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    fire_bolt(c_cold,dir,char_row,char_col,
		      damroll("4d8")+1,"Frost Bolt");
	}
	break;

    case 16 :  /*{ Create Food }*/
	create_food(7,7,0,0,0);
	break;

    case 17 : /*{ Infravision }*/
	py.flags.tim_infra += 50 + randint(50);
	break;

    case 18 : /*{ Invisibility }*/
	py.flags.temp_stealth += randint(15)+10;
	break;

    case 19 :  /*{ Turn Stone to Mud }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  wall_to_mud(dir,char_row,char_col);
	}
	break;

    case 20 :  /*{ Recharge Item I }*/
	recharge(20);
	break;

    case 21 :  /*{ Sleep II }*/
	sleep_monsters1(char_row,char_col);
	break;

    case 22 :  /*{ Phantasmal Force }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  fire_bolt(c_illusion,dir,char_row,char_col,damroll("8d4"),
                        "phantasmal force");
	}
	break;

    case 23 :  /*{ Polymorph Other }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  poly_monster(dir,char_row,char_col);
	}
	break;

    case 24 :  /*{ Identify }*/
	ident_spell();
	break;

    case 25 : /*{ Ring of Frost }*/
	py.flags.ring_ice += 3 + randint(3);
	break;

    case 26 :  /*{ Sleep III }*/
	zap_area(0,0,c_sleep);
	break;

    case 27 :  /*{ Hold Monster }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  zap_monster(dir,char_row,char_col,4+randint(4),c_hold);
	}
	break;

    case 28 :  /*{ Fire Bolt }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  fire_bolt(c_fire,dir,char_row,char_col,
                                        damroll("6d8")+1,"Fire Bolt");
	}
	break;

    case 29 :  /*{ Slow Creature }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    zap_monster(dir,char_row,char_col,-1,c_speed);
	}
	break;

    case 30 : /*{ Protection From Magic }*/
	py.flags.magic_prot += 20 + randint(20);
	break;

    case 31 :  /*{ Frost Ball }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  fire_ball(c_cold,dir,char_row,char_col,40,"Frost Ball");
	}
	break;

    case 32 :  /*{ Death Spell }*/
	zap_area(0,py.misc.lev div 2+damroll("4d8"),c_drain);
	break;

    case 33 :  /*{ Ring of Fire }*/
	py.flags.ring_fire += 3 + randint(3);
	break;

    case 34 :  /*{ Recharge Item II }*/
	recharge(50);
	break;

    case 35 :  /*{ Teleport Other }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  teleport_monster(dir,char_row,char_col);
	}
	break;

    case 36 :  /*{ Haste Self }*/
	PF.fast += randint(20) + py.misc.lev;
	break;

    case 37 :  /*{ Fire Ball }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
                  fire_ball(c_fire,dir,char_row,char_col,59,"Fire Ball");
	}
	break;

    case 38 :  /*{ Power Word: Destruction }*/
	destroy_area(char_row,char_col);
	break;

    case 39 :  /*{ Power Word: Kill }*/
	if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
	    fire_bolt(0,dir,char_row,char_col,
		      damroll("24d8"),"Black Bolt");
	}
	break;

    case 40 :  /*{ Genocide }*/
	genocide();
	break;

    default:
	break;
    }
/*{ End of spells...                              }*/
    
};
//////////////////////////////////////////////////////////////////////


void cast()
{
    /*{ Throw a magic spell                                   -RAK-   }*/

    treas_ptr    i1,item_ptr;
    integer      choice,chance,i2;
    char         trash_char;
    boolean      redraw;
    obj_set      magic_books = {Magic_Book, 0};

    reset_flag = true;
    if (py.flags.blind > 0) {
	msg_print("You can't see to read your spell book!");
    } else if (no_light()) {
	msg_print("You have no light to read by.");
    } else if (py.flags.confused > 0) {
	msg_print("You are too confused...");
    } else if (class[py.misc.pclass].mspell) {
	if (inven_ctr > 0) {
	    if (find_range(magic_books,false,&i1,&i2)) {
		redraw = false;
		if (get_item(&item_ptr,"Use which spell-book?",&redraw,
			     i2,&trash_char,false,false)) {
		    
		    if (cast_spell("Cast which spell?",item_ptr,
				   &choice,&chance,&redraw)) {
			
                        //with magic_spell[py.misc.pclass,choice] do;
			reset_flag = false;
			if (randint(100) < chance) {
			    msg_print("You failed to get the spell off!");
			} else {
			    c__mage_spell_effects(choice);
			    if (!(reset_flag)) {
				//with py.misc do;
				PM.exp += magic_spell[PM.pclass][choice].sexp;
				prt_experience();
				magic_spell[PM.pclass][choice].sexp = 0;
			    }
			}
			
			//with py.misc do;
			if (!(reset_flag)) {
			    if (magic_spell[PM.pclass][choice].smana > PM.cmana) {
				msg_print("You faint from the effort!");
				py.flags.paralysis =
				    randint(5*trunc(magic_spell[PM.pclass][choice].smana-PM.cmana));
				PM.cmana = 0;
				if (randint(3) == 1) {
				    /*{lower_stat == no sustain}*/
				    lower_stat(CON,"You have damaged your health!");
				}
			    } else {
				PM.cmana -= magic_spell[PM.pclass][choice].smana;
			    }
			    prt_mana();
			}
		    } /* end if which spell */
                    
		} else {
                    if (redraw) {
			draw_cave();
		    }
		}	    
	    } else {
                msg_print("But you are not carrying any spell-books!");
	    }            
	} else {
            msg_print("But you are not carrying any spell-books!");
	}
    } else {
	msg_print("You can't cast spells!");
    }
};

/* END FILE  magic.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
