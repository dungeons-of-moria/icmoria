/* horse.c */
/* horse race code for the casino */

#include "imoria.h"
#include "casino.h"
#include "horse.h"


void hr__display_commands()
{
  c__display_gold();
  prt("You may:",20,1);
  prt(" p) place a bet                      v) view the program.",21,2);
  prt(" s) skip a race.                   Esc) Exit horseracing.",22,2);
  prt("^R) Redraw the screen.",23,2);
}
 
 
void hr__display_track3()
{
  clear_screen();
  put_buffer("-- -- -- -- -- -- -- -- -- -- -- -- -- -- __ __",3,1);
  put_buffer("-_",4,51);  put_buffer("-_",5,55);  put_buffer("-_",6,58);
  put_buffer("-_",7,61);  put_buffer("-_",8,63);  put_buffer("-_",9,65);
  put_buffer("-_",10,67); put_buffer("-_",11,69); put_buffer("_",12,71);
  put_buffer("_",13,72);  put_buffer("_",14,73);  put_buffer("_",15,74);
  put_buffer("_",16,75);  put_buffer("-- -- -- -- -- -- -- -- -- -- __ ",16,1);
  put_buffer("-_",17,35); put_buffer("-_",18,38); put_buffer("-_",19,41);
  put_buffer("-_",20,43); put_buffer("_",21,45);  put_buffer("_",22,46);
  put_buffer("_",23,46);  put_buffer("_",17,76);  put_buffer("_",18,77);
  put_buffer("_",19,77);  put_buffer("_",20,77);  put_buffer("_",21,77);
  put_buffer("_",22,77);  put_buffer("_",23,77);
  put_qio();
}
 
 
void hr__display_track2()
{
  clear_screen();
  put_buffer("_",2,46);   put_buffer("_",3,46);   put_buffer("_",4,45);
  put_buffer("_-",5,43);  put_buffer("_-",6,41);  put_buffer("_-",7,38);
  put_buffer("__",8,34);  put_buffer("_",2,77);   put_buffer("_",3,77);
  put_buffer("_",4,77);   put_buffer("_",5,77);   put_buffer("_",6,77);
  put_buffer("_",7,77);   put_buffer("_",8,77);   put_buffer("_",9,77);
  put_buffer("__ __ __ __ __ __ __ __ __ __ --",9,1);
  put_buffer("_",10,76);  put_buffer("_",11,75);  put_buffer("_",12,73);
  put_buffer("_",13,71);  put_buffer("_-",14,69); put_buffer("_-",15,67);
  put_buffer("_-",16,65); put_buffer("_-",17,63); put_buffer("_-",18,60);
  put_buffer("_-",19,57); put_buffer("_-",20,53); put_buffer("_-",21,49);
  put_buffer("__ __ __ __ __ __ __ __ __ __ __ __ __ -- -- --",22,1);
  put_qio();
}
 
 
void hr__display_track4()
{
  clear_screen();
  put_buffer("__ __ -- -- -- -- -- -- -- -- -- -- -- -- -- -- --",3,29);
  put_buffer("_-",4,25);  put_buffer("_-",5,22);  put_buffer("_-",6,19);
  put_buffer("_-",7,17);  put_buffer("_-",8,15);  put_buffer("_-",9,13);
  put_buffer("_-",10,11); put_buffer("_-",11,9);  put_buffer("_",12,8);
  put_buffer("_",13,7);   put_buffer("_",14,6);   put_buffer("_",15,5);
  put_buffer("_",16,4);   put_buffer("__ -- -- -- -- -- -- -- -- -- --",16,46);
  put_buffer("_-",17,43); put_buffer("_-",18,40); put_buffer("_-",19,37);
  put_buffer("_-",20,35); put_buffer("_",21,34);  put_buffer("_",22,33);
  put_buffer("_",23,33);  put_buffer("_",17,3);   put_buffer("_",18,2);
  put_buffer("_",19,2);   put_buffer("_",20,2);   put_buffer("_",21,2);
  put_buffer("_",22,2);   put_buffer("_",23,2);
  put_qio();
}
 
