/* quest.c */
/* code for the quests.  quests are given out at the fortress */

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
void q__change_money()
{
  int amount;

  amount = abs(py.misc.money[TOTAL_] - gld)*GOLD_VALUE;
  if (gld > py.misc.money[TOTAL_]) {
    add_money(amount);
  } else {
    subtract_money(amount,true);
  }
};
*/

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
void q__display_gold()
{
  vtype out_val;
 
  sprintf(out_val, "gold remaining : %ld",gld);
  prt( out_val, 19, 22)

};
*/

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void q__reward_money(integer reward)
{
  vtype out_val;

  msg_print("I've sent your reward with a page to the bank.");
  sprintf(out_val, "He deposited %ld gold pieces under your name.", reward);
  msg_print(out_val);
  py.misc.account += reward;
};
//////////////////////////////////////////////////////////////////////
void q__reward_item(integer target)
{
  // alloc several items, plus the money;

#define QUEST_ITEMS   9
#define QUEST_ITEM_FREQUENCY  10

  integer         i1, i2, i3;
  treasure_type   rewards[QUEST_ITEMS];
  char            command;
  vtype           reward_names[QUEST_ITEMS], out_val;
  boolean         flag;

  integer         q1 = 15;                            /* allocs to attempt */
  integer         q2 = PLACE_OBJECT_TRIES;            /* const for alloc  */
  float           q3 = ( 0.8 * target * GOLD_VALUE ); /* value to stop at */
  float           q4 = ( 1.2 * target * GOLD_VALUE ); /* value to stop at */

  obj_set   wand_set     = {staff, wand, 0};
  obj_set   weapon_set   = {bow_crossbow_or_sling, hafted_weapon, pole_arm,
			    dagger, sword, maul, 0};
  obj_set   misc1_set    = {bracers, belt, amulet, ring, 0};
  obj_set   misc2_set    = {valuable_gems, bag_or_sack, valuable_gems_wear,
			    chime, horn, 0};
  obj_set   ammo_set     = {bolt, arrow, 0};
  obj_set   armor1_set   = {gem_helm, boots, gloves_and_gauntlets, cloak, 0};
  obj_set   armor2_set   = {helm, shield, soft_armor, 0};
  obj_set   armor3_set   = {hard_armor, 0};
       
  obj_set  *toys[9];

  toys[0] = &wand_set;
  toys[1] = &weapon_set;
  toys[2] = &weapon_set;
  toys[3] = &misc1_set;
  toys[4] = &misc2_set;
  toys[5] = &ammo_set;
  toys[6] = &armor1_set;
  toys[7] = &armor2_set;
  toys[8] = &armor3_set;


  popt(&i1);

  for (i2=0; i2<QUEST_ITEMS; i2++) {
    rewards[i2].cost = -1;

    for (i3=0 ; (i3<q1) && (rewards[i2].cost < q3) ; ) {

      t_list[i1] = object_list[get_obj_num(PM.max_lev+10,q2)];
      if (is_in(t_list[i1].tval,*toys[i2])) {
	magic_treasure(i1,PM.max_lev + 10, true);

	if (rewards[i2].cost < t_list[i1].cost) {
	  if ( t_list[i1].cost < q4 ) {
	    rewards[i2] = t_list[i1];
	  }
	}
	i3++;
      }
    }

    unquote(rewards[i2].name);
    known1(rewards[i2].name);
    known2(rewards[i2].name);
    inven_temp->data = rewards[i2];
    objdes(reward_names[i2],inven_temp,true);
  }

  pusht(i1);

  clear_screen();

  for (i2 = 0; i2 < QUEST_ITEMS; i2++) {
    sprintf(out_val, "%c)     %s",(char)(i2+97),reward_names[i2]);
    prt(out_val ,3+i2,10);
  }
  sprintf(out_val, "%c)     a check for %ld gold pieces",(char)(i2+97),target);
  prt(out_val ,3+i2,10);

  for (flag = false ; !flag ;) {
    get_com( "Pick an item for your reward: ", &command);
    if ((command >= 'a') && ((command - 'a') <= QUEST_ITEMS)) {

      command -= 'a';
      
      if (command == QUEST_ITEMS) {
	q__reward_money(target);
	flag = true;
      } else {
	inven_temp->data = rewards[(int)command];
	if (inven_check_num()) {       /*{ Too many objects?     }*/
	  if (inven_check_weight()) {  /*{ Weight limit check    }*/
	    inven_carry();
	    flag = true;
	  } else {
	    msg_print("You can't carry that much weight.");
	  }
	} else {
	  msg_print("You can't carry that many items.");
	}
      }
    }
  } /* end for */
  clear_rc(3,1);
};
//////////////////////////////////////////////////////////////////////

