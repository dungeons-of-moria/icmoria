/* bank.c */
/* */

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void eb__display_money()
{
    vtype      out_val;

    sprintf(out_val, " Gold remaining : %ld", py.misc.money[TOTAL_]);
    prt(out_val, 18, 18);
    sprintf(out_val," Account : %ld",py.misc.account);	
    prt(out_val, 16, 20);

    sprintf(out_val, "You have ");
    put_buffer(out_val, 6, 25);
    sprintf(out_val, "Mithril  : %10ld",py.misc.money[MITHRIL]);
    put_buffer(out_val, 8, 25);
    sprintf(out_val, "Platinum : %10ld",py.misc.money[PLATINUM]);
    put_buffer(out_val, 9, 25);
    sprintf(out_val, "Gold     : %10ld",py.misc.money[GOLD]);
    put_buffer(out_val, 10, 25);
    sprintf(out_val, "Silver   : %10ld",py.misc.money[SILVER]);
    put_buffer(out_val, 12, 25);
    sprintf(out_val, "Copper   : %10ld",py.misc.money[COPPER]);
    put_buffer(out_val, 13, 25);
    sprintf(out_val, "Iron     : %10ld",py.misc.money[IRON]);
    put_buffer(out_val, 14, 25);

    if(wizard2) {
      put_buffer("Bank has ", 6, 50);
      sprintf(out_val,"%10ld",bank[MITHRIL]);
      put_buffer(out_val, 8, 50);
      sprintf(out_val,"%10ld",bank[PLATINUM]);
      put_buffer(out_val, 9, 50);
      sprintf(out_val,"%10ld",bank[GOLD]);
      put_buffer(out_val, 10, 50);
    }

    put_qio();

}; /* end eb__display_money */

//////////////////////////////////////////////////////////////////////

void eb__display_store(vtype shop_owner)
{
    /* {Clear the screen and display the bank.} */

  clear_screen();
  /*    clear_rc(1,1);*/
    prt(shop_owner,4,10);
    eb__display_money();
    prt("You may:",19,1);
    prt(" d) Deposit money.             w) Withdraw money.",20,1);
    prt(" c) Change small currency.     i) Buy insurance.",21,1);
    prt("^R) Redraw the screen.       Esc) Exit from building.",22,1);
    prt(" p) Put item in vault.         r) Remove item from vault.",23,1);

}; /* end eb__display_store */

//////////////////////////////////////////////////////////////////////

boolean eb__get_entry(vtype comment, integer * num)
{
/* { Returns true if a number >= 0 is entered, false if escaped, }*/
/* { negative numbers not permitted.                      -MKC-  }*/

    boolean return_value = false;
    boolean valid;
    vtype   in_val;

    *num = -1;
    do {
	valid = true;
	msg_print(comment);
	msg_flag = false;
	if (get_string(in_val, 1, strlen(comment)+2, 40)) {
	    *num = atoi(in_val);
	    if (*num >= 0) {
		return_value = true;
	    } else {
		msg_print("Invalid entry.");
		valid = false;
	    }
	} else {
	    /* user escaped out */
	    erase_line(msg_line, msg_line);
	}
    } while (!valid);

    return return_value;
}; /* end eb__get_entry */

//////////////////////////////////////////////////////////////////////

void eb__dep_munny(integer mon_type)
{
    integer    deposit;
    vtype      out_val;

    if (py.misc.money[mon_type] > 0) {
	do {
	    sprintf(out_val,"How much %s to deposit?",coin_name[mon_type]);
	    if(eb__get_entry(out_val, &deposit)) {
		if (deposit > py.misc.money[mon_type]) {
		    sprintf(out_val,"You do not have that much %s!",
			    coin_name[mon_type]);
		    msg_print(out_val);
		}
	    }
	} while (deposit > py.misc.money[mon_type]);

	if (deposit > 0) {
	    bank[mon_type] += deposit;
	    py.misc.money[mon_type] -= deposit;
	    inven_weight -= COIN_WEIGHT * deposit;
	    py.misc.account += trunc(deposit*BANK_SKIM*coin_value[mon_type]) div GOLD_VALUE;
	    eb__display_money();
	}
    }
}; /* end eb__dep_munny */