void hr__display_track5()
{
  clear_screen();
  put_buffer("_",2,3);    put_buffer("_",3,3);    put_buffer("_",4,3);
  put_buffer("_",5,3);    put_buffer("_",6,3);    put_buffer("_",7,3);
  put_buffer("_",8,3);    put_buffer("_",2,32);   put_buffer("_",3,32);
  put_buffer("_",4,33);   put_buffer("-_",5,34);  put_buffer("-_",6,36);
  put_buffer("-_",7,39);  put_buffer("__",8,43);  put_buffer("_",9,4);
  put_buffer("-- __ __ __ __ __ __ __ __ __ __",9,46);
  put_buffer("_",10,5);   put_buffer("_",11,6);   put_buffer("_",12,7);
  put_buffer("_",13,8);   put_buffer("-_",14,9);  put_buffer("-_",15,11);
  put_buffer("-_",16,13); put_buffer("-_",17,15); put_buffer("-_",18,18);
  put_buffer("-_",19,21); put_buffer("-_",20,25); put_buffer("-_",21,29);
  put_buffer("-- -- __ __ __ __ __ __ __ __ __ __ __ __ __",22,34);
  put_qio();
}
 
void hr__display_track1()
{
  clear_screen();
  put_buffer("__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __",2,1);
  put_buffer("__ __ __ __ __ __ __",2,58);
  put_buffer("__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __",16,1);
  put_buffer("__ __ __ __ __ __ __",16,58);
  put_qio();
}
 
 
void hr__get_horse_stats(integer *num_horse, h_name horse, h_stat hstat,
			 integer *sum_bets, h_stat bets, h_stat true_bets,
			 statr odds, statr true_odds, integer *true_sum_bets)
{
  integer    i1,i2;
  boolean    horse_not_chosen[MAX_HORSE_NAMES+1];

  h_name horsename = 
    {"bogus horse","Man 'o War","Secretariat","Seattle Slew","Moksha",
     "Beast Master","Prince of Darkness","Beelzebub",
     "Black Shadow","Gargle Blaster","George Jetson","Covenant",
     "Turiya","Neysa","Sure Thing","Battle Cruiser","Mobie",
     "Arthur Dent","Mr. Creosote","Shadowfax","Bet Twice",
     "I'm a Lumberjack","Dr. Science","Harry The Horse",
     "Tiberius","Hellfire","Mephistopheles","Belial","Pluto",
     "Firesweeper","Cloudminder","Spectacular Bid","Noor",
     "Affirmed","Citation","Dr. Fagen","War Admiral","Epitaph",
     "Not A Chance","Death Pizza","Relay(Cop's)Horse","Nightmare",
     "Excelsior","Paul Revere","Myxilplick","Mercury","Robohorse",
     "Hellfire","Bladerunner","Arch Mage","Shadow Runner",
     "Golden Hoof","Necromancer","Heirophant","Jehannum","Transwarp"};
 
  h_stat horsestat = 
  {99,25,30,28,22,21,24,23,22,23,20,26,23,30,23,19,23,22,19,27,
   25,23,28,24,26,22,25,21,22,26,24,28,27,29,29,28,26,26,17,
   25,18,26,31,28,23,28,26,25,27,29,25,23,26,27,26,27};

  *num_horse = 5 + randint(4);

  for (i1 = 1; i1 <= MAX_HORSE_NAMES; i1++) {
    horse_not_chosen[i1] = true;
  }

  for (i1 = 1; i1 <= *num_horse; i1++) {
    do {
      i2 = randint(MAX_HORSE_NAMES);
    } while (!horse_not_chosen[i2]);
    strcpy(horse[i1], horsename[i2]);
    hstat[i1] = 2*horsestat[i2]-randint(2);
    horse_not_chosen[i2] = false;
  }

  i2 = randint(250) + randint(250);
  *sum_bets = 0;
  for (i1 = 1; i1 <= *num_horse; i1++) {
    bets[i1]  = pow((hstat[i1]-20),4) / i2;
    *sum_bets += bets[i1];
  }

  for (i1 = 1; i1 <= *num_horse; i1++) {
    odds[i1] = 0.95 * (*sum_bets)/bets[i1] - 1.0;
  }

  i1 = randint(*num_horse);
  hstat[i1] += randint(2);
  i1 = randint(*num_horse);
  hstat[i1] += randint(2);
  i1 = randint(*num_horse);
  hstat[i1] -= randint(5);

  *true_sum_bets = 0;
  for (i1 = 1; i1 <= *num_horse; i1++) {
      true_bets[i1] = pow((hstat[i1]-20),4) / i2;
      (*true_sum_bets) += true_bets[i1];
  }

  for (i1 = 1; i1 <= *num_horse; i1++) {
    true_odds[i1] = 0.95 * (*true_sum_bets)/true_bets[i1] - 1.0;
  }
};
 
 
integer hr__move(real amount)
{
  integer return_value = 0;

  if ((amount/10) >= 1) { return_value = 1; }
  if ((amount/100) >= 1) { return_value = 2; }
  if ((amount/1000) >= 1) { return_value = 3; }

  return return_value;
};
 
 
void hr__list_horses(integer num_horse, h_name horse, statr odds)
{
  /*  list horses + odds for the next race */
  integer   i1;
  vtype     out_val;
  char      command;

  clear_rc(2,1);
  for (i1 = 1; i1 <= num_horse; i1++) {
    prt("odds",3,36);
    sprintf(out_val,"%2ld.   %s",i1,horse[i1]);
    prt(out_val,4+i1,3);
    if (odds[i1] < 0.2) { odds[i1] = 0.2; }
    if (odds[i1] >= 4.75) { 
      sprintf(out_val,"%3d",(int)(odds[i1]+0.5)); 
    } else if (odds[i1] < 0.90) {
      sprintf(out_val,"%d/5",(int)(5*odds[i1]+0.5));
    } else if ((int)(odds[i1]*2+0.5) % 2 == 0) {
      sprintf(out_val,"%3d",(int)(odds[i1]+0.25));
    } else {
      sprintf(out_val,"%d/2",(int)(odds[i1]*2+0.5));
    }
    prt(out_val,4+i1,37);
  }
  
  if (bet == 0) {
    prt("[hit any key to continue]",23,24);
    get_com("",&command);
    clear_rc(2,1);
    hr__display_commands();
  }
};


