/* staffs.c */
/**/

#include "imoria.h"
#include "dungeon.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void us__staff_effect(integer effect, boolean *idented)
{
  integer          i3,randnum;
  integer          y,x;
  boolean          ident;


  ident = *idented;

  /*{ Staffs...                                             }*/

  switch (effect) {

  case 1  :  
    ident = light_area(char_row,char_col);
    break;

  case 2  :  
    ident = detect_sdoor();
    break;

  case 3  :  
    ident = detect_trap();
    break;
    
  case 4  :
    ident = detect_item(c_treasure);
    break;
    
  case 5  :  
    ident = detect_item(c_object);
    break;
    
  case 6  :  
    teleport(100);
    ident = true;
    break;
    
  case 7  :  
    ident = earthquake();
    break;
    
  case 8  :  
    for (randnum=randint(4),i3 = 0; i3 < randnum; i3++) {
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
  
  case 9  : 
    ident = genocide();
    break;
    
  case 10 : 
    ident = destroy_area(char_row,char_col);
    break;
    
  case 11 :  
    msg_print("The end of the staff bursts into a blue shimmering light.");
    ident = starlite(char_row,char_col);
    break;
  
  case 12 :
    ident = zap_area(0,1,c_speed); /*{haste}*/
    break;
    
  case 13 :
    ident = zap_area(0,-1,c_speed); /*{slow}*/
    break;
    
  case 14 :  
    ident = zap_area(0,0,c_sleep);
    break;
    
  case 15 :
    ident = hp_player(randint(8),"a staff.");
    break;
    
  case 16 : 
    ident = detect_creatures(c_invisible);
    break;
    
  case 17 :  
    py.flags.fast += randint(30) + 15;
    ident = true;
  break;
  
  case 18 :
    py.flags.slow += randint(30) + 15;
    ident = true;
    break;
  
  case 19 :  
    ident = mass_poly();
    break;
    
  case 20 :  
    if (remove_curse()) {
      msg_print("The staff glows blue for a moment...");
      ident = true;
    }
  break;
  
  case 21 :  
    ident = detect_creatures(c_evil);
    break;
    
  case 22 :
    //with py.flags do;
    ident  = cure_me(&(PF.blind));
    ident |= cure_me(&(PF.poisoned));
    ident |= cure_me(&(PF.confused));
    break;
  
  case 23 : 
    ident = zap_area(0x0004,60,c_hp);
    break;
    
  case 24 : 
    ident = mass_genocide();
    break;
    
  case 25 :
    ident = unlight_area(char_row,char_col);
    break;
    
  case 26 :  
    ident = ident_spell();
    break;
    
  default:
    break;
    
  }
  /*{ End of staff actions...                               }*/

  *idented = ident;
};
//////////////////////////////////////////////////////////////////////
void use_staff()
{
  /*{ Use a staff...                                        -RAK-   }*/

  unsigned long   i1;
  integer         i3,chance,i4;
  treas_ptr       i2,item_ptr;
  char            trash_char;
  boolean         redraw,ident;
  obj_set         this_be_a_staff = {staff, 0};

  
  reset_flag = true;
  
  if (inven_ctr > 0) {
    if (find_range(this_be_a_staff,false,&i2,&i3)) {
      
      redraw = false;
      if (get_item(&item_ptr,"Use which staff?",
		   &redraw,i3,&trash_char,false,false)) {
	//with item_ptr^.data do;
	
	if (redraw) {
	  draw_cave();
	}
	reset_flag = false;

	//with py.misc do;
	chance = PM.save + PM.lev + spell_adj(INT) - item_ptr->data.level - 5;

	if (py.flags.confused > 0) {
	  chance /= 2;
	}
	if (chance < 0) {
	  chance = 0;
	}
	
	if (randint(chance) < USE_DEVICE) {
	  msg_print("You failed to use the staff properly.");
	} else if (item_ptr->data.p1 > 0) {
	  i1 = item_ptr->data.flags;
	  ident = false;
	  item_ptr->data.p1--;
	  for ( ; i1 > 0 ; ) {
	    i4 = bit_pos(&i1) + 1;
	    
	    
	    us__staff_effect(i4, &ident);	
	  }
	  identify(&(item_ptr->data));
	  if (ident) {
	    if (item_ptr->data.flags != 0) {
	      //with py.misc do;
	      PM.exp += (item_ptr->data.level / (real)PM.lev) + .5;
	      prt_experience();
	    }
	  }
	  desc_charges(item_ptr);
	}
	
      } else {
	if (redraw) {
	  draw_cave();
	}
      }
    } else {
      msg_print("You are not carrying any staffs.");
    }
  } else {
    msg_print("But you are not carrying anything.");
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  staffs.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
