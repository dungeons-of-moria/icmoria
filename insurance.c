/* insurance.c */
/**/

#include "imoria.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bi__display_gold()
{
  vtype     out_val;

  sprintf(out_val,"Gold remaining : %ld",py.misc.money[TOTAL_]);
  prt(out_val,19,18);

};
//////////////////////////////////////////////////////////////////////
void bi__display_commands()
{
  prt("You may:",21,1);
  prt(" i) Insure an item.          a) Insure all items.",22,1);
  prt(" e) Insure all equipment.    p) Insure person.",23,1);
  prt("^R) Redraw the screen.     Esc) Exit from building.",24,1);
};
//////////////////////////////////////////////////////////////////////
void bi__display_store(vtype shop_owner)
{
  clear_screen();
  prt(shop_owner,4,10);
  prt("(Protects character against most system failures.)",7,15);
  bi__display_commands();
  bi__display_gold();
};
//////////////////////////////////////////////////////////////////////
void bi__insure_all_items()
{
  integer    tot_cost,temp;
  treas_ptr  ptr;
  boolean    flag;
  string     out, out2;

  tot_cost = 0;

  for(ptr = inventory_list ; (ptr != nil) ; ptr = ptr->next ) {
    if ((Insured_bit & ptr->data.flags2) == 0) {
      temp = abs(ptr->data.cost * ptr->data.number) / 40;
      if (temp < (MITHRIL_VALUE / 10)) {
	temp = (MITHRIL_VALUE / 10);
      }
      tot_cost += temp;
    }
  }

  if (tot_cost > 0) {

    flag = false;
    sprintf(out,"Do you wish to pay %s?",cost_str(tot_cost,out2));

    if (get_yes_no(out)) {
      if ((py.misc.money[TOTAL_] * GOLD_VALUE) >= tot_cost) {
	subtract_money(tot_cost,true);
	flag = true;
      } else {
	msg_print("Get some more cash, you fool!");
      }
    }

    if (flag) {
      for (ptr = inventory_list ; ptr != nil ; ptr = ptr->next) {
	ptr->data.flags2 |= Insured_bit;
      }
      bi__display_gold();
      msg_print("Your inventory is now insured");
    }
  } else {
    msg_print("You have no inventory that needs to be insured.");
  }
};
//////////////////////////////////////////////////////////////////////
void bi__insure_item(vtype shop_owner)
{
  treas_ptr    ptr;
  integer      count,temp;
  boolean      redraw,flag;
  string       out, out2;
  vtype        out_val;
  char         trash_char;

  count = 0;
  change_all_ok_stats(false,false);
  ptr = inventory_list;

  for ( ; ptr !=  nil ; ptr = ptr->next) {
    if ((ptr->data.flags2 & Insured_bit) == 0) {
      ptr->ok = true;
      count++;
    }
  }

  if (count > 0) {
    if (get_item(&ptr,"Insure which item?",&redraw,count,&trash_char,false,false)) {
      temp = (int)(abs(ptr->data.cost * ptr->data.number) / 40);
      if (temp < (MITHRIL_VALUE div 10)) {
	temp = (MITHRIL_VALUE div 10);
      }
      
      flag = false;
      sprintf(out,"Do you wish to pay %s?",cost_str(temp,out2));
      if (get_yes_no(out)) {
	if ((py.misc.money[TOTAL_] * GOLD_VALUE) >= temp) {
	  subtract_money(temp,true);
	  flag = true;
	} else {
	  msg_print("Why don't you try again when you have more cash?");
	}
      }
      
      if (flag) {
	ptr->data.flags2 |= Insured_bit;
	bi__display_store(shop_owner);
	objdes(out_val,ptr,true);
	if (ptr->data.number > 1) {
	  sprintf(out,"Your %s are now insured", out_val);
	  msg_print(out);
	} else {
	  sprintf(out,"Your %s is now insured", out_val);
	  msg_print(out);
	}
      }
    } else {
      bi__display_store(shop_owner);
    }
  } else {
    msg_print("None of your items need insurance");
  }
};
//////////////////////////////////////////////////////////////////////
real bi__death_adj()
{
  /* Returns the rate to rape the character at for insurance,
     based on the number of times they have been restored in the past.
     Change this after seeing how bad ppl get screwed           -DMF- */

  real   temp;

  //with py.misc do;
  PM.premium = PM.exp;
  if (PM.premium < 100) {
    PM.premium = 100;
  }
  temp = 100 * sqrt(PM.premium) + PM.premium * PM.deaths;
  PM.premium = (int)(temp);

  return temp;
};
//////////////////////////////////////////////////////////////////////
void bi__insure_person()
{
  integer    tot_cost;
  boolean    flag;
  string     out, out2;

  if (py.flags.insured) {
    msg_print("Your person is already insured.");
  } else if (py.misc.deaths > 7) {
    msg_print("You are deemed a security risk.  We will not insure you.");
  } else {
    tot_cost = (int)(bi__death_adj());
    if (tot_cost < (2*MITHRIL_VALUE)) {
      tot_cost = (2*MITHRIL_VALUE);
    }
    
    flag = false;
    sprintf(out,"Do you wish to pay %s?",cost_str(tot_cost,out2));
    if (get_yes_no(out)) {
      if ((py.misc.money[TOTAL_] * GOLD_VALUE) >= tot_cost) {
	subtract_money(tot_cost,true);
	flag = true;
      } else {
	msg_print("Nope, not enough cash with you.");
      }
    }
    
    if (flag) {
      bi__display_gold();
      py.flags.insured = true;
      msg_print("Your person is now insured");
    }
  }
};
//////////////////////////////////////////////////////////////////////
void bi__insure_all_equip()
{
  integer     i1,tot_cost,temp;
  boolean     flag;
  string      out, out2;

  tot_cost = 0;
  for (i1 = Equipment_min; i1 < EQUIP_MAX; i1++) {
    //with equipment[i1]. do;
    if (equipment[i1].tval > 0) {
      if ((Insured_bit & equipment[i1].flags2) == 0) {
	temp = (int)(abs(equipment[i1].cost * equipment[i1].number) / 40);
	if (temp < (MITHRIL_VALUE div 10)) {
	  temp = (MITHRIL_VALUE div 10);
	}
	tot_cost += temp;
      }
    }
  }


  if (tot_cost > 0) {
    flag = false;
    sprintf(out,"Do you wish to pay %s?",cost_str(tot_cost,out2));

    if (get_yes_no(out)) {
      if ((py.misc.money[TOTAL_] * GOLD_VALUE) >= tot_cost) {
	subtract_money(tot_cost,true);
	flag = true;
      } else {
	msg_print("You don't have enough money with you.  Maybe you should make a withdrawal.");
      }
    }

    if (flag) {
      for (i1 = Equipment_min; i1 < EQUIP_MAX; i1++) {
	//with equipment[i1]. do;
	if (equipment[i1].tval > 0) {
	  equipment[i1].flags2 |= Insured_bit;
	}
      }
      bi__display_gold();
      msg_print("Your equipment is now insured");
    }      
  } else {
    msg_print("You have no equipment that needs to be insured.");
  }
};
//////////////////////////////////////////////////////////////////////
void bi__parse_command(boolean *exit_flag, vtype shop_owner)
{
  char     command;

  if (get_com("", &command)) {
    switch (command) {
    case 18  : 
      bi__display_store(shop_owner);
      break;

    case 112 : 
      bi__insure_person();
      break;

    case 97  : 
      if (py.flags.insured) {
	bi__insure_all_items();
      } else {
	prt("Insure your body first.",1,1);
      }
      break;

    case 105 : 
      if (py.flags.insured) {
	bi__insure_item(shop_owner);
      } else {
	prt("Insure your body first.",1,1);
      }
      break;

    case 101 : 
      if (py.flags.insured) {
	bi__insure_all_equip();
      } else {
	prt("Insure your body first.",1,1);
      }
      break;

    default  : 
      prt("Invalid Command.",1,1);   break;
    }
  } else {
    *exit_flag = true;
  }
};
//////////////////////////////////////////////////////////////////////
void buy_insurance()
{
  boolean   exit_flag = false;
  vtype     shop_owner;
  integer   tics = 1;
  integer   starting_money;

  starting_money = PM.money[TOTAL_];

  if (py.misc.max_exp > 30+randint(30)) {

    strcpy(shop_owner,"Mangy Dragon Flye        (Scum)       Insurance");
    bi__display_store(shop_owner);

    do {
      bi__parse_command(&exit_flag, shop_owner);
      adv_time(false);
      tics++;
      check_kickout_time(tics,2);
    } while (!exit_flag);
    
    clear_screen();
    draw_cave();
    if (PM.money[TOTAL_] != starting_money) {
      msg_print("Be sure to save your character for your insurance to begin.");
    }

  } else {
    msg_print("Hmmmm... come back when you are a bit older.");
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/* END FILE  insurance.c */