void hr__print_positions(integer row, integer col, integer horse_bet,
			 h_name horse, h_stat fastest)
{
  vtype      out_val;
  integer    i1;

  for (i1 = 1; i1 <= 4; i1++) {
    if (horse_bet == fastest[i1]) {
      put_buffer("-->",row-1+i1,col);
    } else {
      put_buffer("   ",row-1+i1,col);
    }
    sprintf(out_val,"%ld  %s",fastest[i1],horse[fastest[i1]]);
    put_buffer(out_val,row-1+i1,col+3);
  }
};

///////////////////

void s__start_race(integer num_horse)
{
  integer  i1;
  vtype    out_val;

  for (i1 = 1; i1 <= num_horse; i1++) {
    sprintf(out_val,"%ld",i1);
    put_buffer(out_val,i1+3,2);
  }

  msg_print("The horses are now entering the starting gate...");
  msg_print("And they're off!!");

  for (i1 = 1; i1 <= num_horse; i1++) {
    put_buffer("  ",3+i1,2);
  }
};

void s__get_positions(integer num_horse, integer win_count, h_stat dist,
		      h_stat fastest, h_stat order)
{
  integer   i1,i2,t;

  for (i1 = win_count+1; i1 <= num_horse-1; i1++) {
    for (i2 = num_horse-1 ; i2 >= i1; i2--) {
      if (dist[fastest[i2]] < dist[fastest[i2+1]]) {
	t             = fastest[i2];
	fastest[i2]   = fastest[i2+1];
	fastest[i2+1] = t;
      }
    }
  }
  for (i1 = win_count+1; i1 <= num_horse; i1++) {
    order[fastest[i1]] = i1;
  }
};

 
boolean s__find_plot(integer horse, integer *row, integer *col, h_stat dist,
		     integer screen_low, integer screen_high, h_stat lane)
{
  integer    t;
  boolean    return_value = false;

  t = dist[horse];
  if ((t >= screen_low) && (t <= screen_high)) {
    return_value = true;
    if (t <= 730) {
      *row = 3+ (horse*(750-t) + (lane[horse]*t) / 2) / 730;
    } else if (t > 3770) {
      *row = 8+lane[horse];
    } else if ((t <= 1090) || (t > 3410)) {
      *row = 11+lane[horse] / 2;
    } else if (t <= 1310) {
      *row = 3+(int)(cos((t-1090)/140)*(120+10*lane[horse])/15);
    } else if (t <= 1530) {
      *row = 23+(int)(cos((t-1090)/140)*(120+10*lane[horse])/15);
    } else if (t <= 2970) {
      *row = 14-lane[horse] / 2;
    } else if (t <= 3190) {
      *row = 23-(int)(cos((t-2970)/140)*(120+10*lane[horse])/15);
    } else {
      *row = 3 - (int)(cos((t-2970)/140)*(120+10*lane[horse])/15);
    }

    if (t <= 730) {
      *col = 1 + t / 10;
    } else if (t <= 1090) {
      *col = t / 10 - 72;
    } else if (t <= 1530) {
      *col = 37+(int)(sin((t-1090)/140)*(120+lane[horse]*10)/10);
    } else if (t <= 1890) {
      *col = 190 - t / 10;
    } else if (t <= 2610) {
      *col = 262 - t / 10;
    } else if (t <= 2970) {
      *col = 338 - t / 10;
    } else if (t <= 3410) {
      *col = 41-(int)(sin((t-2980)/140)*(120+lane[horse]*10)/10);
    } else if (t <= 3770) {
      *col = t / 10 - 300;
    } else { 
      *col = (t - 3761) / 10;
    }
  }
  return  return_value;
};

