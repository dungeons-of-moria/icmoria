/* monk.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void d__monk_effects(integer effect)
{
    /*{ Disciplines....}*/
    switch (effect+1) {

    case 1 :  /*{ Self-Healing }*/
	hp_player(damroll("4d4"),"a magic spell."); 
	break;


    case 2 :  /*{ Courage } */
	cure_me(&py.flags.afraid);
	break;


    case 3 :  /*{ Slow Poison } */
	slow_poison();
	break;


    case 4 :  /*{ Negate Hunger } */
	py.flags.foodc   = PLAYER_FOOD_FULL + 4000;     
	py.flags.status &= ~(IS_HUNGERY | IS_WEAK);
	prt_hunger();
	msg_print("You are full.");
	break;


    case 5 :  /*{ Sense Enemies }*/
	detect_creatures(c_creature);
	break;


    case 6 :  /*{ Self-Healing II }                 */
	hp_player(damroll("8d4"),"a prayer.");
	break;


    case 7 :  /*{ Night Vision }*/
	py.flags.tim_infra += randint(25) + 25;
	break;


    case 8 :  /*{ Poison Immunity }*/
	cure_me(&(py.flags.poisoned));
	break;


    case 9 :  /*{ See Invisible } */
	detect_inv2(randint(24)+24);
	break;


    case 10 :  /*{ Advanced Self-Healing } */
	hp_player(damroll("16d4"),"a prayer."); 
	break;


    case 11 :  /*{ Resist Petrification }*/
	py.flags.resist_petri += randint(15) + 10;
	break;


    case 12 :  /*{ Stealth }*/
	py.flags.temp_stealth += randint(15)+10;
	break;


    case 13 :  /*{ Free Action } */
	py.flags.free_time += (randint(10) + py.misc.lev); 
	break;


    case 14 :  /*{ Improved Speed }*/
	PF.fast += randint(20) + py.misc.lev;
	break;

    default:
	break;
    }
    /*{ End of Disciplines...}*/
};
//////////////////////////////////////////////////////////////////////

void discipline()
{
    /*{  Use a monk's mental discipline. . . .                  -RAD-   }*/
    integer      i2;
    integer      choice,chance;
    integer      y_dumy,x_dumy;
    boolean      redraw;
    boolean      flag;

    redraw = false;
    reset_flag = true;
    if (py.flags.confused > 0) {
	msg_print("You are too confused to concentrate....");
    } else if (py.flags.image > 0) {
	msg_print("Colors and images run through your head, distracting you.");
    } else {
	flag = false;
	for (i2=0 ; (i2 < 40) && !flag ; i2++) {
	    if (magic_spell[py.misc.pclass][i2].learned) {
		flag = true;
	    }
	}
	
	if (flag) {
	    inven_temp->data = monk_book;
	    if (cast_spell("Use which discipline?",inven_temp,
			   &choice,&chance,&redraw)) {
		//with magic_spell[py.misc.pclass][choice]. do;
		reset_flag = false;
		if (randint(100) < chance) {
		    msg_print("You lost your concentration!");
		} else {
		    
		    y_dumy = char_row;
		    x_dumy = char_col;
		    d__monk_effects(choice);
		    if (!reset_flag) {
			//with py.misc do;
			PM.exp += magic_spell[py.misc.pclass][choice].sexp;
			prt_experience();
			magic_spell[py.misc.pclass][choice].sexp = 0;
		    }
		}
		//with py.misc do;
		if (!reset_flag) {
		    if (magic_spell[py.misc.pclass][choice].smana > PM.cmana) {
			msg_print("You are distracted by the effort!");
			py.flags.paralysis =
			    randint(5*trunc(magic_spell[py.misc.pclass][choice].smana-PM.cmana));
			PM.cmana = 0;
		    } else {
			PM.cmana -= magic_spell[py.misc.pclass][choice].smana;
		    }
		    prt_mana();
		}
		
	    } else {
		erase_line(1,1);
	    }
	} else {
	    msg_print("You don't know any disciplines!");
	}
    }
}
/* END FILE  monk.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