//////////////////////////////////////////////////////////////////////

void eb__deposit_money()
{
/*{ Deposit a given number of mithril, platinum, and gold in the bank, } */
/*{ but the bank takes its percentage                      -ADW-MKC-   } */

    eb__dep_munny(MITHRIL);
    eb__dep_munny(PLATINUM);
    eb__dep_munny(GOLD);
    reset_total_cash();
    eb__display_money();

}; /* end eb__deposit_money */

//////////////////////////////////////////////////////////////////////

void eb__withdraw_money()
{
/*{ Withdraw a given amount, in gold pieces, teller makes change. }*/
/*{ Bank may not have enough!                              -MKC-  }*/

    boolean   deliver,is_some;
    integer   amt_given[MITHRIL+1];
    integer   mon_type,withdraw,weight_left;
    string    out_val;

    do { /*{ get amount to withdraw }*/
        if (eb__get_entry("How much money to withdraw (in gold)?",&withdraw)) {
	    if(withdraw > py.misc.account) {
		msg_print("You do not have that much!");
		msg_print("You should keep better track of your account!");
	    }
	}
    } while (withdraw > py.misc.account);

/*{ The amount actually given is the minimum of how much the user wants, }*/
/*{ how much the bank has, and how much the user can carry.              }*/
    if(withdraw>0) {
	weight_left = (weight_limit()*100)-inven_weight;
	for (mon_type = MITHRIL; mon_type >= GOLD; mon_type--) {
	    amt_given[mon_type] = min3((withdraw*GOLD_VALUE) div coin_value[mon_type],
					bank[mon_type],
					weight_left div COIN_WEIGHT);
	    weight_left -= amt_given[mon_type]*COIN_WEIGHT;
	    withdraw -= amt_given[mon_type]*(coin_value[mon_type] div GOLD_VALUE);
	} /* end for mon_type */
	
        deliver = true;
	is_some = ((amt_given[MITHRIL]+amt_given[PLATINUM]+amt_given[GOLD])>0);
	
	if (withdraw>0) {  
	    /*{ if unable to give the entire amount }*/

	    if (weight_left<=0) {  /*{ they can't carry it all }*/
		if (is_some) {
		    msg_print("You cannot carry it all.");
		    deliver=get_yes_no("Do you want as much as you can carry?");
		} else{
		    msg_print("You cannot carry any more money.");
		    deliver=false;
		}
	    } else { /* { the bank doesn't have enough money }*/
		if (is_some) {
		    msg_print("We are rather short of cash today.");
		    deliver=get_yes_no("Do you want as much as you can get?");
		} else {
		    msg_print("We do not have the cash to give you.");
		    msg_print("Try again tomorrow.");
		    deliver=false;
		}
	    } /* endif weight_left */
	} /* withdraw */

        if (deliver) {
	  for (mon_type = MITHRIL; mon_type >= GOLD; mon_type--) {
	    if (amt_given[mon_type] > 0)
	      {
		sprintf(out_val,"The teller gives you %ld %s piece%s."
			,amt_given[mon_type],coin_name[mon_type],
			(amt_given[mon_type] == 1) ? "" : "s");
		msg_print(out_val);
		py.misc.money[mon_type] += amt_given[mon_type];
		bank[mon_type] -= amt_given[mon_type];
		py.misc.account -= amt_given[mon_type]*coin_value[mon_type]
		  div GOLD_VALUE;
	      }
	  } /* end for */
	  inven_weight = weight_limit()*100-weight_left;
	  reset_total_cash();
	  eb__display_money();
	} /* end if deliver */
    } /* end if withdraw */
}; /* end eb__withdraw_money */

//////////////////////////////////////////////////////////////////////

void eb__safe_deposit(boolean deposit)
{
    /* XXXX major work. I don't think it ever worked at the U */
    prt("The dwarves are still installing it, sorry.",1,1);
}; /* end eb__safe_deposit */

//////////////////////////////////////////////////////////////////////

