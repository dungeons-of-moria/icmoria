/* bj.c */
/* blackjack code for the casino */

#include "imoria.h"
#include "casino.h"
#include "bj.h"

boolean    deal_bust;
boolean    card5_save,card5;
drawcard   dummy,dummyd;
hand       dealerh,playerh;
integer    vald,valp,save;
boolean    bust_flag,bust_save;
boolean    split_flag,already_split;
integer    py_index;
integer    hand_start;
boolean    win_draw;
boolean    blackjack,blackjack_save;
boolean    pl_stay_flag;
boolean    dl_ace_flag;
boolean    double_flag,double_flag_save;
 
 
void bj__display_bj()
{ 
  clear_rc(21,1);
  c__display_gold();
  prt("You may:",22,2);
  prt(" p) place a bet.                         v) view the rules",23,2);
  prt("^R) Redraw the screen.                 Esc) Exit from building.",24,2);
};
 
 
void bj__display_bj_game()
{
  clear_rc(21,1);
  prt("Your hand: ",5,1);
  prt("Dealer's hand: ",12,1);
  c__display_gold();
  prt("You may: ",21,1);
  prt(" s) stand.                             h) hit.",22,2);
  prt(" d) double down.                       /) split.",23,2);
  prt("^R) Redraw the screen.                 v) view the rules. ",24,2);}
;
 
void bj__opening_screen()
{
  /*     1         2         3         4         5         6               */
  /*   890123456789012345678901234567890123456789012345678901234567890     */
  prt(                                     "____",              2,45);
  prt(                                    "|A   | ____",        3,44);
  prt(           "XXX                      |    ||3   | ____",  4,19);
  prt(           "X  X  X                  |   A||    ||7   |", 5,19);
  prt(           "XXX   X     XX            ---- |   3||    |", 6,19);
  prt(           "X  X  X    X  X XXXX            ---- |   7|", 7,19);
  prt(           "XXX   X    XXXX X    X  X             ----",  8,19);
  prt(                 "XXXX X  X X    X X     X",              9,25);
  prt(                      "X  X X    XX      X  XX",          10,30);
  prt(     "____                  XXXX X X     X X  X XXXX",    11,13);
  prt(    "|A   | ____                 X  X X  X XXXX X    X  X",12,12);
  prt(    "|    ||J   |                     XXXX X  X X    X X",13,12);
  prt(    "|   A||    |                          X  X X    XX", 14,12);
  prt(     "---- |   J|                               XXXX X X",15,13);
  prt(           "----                                     X  X",16,19);
};
 
 
 
void bj__display_rules()
{ 
    char     command;
    boolean  exit;
 
  clear_screen();
  prt("MORIA   BLACKJACK   RULES ",2,21);
  prt("The object, of course, is to get as close as you can to 21 without",5,2);
  prt("going over.  Aces can count as either one or eleven, while all",6,2);
  prt("face cards count as ten.  The other cards are worth their face",7,2);
  prt("value.",8,2);
  prt("Once you have played your hand, the dealer will draw cards until",10,2);
  prt("he has at least 17.  However, if his total is 17 and he has an",11,2);
  prt("ace, he will hit. ",12,2);
  prt("Splitting and Doubling:",14,25);
  prt("After drawing your first two cards, you have the option of",16,2);
  prt("doubling your bet and taking one, and only one more card. ",17,2);
  prt("If your first two cards are the same, you may split, in which case",19,2);
  prt("you will then play out both hands, each with the same bet as the",20,2);
  prt("original hand.  Splitting is only allowed once per hand.",21,2);

  prt("[hit any key to continue]",24,23);
  exit = get_com("",&command);

  clear_rc(2,1);
  prt("A natural pays  3/2  times your bet, unless the dealer also has",3,2);
  prt("a blackjack, in which case you push.  The dealer will not offer ",4,2);
  prt("you insurance.",5,2);
  prt("If you draw five cards, without going over 21, you automatically",7,2);
  prt("win, regardless of the dealer's hand.",8,2);

  prt("[hit any key to continue]",24,23);
  exit = get_com("",&command);
};
 
 
 
void bj__initialize_hand()
{
    integer    i;
 

    for (i = 1; i <= 10; i++) {
	dealerh[i] = 0;
	strcpy(dummy[i]," ");
	playerh[i] = 0;
	strcpy(dummyd[i]," ");
    }

    bust_flag      = false;
    hand_start     = 1;
    pl_stay_flag   = false;
    card5          = false;
    blackjack      = false;
    py_index       = 2;
    win_draw       = false;
    already_split  = false;
    split_flag     = false;
    double_flag    = false;
};
 
 
 
