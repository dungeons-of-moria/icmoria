/* store.c */
/**/

#include "imoria.h"
#include "dungeon.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void store_init()
{
//	{ Initializes the stores with owners			-RAK-	}
  int  i1, i2, i3;

  ENTER("store_init","s");

  i1 = MAX_OWNERS / MAX_STORES;
  for (i2 = 0; i2 < MAX_STORES; i2++) {
    stores[i2].owner            = MAX_STORES*(randint(i1)-1) + i2;
    stores[i2].insult_cur       = 0;
    stores[i2].store_open.year  = 0;
    stores[i2].store_open.month = 0;
    stores[i2].store_open.day   = 0;
    stores[i2].store_open.hour  = 0;
    stores[i2].store_open.secs  = 0;
    stores[i2].store_ctr        = 0;
    for (i3=0; i3 <= STORE_INVEN_MAX; i3++) {
      stores[i2].store_inven[i3].sitem = blank_treasure;
      stores[i2].store_inven[i3].scost = 0;
    } /* end for i3 */
  } /* end for i2 */

//  for (i2 = 0; i2 < max_stores; i2++) {
//    printf("store owner[%ld] = %s\n", i2, owners[stores[i2].owner].owner_name);
//  }

   LEAVE("store_init","s");
} /* end store_init */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void bank_init()
{
//	{ Initialize the bank					-DMF-	}
  integer starting;

  starting        = (randint(2000) + 1000) * 1000;
  bank[IRON]      = starting div 8;
  bank[COPPER]    = starting div 30;
  bank[SILVER]    = starting div 50;
  bank[GOLD]      = starting div 250;
  bank[PLATINUM]  = starting div 5000;
  bank[MITHRIL]   = starting div 100000;
  bank[TOTAL_]    = (bank[MITHRIL] * coin_value[MITHRIL] + bank[PLATINUM] *
		     coin_value[PLATINUM]) div GOLD_VALUE + bank[GOLD];

//  for(starting = TOTAL_; starting <= MITHRIL; starting++) {
//    printf("bank[%ld] = %ld\n", starting, bank[starting]);
//  }

}; /* end bank_init */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sm__rndcash(integer *amt, integer target)
{
  *amt = (199*(*amt)+randint(2*target)) div 200;
}