void eb__change_money()
{
/*{  Changes money of one type to money of another type.        -JPS- }*/

    boolean    change_flag;   /*{ Did they enter a valid entry? }*/
    integer    amount_from;   /*{ Amount before changing. }*/
    integer    amount_to;     /*{ Amount remaining after changing. }*/
    char       key_in;	      /*{ input character }*/
    integer    typ_from,typ_to;/*   { Types of money }*/
    string     prompt;        /*{ Prompt used.}*/

      key_in = (char)get_money_type("Change what coin? ",&change_flag,false);
      if (change_flag) {
	  coin_stuff(key_in,&typ_from);
	  key_in = (char)get_money_type("Change to? ",&change_flag,true);
      }
      if (change_flag) {
	  coin_stuff(key_in, &typ_to);
	  sprintf(prompt, "Number of coins to change? (1-%ld)",
		  py.misc.money[typ_from]);
	  change_flag = eb__get_entry(prompt, &amount_from);
      }
      if (change_flag) {
          amount_to = (amount_from * coin_value[typ_from]) div
	      coin_value[typ_to]; /*{NO surcharge}*/
	  if (amount_to == 0) {
	      msg_print("You don't have enough to trade for that type of coin!");
	  } else if (amount_to > bank[typ_to]) {
	      msg_print("The bank doesn't have enough of that kind of coin!");
	  } else if (py.misc.money[typ_from] < amount_from) {
	      msg_print("You don't have enough of that coin!");
          } else if ((  inven_weight + COIN_WEIGHT*(amount_to-amount_from) )
		     > weight_limit()*100) {
	      msg_print("You can't carry that much weight.");
	  } else {
	      py.misc.money[typ_from] -= amount_from;
	      bank[typ_from]          += amount_from;
	      py.misc.money[typ_to]   += amount_to;
	      bank[typ_to]            -= amount_to;
	      inven_weight += COIN_WEIGHT*(amount_to-amount_from);
	      msg_print("The money changer hands you your money.");
	      eb__display_money();
	  }
      } /* endif change_flag */

}; /* end eb__change_money */

//////////////////////////////////////////////////////////////////////

void eb__parse_command(boolean *exit_flag, vtype shop_owner)
{
    char      command;

    *exit_flag = false;

    if (get_com(" ", &command)) {
	switch (command) {
	case CTRL_R: eb__display_store(shop_owner); break;
	case 'd':    eb__deposit_money(); break;
	case 'w':    eb__withdraw_money(); break;
	case 'c':    eb__change_money(); break;
	case 'i':    prt("The insurance shop has gone out of business.",1,1);
	    break;
	case 'p':    eb__safe_deposit(true); break;
	case 'r':    eb__safe_deposit(false); break;
	default:     prt( "Invalid Command.", 1, 1 ); break;
	    
	} /* end switch */
    } else {
	*exit_flag = true;
    }

}; /* end eb__parse_command */

//////////////////////////////////////////////////////////////////////

void enter_bank()
{
  boolean    exit_flag = false;
  integer    tics = 1;
  vtype      shop_owner;
  
  switch (randint(7)) {
  case 1 : 
    strcpy(shop_owner,"Milton Drysdale      (tightwad)      Bank");break;
  case 2 : 
    strcpy(shop_owner,"Mr. Potter           (slumlord)      Bank");break;
  case 3 :
    strcpy(shop_owner,"Ebeneezer Scrooge    (broker)        Bank");break;
  case 4 :
    strcpy(shop_owner,"Scrooge McDuck       (avian)         Bank");break;
  case 5 :
    strcpy(shop_owner,"Andrew Mellon        (treasury)      Bank");break;
  case 6 :
    strcpy(shop_owner,"Loony Looby          (pizza!)        Bank");break;
  case 7 :
    strcpy(shop_owner,"Ram the Booger Eater (Nosepicker)    Bank");break;
    }
  eb__display_store(shop_owner);
  
  do {
    eb__parse_command(&exit_flag, shop_owner);
    adv_time(false);
    tics++;
    check_kickout_time(tics,2);
  } while(!exit_flag);
  draw_cave();
  
}; /* end enter_bank */


/* end file bank.c */