void s__predict_order(integer num_horse, statr true_odds, h_stat final_order,
		      h_stat final_fastest)
{
  integer   i1,i2,sum,t;
  h_stat    temp;
  
  for (i2 = 1; i2 <= num_horse; i2++) {
    final_order[i2] = 0;
  }
  for (i1 = 1; i1 <= num_horse; i1++) {
    sum = 0;
    for (i2 = 1; i2 <= num_horse; i2++) {
      /*{find chance of horse finishing in place i1}*/
      if (final_order[i2] == 0) {
	sum += (int)(10000*i1*true_odds[i2] /
		     ((i1*(i1+1)+2*true_odds[i2])*
		      (i1*(i1-1)+2*true_odds[i2])));
      }
      temp[i2] = sum;
    }
    t = randint(sum);
    i2 = 0;
    do {
      i2++;
    } while (t > temp[i2]);
    final_order[i2]   = i1;
    final_fastest[i1] = i2;
  }
};

void s__predict_race(integer num_horse, h_stat h_times[], h_stat final_fastest,
		     h_stat hstat)
{
  integer    t,t2,try_speedy,speedy,old_speedy,i1,i2,seg,pokey;

  old_speedy = 99999;

  for (i1 = 1; i1 <= num_horse; i1++) {
    i2 = 0;
    t  = final_fastest[i1];
    t2 = hstat[t]*hstat[t];
    speedy = 0;

    for (i2 = 1; i2 <= 6; i2++) {
      try_speedy = ((randint(t2)+randint(t2)+randint(t2)) / 100 +
		    2*(num_horse-i1+1));
      if ((try_speedy > speedy) && (try_speedy < old_speedy)) {
	speedy = try_speedy;
      }
    }

    if (speedy == 0) {
      speedy = old_speedy - randint(3) + 1;
    }
    old_speedy = speedy;

    /*{ horse run time = 6000 - 2*speedy; */
    /*  h_times[1..20] is time for each segment }*/

    h_times[0][t] = 0;

    for (i2 = 1; i2 <= 20; i2++) {
      h_times[i2][t] = 31 - speedy / 10; /*{time to move distance x}*/
    }

    pokey  = randint(randint(6));
    speedy = speedy % 10 + 10 + pokey*2;

    for (i2 = 1; i2 <= pokey; i2++) {
      /*{slowdown near end of race}*/
      seg = 21 - randint(randint(4));
      h_times[seg][t] += 4;
    }

    for (i2 = 1; i2 < speedy; i2++) {
      if (randint(2)==1) {
	seg = randint(20);
      } else {
	seg = (13+randint(5)+randint(5)) % 20 + 1;
      }
      h_times[seg][t] -= 2;
    }

    /*{make moves cumulative}*/
    for (i2 = 1; i2 <= 20; i2++) {
      h_times[i2][t] += h_times[i2-1][t];
    }
    h_times[21][t] = h_times[20][t] + 40;
  }
};