//////////////////////////////////////////////////////////////////////
void store_maint()
{
  /*{ Initialize and up-keep the store's inventory.		-RAK-	}*/

  integer   i1,i2,cnt,store_ctr;

  /*printf ("\n         enter store_maint\n");  fflush(stdout);*/

  for ( ; stores[S_BLACK_MARKET].store_ctr > 0 ; ) {
    store_destroy(S_BLACK_MARKET,stores[S_BLACK_MARKET].store_ctr,false);
  }

  for (i1 = 0; i1 < MAX_STORES; i1++) {
    stores[i1].insult_cur = 0;
    store_ctr = stores[i1].store_ctr;
    if (store_ctr > STORE_MAX_INVEN) {
      /* too much stuff */
      for (i2=0 , cnt=(store_ctr-STORE_MAX_INVEN+2); i2 < cnt; i2++) {
	store_destroy(i1,randint(stores[i1].store_ctr),false);
      }
    } else if (store_ctr < STORE_MIN_INVEN) {
      /* make new stuff */
      for (i2=0 , cnt=(STORE_MIN_INVEN-store_ctr+2); i2 < cnt; i2++) {
	store_create(i1);
      }
    } else {
      /* make some, ruin some */
      for (i2=0 , cnt=(1+randint(STORE_TURN_AROUND)); i2 < cnt; i2++) {
	store_destroy(i1,randint(stores[i1].store_ctr),true);
      }
      for (i2=0 , cnt=(1+randint(STORE_TURN_AROUND)); i2 < cnt; i2++) {
	store_create(i1);
      }
    }	     	     

    store_ctr = stores[i1].store_ctr;
    if (i1 == S_INN) {
      if (randint(8) == 1) {
	for (i2 = 0; i2 < store_ctr; i2++) {
	  store_destroy(i1,i2,false);
	}
	for (i2 = 0; i2 < STORE_MIN_INVEN+2; i2++) {
	  store_create(i1);
	}
      }
    }

  } /* end for */
    
  sm__rndcash(&(bank[IRON]),500000);
  sm__rndcash(&(bank[COPPER]),200000);
  sm__rndcash(&(bank[SILVER]),100000);
  sm__rndcash(&(bank[GOLD]),50000);
  sm__rndcash(&(bank[PLATINUM]),5000);
  sm__rndcash(&(bank[MITHRIL]),1000);
  bank[TOTAL_] = (bank[MITHRIL] * MITHRIL_VALUE + bank[PLATINUM] *
		  PLATINUM_VALUE) div GOLD_VALUE + bank[GOLD];

//  printf ("\n         exit store_maint\n");  fflush(stdout);
}; 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void store_create(integer store_num)
{
  /*{ Creates an item and inserts it into store's inven	-RAK-	}*/

  integer  i1,tries,cur_pos,dummy;

//  printf ("\n           enter  store_create (%ld)",store_num); fflush(stdout);

  tries = 0;
  popt(&cur_pos);
  //with stores[store_num] do;
  do {
    if (store_num != S_BLACK_MARKET) {
      i1 = store_choice[store_num][randint(STORE_CHOICES)-1];
      t_list[cur_pos] = inventory_init[i1];
      magic_treasure(cur_pos,obj_town_level,false);
    } else {
      /* black market item */
      do {
	t_list[cur_pos] = object_list[get_obj_num(obj_town_level*2+
						  randint(obj_town_level*15),
						  PLACE_OBJECT_TRIES*2)];
	magic_treasure(cur_pos,
		       obj_town_level*2 + randint(obj_town_level*15),true);

      } while (t_list[cur_pos].cost < 500 * GOLD_VALUE);
      t_list[cur_pos].cost *= 2;
      t_list[cur_pos].flags2 |= Blackmarket_bit;
    }
    inven_temp->data = t_list[cur_pos];
    if (store_check_num(store_num)) {
      //with t_list[cur_pos] do;
      if (t_list[cur_pos].cost > 0) {	/*{ Item must be good	}*/
	if (t_list[cur_pos].cost < 
	    (owners[stores[store_num].owner].max_cost * GOLD_VALUE)) {
	  store_carry(store_num,&dummy);
	  tries = 10;
	}
      }
    }
    tries++;
  } while(!(tries > 3));
  pusht(cur_pos);
//  printf ("\n           exit  store_create");  fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void store_destroy(integer store_num,integer item_val,boolean one_of)
{
  /*{ Destroy an item in the stores inventory.  Note that if	}*/
  /*{ 'one_of' is false, an entire slot is destroyed	-RAK-	}*/

  integer   i2, store_ctr;

  //with stores[store_num] do;
  inven_temp->data = stores[store_num].store_inven[item_val].sitem;
  //with stores[store_num].store_inven[item_val].sitem do;
  if ((stores[store_num].store_inven[item_val].sitem.number > 1) && 
      (stores[store_num].store_inven[item_val].sitem.subval < 512) &&
      (one_of)) {
    inven_temp->data.number = 1;
    stores[store_num].store_inven[item_val].sitem.number--;
  } else {
    store_ctr = stores[store_num].store_ctr;
    for (i2 = item_val; i2 < store_ctr; i2++) {
      stores[store_num].store_inven[i2] = stores[store_num].store_inven[i2+1];
    }
    stores[store_num].store_inven[store_ctr].sitem = blank_treasure;
    stores[store_num].store_inven[store_ctr].scost = 0;
    stores[store_num].store_ctr--;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void sc__insert(integer store_num,integer pos,integer icost)
{
  /*{ Insert INVEN_MAX at given location	}*/

  integer  i1;

  //with stores[store_num] do;
  for (i1 = stores[store_num].store_ctr; i1 >= pos; i1--) {
    stores[store_num].store_inven[i1+1] = stores[store_num].store_inven[i1];
  }

  stores[store_num].store_inven[pos].sitem = inven_temp->data;
  stores[store_num].store_inven[pos].scost = -icost * GOLD_VALUE;
  stores[store_num].store_ctr++;
};
//////////////////////////////////////////////////////////////////////
void store_carry(integer store_num, integer *ipos)
{
  /*{ Add the item in INVEN_MAX to stores inventory.	-RAK-	}*/

  integer   item_num,item_val;
  integer   typ,subt,icost,dummy;
  boolean   flag;

  /*{ Store_carry routine			}*/

  *ipos = 0;
  identify(&(inven_temp->data));
  unquote(inven_temp->data.name);
  known1(inven_temp->data.name);
  known2(inven_temp->data.name);
  sell_price(store_num,&icost,&dummy,&(inven_temp->data));

  if (icost > 0) {
    //with inven_temp->data do;
    //with stores[store_num] do;
    item_val = 0;
    item_num = inven_temp->data.number;
    typ  = inven_temp->data.tval;
    subt = inven_temp->data.subval;
    flag = false;

    do {
      item_val++;
      //with store_inven[item_val].sitem do;
      if (typ == stores[store_num].store_inven[item_val].sitem.tval) {
	if (subt == stores[store_num].store_inven[item_val].sitem.subval) {
	  /*{ Adds to other item	}*/
	  if (subt > 255) {
	    if (stores[store_num].store_inven[item_val].sitem.number < 24) {
	      stores[store_num].store_inven[item_val].sitem.number += item_num;
	    }
	    flag = true;
	  }
	}
      } else if (typ > stores[store_num].store_inven[item_val].sitem.tval) {
	/*{ Insert into list		}*/
	sc__insert(store_num,item_val,icost);
	flag = true;
	*ipos = item_val;
      }
      /* end of thrid with */
    } while (!((item_val >= stores[store_num].store_ctr) || (flag)));
    if (!(flag)) {  /*{ Becomes last item in list	}*/
      sc__insert(store_num,stores[store_num].store_ctr+1,icost);
      *ipos = stores[store_num].store_ctr;
    }
  } /* end if icost */
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean store_check_num(integer store_num)
{
  /*{ Check to see if he will be carrying too many objects	-RAK-	}*/

  integer   i1;
  boolean   return_value = false;

  //with stores[store_num] do;
  if (stores[store_num].store_ctr < STORE_INVEN_MAX) {
    return_value = true;
  } else if ((inven_temp->data.subval > 255) &&
	     (inven_temp->data.subval < 512)) {
    for (i1 = 0; i1 < stores[store_num].store_ctr; i1++) {
      //with store_inven[i1].store_inven[i1].sitem do;
      if (stores[store_num].store_inven[i1].sitem.tval == 
	  inven_temp->data.tval) {
	if (stores[store_num].store_inven[i1].sitem.subval == 
	    inven_temp->data.subval) {
	  return_value = true;
	}
      }
    } /* end for */
  }
  return return_value;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer sell_price (integer snum,integer *max_sell,integer *min_sell,
		    treasure_type *item)
{
  /*{ Asking price for an item				-RAK-	}*/

  integer   i1;
  integer   return_value = 0;

  //with stores[snum] do;
  i1 = item_value(item);

  if (item->cost > 0) {
    i1 += trunc(i1*rgold_adj[owners[stores[snum].owner].owner_race][py.misc.prace]);
    if (i1 < 1) {
      i1 = 1;
    }
    *max_sell = trunc(i1*(1+owners[stores[snum].owner].max_inflate));
    *min_sell = trunc(i1*(1+owners[stores[snum].owner].min_inflate));
    if (*min_sell > *max_sell) {
      *min_sell = *max_sell;
    }
    return_value = i1;
  } else { /*{quack}*/
    *max_sell     = 0;
    *min_sell     = 0;
    return_value  = 0;
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer iv__search_list(integer x1, integer x2)
{
  integer   i1,i2;

  i1 = 0;
  i2 = 0;

  do {
    i1++;
    if ((object_list[i1].tval == x1) && (object_list[i1].subval == x2)) {
		i2 = object_list[i1].cost;
    }
  } while (!((i1 == MAX_OBJECTS) || (i2 > 0)));

  return (i2 div GOLD_VALUE);
};
//////////////////////////////////////////////////////////////////////
integer item_value(treasure_type *item)
{
  /*{ Returns the value for any given object		-RAK-	}*/
  integer   return_value;

  //with item do;
  return_value = item->cost div GOLD_VALUE;

  switch (item->tval) {
    
    /*{ Weapons and armor	}*/

  case bow_crossbow_or_sling:
  case hafted_weapon:
  case pole_arm:
  case sword:
  case dagger:
  case maul:
  case boots:
  case gloves_and_gauntlets:
  case gem_helm:
  case Cloak:
  case helm:
  case shield:
  case hard_armor:
  case soft_armor:

    if (strstr(item->name,"^") != NULL) {
      return_value = iv__search_list(item->tval,item->subval)*item->number;
    } else {
      switch (item->tval) {
      case bow_crossbow_or_sling:
      case hafted_weapon:
      case pole_arm:
      case sword:
      case dagger:
      case maul:
	if (item->tohit < 0) {
	  return_value = 0;
	} else if (item->todam < 0) {
	  return_value = 0;
	} else if (item->toac < 0) {
	  return_value = 0;
	} else {
	  return_value = ((item->cost div GOLD_VALUE +
			  (item->tohit+item->todam+item->toac)*100) *
			  item->number);
	}
	break;

      default:
	if (item->toac < 0) {
	  return_value = 0;
	} else {
	  return_value = ((item->cost div GOLD_VALUE +item->toac*100) *
			  item->number);
	}
	break;
      } /* end switch */
    }
    break;

    /*{ Ammo }*/

  case sling_ammo:
  case bolt:
  case arrow:
  case spike:

    if (strstr(item->name,"^") != NULL) {
      return_value = iv__search_list(item->tval,1)*(item->number);
    } else {
      if (item->tohit < 0) {
	return_value = 0;
      } else if (item->todam < 0) {
	return_value = 0;
      } else if (item->toac < 0) {
	return_value = 0;
      } else {
	return_value = ((item->cost div GOLD_VALUE +
			 (item->tohit+item->todam+item->toac)*10)*
			(item->number));
      }
    }
    break;
    
    /*{ Potions, Scrolls, and Food	}*/

  case scroll1:
  case scroll2:
  case potion1:
  case potion2:
  case Food:

    if (strstr(item->name,"|") != NULL) {
      switch (item->tval) {
      case scroll1:
      case scroll2:
	return_value =  20;  break;

      case potion1:
      case potion2:
	return_value =  20;  break;

      case Food:
	return_value =   1;  break;
      }
    }
    break;

    /*{ Rings and amulets	}*/

  case amulet:
  case ring:

    if (strstr(item->name,"|") != NULL) {
      switch (item->tval) {
      case amulet:
	return_value =   45;  break;
      case ring:
	return_value =   45;  break;
      }
    } else if (strstr(item->name,"^") != NULL) {
      return_value = (item->cost > 0) ? (item->cost div GOLD_VALUE) : 0;
    }
    break;

    /*{ Horns and Chimes	}*/

  case chime:
  case horn:

    if (strstr(item->name,"|") != NULL) {
      switch (item->tval) {
      case chime:
	return_value =   50;  break;
      case horn:
	return_value =   80;  break;
      }
    } else if (strstr(item->name,"^") != NULL) {
      return_value = ((item->cost div GOLD_VALUE) + 
		      trunc(item->cost/COST_ADJ/20.0)*(item->p1));
    }
    break;

    /* { Wands rods, and staffs}*/

  case staff:
  case rod:
  case wand:

    if (strstr(item->name,"|") != NULL) {
      switch (item->tval) {
      case staff:
	return_value =   70;  break;
      case rod:
	return_value =   60;  break;
      case wand:
	return_value =   50;  break;
      }
    } else if (strstr(item->name,"^") != NULL) {
      return_value = ((item->cost div GOLD_VALUE) + 
		      trunc(item->cost/COST_ADJ/20.0)*(item->p1));
    }
    break;

    /*{Gems and jewelry of all types}*/

  case valuable_jewelry:
  case valuable_gems:

    if (strstr(item->name,"|") != NULL) {
      switch (item->tval) {
      case valuable_jewelry:
	return_value =   20;  break;
      case valuable_gems:
	return_value =   20;  break;
      }
    } else if (strstr(item->name,"^") != NULL) {
      return_value = (item->cost div GOLD_VALUE);
    }
    break;

  } /* end switch */

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean check_store_hours(integer st,integer sh)
{
  /*{ Check to see if a store is open, message when closed  -DMF-   }*/

  /* st is the S_GENERAL or whatever store it is */
  /* sh is the same as st, or -1 for the unnamed stores (S_TRADE_POST and up)*/

  string   name,prop;
  vtype    out_val;
  char     ope;
  boolean  flag;
  boolean  return_value = false;

  if (sh != -1) {
    //with stores[sh].store_open do;
    //with py.misc do;
    flag = ((PM.cur_age.year > stores[sh].store_open.year) ||
	    ((PM.cur_age.year == stores[sh].store_open.year) &&
	     ((PM.cur_age.month > stores[sh].store_open.month) ||
	      ((PM.cur_age.month == stores[sh].store_open.month) &&
	       ((PM.cur_age.day > stores[sh].store_open.day) ||
		((PM.cur_age.day == stores[sh].store_open.day) &&
		 ((PM.cur_age.hour > stores[sh].store_open.hour) ||
		  ((PM.cur_age.hour == stores[sh].store_open.hour) &&
		   ((PM.cur_age.secs > stores[sh].store_open.secs))))))))));
  } else {
    flag = true;
    sh = st;
  }

  if (flag) {
    strcpy(name, store_door[st].name);
    insert_str(name,"the entrance to the ","");
    ope = store_hours[sh][PM.cur_age.day % 7 + 0]  /* was + 1 */
      [PM.cur_age.hour div 2 + 0]; /* was + 1 */
    switch (ope) {
    case ' ' : return_value = true; break;
      
    case 'N' :
    case 'W' :
    case 'D' : 
      switch (ope) {
      case 'N' : strcpy(prop,"night.");   break;
      case 'W' : strcpy(prop,"weekend."); break;
      case 'D' : strcpy(prop,"day.");     break;
      }
      if (wizard2) {
	msg_print("Being a wizard, you break into the shop.");
	msg_print("");
	return_value = true;
      } else {
	sprintf(out_val, "Sorry, the %s is closed for the %s", name, prop);
	msg_print(out_val);
	return_value = false;
      }
      break;
      
    case 'B' :
      sprintf(prop, "Do you wish to play %ld gold to bribe the owner?",
	      store_bribe[st]);
      if (get_yes_no(prop)) {
	if (py.misc.money[TOTAL_] >= store_bribe[st]) {
	  subtract_money(store_bribe[st]*GOLD_VALUE,false);
	  msg_print("The owner reluctantly lets you in.");
	  msg_print("");
	  return_value = true;
	} else {
	  return_value = false;
	  msg_print("You haven't the money to bribe the owner!");
	}
      } else {
	return_value = false;
	msg_print("The owner complains bitterly about being woken up for no reason.");
      }
      break;
      
    default: 
      return_value = false;          
    }
  } else {
    msg_print("The doors are locked.");
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void check_store_hours_and_enter(integer st, integer sh, integer store_num)
{
  /* st is the S_GENERAL or whatever store it is */
  /* sh is the same as st, or -1 for the unnamed stores (S_TRADE_POST and up)*/
  /* store_num should probably always be the same as st */

  if (check_store_hours(st, sh)) {
    enter_store(store_num);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void st__reset_flag (integer time_spent, integer *flag)
{
  if (*flag > 1) {
    (*flag) -= time_spent;
    if (*flag < 1) {
      *flag = 1;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void spend_time(integer days_spent, vtype place,boolean whole_days)
{
  /*{ if not whole_days then it is actually turns... }*/

  integer   mornings,time_spent,turns_today,t2,i1;
  real      regen_percent;
  boolean   new_screen;
  vtype     out_val;
  
  //with py.misc.cur_age do;
  turns_today = PM.cur_age.hour*400 + PM.cur_age.secs;
  
  if (!whole_days) {
    time_spent = days_spent; /*{if a 6:00 threshold is passed}*/
    
    new_screen = ((turns_today + time_spent + 2400) div 4800) >
      ((turns_today + 2400) div 4800);
    
    mornings   = (turns_today + time_spent - 2400) div 9600 -
      (turns_today - 2400) div 9600;
    days_spent = 0;
  } else {
    time_spent = DAY_LENGTH * days_spent - turns_today;
    new_screen = true;
    mornings   = days_spent;
  }
  
  switch (days_spent) {
  case 0 : 
    PM.cur_age.secs += time_spent;
    PM.cur_age.hour += PM.cur_age.secs div 400;
    PM.cur_age.secs  = PM.cur_age.secs % 400;
    add_days(&(PM.cur_age),PM.cur_age.hour div 24);
    PM.cur_age.hour  = PM.cur_age.hour % 24;
    break;
    
  case 1 :
    if (PM.cur_age.hour < 6) {
      sprintf(out_val, "You spend the remainder of the night %s", place);
      msg_print(out_val);
      PM.cur_age.hour = 8;  /*{why get up before shops open?}*/
      PM.cur_age.secs = randint(400) - 1;
      time_spent = (time_spent - DAY_LENGTH + 
		    400*PM.cur_age.hour + PM.cur_age.secs);
    } else {
      sprintf(out_val, "You spend the night %s", place);
      msg_print(out_val);
      PM.cur_age.hour = 8;
      add_days(&(PM.cur_age),1);
      PM.cur_age.secs = randint(400) - 1;
      time_spent += 400*PM.cur_age.hour + PM.cur_age.secs;
    }
    break;
    
  case 3 : 
    msg_print("You spend three days in the inn.");
    // 10/26/00 --JEB.  what the hell was "28+randint(3)" doing in this context?
    //    add_days(&(PM.cur_age),28+randint(3));
    add_days(&(PM.cur_age),3);
    PM.cur_age.hour = 8 + randint(4);
    PM.cur_age.secs = randint(400) - 1;
    time_spent += 400*PM.cur_age.hour + PM.cur_age.secs;
    break;
    
  case 7 : 
    msg_print("You spend the week in the inn.");
    add_days(&(PM.cur_age),7);
    PM.cur_age.hour = 8 + randint(4);
    PM.cur_age.secs = randint(400) - 1;
    time_spent += 400*PM.cur_age.hour + PM.cur_age.secs;
    break;
  }

  put_qio();
  turn += time_spent;
  turn_counter += QUEST_DELAY;

  if (new_screen) {
    sleep_(1);
  }

  //with py.flags do;
  for (t2 = time_spent; (PF.poisoned > 0) && (t2 > 0);) {
    PF.poisoned--;
    time_spent--;

    switch (con_adj()) {
    case -4:	   take_hit(4,"poison"); break;
    case -3: case -2:  take_hit(3,"poison"); break;
    case -1:           take_hit(2,"poison"); break;
    case  0:	   take_hit(1,"poison"); break;
    case  1: case 2: case 3: 
      if ((t2 % 2) == 0) {     
	take_hit(1,"poison");
      }
      break;
    case  4: case 5:
      if ((t2 % 3) == 0) {
	take_hit(1,"poison.");
      }
      break;
    case  6: 
      if ((t2 % 4) == 0) {
	take_hit(1,"poison.");
      }
      break;
    } /* end switch */

    if (PF.poisoned == 0) {
      PF.status &= ~IS_POISONED;
      msg_print("You feel better.");
      put_qio();
    }
  } /* end for */

  st__reset_flag (time_spent, &(PF.blind) ) ;
  st__reset_flag (time_spent, &(PF.confused) ) ;
  st__reset_flag (time_spent, &(PF.protection) ) ;
  st__reset_flag (time_spent, &(PF.fast) ) ;
  st__reset_flag (time_spent, &(PF.slow) ) ;
  st__reset_flag (time_spent, &(PF.afraid) ) ;
  st__reset_flag (time_spent, &(PF.image) ) ;
  st__reset_flag (time_spent, &(PF.protevil) ) ;
  st__reset_flag (time_spent, &(PF.invuln) ) ;
  st__reset_flag (time_spent, &(PF.hero) ) ;
  st__reset_flag (time_spent, &(PF.shero) ) ;
  st__reset_flag (time_spent, &(PF.blessed) ) ;
  st__reset_flag (time_spent, &(PF.resist_heat) ) ;
  st__reset_flag (time_spent, &(PF.resist_cold)       ) ;
  st__reset_flag (time_spent, &(PF.detect_inv) ) ;
  st__reset_flag (time_spent, &(PF.word_recall) ) ;
  st__reset_flag (time_spent, &(PF.tim_infra) ) ;
  st__reset_flag (time_spent, &(PF.resist_lght) ) ;
  st__reset_flag (time_spent, &(PF.free_time) ) ;
  st__reset_flag (time_spent, &(PF.ring_fire) ) ;
  st__reset_flag (time_spent, &(PF.protmon) ) ;
  st__reset_flag (time_spent, &(PF.hoarse) ) ;
  st__reset_flag (time_spent, &(PF.magic_prot) ) ;
  st__reset_flag (time_spent, &(PF.ring_ice) ) ;
  st__reset_flag (time_spent, &(PF.temp_stealth) ) ;
  st__reset_flag (time_spent, &(PF.resist_petri) ) ;
  st__reset_flag (time_spent, &(PF.blade_ring) ) ;
  
  switch (days_spent) {
  case 0: case 1:
    PF.foodc -= time_spent;
    if (PF.foodc <= PLAYER_FOOD_ALERT) {
      /* free food if you were hungry when you got here? */
      PF.foodc = PLAYER_FOOD_ALERT + 1;
    }
    break;
    
  default:
    PF.foodc = PLAYER_FOOD_FULL - 1;
    break;
  }

  PF.confuse_monster = false;

  for (i1 = 1; i1 <= mornings; i1++) {
    store_maint();
  }

  //with py.misc do;
  regen_percent = regen_amount*2*time_spent;
  if (regen_percent > 1.00) {
    regen_percent = 1.00;
  }
  if (PM.chp < PM.mhp) {
    regenhp(regen_percent);
  }
  if (PM.chp > PM.mhp) {
    PM.chp = PM.mhp;
  }
  if (PM.cmana < PM.mana) {
    regenmana(regen_percent);
  }
  if (PM.cmana > PM.mana) { 
    PM.cmana = PM.mana;
  }
  
  if (new_screen) {
    moria_flag = true;
    msg_print("");
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void enter_store(integer store_num)
{
  /*{ Entering a store					-RAK-	}*/

  integer   com_val,cur_top,tics;
  treas_ptr trash_ptr;
  char      command;
  boolean   exit_flag;

  tics = 1;
  //with stores[store_num] do;
  exit_flag = false;
  cur_top = 1;

  display_store(store_num,cur_top);

  do {
    if (get_com("",&command)) {
      msg_flag  = false;
      com_val   = (integer)(command);

      switch (com_val) {
      case 18      : display_store(store_num,cur_top);
      case 73      :      /*{ Selective Inventory   }*/
	if (inven_command('I',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	}
	break;
      case 32      : 
	if (cur_top == 1) {
	  if (stores[store_num].store_ctr > 12) {
	    cur_top = 13;
	    display_inventory(store_num,cur_top);
	  } else {
	    prt("Entire inventory is shown.",1,1);
	  }
	} else {
	  cur_top = 1;
	  display_inventory(store_num,cur_top);
	}
	break;
      case 101     :      /*{ Equipment List        }*/
	if (inven_command('e',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	} 
	break;
      case 105     :      /*{ Inventory             }*/
	if (inven_command('i',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	}
	break;
      case 116     :      /*{ Take off              }*/
	if (inven_command('t',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	}
	break;
      case 119     :      /*{ Wear                  }*/
	if (inven_command('w',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	}
	break;
      case 120     :      /*{ Switch weapon         }*/
	if (inven_command('x',&trash_ptr,"")) {
	  display_store(store_num,cur_top);
	}
	break;
      case 112   : exit_flag = store_purchase(store_num,&cur_top,false); break;
      case 80    : exit_flag = store_purchase(store_num,&cur_top,true);  break;
      case 115   : exit_flag = store_sell(store_num,cur_top,false);      break;
      case 83    : exit_flag = store_sell(store_num,cur_top,true);       break;
      default    : prt("Invalid Command.",1,1); break;
      } /* end switch */
    }
    else {
      exit_flag = true;
    }
    adv_time(false);
    tics++;
    check_kickout_time(tics,2);
  } while (!exit_flag);
  
  if (moria_flag) {
    clear_rc(1,1);
    prt_stat_block();
  } else {
    draw_cave();
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void display_store(integer store_num,integer cur_top)
{
  /*{ Displays store					-RAK-	}*/
  
  //with stores[store_num] do;
  clear_screen();
  prt(owners[stores[store_num].owner].owner_name,4,10);
  prt("   Item",5,1);
  prt("Asking Price",5,61);
  store_prt_gold();
  display_commands();
  display_inventory(store_num,cur_top);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void store_prt_gold()
{
  /*{ Displays players gold					-RAK-	}*/

  vtype    out_val;

  sprintf(out_val,"Gold Remaining : %ld",py.misc.money[TOTAL_]);
  prt(out_val,19,18);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void display_cost(integer store_num,integer pos)
{
  /*{ Re-displays only a single cost                        -RAK-   }*/

  integer   i1,i2;
  vtype     out_val;

  //with stores[store_num] do;
  i1 = ((pos-1) % 12);
  if (stores[store_num].store_inven[pos].scost < 0) {
    i2  = abs(stores[store_num].store_inven[pos].scost);
    i2 += (integer)(i2*chr_adj());
    sprintf(out_val,"%6ld",(i2 div GOLD_VALUE));
  } else {
    sprintf(out_val,"%6ld [FIXED]",
	    (stores[store_num].store_inven[pos].scost div GOLD_VALUE));
  }

  prt(out_val,i1+6,60);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void display_commands()
{
  /*{ Displays the set of commands				-RAK-	}*/
prt("You may:",21,1);
prt(" p/P) Purchase an item.          <space> browse store's inventory.",22,1);
prt(" s/S) Sell an item.              i) Inventory and Equipment Lists.",23,1);
prt(" ^R) Redraw the screen.        Esc) Exit from building.",24,1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void haggle_commands(integer typ)
{
  /*{ Displays the set of commands				-RAK-	}*/

  if (typ == -1) {
    prt("Specify an asking-price in gold pieces.",22,1);
  } else {
    prt("Specify an offer in gold pieces.",22,1);
  }
  prt("Esc) Quit Haggling.",23,1);
  prt("",24,1);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void display_inventory(integer store_num,integer start)
{
  /*{ Displays a store's inventory				-RAK-	}*/

  integer   i1,i2,stop;
  vtype     out_val1,out_val2;

  //with stores[store_num] do;
  i1   = ((start-1) % 12);
  stop = (((start-1) div 12) + 1)*12;

  if (stop > stores[store_num].store_ctr) {
    stop = stores[store_num].store_ctr;
  }

  for (; (start <= stop);) {
    inven_temp->data = stores[store_num].store_inven[start].sitem;
    //with inven_temp->data do;
    if ((inven_temp->data.subval > 255) && 
	(inven_temp->data.subval < 512)) {
      inven_temp->data.number = 1;
    }
    objdes(out_val1,inven_temp,true);
    sprintf(out_val2,"%c) %s",(97+(int)i1),out_val1);
    prt(out_val2,i1+6,1);
    if (stores[store_num].store_inven[start].scost < 0) {
      /*{quack}*/
      i2 = abs(stores[store_num].store_inven[start].scost);
      i2 = i2 + trunc(i2*chr_adj());
      sprintf(out_val2,"%6ld",((i2+GOLD_VALUE-1) / GOLD_VALUE));
    } else {
      sprintf(out_val2,"%6ld [Fixed]",
	      (stores[store_num].store_inven[start].scost / GOLD_VALUE));
    }
    prt(out_val2,i1+6,60);
    i1++;
    start++;
  }
  if (i1 < 12) {
    for (i2 = 1; i2 <= (12 - i1 + 1); i2++) {
      prt("",i2+i1+5,1);
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean store_purchase(integer store_num,integer *cur_top,boolean blitz)
{
  /*{ Buy an item from a store                              -RAK-   }*/

  integer    i1,item_val,price,to_bank;
  integer    choice;
  treas_ptr  item_new;
  integer    save_number;
  vtype      out_val,foo,out2;
  boolean    flag;
  boolean    return_value = false;

  item_new = nil;
  //with stores[store_num] do;
  if (blitz) {
    strcpy(foo, "BLITZ-PURCHASE item? ");
  } else {
    strcpy(foo, "Purchase which item? ");
  }
  /* { i1 = number of objects shown on screen        }*/
  
  if (*cur_top == 13) {
    i1 = stores[store_num].store_ctr - 12;
  } else if (stores[store_num].store_ctr > 12) {
    i1 = 12;
  } else {
    i1 = stores[store_num].store_ctr;
  }
  
  if (stores[store_num].store_ctr < 1) {
    msg_print("I am currently out of stock.");
    /*{ Get the item number to be bought              }*/
  } else  if (get_store_item(&item_val,foo,1,i1)) {
    /*{"Which item are you interested in? ",1,i1)) then}*/
    item_val += *cur_top - 1;     /*{ true item_val }*/
    inven_temp->data = stores[store_num].store_inven[item_val].sitem;
    
    //with inven_temp->data. do;
    if ((inven_temp->data.subval > 255) && (inven_temp->data.subval < 512)) {
      save_number = inven_temp->data.number;
      inven_temp->data.number = 1;
    } else {
      save_number = 1;
    }
    
    if (inven_check_weight() || (store_num == S_INN)) {
      if (inven_check_num() || (store_num == S_INN)) {
	if (stores[store_num].store_inven[item_val].scost > 0) {
	  price = stores[store_num].store_inven[item_val].scost div GOLD_VALUE;
	  choice = 0;
	} else {
	  choice = purchase_haggle(store_num,&price,&(inven_temp->data),blitz);
	}
	
	switch (choice) {
	  
	case 0 :
	  //with py.misc do;
	  flag = false;
	  if (PM.money[TOTAL_] >= price) {
	    subtract_money(price*GOLD_VALUE,true);
	    flag = true;
	  } else {
	    to_bank = price - PM.money[TOTAL_];
	    flag = send_page(to_bank);
	  }
	  
	  if (flag) {
	    prt_comment1();
	    decrease_insults(store_num);
	    if (store_num == S_INN) {
	      //with store_inven[item_val] do;
	      if (stores[store_num].store_inven[item_val].scost < 0) {
		stores[store_num].store_inven[item_val].scost = 
		  price * GOLD_VALUE;
	      }
	      spend_time(stores[store_num].store_inven[item_val].sitem.p1,
			 "at the Inn.",true);
	      if (stores[store_num].store_inven[item_val].sitem.subval==303) {
		spend_time(600,"eating.",false);
		msg_print("You eat a leisurely meal of buckwheat cakes and bacon.");
		py.flags.foodc = PLAYER_FOOD_FULL;
		py.flags.status &= ~(IS_WEAK | IS_HUNGERY);
		msg_print(" ");
	      }
	      return_value = true;
	    } else {
	      store_destroy(store_num,item_val,true);
	      item_new = inven_carry();
	      objdes(out_val,item_new,true);
	      sprintf(out2, "You have %s", out_val);
	      msg_print(out2);
	      if (*cur_top > stores[store_num].store_ctr) {
		*cur_top = 1;
		display_inventory(store_num,*cur_top);
	      } else {
		//with store_inven[item_val] do;
		if (save_number > 1) {
		  if (stores[store_num].store_inven[item_val].scost < 0) {
		    stores[store_num].store_inven[item_val].scost = 
		      price * GOLD_VALUE;
		    display_cost(store_num,item_val);
		  }
		} else {
		  display_inventory(store_num,item_val);
		}
	      }
	      store_prt_gold();
	    }
	  } else {
	    if (increase_insults(store_num)) {
	      return_value = true;
	    } else {
	      prt_comment1();
	      msg_print("Liar!  You have not the gold!");
	    }
	  }
	  
	  break;
	  
	case 2 : 
	  return_value = true;
	  break;
	  
	default:
	  break;
	} /* end switch */
	prt("",2,1);
	
      } else {
	prt("You cannot carry that many different items.",1,1);
      }
    } else {
      prt("You can not carry that much weight.",1,1);
    }
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean store_sell(integer store_num,integer cur_top,boolean blitz)
{
  /*{ Sell an item to the store                             -RAK-   }*/

  integer    count;
  treas_ptr  item_ptr;
  integer    item_pos,price;
  vtype      out_val,foo,out2;
  char       trash_char;
  boolean    redraw;
  boolean    return_value = false;

  if (blitz) {
    strcpy(foo, "BLITZ-SELLING item? ");
  } else {
    strcpy(foo, "Which one? ");
  }

  //with stores[store_num] do;
  redraw = false;
  if (!(find_range(store_buy[store_num],false,&item_ptr,&count))) {
    msg_print("You have nothing the store wishes to buy.");
  } else if (get_item(&item_ptr,foo,&redraw,count,&trash_char,false,false)) {
    if (redraw) {
      display_store(store_num,cur_top);
    }
    inven_temp->data = item_ptr->data;
    //with inven_temp->data. do;
    if ((inven_temp->data.subval > 255) &&
	(inven_temp->data.subval < 512)) {
      inven_temp->data.number = 1;                /*{But why????}*/
    }
    objdes(out_val,inven_temp,true);
    sprintf(out2, "Selling %s", out_val);
    msg_print(out2);
    msg_print(" ");
    if (is_in(inven_temp->data.tval, store_buy[store_num])) {
      if (store_check_num(store_num)) {

	switch (sell_haggle(store_num,&price,&(inven_temp->data),blitz)) {
	case 0 : 
	  prt_comment1();
	  add_money(price*GOLD_VALUE);
	  inven_destroy(item_ptr);
	  store_carry(store_num,&item_pos);
	  if (item_pos > 0) {
	    if (item_pos < 13) {
	      if (cur_top < 13) {
		display_inventory(store_num,item_pos);
	      } else {
		display_inventory(store_num,cur_top);
	      }
	    } else if (cur_top > 12) {
	      display_inventory(store_num,item_pos);
	    }
	  }      
	  store_prt_gold();
	  break;
	  
	case 2 :
	  return_value = true;
	  break;

	case 3 : 
	  msg_print("How dare you!");
	  msg_print("I will not buy that!");
	  return_value = increase_insults(store_num);
	  break;

	case 4 : 
	  /* black market or insured stuff after a death */
	  msg_print("Hmmmmm, that looks hot. I will not buy it!");
	  msg_print("I am an honest merchant!");
	  
	default :
	  break;
	}
      } else {
	prt("I have not the room in my store to keep it...",1,1);
      }
    } else {
      prt("I do not buy such items.",1,1);
    }
  } else if (redraw) {
    display_store(store_num,cur_top);
  }
  
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean increase_insults(integer store_num)
{
  /*{ Increase the insult counter and get pissed if too many -RAK-  }*/
  boolean   return_value = false;

  //with stores[store_num] do;
  stores[store_num].insult_cur++;
  if (stores[store_num].insult_cur >
      owners[stores[store_num].owner].insult_max) {
    prt_comment4();
    stores[store_num].insult_cur = 0;
    change_rep(-5);
    shut_store(store_num);
    return_value = true;
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void decrease_insults(integer store_num)
{
  //with stores[store_num] do;
  stores[store_num].insult_cur -= 2;
  if (stores[store_num].insult_cur < 0) {
    stores[store_num].insult_cur = 0;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void shut_store(integer store_num)
{
  //with stores[store_num] do;
  //with py.misc.cur_age do;
  stores[store_num].store_open.year  = PM.cur_age.year;
  stores[store_num].store_open.month = PM.cur_age.month;
  stores[store_num].store_open.day   = PM.cur_age.day;
  stores[store_num].store_open.hour  = PM.cur_age.hour;
  stores[store_num].store_open.secs  = PM.cur_age.secs;
  
  //with store_open do;

  stores[store_num].store_open.day++;
  stores[store_num].store_open.hour = 6;
  stores[store_num].store_open.secs = randint(400) - 1;

  if (stores[store_num].store_open.day > 28) {
    stores[store_num].store_open.day   = 1;
    stores[store_num].store_open.month++;
    if (stores[store_num].store_open.month > 13) {
      stores[store_num].store_open.month = 1;
      stores[store_num].store_open.year++;
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean get_store_item(integer *com_val,vtype pmt,integer i1, integer i2)
{
  /*{ Get the ID of a store item and return its value       -RAK-   }*/

  char      command;
  vtype     out_val;
  boolean   flag = true;

  *com_val = 0;
  sprintf(out_val,"(Items %c-%c, Esc to exit) %s",(int)i1+96,(int)i2+96,pmt);
  
  while (((*com_val < i1) || (*com_val > i2)) && (flag)) {
    prt(out_val,1,1);
    command  = inkey();
    *com_val = (integer)(command);
    switch (*com_val) {
    case 3: case 25: case 26: case 27:
      flag = false;
      break;
    default:
      *com_val -= 96;
      break;
    }
  }
  
  msg_flag = false;
  erase_line(msg_line,msg_line);
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer sell_haggle(integer store_num,integer *price,treasure_type *item,
                    boolean blitz)
{
  /*{ Haggling routine                                      -RAK-   }*/
  /*{ Return value shows the result of the haggling:                 */
  /*    0 = Sold, 2 = Aborted, 3 or 4 = Owner will not buy }         */

  integer    max_sell=1,max_buy=1,min_buy=1;
  integer    cost,cur_ask,final_ask,min_offer;
  integer    last_offer,new_offer,final_flag,x3;
  integer    max_gold=0,delta;
  real       x1,x2;
  real       min_per=1.0,max_per=1.0;
  boolean    flag,loop_flag;
  vtype      comment,out_val;
  integer    return_value = 0;
  
  ENTER("sell_haggle","s");

  flag = false;
  *price = 0;
  final_flag = 0;
  msg_flag = false;
  
  //with stores[store_num] do;
  cost = item_value(item);
  if (cost < 1) {
    return_value = 3;
    flag = true;
  } else if ( item->flags2 & Blackmarket_bit ) {
    return_value = 4;
    flag = true;
  } else {
    //with owners[owner] do;
    cost = cost - (integer)(cost*chr_adj()) -
      (integer)(cost*rgold_adj[owners[stores[store_num].owner].owner_race][py.misc.prace]);
    if (cost < 1) {
      cost = 1;
    }
    max_sell = trunc(cost*(1+(owners[stores[store_num].owner]).max_inflate));
    max_buy  = trunc(cost*(1-(owners[stores[store_num].owner]).max_inflate));
    min_buy  = trunc(cost*(1-(owners[stores[store_num].owner]).min_inflate));
    if (min_buy < max_buy) {
      min_buy = max_buy;
    }
    min_per  = (owners[stores[store_num].owner]).haggle_per;
    max_per  = min_per*3.0;
    max_gold = (owners[stores[store_num].owner]).max_cost;
  }

  if (blitz) {
    delta = (min_buy - max_buy);
    last_offer = min_buy - (delta div 7);
    //with stores[store_num] do;
    *price = last_offer - ((stores[store_num].insult_cur * delta) / owners[stores[store_num].owner].insult_max);
    strcpy(comment, "Need cash quick?  I'll pay you ");
    sprintf(out_val,"%s%ld   ",comment,*price);
    msg_print(out_val);
    msg_print(" ");
  } else {
    /*{haggling}*/
    if (!(flag)) {
      haggle_commands(-1);
      if (max_buy > max_gold) {
        strcpy(comment, "Final offer : ");
        final_flag = 1;
        cur_ask    = max_gold;
        final_ask  = max_gold;
        msg_print("I am sorry, but I have not the money to afford such a fine item.");
        msg_print(" ");
      } else {
        cur_ask   = max_buy;
        final_ask = min_buy;
        if (final_ask > max_gold) {
          final_ask = max_gold;
        }
        strcpy(comment, "Offer : ");
      }
      min_offer  = max_sell;
      last_offer = min_offer;
      if (cur_ask < 1) {
        cur_ask = 1;
      }
      do {
        do {
          loop_flag = true;
          sprintf(out_val,"%s%ld       ",comment,cur_ask);
          put_buffer(out_val,2,1);
          switch (receive_offer(store_num,"What price do you ask? ",
                                &new_offer,last_offer,-1)) {
          case 1 : 
            flag   = true;
            return_value = 1;
            break;
            
          case 2 : 
            return_value = 2;
            flag   = true;
            break;
            
          default:
            if (new_offer < cur_ask) {
	      /* they probably missed a key... be kind */
              prt_comment6();
              loop_flag = false;
            } else if (new_offer == cur_ask) {
	      /* sold!!! */
              flag   = true;
              *price = new_offer;
            }
            break;
          }
        } while (!((flag) || (loop_flag)));

        if (!(flag)) {
          msg_flag = false;
          x1 = (real)(last_offer - new_offer)/(real)(last_offer - cur_ask);

          if (x1 < min_per) {
            flag = haggle_insults(store_num);
            if (flag) {
              return_value = 2;
            }
          } else {
            if (x1 > max_per) {
              x1 *= 0.75;
              if (x1 < max_per) {
                x1 = max_per;
              }
            }
            x2 = (x1 + (randint(5) - 3)/100.0);
            x3 = trunc((new_offer-cur_ask)*x2) + 1;
            cur_ask += x3;

            if (cur_ask > final_ask) {
              cur_ask = final_ask;
              strcpy(comment,"Final Offer : ");
              final_flag++;

              if (final_flag > 3) {

                if (increase_insults(store_num)) {
                  return_value = 2;
                } else {
                  return_value = 1;
                }
                flag = true;

              }

	    } else if (new_offer <= cur_ask) {
	      flag   = true;
	      *price = new_offer;
	    }

	    if (!(flag)) {
	      last_offer = new_offer;
	      prt(" ",2,1);
	      sprintf(out_val,"Your last bid   : %ld   ",last_offer);
	      put_buffer(out_val,2,40);
	      prt_comment3(cur_ask,last_offer,final_flag);
	    }
	  }
	}
      } while (!flag);
      prt(" ",2,1);
      display_commands();
    }
  }

  RETURN("sell_haggle","s",'d',"status:",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer purchase_haggle(integer store_num,integer *price, 
			treasure_type *item,boolean blitz)
{
  /*{ Haggling routine                                      -RAK-   }*/

  integer   max_sell,min_sell,max_buy;
  integer   cost,cur_ask,final_ask,min_offer;
  integer   last_offer,new_offer,final_flag,x3;
  integer   delta;
  real      x1,x2;
  real      min_per,max_per;
  boolean   flag,loop_flag;
  vtype     out_val,comment;
  integer   return_value = 0;

  flag       =  false;
  *price     =  0;
  final_flag =  0;
  msg_flag   =  false;

  //with stores[store_num]. do;
  //with owners[stores[store_num].owner]. do;
  cost = sell_price(store_num,&max_sell,&min_sell,item);
  max_sell += (max_sell*chr_adj());
  if (max_sell < 0) {
    max_sell = 1;
  }
  min_sell += (min_sell*chr_adj());
  if (min_sell < 0) {
    min_sell = 1;
  }
  max_buy  = trunc(cost*(1-owners[stores[store_num].owner].max_inflate));
  min_per  = owners[stores[store_num].owner].haggle_per;
  max_per  = min_per*3.0;
  

  haggle_commands(1);
  cur_ask    = max_sell;
  final_ask  = min_sell;
  min_offer  = max_buy;
  last_offer = min_offer;
  strcpy(comment,"Asking : ");
  
  if (blitz) {
    delta = (max_sell - min_sell);
    last_offer = min_sell + (delta div 4);
    //with stores[store_num]. do;
    *price = last_offer + ((stores[store_num].insult_cur * delta) / 
			   owners[stores[store_num].owner].insult_max);
    strcpy(comment,"In a hurry, eh?  It's yours for a mere ");
    sprintf(out_val,"%s%ld       ",comment,*price);
    msg_print(out_val);
    msg_print(" ");
  } else { 
    /*{ go ahead and haggle }*/
    do {
      do {
	loop_flag = true;
	sprintf(out_val,"%s%ld          ",comment,cur_ask);
	put_buffer(out_val,2,1);
	switch (receive_offer(store_num,"What do you offer? ",
			      &new_offer,last_offer,1)) {
	case 1 :
	  return_value = 1;
	  flag   = true;
	  break;
	  
	case 2 : 
	  return_value = 2;
	  flag   = true;
	  break;
	  
	default:
	  if (new_offer > cur_ask) {
	    prt_comment6();
	    loop_flag = false;
	  } else if (new_offer == cur_ask) {
	    flag  = true;
	    *price = new_offer;
	  }
	} /* end switch */
	
      } while (!((flag) || (loop_flag)));
      
      if (!(flag)) {
	
	x1 = (real)(new_offer - last_offer)/(real)(cur_ask - last_offer);
	if (x1 < min_per) {
	  flag = haggle_insults(store_num);
	  if (flag) {
	    return_value = 2;
	  }
	} else {
	  if (x1 > max_per) {
	    x1 *= 0.75;
	    if (x1 < max_per) {
	      x1 = max_per;
	    }
	  }
	  x2 = (x1 + (randint(5) - 3)/100.0);
	  x3 = (integer)((cur_ask-new_offer)*x2) + 1;
	  cur_ask -= x3;
	  if (cur_ask < final_ask) {
	    cur_ask = final_ask;
	    strcpy(comment,"Final Offer : ");
	    final_flag++;
	    if (final_flag > 3) {
	      if (increase_insults(store_num)) {
		return_value = 2;
	      } else {
		return_value = 1;
	      }
	      flag = true;
	    } else if (new_offer >= cur_ask) {
	      flag  = true;
	      *price = new_offer;
	    }

	    if (!flag) {
	      last_offer = new_offer;
	      prt("",2,1);
	      sprintf(out_val,"Your last offer : %ld   ",last_offer);
	      put_buffer(out_val,2,40);
	      prt_comment2(last_offer,cur_ask,final_flag);
	    }
	  }
	}
      }
    } while (!flag);
  }

  prt("",2,1);
  display_commands();
  
  return return_value;
};   
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean haggle_insults(integer store_num)
{
  /*{ Have insulted while haggling                          -RAK-   }*/

  boolean   return_value = false;

  if (increase_insults(store_num)) {
    return_value = true;
  } else {
    prt_comment5();
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean ro__get_haggle(vtype comment, integer *num)
{
  integer   i1,clen;
  vtype     out_val;
  boolean   flag = true;

  ENTER("ro__get_haggle","s");

  i1 = 0;
  clen = strlen(comment) + 1;

  do {
    msg_print(comment);
    msg_flag = false;

    if (!(get_string(out_val,1,clen,40))) {
      flag = false;
      erase_line(msg_line,msg_line);
    }

    i1 = 0;
    sscanf(out_val,"%ld",&i1);
  } while (!((i1 != 0) || (!flag)));

  if (flag) {
    *num = i1;
  }

  RETURN("ro__get_haggle","s",'b',"flag:",&flag);
  return  flag;
};
//////////////////////////////////////////////////////////////////////
integer receive_offer(integer store_num,vtype comment,integer *new_offer,
		      integer last_offer,integer factor)
{
  boolean   flag;
  integer   return_value = 0;
  
  ENTER("receive_offer", "i");
  
  flag = false;
  
  do {
    if (ro__get_haggle(comment,new_offer)) {
      if ((*new_offer * factor) >= (last_offer * factor)) {
	flag = true;
      } else if (haggle_insults(store_num)) {
	return_value = 2;
	flag = true;
      }
    } else {
      return_value = 1;
      flag = true;
    }
  } while (!flag);
    
  RETURN("receive_offer", "i",'d',"123: ", &return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment1()
{
  /*{ Comment one : Finished haggling                               }*/

  msg_flag = false;
  switch (randint(15)) {
  case  1 : msg_print("Done!");break;
  case  2 : msg_print("Accepted!");break;
  case  3 : msg_print("Fine...");break;
  case  4 : msg_print("Agreed!");break;
  case  5 : msg_print("Ok...");break;
  case  6 : msg_print("Taken!");break;
  case  7 : msg_print("You drive a hard bargain, but taken...");break;
  case  8 : msg_print("You'll force me bankrupt, but it's a deal...");break;
  case  9 : msg_print("Sigh...  I'll take it...");break;
  case 10 : msg_print("My poor sick children may starve, but done!");break;
  case 11 : msg_print("Finally!  I accept...");break;
  case 12 : msg_print("Robbed again...");break;
  case 13 : msg_print("A pleasure to do business with you!");break;
  case 14 : msg_print("My spouse shall skin me, but accepted.");break;
  case 15 : msg_print("Fine! Just be that way!");break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment2(integer offer,integer asking,integer final)
{
  /*{ %A1 is offer, %A2 is asking...                }*/

  vtype   comment;

  if (final > 0) {
    switch (randint(3)) {
    case 1 : 
      strcpy(comment,"%A2 is my final offer; take it or leave it...");
      break;
    case 2 : 
      strcpy(comment,"I'll give you no more than %A2.");
      break;
    case 3 : 
      strcpy(comment,"My patience grows thin...  %A2 is final.");
      break;
    }
  } else {
    switch (randint(16)) {
    case  1 : 
      strcpy(comment,"%A1 for such a fine item?  HA!  No less than %A2.");
      break;
    case  2 : 
      strcpy(comment,"%A1 is an insult!  Try %A2 gold pieces...");
      break;
    case  3 : 
      strcpy(comment,"%A1???  Thou would rob my poor starving children?");
      break;
    case  4 : 
      strcpy(comment,"Why I'll take no less than %A2 gold pieces.");
      break;
    case  5 : 
      strcpy(comment,"Ha!  No less than %A2 gold pieces.");
      break;
    case  6 : 
      strcpy(comment,"Thou blackheart!  No less than %A2 gold pieces.");
      break;
    case  7 : 
      strcpy(comment,"%A1 is far too little, how about %A2?");
      break;
    case  8 : 
      strcpy(comment,"I paid more than %A1 for it myself, try %A2.");
      break;
    case  9 : 
      strcpy(comment,"%A1?  Are you mad???  How about %A2 gold pieces?");
      break;
    case 10 : 
      strcpy(comment,"As scrap this would bring %A1.  Try %A2 in gold.");
      break;
    case 11 : 
      strcpy(comment,"May fleas of a 1000 orcs molest you.  I want %A2.");
      break;
    case 12 : 
      strcpy(comment,"My mother you can get for %A1, this costs %A2.");
      break;
    case 13 : 
      strcpy(comment,"May your chickens grow lips.  I want %A2 in gold!");
      break;
    case 14 : 
      strcpy(comment,"Sell this for such a pittance.  Give me %A2 gold.");
      break;
    case 15 : 
      strcpy(comment,"May the Balrog find you tasty!  %A2 gold pieces?");
      break;
    case 16 : 
      strcpy(comment,"Your mother was a Troll!  %A2 or I'll tell...");
      break;
    }
    
    insert_num(comment,"%A1",offer,false);
    insert_num(comment,"%A2",asking,false);
    msg_print(comment);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment3(integer offer,integer asking,integer final)
{
  vtype   comment;

  if (final > 0) {
    switch (randint(3)) {
    case 1 :
      strcpy(comment,"I'll pay no more than %A1; take it or leave it.");
      break;
    case 2 :
      strcpy(comment,"You'll get no more than %A1 from me...");
      break;
    case 3 :
      strcpy(comment,"%A1 and that's final.");
      break;
    }
   } else {
     switch (randint(15)) {
     case  1 :
       strcpy(comment,"%A2 for that piece of junk?  No more than %A1");
       break;
     case  2 :
       strcpy(comment,"For %A2 I could own ten of those.  Try %A1.");
       break;
     case  3 :
       strcpy(comment,"%A2?  NEVER!  %A1 is more like it...");
       break;
     case  4 :
       strcpy(comment,"Let's be reasonable... How about %A1 gold pieces?");
       break;
     case  5 :
       strcpy(comment,"%A1 gold for that junk, no more...");
       break;
     case  6 :
       strcpy(comment,"%A1 gold pieces and be thankful for it!");
       break;
     case  7 :
       strcpy(comment,"%A1 gold pieces and not a copper more...");
       break;
     case  8 :
       strcpy(comment,"%A2 gold?  HA!  %A1 is more like it...");
       break;
     case  9 :
       strcpy(comment,"Try about %A1 gold...");
       break;
     case 10 :
       strcpy(comment,"I wouldn't pay %A2 for your children, try %A1.");
       break;
     case 11 :
       strcpy(comment,"*CHOKE* For that!?  Let's say %A1.");
       break;
     case 12 :
       strcpy(comment,"How about %A1.");
       break;
     case 13 :
       strcpy(comment,"That looks war surplus!  Say %A1 gold.");
       break;
     case 14 :
       strcpy(comment,"I'll buy it as scrap for %A1.");
       break;
     case 15 :
       strcpy(comment,"%A2 is too much, let us say %A1 gold.");
       break;
     }
   }
  insert_num(comment,"%A1",offer,false);
  insert_num(comment,"%A2",asking,false);
  msg_print(comment);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment4()
{
  /*{ Kick 'da bum out...                                   -RAK-   }*/

  msg_flag = false;
  switch (randint(5)) {
  case 1 :   
    msg_print("ENOUGH!  Thou hath abused me once too often!");
    msg_print("Out of my place!");
    msg_print(" ");
    break;

  case 2 :   
    msg_print("THAT DOES IT!  You shall waste my time no more!");
    msg_print("out... Out... OUT!!!");
    msg_print(" ");
    break;    
    
  case 3 :   
    msg_print("This is getting no where...  I'm going home!");
    msg_print("Come back tomorrow...");
    msg_print(" ");
    break;    
    
  case 4 :   
    msg_print("BAH!  No more shall you insult me!");
    msg_print("Leave my place...  Begone!");
    msg_print(" ");
    break;    
    
  case 5 :   
    msg_print("Begone!  I have had enough abuse for one day.");
    msg_print("Come back when thou art richer...");
    msg_print(" ");
    break;    
  }
  msg_flag = false;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment5()
{
  switch (randint(10)) {
    case  1 :  msg_print("You will have to do better than that!");  break;
    case  2 :  msg_print("That's an insult!");                      break;
    case  3 :  msg_print("Do you wish to do business or not?");     break;
    case  4 :  msg_print("Hah!  Try again...");                     break;
    case  5 :  msg_print("Ridiculous!");                            break;
    case  6 :  msg_print("You've got to be kidding!");              break;
    case  7 :  msg_print("You better be kidding!!");                break;
    case  8 :  msg_print("You try my patience.");                   break;
    case  9 :  msg_print("I don't hear you.");                      break;
    case 10 :  msg_print("Hmmm, nice weather we're having...");     break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void prt_comment6()
{
  switch (randint(5)) {
    case 1 :  msg_print("I must of heard you wrong...");  break;
    case 2 :  msg_print("What was that?");                break;
    case 3 :  msg_print("I'm sorry, say that again...");  break;
    case 4 :  msg_print("What did you say?");             break;
    case 5 :  msg_print("Sorry, what was that again?");   break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  store.c */
