/* eat.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void e__eyeball_of_drong(treas_ptr item_ptr, boolean *ident)
{
  integer  i1;

  i1 = damroll("10d8")+100;
  take_hit(i1, "the Wrath of Ned");

  cure_me(&PF.afraid);
  bless(randint(100)+100);
  PF.hero     += randint(100) + 100;
  PF.shero    += randint(50)  + 75;
  PF.invuln   += randint(15)  + 10;

  PF.image    += randint(500) + randint(500) + randint(1000) + 5000;
  PF.poisoned += randint(100) + 150;
  PF.confused += randint(30)  + 50;
  PF.blind    += randint(3)   + 10;
  PF.paralysis = 4;

  msg_print("You hear a distant rumble of laughter...");
  msg_print("You throw back your head and laugh back!");
  aggravate_monster(30);

  i1 = randint(5);
  if (i1 == 1) {
    gain_stat(STR,"Ned smiles upon you.");
    gain_stat(INT,"");
    gain_stat(WIS,"");
    gain_stat(DEX,"");
    gain_stat(CON,"");
    gain_stat(CHR,"");
  }

  msg_print("Your head begins to spin. Spots swirl before you.");
  msg_print("A veil of darkness clings to your eyes.");
  msg_print("You are unable to move!");

  *ident = true;
};
//////////////////////////////////////////////////////////////////////
void eat()
{
/* { Eat some food...					-RAK-	}*/

  unsigned long	    i1;
  integer           i3,i4,i5;
  treas_ptr         i2,item_ptr;
  char              trash_char;
  boolean           redraw,ident;
  integer           dam_pts = 0;

  obj_set           things_to_eat = {Food, Junk_food, 0};

  reset_flag = true;

  if (inven_ctr > 0) {
    if (find_range(things_to_eat,false,&i2,&i3)) {
      redraw = false;
      if (get_item(&item_ptr,"Eat what?",&redraw,i3,&trash_char,false,false)) {

	//with item_ptr->data. do;

	if (redraw) { 
	  draw_cave();
	}

	reset_flag = false;
	ident = false;

	if ((item_ptr->data.tval == junk_food) &&
	    (item_ptr->data.subval == 270)) { /* eyeball of drong */
	  e__eyeball_of_drong(item_ptr, &ident);
	} else {

	i1 = item_ptr->data.flags;
	
	for (;i1 > 0;) {
	  i4 = bit_pos(&i1) + 1;

	  /*{ Foods }*/

	  switch (i4) {
          case 1  :  
	    //with py.flags do;
	    PF.poisoned += randint(10) + item_ptr->data.level;
	    ident = true;
	    break;

          case 2  :  
	    //with py.flags do;
	    PF.blind += randint(250) + 10*item_ptr->data.level + 100;
	    draw_cave();
	    msg_print("A veil of darkness surrounds you.");
	    ident = true;
	    break;

          case 3  :  
	    //with py.flags do;
	    PF.afraid += randint(10) + item_ptr->data.level;
	    msg_print("You feel terrified!");
	    ident = true;
	    break;
	  
          case 4  :  
	    //with py.flags do;
	    PF.confused += randint(10) + item_ptr->data.level;
	    msg_print("You feel drugged.");
	    break;
	  
          case 5  :  
	    //with py.flags do;
	    PF.image += randint(200) + 25*item_ptr->data.level + 200;
	    break;

          case 6  :  
	    ident = cure_me(&py.flags.poisoned);
	    break;

          case 7  :  
	    ident = cure_me(&py.flags.blind);
	    break;

          case 8  :  
	    //with py.flags do;
	    if (PF.afraid > 1) {
	      PF.afraid = 1;
	      ident  = true;
	    }
	    break;
	    
          case 9  :  
	    ident = cure_me(&py.flags.confused);
	    break;

          case 10 :  
	    ident = lose_stat(STR,"X","X");
	    break;

          case 11 :  
	    ident = lose_stat(CON,"X","X");
	    break;

          case 12 :  
	    ident = lose_stat(INT,"X","X");
	    break;

          case 13 :
	    ident = lose_stat(WIS,"X","X");
	    break;

          case 14 :  
	    ident = lose_stat(DEX,"X","X");
	    break;

          case 15 :  
	    ident = lose_stat(CHR,"X","X");
	    break;

          case 16 :  
	    ident = restore_stat(STR,"You feel your strength returning.");
	    break;

          case 17 :  
	    ident = restore_stat(CON,"X");
	    break;

          case 18 :
	    ident = restore_stat(INT,"Your head spins for a moment.");
	    break;

          case 19 :
	    ident = restore_stat(WIS,"X");
	    break;

          case 20 : 
	    ident = restore_stat(DEX,"You feel more agile.");
	    break;

          case 21 :  
	    ident = restore_stat(CHR,"X");
	    break;

          case 22 :
	    dam_pts += randint(3);
	    break;

          case 23 :  
	    dam_pts += randint(6);
	    break;

          case 24 :  
	    dam_pts += randint(12);
	    break;

          case 25 :
	    dam_pts += damroll("2d12");
	    break;

          case 26 :
	    dam_pts += damroll("4d12");
	    break;

          case 27 :  
	    dam_pts -= randint(4);
	    break;

          case 28 :  
	    dam_pts -= randint(8);
	    break;

          case 29 : 
	    ident = cure_me(&py.flags.hoarse);
	    break;

/*{ fill player to full, then adds food value }*/
          case 30 : 
	    py.flags.foodc = PLAYER_FOOD_FULL;
	    msg_print("Yum!");
	    break;

          case 31 :
	    i5 = bit_pos(&item_ptr->data.flags2) + 1;
	    switch (i5) {
	      case 1 :    /*{crunchy}*/          
		switch (randint(4)) {
		case 1 : msg_print("*munch* *munch*");   break;
		case 2 : msg_print("*crunch* *crunch*"); break;
		case 3 : msg_print("*munch* *crunch*");  break;
		case 4 : msg_print("*crunch* *munch*");  break;
		}
		break;

	    case 2 :   /*{liquid}*/
	      switch (randint(3)) {
	      case 1 : msg_print("*guzzle* *guzzle*");    break;
	      case 2 : msg_print("*glug* *glug* *glug*"); break;
	      case 3 : msg_print("*slurp*");              break;
	      }
	      
	      if (randint(3) == 1) {
		msg_print("*Burp*");
		msg_print("('scuse me....)");
	      } /*{Belch}*/
	      break;

	    default:
	      break;
	    } /* end switch i5 */

	    break; /* end case 31 */

	  default:
	    break;
	  } /* end switch i4 */

          if (dam_pts != 0) {
            ident = hp_player(dam_pts,"poisonous food.");
          }

	} /* end for */
	} /* end else not eyeball of drong */
        /*{ End of food actions...                                }*/

	if (ident) {
	  identify(&(item_ptr->data));
	}

	if (item_ptr->data.flags != 0) {
	  //with py.misc do;
	  if (item_ptr->data.level > 0) {
	    PM.exp += (((real)item_ptr->data.level/(real)PM.lev) + .5);
	    prt_experience();
          }
	}

	add_food(item_ptr->data.p1);
	py.flags.status &= ~(IS_HUNGERY | IS_WEAK);
	prt_hunger();
	desc_remain(item_ptr);
	inven_destroy(item_ptr);
	prt_weight();

      } else {
	if (redraw) {
	  draw_cave();
	}
      }
    } else {
      msg_print("You are not carrying any food.");
    }
  } else {
    msg_print("But you are not carrying anything.");
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  eat.c */