void hr__start(integer num_horse, integer *screen_high, integer *which_screen,
	       integer *win_count, integer *new_win_count, integer *screen_low,
	       statr true_odds, h_stat final_order, h_stat final_fastest,
	       h_stat fastest, h_stat hstat, h_stat area, h_stat dist,
	       h_stat order, integer *horse_bet, h_name horse, h_stat lane)
{
  /* row is the dependant variable,  column depends on horse stats  */

  h_stat     h_times[22];             /*  : array [0..21] of h_stat;*/
  integer    i1,row,col;
  boolean    exit_flag;
  integer    count;
  h_bool     still_racing;
  char       command;
  vtype      out_val;

  *which_screen = 1;
  *screen_high  = 0;
  *win_count = 0;
  *new_win_count = 0;
  count = 0; 

  s__predict_order(num_horse, true_odds, final_order, final_fastest);
  s__predict_race(num_horse, h_times, final_fastest, hstat);
  for (i1 = 1; i1 <= num_horse; i1++) {
    fastest[i1] = i1;
    area[i1] = 0;
    still_racing[i1] = true;
  }
  
  for (*which_screen = 1; *which_screen <= 7; (*which_screen)++) {
    *screen_low = *screen_high + 1;
    s__get_positions(num_horse, *win_count, dist, fastest, order);
    switch (*which_screen) {
    case 1 : 
      hr__display_track1();
      s__start_race(num_horse);
      *screen_high = 730;
      break;
      
    case 2 : 
      hr__display_track2();
      hr__print_positions(3,3,*horse_bet,horse,fastest);
      *screen_high = 1310;
      break;
      
    case 3 : 
      hr__display_track3();
      hr__print_positions(18,3,*horse_bet,horse,fastest);
      *screen_high = 1890;
      break;
      
    case 4 :
      *screen_high = 2610;
      hr__display_track1();
      hr__print_positions(18,30,*horse_bet,horse,fastest);
      break;
      
    case 5 :
      hr__display_track4();
      hr__print_positions(18,55,*horse_bet,horse,fastest);
      *screen_high = 3190;
      break;
      
    case 6 :
      hr__display_track5();
      hr__print_positions(3,55,*horse_bet,horse,fastest);
      *screen_high = 3770;
      break;
      
    case 7 :
      hr__display_track1();
      hr__print_positions(18,30,*horse_bet,horse,fastest);
      *screen_high = 4549; /*{but race stops at 4400}*/
      for (i1 = 3; i1 <= 15; i1++) {
	put_buffer("|",i1,63);
      }
      break;
    } /* end switch which_screen */
    
    for (i1 = 1; i1 <= num_horse; i1++) {
      switch (order[i1] % 4) {
      case 0 : lane[i1] = 2; break;
      case 1 : lane[i1] = 1; break;
      case 2 : lane[i1] = 3; break;
      case 3 : lane[i1] = 0; break;
      }
    }
    
    exit_flag = false;
    do {
      count += 2;
      for (i1 = 1; i1 <= num_horse; i1++) {
	if (s__find_plot(i1,&row,&col,dist,*screen_low,*screen_high,lane)) {
	  put_buffer(" ",row,col);
	}
	if ((count > h_times[area[i1]][i1]) && (area[i1] < 21)) {
	  area[i1] += 1;                 
	}
	dist[i1] = 220*area[i1]-((h_times[area[i1]][i1] - count)*220) /
	  (h_times[area[i1]][i1]-h_times[area[i1]-1][i1]) + randint(2);
	
	if (s__find_plot(i1,&row,&col,dist,*screen_low,*screen_high,lane)) {
	  if (i1 == *horse_bet) {
	    put_buffer("*",row,col);
	  } else {
	    sprintf(out_val,"%ld",i1);
	    put_buffer(out_val,row,col);
	  }
	}
	
	if (*which_screen < 7) {
	  if (dist[i1] > *screen_high) {
	    exit_flag = true;
	  }
	} else {
	  if ((dist[i1] > 4400) && still_racing[i1]) {
	    (*new_win_count)++;
	    still_racing[i1] = false;
	    put_buffer("|",row,63);
	  }
	}
      } /* end for i1 */
      
      if (*which_screen == 7) {
	if (*new_win_count > *win_count) {
	  s__get_positions(num_horse, *win_count, dist, fastest, order);
	  *win_count = *new_win_count;
	  exit_flag = (*win_count >= 4);
	}
      }
    } while (!exit_flag);
  } /* end for which_screen */
  
  for (i1 = 1; i1 <= num_horse; i1++) {
    order[i1]   = final_order[i1];
    fastest[i1] = final_fastest[i1];
  }
  get_com("",&command);
  clear_screen();
};
 