void q__reward_quest()
{
  integer    reward;
  vtype      out_val;
  boolean    redraw = false;

  reward = c_list[py.misc.cur_quest].mexp * (randint(3)+5) +
            py.misc.lev * (randint(2)* 100) +
            (randint(100) + py.stat.c[CHR]) * 2 +
            py.stat.c[INT] * randint(50) + 200;

  sprintf(out_val, "Ah... %s, I was expecting you.", py.misc.name);
  msg_print(out_val);
  sprintf(out_val, "I see you've killed the %s.  That's good.",
	  c_list[py.misc.cur_quest].name);
  msg_print(out_val);

  if (((py.misc.quests % QUEST_ITEM_FREQUENCY) == 0)){
    q__reward_item(reward);
    redraw = true;
  } else {
    q__reward_money(reward);
  }

  py.misc.rep += randint(5) + 2;
  if (py.misc.rep > 50) {
    py.misc.rep = 50;
  }
  py.misc.cur_quest = 0;
  py.flags.quested  = false;
  turn_counter = QUEST_DELAY;
  prt_quested();

  if (redraw) {
    msg_flag = false;
    draw_cave();
  }

  msg_print("Have a good day.  Perhaps you should rest a night at the inn.");
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

integer q__select_quest()
{
  integer    count = 0;
  boolean    exit_flag = false;
  integer    tmp_select = 0;

  do {
    count++;

    if (c_list[count].level > py.misc.lev) {
      exit_flag = true;
      do {
	tmp_select = count + randint(80);
	if (tmp_select > MAX_CREATURES) {
	  tmp_select = MAX_CREATURES-1;
	}
      } while ((! ((c_list[tmp_select].cmove & 0x00008000) == 0)) &&
	       tmp_select < MAX_CREATURES-1) ; 
    }

    if (count == MAX_CREATURES) {
      tmp_select = MAX_CREATURES - 1;
      exit_flag = true;
    }
  } while  (!exit_flag);

  return tmp_select;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


void q__draw_fortress (boolean enter_flag)
{
  vtype    shop_owner, out_val;
  integer  count,count2,count3;
  boolean  exit_flag;
  
  if (!enter_flag) {
    for (count = 1; count <= NUM_QUESTS; count++) {
      count3 = 0;
      exit_flag = true;
      do {
	count3++;
	quest[count] = q__select_quest();
	for (count2 = 1; count2 <= count-1; count2++) {
          if (quest[count] == quest[count2]) {
	    exit_flag = false;
	  }
	}
	if (count3 > 100) {
	  exit_flag = true;
	}
      } while (!exit_flag);
    }
  }
  
  clear_screen();
  strcpy(shop_owner,"Leckin           (Arch-Mage)            Quests");
  prt(shop_owner, 4, 10);
  for (count = 1; count <= NUM_QUESTS; count++) {
    sprintf(out_val, "%c)     %s",(char)(count+96),c_list[quest[count]].name);
    prt(out_val ,5+count,20);
  }
  /*{                      q__display_gold;}*/
  prt("You may:",21,1);
  prt(" p) Pick a quest.                 i) Info on a quest.",22,2);
  prt("^R) Redraw the screen.          Esc) Exit from building.",23,2);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean q__completed_quest()
{
  boolean  return_value = false;

  if ((! py.flags.quested) && (py.misc.cur_quest != 0)) {
    py.flags.quested = false;     /* { not under quest          } */
    py.misc.quests++;             /* { one more is now complete } */
    return_value = true;
  }

  return return_value;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean q__evaluate_char()
{
  boolean return_value;

  if ((py.flags.quested) || (py.misc.lev > py.misc.quests)) {
    return_value = true;
  } else {
    return_value = false;
  }

  return return_value;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void q__reject_char()
{
  msg_print("A guard meets you at the entrance and says:");
  switch (randint(4)) {
  case 1 : msg_print("'M'lord, the Arch-Mage does not wish to be disturbed.'");
  case 2 : msg_print("'My master has other business at the present time.'");
  case 3 : msg_print("'Piss off you inexperienced peon.'");
  case 4 : msg_print("'You have yet to prove yourself worthy.'");
  }
  msg_print("The guard escorts you back outside and locks the door.");
  msg_print("");
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

boolean q__new_victim()
{
  return ((py.misc.cur_quest == 0) && 
	  (py.misc.quests == 0) && 
	  (!py.flags.quested));
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void q__explain_quests()
{
  char   in_char;
        
  clear_screen();

prt("Home of Leckin the Arch-Mage",2,26);
prt("Greetings, adventurer, and welcome to my humble quarters.",6,10);
prt("I see that you have come, like many before you, in an effort to",8,10);
prt("defeat the great evil that lies deep within the darkest bowels of",9,10);
prt("the caves of Moria.  As you well know, none have yet succeeded.",10,10);
prt("You, however . . . I feel that you are going to be different than",12,10);
prt("the rest.  You have it within you to defeat the accursed Balrog,",13,10);
prt("and restore peace and happiness to the people of this fair town.",14,10);
prt("But you cannot do it alone.  You will need someone to guide you.",15,10);
prt("I am willing to be that person, if you will have me.  I will aid",17,10);
prt("you in your quest to defeat the Balrog.  But in return, you must",18,10);
prt("complete many other, simpler quests.  These will strengthen you",19,10);
prt("and prepare you for your final conflict, many moons hence.",20,10);
prt("[Hit space to continue]",24,28);

in_char = inkey();
clear_screen();
prt("Home of Leckin the Arch-Mage",2,26);
prt("I will give you a cash reward for the completion of each quest,",6,10);
prt("to provide incentive for you.  In addition, I may occasionally",7,10);
prt("give you an item that will aid you in your overall quest.",8,10);
prt("If you agree to these conditions, then select one of the following",9,10);
prt("quests.  Either way, I wish you the best of luck.",10,10);
prt("[Hit space to continue]",24,28);
in_char = inkey();

};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void q__repeat_quest()
{
  vtype    out_val;

  msg_print("Hmmm. . .  I see you haven't completed your quest.");
  msg_print("Have you forgotten it already?");
  sprintf(out_val, "Go kill a %s!", c_list[py.misc.cur_quest].name);
  msg_print(out_val);
  msg_print("");
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void q__parse_command(boolean enter_flag)
{
  char     command;
  boolean  exit_flag = false;

  do {
    if (get_com( "", &command)) {
      switch (command) {
      case 97: case 98: case 99: case 100: case 101:  /*{a,b,c,d,e}*/
	if ((turn_counter < QUEST_DELAY) && (!wizard1)) {
	  msg_print("You were just in here... Come back later.");
	  msg_print(" ");
	} else {
	  if ((turn_counter < QUEST_DELAY) && wizard1) {
	    
	    msg_print("Being a Wizard you choose a quest regardless of the turn_counter.");
	    msg_print(" ");
	  }
	  py.misc.cur_quest = quest[command-96];
	  py.flags.quested  = true;
	  exit_flag         = true;
       }

	break;

      case 112 : /* p */
	msg_print("Which quest would you like? [a-e]  ");
	break;

      case 105 :  /* i */
	msg_print("Kill 'em, of course!");
	break;

      case 18  :  /* ^R */
	q__draw_fortress(enter_flag);
	break;

//      case 26  :  /* ^Z */
//	exit_flag = true;
//	break;

      default: 
	prt("Invalid Command.",1,1);
	break;
      }
    } else {
      exit_flag = true;
    }
  } while (!exit_flag);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void enter_fortress()
{
  boolean   complete_flag  = false;
  boolean   enter_flag     = false;

  seed = get_seed();
  /*{gld = py.misc.money[TOTAL_];}*/
  msg_line = 1;
  if (py.misc.quests <= MAX_QUESTS) {
    if (q__evaluate_char()) {
      if (q__completed_quest()) {
	q__reward_quest();
	complete_flag = true;
      } else {
	if (q__new_victim()) {
	  q__explain_quests();
	}
	if ((q__new_victim()) || (py.misc.cur_quest < 1)) {
	       q__draw_fortress(enter_flag);
	       enter_flag = true;
	       q__parse_command(enter_flag);
  	       clear_rc(1,1);
  	       draw_cave();
	} else {
	  q__repeat_quest();
	}
      }
    } else {
      q__reject_char();
      complete_flag = true;
    }

    if ((! complete_flag) && (turn_counter > QUEST_DELAY)) {
      turn_counter = 0;
    }
  }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  quest.c */