void bj__evaluate_pl_hand()
{
    integer    i;
    boolean    py_ace_flag;
 

  bust_flag = false;
  py_ace_flag = false;
  valp = 0;

  for (i = hand_start; i <= hand_start + 4; i++) {

      if (playerh[i] == 14) {  /* an ace is worth 1 or 11 */
	  valp += 1;
	  py_ace_flag = true;
      } else if (playerh[i] > 10) {
	  valp += 10;
      } else {
	  valp += playerh[i];
      }
      
  }

  if ((valp < 12) && (py_ace_flag)) {
      valp += 10;                /* turn an ace into an 11 */
  }
  
  if ((valp > 21)) {
      bust_flag = true;
  }
};
 
 
 
void bj__evaluate_dl_hand(integer index)
{
    integer    i;
    
    deal_bust   = false;
    dl_ace_flag = false;
    vald = 0;
    
    for (i = 1; i <= index; i++) {
	if (dealerh[i] == 14) {
	    vald += 1;
	} else if (dealerh[i] > 10) {
	    vald += 10;
	} else {
	    vald += dealerh[i];
	}
	
	if ((dealerh[i] == 14) &&  (vald < 12)) {
	    dl_ace_flag = true;
	    vald += 10;
	}
	if ((vald > 21) && dl_ace_flag) {
	    vald -= 10;
	    dl_ace_flag = false;
	}
    }
    
    if (vald > 21) {
	deal_bust = true;
    }
};
 
 
 
void bj__hand_save()
{
      save             = valp;
      bj__evaluate_pl_hand();
      pl_stay_flag     = false;
      card5_save       = card5;
      card5            = false;
      if (double_flag) {
	  bet /= 2;
      }
      double_flag_save = double_flag;
      double_flag      = false;
      blackjack_save   = blackjack;
      blackjack        = false;
      hand_start       = 6;
      already_split    = true;
      py_index         = 7;
      bust_save        = bust_flag;
      bust_flag        = false;
};
 
 
 
void bj__check_exit(boolean *exit_flag)
{
  if (double_flag) {
      *exit_flag = true;
  }

  if ((valp == 21) && ((py_index == 2) || (py_index == 7))) {
        *exit_flag = true;
        blackjack  = true;
  }

  if (pl_stay_flag) {
      *exit_flag = true;
  }

  if ((py_index == 5) || (py_index == 10)) {
     *exit_flag = true;
     card5      = true;
  }

  if (bust_flag) { 
      *exit_flag = true;
  }

  if (split_flag && (*exit_flag) && !already_split) {
      bj__hand_save();
      msg_print("Now play the hand on the right.");
      *exit_flag = false;
  }
};
 
 
void bj__get_first_dealc()
{
    vtype      draw;
 
    strcpy(draw, " ____");
    put_buffer(draw,13,8);
    strcpy(draw, "|\\  /|");
    put_buffer(draw,14,8);
    strcpy(draw, "|-**-|");
    put_buffer(draw,15,8);
    strcpy(draw, "|/  \\|");
    put_buffer(draw,16,8);
    strcpy(draw, " ----");
    put_buffer(draw,17,8);
};
 
 
void bj__card_draw (integer index, integer r, vtype card)
{
    integer    c;
    vtype      draw;
    
    
    if (strcmp(card, " ")) {
	if ((r == 13) && (index == 1) && (!win_draw)) {
	    bj__get_first_dealc();
	} else {
	    c = 1 + 7*index;
	    sprintf(draw, " ____");
	    put_buffer(draw,r,c);
	    sprintf(draw, "|    |");
	    put_buffer(draw,r+1,c);
	    sprintf(draw, "|    |");
	    put_buffer(draw,r+2,c);
	    sprintf(draw,"|    |");
	    put_buffer(draw,r+3,c);
	    sprintf(draw, " ----");
	    put_buffer(draw,r+4,c);

	    sprintf(draw,"%s",card);
	    put_buffer(draw,r+1,c+1);

	    if (!strcmp(card,"10")) {
		put_buffer(draw,r+3,c+3);  /* right justify the ten */
	    } else {
		put_buffer(draw,r+3,c+4);
	    }
	}
    }
}; 
 
 
void bj__re_draw()
{
    integer   i;
 

    clear_screen();
    prt("Your hand:",5,1);
    prt("Dealer's hand: ",12,1);
    for (i = 1; i <= 10; i++) {
      bj__card_draw(i,6,dummy[i]);
      bj__card_draw(i,13,dummyd[i]);
    }
    if (bet>0) {
	bj__display_bj_game();
    } else {
	bj__display_bj();
    }
};
 
 
void bj__get_dealer_card(integer i)
{

  dealerh[i] = randint(13)+1;

  switch (dealerh[i]) {
  case 2  :  strcpy(dummyd[i], "2");   break;
  case 3  :  strcpy(dummyd[i], "3");   break;
  case 4  :  strcpy(dummyd[i], "4");   break;
  case 5  :  strcpy(dummyd[i], "5");   break;
  case 6  :  strcpy(dummyd[i], "6");   break;
  case 7  :  strcpy(dummyd[i], "7");   break;
  case 8  :  strcpy(dummyd[i], "8");   break;
  case 9  :  strcpy(dummyd[i], "9");   break;
  case 10 :  strcpy(dummyd[i],"10");   break;
  case 11 :  strcpy(dummyd[i], "J");   break;
  case 12 :  strcpy(dummyd[i], "Q");   break;
  case 13 :  strcpy(dummyd[i], "K");   break;
  case 14 :  strcpy(dummyd[i], "A");   break;
  }
  bj__card_draw(i,13,dummyd[i]);
  bj__evaluate_dl_hand(i);
};
 
 
 
