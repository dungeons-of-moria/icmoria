/* traps.c */
/* all sorts of nasty traps (stores too)! */

#include "imoria.h"
#include "dungeon.h"
#include "trade.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_trap(integer y,integer x,integer typ,integer subval)
{
  /*{ Places a particular trap at location y,x		-RAK-	}*/

  integer         cur_pos;
  treasure_type   cur_trap;

  if (typ == 1) {
    cur_trap = trap_lista[subval];
  } else {
    cur_trap = trap_listb[subval];
  }

  popt(&cur_pos);
  cave[y][x].tptr  = cur_pos;
  t_list[cur_pos]  = cur_trap;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void change_trap(integer y,integer x)
{
  /*{ Change a trap from invisible to visible               -RAK-   }*/
  /*{ Note: Secret doors are handled here                           }*/

  integer   i3;
  obj_set   little_things = {Unseen_trap,Secret_door,0};

  //with cave[y,x] do;
  if (is_in(t_list[cave[y][x].tptr].tval, little_things)) {
    i3 = cave[y][x].tptr;
    place_trap(y,x,2,t_list[i3].subval);
    pusht(i3);
    lite_spot(y,x);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_rubble(integer y,integer x)
{
  /*{ Places rubble at location y,x				-RAK-	}*/

  integer   cur_pos;

  popt(&cur_pos);

  cave[y][x].tptr  = cur_pos;
  cave[y][x].fopen = false;

  t_list[cur_pos] = some_rubble;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ht__open_pit(integer dam)
{
  msg_print("You fell into a pit!");
  if (py.flags.ffall) {
    msg_print("You gently float down.");
  } else {
    take_hit(dam,"an open pit");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__arrow(integer dam)
{
  if (test_hit(125,0,0,PM.pac+PM.ptoac)) {
    take_hit(dam,"an arrow trap");
    msg_print("An arrow hits you.");
  } else {
    msg_print("An arrow barely misses you.");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__covered_pit(integer dam, integer y, integer x)
{
  msg_print("You fell into a covered pit.");
  if (py.flags.ffall) {
    msg_print("You gently float down.");
  } else {
    take_hit(dam,"a covered pit");
    place_trap(y,x,2,1);
  }
};
//////////////////////////////////////////////////////////////////////
void ht__trap_door(integer dam)
{
  msg_print("You fell through a trap door!");
  msg_print(" ");
  moria_flag = true;
  dun_level++;
  if (py.flags.ffall) {
    msg_print("You gently float down.");
  } else {
    take_hit(dam,"a trap door");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__sleep_gas(integer dam)
{

  if (py.flags.paralysis == 0) {
    msg_print("A strange white mist surrounds you!");
    if (py.flags.free_act) {
      msg_print("You are unaffected.");
    } else {
      msg_print("You fall asleep.");
      py.flags.paralysis += randint(10) + 4;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void ht__hidden_object(integer dam, integer y, integer x)
{
  cave[y][x].fm = false;
  pusht(cave[y][x].tptr);
  place_object(y,x);
  msg_print("Hmmm, there was something under this rock.");
};
//////////////////////////////////////////////////////////////////////
void ht__str_dart(integer dam)
{
  if (test_hit(125,0,0,PM.pac+PM.ptoac)) {
    if (lose_stat(STR,"","A small dart hits you.")) {
      take_hit(dam,"a dart trap");
      print_stat |= 0x0001;
      msg_print("A small dart weakens you!");
    }
  } else {
    msg_print("A small dart barely misses you.");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__teleport(integer dam)
{
  teleport_flag = true;
  msg_print("You hit a teleport trap!");
};
//////////////////////////////////////////////////////////////////////
void ht__rockfall(integer dam, integer y, integer x)
{
  take_hit(dam,"falling rock");
  pusht(cave[y][x].tptr);
  place_rubble(y,x);
  msg_print("You are hit by falling rock");
};
//////////////////////////////////////////////////////////////////////
void ht__corrode_gas(integer dam)
{
  corrode_gas("corrosion gas.");
  msg_print("A strange red gas surrounds you.");
};
//////////////////////////////////////////////////////////////////////
void ht__summon_monster(integer dam, integer y, integer x)
{
  integer   i1, ty, tx;

  cave[y][x].fm = false;        /*{ Rune disappears...    }*/
  pusht(cave[y][x].tptr);
  cave[y][x].tptr = 0;
  for (i1 = 1; i1 <= (2+randint(3)); i1++) {
    ty = char_row;
    tx = char_col;
    if (is_in(cave[ty][tx].fval, water_set)) {
      summon_water_monster(&ty,&tx,false);
    } else {
      summon_land_monster(&ty,&tx,false);
    }
  }
};
//////////////////////////////////////////////////////////////////////
void ht__fire(integer dam)
{
  fire_dam(dam,"a fire trap.");
  msg_print("You are enveloped in flames!");
};
//////////////////////////////////////////////////////////////////////
void ht__acid(integer dam)
{
  acid_dam(dam,"an acid trap.");
  msg_print("You are splashed with acid!");
};
//////////////////////////////////////////////////////////////////////
void ht__poison_gas(integer dam)
{
  poison_gas(dam,"a poison gas trap.");
  msg_print("A pungent green gas surrounds you!");
};
//////////////////////////////////////////////////////////////////////
void ht__blind_gas(integer dam)
{
  msg_print("A black gas surrounds you!");
  PF.blind += randint(50) + 50;
};
//////////////////////////////////////////////////////////////////////
void ht__confuse_gas(integer dam)
{
  msg_print("A gas of scintillating colors surrounds you!");
  PF.confused += randint(15) + 15;
};
//////////////////////////////////////////////////////////////////////
void ht__slow_dart(integer dam)
{
  if (test_hit(125,0,0,PM.pac+PM.ptoac)) {
    take_hit(dam,"a dart trap");
    msg_print("A small dart hits you!");
    PF.slow += randint(20) + 10;
  } else {
    msg_print("A small dart barely misses you.");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__con_dart(integer dam)
{
  if (test_hit(125,0,0,PM.pac+PM.ptoac)) {
    if (lose_stat(CON,"","A small dart hits you.")) {
      take_hit(dam,"a dart trap");
      print_stat |= 0x0004;
      msg_print("A small dart weakens you!");
    }
  } else {
    msg_print("A small dart barely misses you.");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__secret_door(integer dam)
{

};
//////////////////////////////////////////////////////////////////////
void ht__chute(integer dam)
{
  msg_print("You fell down a chute!");
  msg_print(" ");
  moria_flag = true;
  dun_level += randint(6);
  if (py.flags.ffall) {
    msg_print("You gently slide down.");
  } else {
    take_hit(dam,"chute landing");
  }
};
//////////////////////////////////////////////////////////////////////
void ht__scare_monster(integer dam)
{

};
//////////////////////////////////////////////////////////////////////
void ht__whirlpool(integer dam)
{
  msg_print("You are swept into a whirlpool!");
  msg_print(" ");
  moria_flag = true;
  do {
    dun_level++;
    if (! (py.flags.ffall) ) { /*{XXX...swimming_worn}*/
      msg_print("You are drowning!");
      take_hit(dam,"drowning");
    }
  } while (randint(2) != 1);
};
//////////////////////////////////////////////////////////////////////
void ht__house(integer dam, integer y, integer x)
{
  switch (t_list[cave[y][x].tptr].p1) {
  case 1 : 
    msg_print("The building is empty.");
    if (react(10)==0) {
      msg_print("The building is being guarded!");
      call_guards("Magic Mouth spell");
    }
    break;
    
  case 2 :
    msg_print("There is a Thieves' Guild meeting here.");
    switch (react(6)) {
    case 0: 
      call_guards("Guildmaster"); break;
      
    case 1: case 2: case 3: case 4: case 5: case 6: case 7:
      kicked_out(); break;
      
    case 8: case 9: case 10:
      thief_games(); break;
    }
    break;

  case 3 : 
    msg_print("This is a town brothel.  Some young prostitutes are here.");
    switch (react(10)) {
      case 0 : 
	call_guards("prostitutes"); break;

    case 1: case 2: case 3: case 4: case 5: case 6:
      kicked_out(); break;

    default:
      if (characters_sex() != FEMALE) {
	msg_print("The girls invite you to prove your abilities.");
	battle_game(spell_adj(CHR),"some playful prostitutes");
      } else {
	msg_print("The girls invite you to work with them.");
	brothel_game();
      }
    }
    break;

  case 4 : 
    msg_print("Some drunken fighters are telling tales here.");
    switch (react(8)) {
      case 0: 
	call_guards("group of fighters"); break;

    case 1: case 2: case 3: case 4: case 5: case 6:
      kicked_out(); break;

    default:
      msg_print("They ask you to demonstrate your fighting skill.");
      battle_game(py.misc.ptohit,"some drunken fighters");
      break;
    }
    break;

  case 5 : 
    msg_print("There is a party in progress here.");
    switch (react(8)) {
    case 0:
      call_guards("party's host"); break;

    case 1: case 2: case 3: case 4: case 5:
      kicked_out(); break;

    default: party();
    }
    break;

  case 6 : 
    switch (randint(2)) {
    case 1: msg_print("The building is a poorhouse."); break;
    case 2: msg_print("This is an orphanage.");        break;
    }

    switch (react(12)) {
      case 0: call_guards("beggars"); break;

    case 1: case 2: case 3: case 4:
      kicked_out(); break;

    default: 
      switch (randint(2)) {
      case 1 : /*beg_food();  break;*/   /* beg_food is unfinished XXXX */
      case 2 : beg_money(); break;
      }
    }
    break;

  case 7: case 8 :
    switch (randint(3)) {
    case 1 : msg_print("This is the home of a peasant family.");       break;
    case 2 : msg_print("These are the quarters of a humble laborer."); break;
    case 3 : msg_print(" This is the home of several poor families."); break;
    }

    switch (react(8)) {
    case 0: call_guards("peasant(s)"); break;

    case 1: case 2: case 3:
      kicked_out(); break;

    case 4: case 5: case 6: case 7:
      invite_for_meal(); break;

    case 8: case 9: case 10:
      spend_the_night("peasant(s)"); break;
    }
    break;

  case 9 : 
    switch (randint(3)) {
    case 1: case 2: msg_print("This is the home of a merchant."); break;
    case 3: msg_print("This is the house of an accomplished craftsman.");break;
    }

    switch (react(5)) {
    case 0: call_guards("owner"); break;

    case 1: case 2: case 3: case 4:
      kicked_out(); break;

    case 5: case 6: case 7: case 8: case 9:
      invite_for_meal(); break;

    case 10:
      spend_the_night("gentleman"); break;
    }
    break;

  case 10 : 
    msg_print("There is a religious service in progress here.");
    switch (react(8)) {
    case 0: call_guards("High Priest"); break;

    case 1: case 2: case 3: case 4: case 5:
      kicked_out(); break;

    default:
      worship(); break;
    }
    break;

  case 11 : 
    switch (randint(3)) {
    case 1:msg_print("This is the house of a wealthy shopkeeper."); break;
    case 2:msg_print("This is the mansion of a affluent noble.");   break;
    case 3:msg_print("This is the estate of an rich guildsman.");   break;
    }

    switch (react(2)) {
    case 0: call_guards("master of the house"); break;

    case 1: case 2: case 3:
      kicked_out(); break;

    case 4: case 5: case 6: case 7: case 8: case 9:
      invite_for_meal(); break;
      
    case 10:
      spend_the_night("master of the house"); break;
    }
    break;

  case 12 : 
    msg_print("This is the home of a powerful mage.");
    switch (react(5)) {
    case 0: call_wizards(); break;

    case 1: case 2: case 3:
	call_guards("mage"); break;

    case 4: case 5: case 6: case 7: case 8: case 9:
      kicked_out(); break;

    case 10:
      invite_for_meal(); break;
    }

  }

  t_list[cave[y][x].tptr].p1 = 1;

  prt_time();
  prt_stat_block();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void hit_trap(integer *y,integer *x)
{
  /*{ Player hit a trap...  (Chuckle)                       -RAK-   }*/

  integer   dam;

  change_trap(*y,*x);
  lite_spot(char_row,char_col);
  find_flag = false;
  
  //with cave[*y][*x]. do;
  //with py.misc do;

  dam = damroll(t_list[cave[*y][*x].tptr].damage);

  switch (t_list[cave[*y][*x].tptr].subval) {
    
  case 1 :  ht__open_pit(dam);            break;
    
  case 2 :  ht__arrow(dam);               break;
    
  case 3 :  ht__covered_pit(dam,*y,*x);   break;
    
  case 4 :  ht__trap_door(dam);           break;
    
  case 5 :  ht__sleep_gas(dam);           break;
    
  case 6 :  ht__hidden_object(dam,*y,*x); break;
    
  case 7 :  ht__str_dart(dam);            break;
    
  case 8 :  ht__teleport(dam);            break;
    
  case 9 :  ht__rockfall(dam,*y,*x);      break;
    
  case 10:  ht__corrode_gas(dam);         break;
    
  case 11:  ht__summon_monster(dam,*y,*x);break;
    
  case 12:  ht__fire(dam);                break;
    
  case 13:  ht__acid(dam);                break;
    
  case 14:  ht__poison_gas(dam);          break;
    
  case 15:  ht__blind_gas(dam);           break;
    
  case 16:  ht__confuse_gas(dam);         break;
    
  case 17:  ht__slow_dart(dam);           break;
    
  case 18:  ht__con_dart(dam);            break;
    
  case 19:  ht__secret_door(dam);         break;
    
  case 20:  ht__chute(dam);               break;
    
  case 99:  ht__scare_monster(dam);       break;
    
    /*{ Town level traps are special, the stores...   }*/
  case 101:  /* { General    } */
    check_store_hours_and_enter(S_GENERAL,S_GENERAL,S_GENERAL);   break;
    
  case 102:  /* { Armory     } */
    check_store_hours_and_enter(S_ARMORY,S_ARMORY,S_ARMORY);   break;
    
  case 103:  /* { Weaponsmith} */
    check_store_hours_and_enter(S_WEAPONS,S_WEAPONS,S_WEAPONS);   break;
    
  case 104:  /* { Temple     } */
    check_store_hours_and_enter(S_TEMPLE,S_TEMPLE,S_TEMPLE);   break;
    
  case 105:  /* { Alchemy    } */
    check_store_hours_and_enter(S_ALCHEMY,S_ALCHEMY,S_ALCHEMY);   break;
    
  case 106:  /* { Magic-User } */
    check_store_hours_and_enter(S_MAGIC,S_MAGIC,S_MAGIC);   break;
    
  case 107:  /* { Inn        } */
    check_store_hours_and_enter(S_INN,S_INN,S_INN);   break;
    
  case 108:  /* { Trade Post } */
    if (check_store_hours(S_TRADE_POST,-1))  {
      enter_trading_post();
    }
    break;
    
  case 109:  /* { Library    } */
    check_store_hours_and_enter(S_LIBRARY,S_LIBRARY,S_LIBRARY);   break;
    
  case 110:  /* { Music Shop } */
    check_store_hours_and_enter(S_MUSIC,S_MUSIC,S_MUSIC);  break;
    
  case 111:  /* { Insurance  } */
    if (check_store_hours(S_INSURANCE,-1)) {
      //msg_print("Moved...to the bank.");
      buy_insurance();
    }
    break;
    
  case 112:  /* { Bank       } */
    if (check_store_hours(S_BANK,-1)) {
      enter_bank();
    }
    break;
    
  case 113:  /* { Gem Shop   } */
    check_store_hours_and_enter(S_GEM,S_GEM,S_GEM); break;
    
  case 114:  /* { $ Changer  } */
    if (check_store_hours(S_CHANGER,-1)) {
      msg_print("Oh, just go to the bloody bank!");
    }
    break;
    
  case 115:  /* { Casino     } */
    if (check_store_hours(S_CASINO,-1)) {
      enter_casino();
    }
    break;
    
  case 116:  /* { Deli       } */
    check_store_hours_and_enter(S_DELI,S_DELI,S_DELI); break;
    
  case 117: /* { Fortress   } */
    enter_fortress();                      break;
    
  case 118:  /* { Black Market } */
    check_store_hours_and_enter(S_BLACK_MARKET,S_BLACK_MARKET,S_BLACK_MARKET);
    break;

  case 120:
  case 121:
  case 122:  /* { House      } */
    ht__house(dam,*y,*x);                  break;

  case 123: ht__whirlpool(dam);            break;

  default:
    msg_print("You got lucky: unknown trap value.");
    break;
  }
  
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