void hr__display_winnings(integer horse_bet, h_stat fastest, statr odds,
			  char horse_bet_type, h_name horse)
{
  /*  displays the amount paid per $2 bet  */
  real   win1,win2,win3,show3,place2,place3,earnings;
  vtype  out_val;
  char   command;

  win1 = odds[fastest[1]]*2 + 2;
  win2 = odds[fastest[1]]/1.5 + 2;
  if (win2 < 2.20) { win2 = 2.20; }
  win3 = odds[fastest[1]]/3 + 2;
  if (win3 < 2.20) { win3 = 2.20; }
  place2 = odds[fastest[2]]/1.5 + 2;
  if (place2 < 2.20) { place2 = 2.20; }
  place3 = odds[fastest[2]]/3 + 2;
  if (place3 < 2.20) { place3 = 2.20; }
  show3  = odds[fastest[3]]/3 + 2;
  if (show3 < 2.20) { show3 = 2.20; }

  earnings = 0;
  if (horse_bet == fastest[1]) {
    switch (horse_bet_type) {
    case 'w' : earnings = win1; break;
    case 'p' : earnings = win2; break;
    case 's' : earnings = win3; break;
    default:                    break;
    }
  } else if ((horse_bet==fastest[2]) && (horse_bet_type=='p')) {
    earnings = place2;
  } else if ((horse_bet==fastest[2]) && (horse_bet_type=='s')) {
    earnings = place3;
  } else if ((horse_bet==fastest[3]) && (horse_bet_type=='s')) {
    earnings = show3;
  }

  gld += (int)(earnings*bet/2);
  if (earnings > 0) {
    switch (randint(2)) {
    case 1 : msg_print("you won.");            break;
    case 2 : msg_print("your horse came in."); break;
    }
  } else {
    msg_print("no luck this time.");
  }

  clear_rc(2,1);
  hr__print_positions(10,1,horse_bet,horse,fastest);

  sprintf(out_val,"%8.2f",win1);
  put_buffer(out_val,10,42);
  sprintf(out_val,"%8.2f",win2);
  put_buffer(out_val,10,53);
  sprintf(out_val,"%8.2f",win3);
  put_buffer(out_val,10,64);
  sprintf(out_val,"%8.2f",place2);
  put_buffer(out_val,11,53);
  sprintf(out_val,"%8.2f",place3);
  put_buffer(out_val,11,64);
  sprintf(out_val,"%8.2f",show3);
  put_buffer(out_val,12,64);
  prt("[hit any key to continue]",23,24);
  get_com("",&command);
  clear_screen();
};


boolean grb__get_bet_type(char *horse_bet_type)
{
  char       command;
  boolean    exit_flag    = false;
  boolean    return_value = true;

  do {
    msg_print("What kind of bet? (<w>in, <p>lace, or <s>how) ");
    if (get_com("",&command)) {
      switch (command) {
      case 119: case 112: case 115: 
	*horse_bet_type = command;
	exit_flag      = true;
	break;
      }
    } else {
      return_value = false;
      exit_flag    = true;
    }
  } while (!exit_flag);

  return return_value;
};
 
 
boolean grb__get_bet_horse(integer num_horse, h_name horse, statr odds,
			   integer *horse_bet)
{
  boolean   exit_flag = false;
  vtype     comment;
  integer   i1;
  boolean   return_value = true;

  hr__list_horses(num_horse, horse, odds);
  do {
    strcpy(comment, "Which horse do you want to bet on?");
    if (c__get_response(comment,&i1)) {
      exit_flag    = ((i1>0) && (i1<num_horse + 1));
    } else {
      exit_flag    = true;
      return_value = false;
    }
  } while (!exit_flag);

  *horse_bet = i1;
  clear_screen();
  hr__display_commands();

  return return_value;
};