void bj__get_player_card(integer i)
{ 
  playerh[i] = randint(13)+1;
  switch (playerh[i]) {
  case 2  :  strcpy(dummy[i], "2");   break;
  case 3  :  strcpy(dummy[i], "3");   break;
  case 4  :  strcpy(dummy[i], "4");   break;
  case 5  :  strcpy(dummy[i], "5");   break;
  case 6  :  strcpy(dummy[i], "6");   break;
  case 7  :  strcpy(dummy[i], "7");   break;
  case 8  :  strcpy(dummy[i], "8");   break;
  case 9  :  strcpy(dummy[i], "9");   break;
  case 10 :  strcpy(dummy[i],"10");   break;
  case 11 :  strcpy(dummy[i], "J");   break;
  case 12 :  strcpy(dummy[i], "Q");   break;
  case 13 :  strcpy(dummy[i], "K");   break;
  case 14 :  strcpy(dummy[i], "A");   break;
  }
  bj__card_draw(i,6,dummy[i]);
  bj__evaluate_pl_hand();
};
 
 
 
void bj__get_dealer_hand()
{
    bj__get_dealer_card(1);
    bj__get_dealer_card(2);
};
 
 
void bj__get_player_hand()
{
    bj__get_player_card(1);
    py_index = 2;
    bj__get_player_card(2);
}
 
 
void bj__get_winning()
{ 
 
    if (deal_bust) {
	gld += 2*bet;
	switch (randint(4)) {
	case 4:
	    msg_print("The dealer busts."); break;
	default: 
	    msg_print("The dealer busts, you win."); break;
	}
    } else {
	if (vald > valp) {
	    switch (randint(5)) {
	    case 4 : msg_print("Now you know why gambling is illegal."); break;
	    case 5 : msg_print("If you had just taken one more card..."); break;
	    default : msg_print("You lose."); break;
	    }
	} else if (vald == valp) {
	    msg_print("You push");
	    gld += bet;
	} else {
	    switch (randint(4)) {
	    case 4  : msg_print("You're not card-counting are you?"); break;
	    default  : msg_print("You win.");  break;
	    }
	    gld += bet*2;
	}
    }
}; 

void bj__play_dealer_hand()
{
    integer   i;
    boolean   stay_flag;
 
    
    stay_flag = false;
    win_draw = true;
    bj__card_draw(1,13,dummyd[1]);
    i = 3;
    if (((vald == 17) && (dl_ace_flag)) || (vald < 17)) {
	do {
	  put_qio();
	  sleep(1);
	  bj__get_dealer_card(i);
	  i++;
	  if (vald > 16) {
	    stay_flag = true;
	  }
	  if ((vald == 17) && (dl_ace_flag)) {
	    stay_flag = false;
	  }
	  if (i > 9) {
	    stay_flag = true;
	  }
	} while (!stay_flag);
    }
    bj__get_winning();
};
 
 
 
void bj__double()
{
    bet *= 2;
    py_index++;
    bj__get_player_card(py_index);
    double_flag = true;
};
 
 
 