boolean hr__get_race_bet(char *horse_bet_type, integer *horse_bet,
			 integer num_horse, h_name horse, statr odds,
			 h_stat bets, integer *sum_bets)
{
  vtype    comment;
  integer  i1;
  boolean  exit_flag = false;
  boolean  return_value = false;

  bet = 0;
  strcpy(comment, "How much would you like to bet (2 to 1000 gp)? ");

  do {
    if (c__get_response(comment, &bet)) {
      if ((bet>1) && (bet<1001)) {
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

  if (bet > 0) {
    if (grb__get_bet_horse(num_horse, horse, odds, horse_bet)) {
      if (grb__get_bet_type(horse_bet_type)) {
	return_value = true;
	gld -= bet;
	bets[*horse_bet] = bets[*horse_bet] + bet;
	(*sum_bets) += bet;
	for (i1 = 1; i1 <= num_horse; i1++) {
	  odds[i1] = (*sum_bets) * 0.9/bets[i1] - 1;
	}
      }
    }
  }

  return return_value;
};
 
 
boolean hr__get_horse_command(boolean *exit_flag, char *horse_bet_type,
			      integer *horse_bet, integer *sum_bets,
			      integer *num_horse, h_name horse, statr odds,
			      h_stat bets, integer *true_sum_bets,
			      h_stat hstat, h_stat true_bets, statr true_odds)
{
  char      command;
  boolean   return_value = false;

  if (get_com("", &command)) {
    switch (command) {
    case 112  : 
      return_value = hr__get_race_bet(horse_bet_type, horse_bet, *num_horse,
				      horse, odds, bets, sum_bets);
      break;
    case 18   :    /*(*^R*)*/
      clear_screen();
      hr__display_commands();
      break;
    case 118  :  
      hr__list_horses(*num_horse, horse, odds);
      break;
    case 115  :
      hr__get_horse_stats(num_horse, horse, hstat, sum_bets, bets,
			  true_bets, odds, true_odds, true_sum_bets);
      msg_print("You skip a race.");
      spend_time(4800 / RACES_PER_DAY,"at the track",false);
      break;
    default:
      prt("Invalid Command.",1,1);
      break;
    }
  } else { 
    *exit_flag  = true;
  }

  return return_value;
};



void hr__game_horse()
{
  h_name     horse;
  h_stat     hstat,dist,area,lane;
  h_stat     fastest,final_fastest;
  h_stat     bets,true_bets;
  h_stat     order,final_order;
  integer    true_sum_bets,sum_bets;
  integer    horse_bet,num_horse;
  char       horse_bet_type;
  statr      odds,true_odds;
  
  integer    win_count,new_win_count;
  integer    which_screen,screen_low,screen_high;
  
  boolean    exit_flag;


  clear_screen();

  exit_flag = false;

  switch (randint(4)) {
  case 4 : prt("It is pouring down rain and the track is muddy.",1,1);  break;
  default: prt("It is a beautiful day at the track.",1,1);              break;
  }

  hr__display_commands();
  hr__get_horse_stats(&num_horse,horse,hstat,&sum_bets,bets,true_bets,
		      odds, true_odds, &true_sum_bets);

  do {
    c__display_gold();
    
    if (hr__get_horse_command(&exit_flag, &horse_bet_type, &horse_bet,
			      &sum_bets, &num_horse, horse, odds, bets,
			      &true_sum_bets, hstat, true_bets, true_odds)) {
      hr__start(num_horse, &screen_high, &which_screen, &win_count,
		&new_win_count, &screen_low, true_odds, final_order, 
		final_fastest, fastest, hstat, area, dist, order, &horse_bet,
		horse, lane);
      hr__display_winnings(horse_bet, fastest, odds, horse_bet_type, horse);
      spend_time(4800 / RACES_PER_DAY,"at the track",false);
      hr__display_commands();
      c__display_gold();
      hr__get_horse_stats(&num_horse,horse,hstat,&sum_bets,bets,true_bets,
			  odds, true_odds, &true_sum_bets);
    }
    
    c__check_casino_kickout();
    
    //with py.misc.cur_age do;
    if (((py.misc.cur_age.day % 7) >= 2) &&
	(py.misc.cur_age.hour >= 6) && 
	(py.misc.cur_age.hour < 18)) {
      
      c_closed = true;
      exit_flag = true;
      msg_print("There are no more races today...Come back tomorrow.");
      msg_print(" ");
    }
  } while (!exit_flag);
};

/* END FILE horse.c */