void bj__split()
{
  playerh[6] = playerh[1];
  strcpy(dummy[6], dummy[1]);
  bj__card_draw(6,6,dummy[6]);
  bj__get_player_card(2);
  bj__get_player_card(7);
  split_flag = true;
};
 
 
 
 
void bj__get_game_command()
{
    char     command;
    integer  com_val = 0;

    if (get_com("", &command)) {
       com_val   = command;
       switch (com_val) {
       case 104 :                                     /* hit */
	   py_index++;
	   bj__get_player_card(py_index);
	   break;
	   
       case 115 :
	   pl_stay_flag = true;                      /* stay */
	   break;
	   
       case 18 : 
	   bj__re_draw();
	   break;
	   
       case 118 :
	   bj__display_rules();
	   bj__re_draw();
	   break;

       case 100 :                                     /* double */
	   if (gld >= bet) { 
	       if ((py_index == 2) || (py_index == 7)) {
		   gld -= bet;
		   c__display_gold();
		   bj__double();
	       }
	   }
	   break;

       case 47 :                                      /* split */
	   if ((!split_flag) && (gld >= bet) &&
	       (py_index == 2) && (playerh[1] == playerh[2])) {
	       gld -= bet;
	       c__display_gold();
	       bj__split();
	       msg_print("First play your hand on the left.");
	   }
	   break;

       default:
	 prt("Invalid Command.",1,1);
	 break;
       }
    }
};
 
 
void bj__play_hand()
{
  integer    num,i;
  
  num = 1;
  if (split_flag) {
    msg_print("As for your second hand...");
    num = 2;
  }
  
  for (i = 1; i <= num; i++) {
    
    if (i == 2) {
      msg_print ("As for your first hand...");
    }
    if ((i == 2) && double_flag) {
      bet /= 2;
    }
    if ((i == 2) && double_flag_save) {
      bet *= 2;
    }
    
    if (!(card5 || bust_flag || blackjack)) {
      bj__play_dealer_hand();
    } else {
      if (card5 && !bust_flag) {
	gld += 2*bet;
	msg_print("Five cards, you win.");
      }
      if (bust_flag) {
	switch (randint(3)) {
	case 1: case 3 :  msg_print("You bust.");              break;
	case 2:           msg_print("You should have stood."); break;
	}
      }
      if (blackjack) {
	attron(A_REVERSE);
	msg_print("Blackjack!");
	attroff(A_REVERSE);
	win_draw = true;
	bj__card_draw(1,13,dummyd[1]);
	if (vald == 21) {
	  msg_print("The dealer also has 21 ... You push.");
	  gld += bet;
	} else {
	  gld += 2*bet + bet div 2;
	}
      }
    }
    valp      = save;
    bust_flag = bust_save;
    blackjack = blackjack_save;
    card5     = card5_save;
    c__display_gold();
  }
  clear_rc(21,1);
  bet = 0;
};
 
 
void bj__play_bj()
{
  boolean   exit_flag = false;
 
  clear_screen();
  bj__display_bj_game();
  bj__initialize_hand();
  bj__get_dealer_hand();
  bj__get_player_hand();
  do {
    if (!((valp==21) && ((py_index==2)||(py_index==7)))) {
      bj__get_game_command();
    }
    bj__check_exit(&exit_flag);
  } while (!exit_flag);
  bj__play_hand();
};

 
 
void bj__get_bj_bet()
{
  vtype    comment;
  integer  num;
  boolean  exit_flag = false;

  strcpy(comment, "How much would you like to bet(50 to 1000 gp)? ");

  do {
    if (c__get_response(comment, &num)) {
      bet = num;
      if ((bet>49) && (bet<1001)) {
	exit_flag = true;
      } else {
	prt("Improper value.",1,1);
      }
    } else {
      exit_flag = true;
      bet = 0;
    }

  } while (!exit_flag);

  if (bet > gld) {
    prt("You have not the gold!",1,1);
    bet = 0;
  }
  gld -= bet;
  c__display_gold();
};
 
 
 
void bj__blackjack_commands()
{
  char     command;
  boolean  exit_flag = false;
 
  bet = 0;
  win_draw = false;
  do {
    if (get_com("", &command)) {
      switch (command) {

      case 112   :
	bj__get_bj_bet();
	if (bet > 0) {
	  bj__play_bj();
	  bj__display_bj();
	}
	break;

      case 118   :
	bj__display_rules();
	bj__re_draw();
	break;

      case 18    :
	clear_screen();
	bj__re_draw();
	break;

      default    :
	prt("Invalid Command.",1,1);
	break;
      }
    } else {
      exit_flag  = true;
    }
    c__check_casino_kickout();
  } while (!exit_flag);
};
 
 
void bj__game_blackjack()
{
  clear_screen();
  bj__display_bj();
  bj__initialize_hand();
  bj__opening_screen();
  prt("The dealer awaits your bet.",1,1);
  bj__blackjack_commands();
};
 
 
 


/* END FILE bj.c */
