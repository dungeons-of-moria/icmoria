/* wizard.c */
/**/

#include "imoria.h"
#include "dungeon.h"

#define LOW_NUM -98765432

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define PB(x,y,z) put_buffer((x),(y),(z))
void game_version()
{
  /*{ Print Moria credits					-RAK-	}*/
  
  /* why is this in the wizard code? */
  
  vtype    tmp_str;
  
  clear_from(1);
  
  sprintf(tmp_str,"              VMS Moria Version 4.8");
  
  PB(tmp_str,1,1);
  PB("Version 0.1  : 03/25/83",2,1);
  PB("Version 1.0  : 05/01/84",3,1);
  PB("Version 2.0  : 07/10/84",4,1);
  PB("Version 3.0  : 11/20/84",5,1);
  PB("Version 4.0  : 01/20/85",6,1);
  PB("Modules :",8,1);
  PB("     V1.0  Dungeon Generator      - RAK",9,1);
  PB("           Character Generator    - RAK & JWT",10,1);
  PB("           Moria Module           - RAK",11,1);
  PB("           Miscellaneous          - RAK & JWT",12,1);
  PB("     V2.0  Town Level & Misc      - RAK",13,1);
  PB("     V3.0  Internal Help & Misc   - RAK",14,1);
  PB("     V4.0  Source Release Version - RAK",15,1);
  PB("Robert Alan Koeneke               Jimmey Wayne Todd Jr.",17,1);
  PB("Student/University of Oklahoma    Student/University of Oklahoma",18,1);
  PB("119 Crystal Bend                  1912 Tiffany Dr.",19,1);
  PB("Norman, OK 73069                  Norman, OK  73071",20,1);
  PB("(405)-321-2925                    (405) 360-6792",21,1);
  
  pause_game(24);
  clear_rc(1,1);
  
  sprintf(tmp_str,"              VMS Imoria Version 4.85");
  PB(tmp_str,1,1);
  PB("Programers : Robert Alan Koeneke / University of Oklahoma",3,1);
  PB("             Jimmey Wayne Todd   / University of Oklahoma",4,1);
  PB(" "                                                        ,5,1);
  PB("UW Modifications by : Kenneth Case, Mary Conner,"         ,6,1);
  PB("                      Robert DeLoura, Dan Flye,"          ,7,1);
  PB("                      Todd Gardiner, Dave Jungck,"        ,8,1);
  PB("                      Andy Walker, Dean Yasuda."          ,9,1);
  PB(" "                                                        ,10,1);
  PB("University of Washington version 4.8"                     ,11,1);
  
  pause_game(24);
  clear_rc(1,1);
  
  sprintf(tmp_str,"          Linux Imoria Version %3.2f, patch level %d",
	  CUR_VERSION, PATCH_LEVEL);
  PB(tmp_str,1,1);
  PB("Version 4.85 : 06/25/98        Finished porting from pascal",3,1);
  PB(" "                                                        ,4,1);
  PB("Linux port by Stephen Kertes, 1997-98."                   ,5,1);
  PB(" "                                                        ,6,1);
  PB("I fixed all the cheats I used while at UW, thanks Paul."  ,7,1);
  PB(" "                                                        ,8,1);
  PB("Terminal, screen_map and (L)ocate taken from umoria 5.5"  ,9,1);
  PB("                                        "                 ,10,1);
#if USE_MTWIST
  PB("Imoria uses the `Mersenne Twister' random number library",11,1);
  PB("Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.",12,1);
  PB("This library is covered by the GNU Library GPL version 2.",13,1);
  PB("See the file mtwist/COPYING for more information.",14,1);
#endif  
  pause_game(24);
  draw_cave();
};
#undef PB
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bpswd()
{
  /*{ Builds passwords                                      -RAK-   }*/

  /*  Do remember that the passwords are max 12 chars plus a null. */

  integer    i1;

  strcpy(password1,"fragrance");
  strcpy(password2,"mopwillow");

  if (1) {

    /* lesser op password */
    set_seed(wdata[0][0]);
    for (i1 = 1; i1 <= 12; i1++) {
      password1[i1-1]  =  wdata[0][i1] ^ randint(255) ;
    }
    password1[12] = 0;

    /* full op password */
    set_seed(wdata[1][0]);
    for (i1 = 1; i1 <= 12; i1++) {
      password2[i1-1]  =  wdata[1][i1] ^ randint(255) ;
    }
    password2[12] = 0;

    set_seed(get_seed());
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean check_pswd(string passwd, boolean present)
{
  integer        i1;
  char           x;
  string         tpw;      /*  : packed array [1..12] of char;*/
//account_type   account;
  boolean        checked_out = false;

  /* perhaps crpyt() should be used?? */

  if (present) {
    strcpy(tpw, passwd);
  } else {
    i1 = 0;
    prt("Password : ",1,1);
    do {
      x = inkey();
      switch (x) {
      case 10 :
      case 13 :
	break;
      default : 
	tpw[i1++] = x;
	break;
      }
    } while (!((i1 == 12) || (x == 13) || (x == 10)));
    
    tpw[i1]   = 0;
  }

  if (!(strcmp(tpw, password1))) {
    checked_out = true;
    wizard1     = true;
  } else if (!strcmp(tpw, password2)) {
    checked_out = true;
    wizard1     = true;
    wizard2     = true;
  }
  /*
        if ( uw$id ) then
          begin
            get_account(account.body);
            account.length := index( account.body, ' ' )-1;
            if index( wizards, ':'+account+':' ) = 0 then
               begin
                 wizard1 := false;
                 wizard2 := false;
                 checked_out := false;
               end;
          end;
  */

  msg_flag = false;
  if (! present) {
    erase_line(msg_line,msg_line);
  }

  py.misc.cheated |= checked_out;
  became_wizard    = checked_out;

  return checked_out;

}; /* end check_pswd */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void wizard_light()
{
    /*{ Light up the dungeon					-RAK-	}*/

    integer    i1,i2,i3,i4;
    boolean    flag;

    if (cave[char_row][char_col].pl) {
	flag = false;
    } else {
	flag = true;
    }

    for (i1 = 0; i1 < cur_height; i1++) {
	for (i2 = 0; i2 < cur_width; i2++) {
	    if (is_in(cave[i1][i2].fval,floor_set)) {
		for (i3 = i1-1; i3 <= i1+1; i3++) {
		    for (i4 = i2-1; i4 <= i2+1; i4++) {
			cave[i3][i4].pl = flag;
			if (!(flag)) {
			    cave[i3][i4].fm = false;
			}
		    }
		}
	    }
	}
    }

   prt_map();
   detect_trap() ;
   detect_sdoor() ;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void monster_summon_by_name(integer y,integer x,ctype name,boolean present,
			    boolean sleepy)
{
    /*{ Wizard routine for summoning a specific monster       -RAD-   }*/

    integer    i1=0,i2,i3,i4;
    ctype      monster;
    boolean    junk;

    if (!present) {
	  prt("Monster desired:  ",1,1);
	  junk = (get_string(monster,1,19,26));
    } else {
	  strcpy(monster,name);
	  junk = true;
    }

    if (junk) {
	i2 = 0;
	sscanf(monster,"%ld", &i2);
	if (i2 < 0) {
	    i2 = 1;
	}
	if (i2 > MAX_CREATURES) {
	    i2 = MAX_CREATURES;
	}

	if ((i2 > 0) && (i2 <= MAX_CREATURES)) {
	    /* summon by number */
	    i1 = 0;
	    do {
		i3 = y - 2 + randint(3);
		i4 = x - 2 + randint(3);
		if (in_bounds(i3,i4)) {
		    /*with cave[i3,i4] do*/
		    if (is_in(cave[i3][i4].fval, floor_set)) {
			if (cave[i3][i4].fopen) {
			    place_monster(i3,i4,i2,sleepy);
			    i1 = 8;
			    y = i3;
			    x = i4;
			}
		    }
		}
		i1++;
	    } while (i1 <= 8);
	} else {
	    /* find by name, then summon */
	    for (i2 = 1; i2 <= MAX_CREATURES; i2++) {
		if ((strstr(c_list[i2].name,monster)!=NULL) && (i1!=10)) {
		    i1 = 0;
		    do {
			i3 = y - 2 + randint(3);
			i4 = x - 2 + randint(3);
			if (in_bounds(i3,i4)) {
			    /*with cave[i3,i4] do*/
			    if (is_in(cave[i3][i4].fval, floor_set)) {
				if (cave[i3][i4].cptr==0) {
				    if (cave[i3][i4].fopen) {
					place_monster(i3,i4,i2,sleepy);
					i1 = 9;
					y = i3;
					x = i4;
				    }
				}
			    }
			}
			i1++;
		    } while  (i1<=9);
		}
	    }
	} /* end else */
    } /* end if junk */

    if (!present) {
	erase_line(msg_line,msg_line);
    }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////// wizard_moo_item
//////////////////////////////////////////////////////////////////////
void wmi__init_data_list(list_elem_ptr *data_list)
{
  /*{ Code streamlined a bit by Dean Yasuda, to eliminate the
    inefficient quicksort/shell-sort combination.  Exact duplicate
    items eliminated from output list.              --MAV }*/

  list_elem_ptr    curse;
  treasure_type   temp_ray[MAX_OBJECTS+1];
  integer         i1,i2,i3,gap;
  treasure_type   tmp;
  
  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {
    temp_ray[i1] = object_list[i1];
  }
  
  for (gap = MAX_OBJECTS / 2 ; gap > 0 ; gap /= 2) {
    for (i1 = gap + 1; i1 <= MAX_OBJECTS; i1++) {
      for (i2 = i1 - gap ; i2 > 0 ; i2 -= gap) {
	i3 = i2 + gap;
	if ((temp_ray[i2].tval > temp_ray[i3].tval) ||
	    ((temp_ray[i2].tval == temp_ray[i3].tval) &&
	     (temp_ray[i2].subval > temp_ray[i3].subval))) {
	  tmp = temp_ray[i2];
	  temp_ray[i2] = temp_ray[i3];
	  temp_ray[i3] = tmp;
	} else {
	  i2 = 0;
	}
      }
    }
  }
  
  *data_list = (list_elem_ptr)safe_malloc(sizeof(list_elem),"init_data_list");
  curse = *data_list;
  curse->data = temp_ray[1];
  
  for (i1 = 2; i1 <= MAX_OBJECTS; i1++) {
    if ((temp_ray[i1].tval != temp_ray[i1-1].tval) ||
	(temp_ray[i1].subval != temp_ray[i1-1].subval)) {
      curse->next = 
	(list_elem_ptr)safe_malloc(sizeof(list_elem),"init_data_list");
      curse = curse->next;
      curse->data = temp_ray[i1];
      curse->next = nil;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void wmi__display_commands()
{
  prt("You may:",22,1);
  prt(" p) Pick an item.              b) Browse to next page.",23,1);
  prt("^R) Redraw screen.           Esc) Exit.",24,1);
};
//////////////////////////////////////////////////////////////////////
void wmi__display_list(list_elem_ptr start, integer *cur_display_size,
		       list_elem_ptr cur_display[], list_elem_ptr *blegga,
		       list_elem_ptr *data_list)
{
  integer    count, old_display_size;
  string     temp;
  
  old_display_size = *cur_display_size;
  count = 0;
  
  for ( ; (start != nil) && (count < MOO_DISPLAY_SIZE) ; ) {
    count++;
    if (cur_display[count] != start) {
      cur_display[count] = start;
      sprintf(temp,"%c) %s",96+(int)count,start->data.name);
      prt(temp,count+1,1);
    }
    start = start->next;
  }
  
  *cur_display_size = count;
  
  for ( ; old_display_size > *cur_display_size ; old_display_size--) {
    erase_line(old_display_size+3,1);
    cur_display[old_display_size] = nil;
  }
  
  if (start == nil) {
    *blegga = *data_list;
  } else {
    *blegga = start;
  }
};
//////////////////////////////////////////////////////////////////////
void wmi__clear_display(integer *cur_display_size, list_elem_ptr cur_display[])
{
  integer    i4;

  *cur_display_size = 0;
  for (i4 = 1; i4 <= MOO_DISPLAY_SIZE; i4++) {
    cur_display[i4] = nil;
  }
};
//////////////////////////////////////////////////////////////////////
void wmi__display_screen(integer       *cur_display_size, 
			 list_elem_ptr  cur_display[], 
			 list_elem_ptr  cur_top,
			 list_elem_ptr *blegga,
			 list_elem_ptr *data_list)
{
  clear_rc(1,1);
  wmi__clear_display(cur_display_size, cur_display);
  wmi__display_list(cur_top, cur_display_size, cur_display, blegga, data_list);
  wmi__display_commands();
};
//////////////////////////////////////////////////////////////////////
boolean wmi__get_list_entry(integer *com_val,vtype pmt, integer i1, integer i2)
{
  char      command;
  string    temp;
  boolean   flag = true;
  
  *com_val = 0;
  
  sprintf(temp,"(Entries %c-%c, ^Z to exit) %s",(int)i1+96,(int)i2+96,pmt);
  for ( ; (((*com_val < i1) || (*com_val > i2)) && (flag)) ; ) {
    prt(temp,1,1);
    command  = inkey();
    *com_val = (integer)command;
    switch (*com_val) {
    case 3: case 25: case 26: case 27: 
      flag = false;
      break;
    default:
      *com_val -= 96;
      break;
    }
  }
  
  erase_line(1,1);
  
  return  flag;
};
//////////////////////////////////////////////////////////////////////
boolean wmi__parse_command(list_elem_ptr *blegga,
			   list_elem_ptr *cur_top,
			   integer       *cur_display_size,
			   boolean       *exit_flag,
			   treasure_type *back,
			   list_elem_ptr  cur_display[],
			   list_elem_ptr *data_list)

{
  char       command;
  integer    com_val,which;
  boolean    flag = false;

  if (get_com("",&command)) {
    com_val = (integer)command;
    switch (com_val) {

    case 18 : /*{^R}*/
      wmi__display_screen(cur_display_size, cur_display, *cur_top,
			  blegga, data_list);
      break;
      
    case 32 : /*{ }*/
    case 98 : /*{b}*/
      if (cur_top == blegga) {
	prt("Entire list is displayed.",1,1);
      } else {
	cur_top = blegga;
	wmi__display_list(*cur_top, cur_display_size,
			  cur_display, blegga, data_list);
      }
      break;
      
    case 112 : /*{p}*/
      if (*cur_display_size > 0) {
	if (wmi__get_list_entry(&which," Pick which one?",1,
				*cur_display_size)) {
	  *exit_flag = true;
	  flag  = true;
	  *back = cur_display[which]->data;
	}
      }
      break;
	
    default:
      prt("Invalid command",1,1);
      break;
    }
  } else {
    *exit_flag = true;
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean wizard_moo_item(treasure_type *back)
{
  /*{ Wizard routine to pick an item from the entire list, and
    magic it until satisfied                              -DMF-   }*/

  list_elem_ptr    data_list;
  list_elem_ptr    cur_top;
  list_elem_ptr    blegga;
  list_elem_ptr    cur_display[MOO_DISPLAY_SIZE+1];
  integer          cur_display_size;
  boolean          exit_flag = false;      
  boolean          flag = false;
  
  *back = blank_treasure;
  wmi__init_data_list(&data_list);
  cur_top = data_list;
  wmi__display_screen(&cur_display_size, cur_display, cur_top,
		      &blegga, &data_list);
  
  do {
    flag = wmi__parse_command(&blegga, &cur_top, &cur_display_size,
			      &exit_flag, back, cur_display, &data_list);
  } while (!exit_flag);
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////// summon_item
//////////////////////////////////////////////////////////////////////
boolean si__get_new_ttype(ttype  s, vtype str, string out_str)
{
  /*{prompts for new string, <CR> leaves old value}*/
  ttype      os;
  boolean    flag = false;

  if (strlen(s) > 0) {
    sprintf(out_str,"%s [%s] : ",str,s);
  } else {
    sprintf(out_str,"%s : ",str);
  }

  prt(out_str,1,1);
  strcpy(os, s);

  if (get_string(s,1,strlen(out_str)+1,40)) {
    flag = true;
    if ((strlen(os) > 0) && (strlen(s) == 0)) {
      strcpy(s, os);
    }
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean si__get_item_descriptions(string out_str, treasure_type moo_item[],
				  boolean *found, integer *num_found)
{
  /*{ask wizard for item information/Moo!, Moo./Moo?}*/
  boolean    ook;
  boolean    flag = false;

  if (si__get_new_ttype(s1,"Item string",out_str)) {
    ook = true;
    
    if (strstr(s1,"Moo!") == s1) {
      moo_item[1] = blank_treasure;
      ook = wizard_moo_item(&(moo_item[1]));
      if (ook) {
	*found = true;
	*num_found = 1;
      }
      draw_cave();
    }
    
    if (ook) {
      if (si__get_new_ttype(s2,"More stuff #1",out_str)) {
	if (si__get_new_ttype(s3,"More stuff #2",out_str)) {
	  if (si__get_new_ttype(s4,"Special",out_str)) {
	    
	    if (i_summ_count > 0) {
	      sprintf(out_str,"Maximum number of tries: [%ld] : ",i_summ_count);
	    } else {
	      strcpy(out_str, "Maximum number of tries: ");
	    }
	    prt(out_str,1,1);
	    if (get_string(out_str,1,strlen(out_str)+1,60)) {
	      flag = true;
	    }
	  }
	}
      }
    }
  }
  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean si__narrow(ttype s, integer *num_found, integer moo_cursor[])
{
  /*{ eliminate all items without string s from array moo_cursor }*/
  integer    i1,i2;
  boolean    flag = false;

  i2 = 1;
  if (strlen(s) > 0) {
    for (i1 = 1; i1 <= *num_found; i1++) {
      if (strstr(object_list[moo_cursor[i1]].name,s) != NULL) {
	moo_cursor[i2] = moo_cursor[i1];
	i2++;
      }
    }
  }

  if (i2 > 1) {
    flag = true; /*{at least one feasible substring found}*/
    *num_found = i2 - 1;
  }

  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean si__narrow_choices(integer *num_found, integer moo_cursor[],
			   treasure_type moo_item[])
{
  /*{ use 3 substrings to narrow down specify possible items }*/

  integer   i1;
  boolean   flag = false;

  ENTER("si__narrow_choices","w");

  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {
    moo_cursor[i1] = i1;
  }
  *num_found = MAX_OBJECTS;

  if (si__narrow(s1, num_found, moo_cursor)) {
    flag = true;
    if (si__narrow(s2, num_found, moo_cursor)) {
      si__narrow(s3, num_found, moo_cursor);
    }
    for (i1 = 1; i1 <= *num_found; i1++) {
      moo_item[i1] = object_list[moo_cursor[i1]];
    }
  }

  RETURN("si__narrow_choices", "w",'b',"found:",&flag);
  return flag;
};
//////////////////////////////////////////////////////////////////////
void si__pesky_stuff(integer *best_value, integer *good_value,
		     treasure_type *best_pick, treasure_type *good_pick,
		     integer *optimize, string out_str,
		     integer *i_summ_count, integer *cur_pos,
		     integer x, integer y)
{
  /*{init variables, see if optimizing (1=best, -1= worst); find # of tries }*/
  integer   omax;

  *best_value = LOW_NUM;
  *good_value = LOW_NUM;
  *best_pick  = yums[5]; /*{rice-a-roni}*/
  *good_pick  = yums[5];

  if (strstr(s4,"Moo.") != NULL) {
    *optimize = 1;
  } else if (strstr(s4,"Moo?") != NULL) {
    *optimize = -1;
  } else {
    *optimize = 0;
  }

  omax = *i_summ_count;
  sscanf(out_str,"%ld",i_summ_count);

  if (*i_summ_count == 0) {
    *i_summ_count = omax;
  }
  if (i_summ_count <= 0) {
    *i_summ_count = 1;
  }
  popt(cur_pos);
  cave[y][x].tptr = *cur_pos;
  t_list[*cur_pos] = blank_treasure;
  sprintf(t_list[*cur_pos].name, "& bogus summoned item %ld", *cur_pos);
};
//////////////////////////////////////////////////////////////////////
boolean si__optimize_item(treasure_type *pick, integer *value,
			  integer optimize, integer cur_pos)
{
  /*{ formula for comparing value of items}*/
  integer    i1;
  boolean    flag = false;

  //with t_list[cur_pos]. do;
  i1 = optimize * (t_list[cur_pos].cost + t_list[cur_pos].tohit + 
		   t_list[cur_pos].todam + t_list[cur_pos].toac);
  if (i1 > *value) {
    *value = i1;
    *pick  = t_list[cur_pos];
    flag   = true;
  }
  
  return flag;
};
//////////////////////////////////////////////////////////////////////
boolean summon_item(integer y,integer x,ttype name1,ttype name2,
		    integer count,boolean present)
{
  /*{ Wizard routine to summon a random item by substring(s) of its }*/
  /*{ name, with a maximum # of tries			-DMF-	    }*/

  integer         i1,i2,num_found;
  integer         optimize;
  integer         best_value,good_value;
  treasure_type   best_pick,good_pick;
  boolean         flag,done,found;
  string          out_str;
  integer         cur_pos;
  char            command;
  treasure_type   moo_item[MAX_OBJECTS+1];
  integer         moo_cursor[MAX_OBJECTS+1];
  boolean         return_value = false;

  ENTER("summon_item","w");
  
  found = false;
  done = false;
  
  if (present) {
    flag = (strlen(name1) != 0);
    strcpy(s1, name1);
    strcpy(s2, name2);
    strcpy(s3, "");
    strcpy(s4, "Moo.");
    sprintf(out_str,"%ld",count);
  } else {
    /*{found := true iff successful Moo!}*/
    flag = si__get_item_descriptions(out_str, moo_item, &found, &num_found);
  }
  
  if (flag) {
    si__pesky_stuff(&best_value, &good_value, &best_pick, &good_pick,
		    &optimize, out_str, &i_summ_count, &cur_pos, x, y);

    if (!found) {
      /*{create array of all ok choices}*/
      found = si__narrow_choices(&num_found, moo_cursor, moo_item);
    }
    
    if (found) {
      if (!present) {
	msg_print("Press any key to abort...");
	put_qio();
      }
      
      for (i1 = 0 ; (i1 < i_summ_count) && (!done) ; i1++) {
	t_list[cur_pos] = moo_item[((num_found*i1) / i_summ_count)+1];

	
	if (!present) {
	  inkey_delay(&command,0);
	  done = (command != 0);
	}
	
	magic_treasure(cur_pos,1000,false);
	
	if (((strlen(s2) == 0) || (strstr(t_list[cur_pos].name,s2) != NULL)) &&
	    ((strlen(s3) == 0) || (strstr(t_list[cur_pos].name,s3) != NULL))) {
	  if (si__optimize_item(&best_pick,&best_value,optimize,cur_pos)) {
	    /*{ leave loop prematurely if not optimizing and item is found }*/
	    if (optimize == 0) {
	      done = true;
	    }
	  }
	  
	  /*{ while no correct pick, get best non-correct item }*/
	} else if ((optimize != 0) && (best_value == LOW_NUM)) {
	  si__optimize_item(&good_pick,&good_value,optimize,cur_pos);
	}

      } /* end for */
    } /* end if found */
    
    if (best_value > LOW_NUM) {
      msg_print("Allocated.");
      t_list[cur_pos] = best_pick;
      //with t_list[cur_pos]. do;
      if (t_list[cur_pos].subval > 255) {
	i2 = t_list[cur_pos].cost;
	if (i2 < 3) {
	  i2 = 3;
	}
	t_list[cur_pos].number = i_summ_count / sqrt(100*i2 / GOLD_VALUE);
	if (t_list[cur_pos].number < 1) { 
	  t_list[cur_pos].number = 1;
	} else if (t_list[cur_pos].number > 100) {
	  t_list[cur_pos].number = 100;
	}
      }
    } else if (good_value > LOW_NUM) {
      msg_print("Found, but not perfect match.");
      t_list[cur_pos] = good_pick;
    } else {
     msg_print("Unfortunately your wish did not come true.");
     msg_print("You have, however, been awarded a valuable consolation gift!");
     t_list[cur_pos] = yums[5]; /*{rice}*/
     t_list[cur_pos].number = 12;
    }
    return_value = true;
  } else { /*{ if flag }*/
    msg_print("Invalid input");
  }
  
  RETURN("summon_item", "w",'b',"allocated:",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void enter_wizard_mode(boolean ask_for_pass)
{
  if (wizard1) {
    msg_print("Wizard mode off.");
    wizard1    = false;
    wizard2    = false;
    reset_flag = true;
    no_controly();
  } else {
    if (became_wizard || !ask_for_pass) {
      /*if (check_pswd("doublespeak",true)) {*/
      if (check_pswd(password1,true)) {
	msg_print("Wizard mode on.");
	controly();
      }
    } else {
      if (check_pswd("",false)) {
	msg_print("Wizard mode on.");
	controly();
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void esf__display_commands()
{
  prt("You may:",21,1);
  prt(" d) Delete an entry.              b) Browse to next page.",22,1);
  prt(" c) Change an entry.",23,1);
  prt(" q) Quit and save changes       Esc) Exit without saving.",24,1);
};
//////////////////////////////////////////////////////////////////////
void esf__display_list(int start, string list[], int n1, int *blegga,
		       int *cur_display_size)
{
  integer    count,old_display_size;
  string     out_val;

  old_display_size = *cur_display_size;

  for (count = 0 ; (start <= n1) && (count < 15) ; start++) {
    count++;

    sprintf(out_val,"%c)%s",(char)(96+count),list[start]);
    if (strlen(out_val) > 80) {
      out_val[79] = 0;
    }
    prt(out_val,count+3,1);
  }

  *cur_display_size = count;
  for ( ; (old_display_size > *cur_display_size) ; old_display_size--) {
    erase_line(old_display_size+3,1);
  }

  if (start > n1) {
    *blegga = 1;
  } else {
    *blegga = start;
  }
};
//////////////////////////////////////////////////////////////////////
void esf__display_screen(int cur_top, string list[], int n1, int *blegga,
			 int *cur_display_size)
{
  clear_screen();
  *cur_display_size = 0;
  put_buffer("  Username     Points  Diff    Character name    Level  Race         Class",2,1);
  put_buffer("  ____________ ________ _ ________________________ __ __________ ______________",3,1);
  esf__display_list(cur_top, list, n1, blegga, cur_display_size);
  esf__display_commands();
};
//////////////////////////////////////////////////////////////////////
boolean esf__get_list_entry(int *com_val, vtype pmt, int cur_top,
			    int i1, int i2)
{
  vtype     out_val;
  boolean   flag = true;

  *com_val = 0;

  sprintf(out_val,"(Entries %c-%c, Esc to exit) %s",
	  (char)(i1+96), (char)(i2+96), pmt);

  for ( ;  (((*com_val < i1) || (*com_val > i2)) && (flag)) ; ) {
    prt(out_val,1,1);
    *com_val = inkey();

    switch (*com_val) {
    case 3: case 25: case 26: case 27: 
      flag = false;
      break;
    default: 
      (*com_val) += (cur_top - 97);
      break;
    }
  }

  erase_line(1,1);
  return flag;
};
//////////////////////////////////////////////////////////////////////
void esf__change_entry(int cur_top, string list[], int n1,
		       int cur_display_size)
{
  int      which, i1;
  string   user,score,name,level;
  string   race,class,diffic;
  integer  sc,lvl,diff,i4;
  boolean  flag, aborted = true;

  if (cur_display_size > 0) {
    if (esf__get_list_entry(&which," Change which one?",
		       cur_top, 1, cur_top + cur_display_size - 1)) {
      prt("Username : ",1,1);
      if (get_string(user,1,12,12)) {
	prt("Score : ",1,1);
	if (get_string(score,1,9,8)) {
	  prt("Difficulty : ",1,1);
	  if (get_string(diffic,1,14,1)) {
	    prt("Character name : ",1,1);
	    if (get_string(name,1,18,24)) {
	      prt("Level : ",1,1);
	      if (get_string(level,1,9,2)) {
		prt("Race : ",1,1);
		if (get_string(race,1,8,10)) {
		  prt("Class : ",1,1);
		  if (get_string(class,1,9,16)) {

		    aborted = false;

		    sscanf(diffic, "%ld", &diff);
		    sscanf(level,  "%ld", &lvl);
		    sscanf(score,  "%ld", &sc);

		    sscanf(&(list[which][13]),"%ld",&i4);

		    if (sc == i4) {
		      i1 = which;
		    } else {
		      /* nuke the current slot */
		      for (i1 = which ; i1 < n1 ; i1++) {
			strcpy(list[i1], list[i1+1]);
		      }
		      
		      /* find the spot to insert at */
		      flag = false;
		      for (i1=1; (i1<n1) && !flag ; ) {/* XXXX corrupted? */
			sscanf(&(list[i1][13]),"%ld",&i4);
			if (i4 < sc) {
			  flag = true;
			} else {
			  i1++;
			}
		      }
		    }		    

		    format_top_score(list[i1], user, sc, diff, name, lvl,
				     race, class);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  if (aborted) {
    prt("Score not changed.",1,1);
  } else {
    prt("Score changed.",1,1);
  }
};
//////////////////////////////////////////////////////////////////////
void esf__delete_entry(int cur_top, string list[], int *n1,
		       int cur_display_size)
{
  int    which, i1;

  if (cur_display_size > 0) {
    if (esf__get_list_entry(&which," Delete which one?",
			    cur_top, 1,cur_top + cur_display_size - 1)) {
    
      for (i1 = which ; i1 < *n1 ; i1++) {
	strcpy(list[i1], list[i1+1]);
      }
      (*n1)--;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void esf__parse_command(string list[], int *cur_top, int *n1, int *blegga,
			int *cur_display_size,
			boolean *exit_flag, boolean *want_save)
{
  char     command;

  if (get_com("",&command)) {

    switch (command) {

    case  18: /*{^R}*/
      esf__display_screen(*cur_top, list, *n1, blegga, cur_display_size);
      break;
      
    case  32: /*{ }*/
    case  98: /*{b}*/
      if (*cur_top == *blegga) {
	prt("Entire list is displayed.",1,1);
      } else {
	*cur_top = *blegga;
	esf__display_list(*cur_top, list, *n1, blegga, cur_display_size);
      }
      break;
      
    case  99: /*{c}*/
      esf__change_entry(*cur_top, list, *n1, *cur_display_size);
      esf__display_list(*cur_top, list, *n1, blegga, cur_display_size);
      break;
      
    case 100: /*{d}*/
      esf__delete_entry(*cur_top, list, n1, *cur_display_size);
      esf__display_list(*cur_top, list, *n1, blegga, cur_display_size);
      break;
      
    case 113: /*{q}*/
      *exit_flag = true;
      *want_save = true;
      break;
      
    default:
      prt("Invalid command",1,1);
      break;
    }
  } else {
    *exit_flag = true;
  }
};

//////////////////////////////////////////////////////////////////////

void edit_score_file()
{
  /*{ Wizard routine to edit high score file                -DMF-   }*/

  string    list[MAX_HIGH_SCORES+2];
  boolean   want_save, exit_flag;
  int       n1, cur_top, blegga, cur_display_size = 0;
  FILE     *f1;
  vtype     s1;

/*
    const
        display_size    = 15;
    type
        list_elem = record
                data    : string;
                next    : ^list_elem;
        end;
        list_elem_ptr = ^list_elem;
    var
        data_list       : list_elem_ptr;
        cur_top         : list_elem_ptr;
        blegga          : list_elem_ptr;
        curse           : list_elem_ptr;
        cur_display     : array [1..display_size] of list_elem_ptr;
        cur_display_size: integer;
        blank           : packed array [1..13] of char;
        i1,i2,i3,i4     : integer;
        trys            : integer;
        f1              : text;
        flag,file_flag  : boolean;
        exit_flag       : boolean;
        temp,temp2      : ntype;
        ch              : char;
*/


  /*{ Wizard routine to edit high score file                -DMF-   }*/
  
  if (!read_top_scores(&f1, MORIA_TOP, list, MAX_HIGH_SCORES, &n1, s1)) {
    prt(s1,2,1);
    prt("",3,1);
  } else {
    
    want_save = false;
    cur_top   = 1;
    
    esf__display_screen(cur_top, list, n1, &blegga, &cur_display_size);
    for ( exit_flag = false ; !exit_flag ; ) {
      esf__parse_command(list, &cur_top, &n1, &blegga, &cur_display_size,
			 &exit_flag, &want_save);
    }
    
    if (want_save) {
      write_top_scores(&f1, list, n1);
      msg_print("Changes saved.");
    } else {
      msg_print("Changes not saved.");
    }
    close_top_scores(&f1);
  }

  draw_cave();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean cc__input_field(string prompt, integer *num, integer min, 
			integer max, boolean *ok)
{
  string    out_val;
  integer   len;
  boolean   return_value = false;

  sprintf(out_val, "Current = %ld, %s", *num, prompt);
  len = strlen(out_val);
  prt(out_val,1,1);

  if (get_string(out_val,1,len+1,10)) {
    len = -999;
    sscanf(out_val,"%ld", &len);
    if ((len >= min) && (len <= max)) {
      *ok  = true;
      *num = len;
    } else {
      *ok = false;
    }
    return_value = true;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
void change_character()
{
  /*{ Wizard routine for gaining on stats                   -RAK-   }*/

  integer   tmp_val;
  vtype     tmp_str;
  stat_set  tstat;
  boolean   flag  = false;
  boolean   abort = false;
  
  //with py.stat do;
  for (tstat = STR; (tstat <= CHR) && !abort; tstat++) {
    switch (tstat) {
    case STR :  prt("(0 - 250) Strength     = ",1,1); break;
    case INT :  prt("(0 - 250) Intelligence = ",1,1); break;
    case WIS :  prt("(0 - 250) Wisdom       = ",1,1); break;
    case DEX :  prt("(0 - 250) Dexterity    = ",1,1); break;
    case CON :  prt("(0 - 250) Constitution = ",1,1); break;
    case CHR :  prt("(0 - 250) Charisma     = ",1,1); break;
    }

    if (!get_string(tmp_str,1,26,10)) {
      abort = true;
    }

    if (!abort) {
      tmp_val = -999;
      sscanf(tmp_str, "%ld", &tmp_val);
      if (tmp_val != -999) {
	tmp_val = squish_stat(tmp_val);
	PS.p[(int)tstat] = tmp_val;
	PS.c[(int)tstat] = tmp_val;
	prt_a_stat(tstat);
      }
    }
  }


  //with py.misc do;
  if (!abort) {
    tmp_val = PM.mhp;
    if (cc__input_field("(1-32767) Hit points = ",&tmp_val,1,32767,&flag)) {
      if (flag) {
	PM.mhp = tmp_val;
	PM.chp = PM.mhp;
	prt_hp();
      }
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.mana;
    if (is_magii) {
      if (cc__input_field("(0-32767) Mana = ",&tmp_val,0,32767,&flag)) {
	if (flag) {
	  PM.mana  = tmp_val;
	  PM.cmana = PM.mana;
	  prt_mana();
	}
      } else {
	abort = true;
      }
    }
  }

  if (!abort) {
    tmp_val = PM.srh;
    if (cc__input_field("(0-200) Searching = ",&tmp_val,0,200,&flag)) {
      PM.srh = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.stl;
    if (cc__input_field("(0-10) Stealth = ",&tmp_val,0,10,&flag)) {
      PM.stl = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.disarm;
    if (cc__input_field("(0-200) Disarming = ",&tmp_val,0,200,&flag)) {
      PM.disarm = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.save;
    if (cc__input_field("(0-100) Save = ",&tmp_val,0,100,&flag)) {
      PM.save = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.bth;
    if (cc__input_field("(0-200) Base to hit = ",&tmp_val,0,200,&flag)) {
      PM.bth = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.bthb;
    if (cc__input_field("(0-200) Bows/Throwing = ",&tmp_val,0,200,&flag)) {
      PM.bthb = tmp_val;
    } else {
      abort = true;
    }
  }

  if (!abort) {
    tmp_val = PM.money[TOTAL_];
    if (cc__input_field("Player Gold = ",&tmp_val,0,100000000,&flag)) {
      if (flag) {
	tmp_val = (tmp_val-PM.money[TOTAL_])*GOLD_VALUE;
	if (tmp_val>0) {
	  add_money(tmp_val);
	} else {
	  subtract_money(-tmp_val,true);
	}
	prt_weight();
	prt_gold();
      }
    } else {
      abort = true;
    }
  }

  if (!abort) {
    if (!cc__input_field("Account Gold = ",&PM.account,0,1000000000,&flag)) {
      abort = true;
    }
  }


  if (!abort) {
    tmp_val = inven_weight;
    if (cc__input_field("Current Weight (100/unit weight) = ",&tmp_val,0,900000,&flag)) {
      inven_weight = tmp_val;
      prt_weight();
    } else {
      abort = true;
    }
  }

  erase_line(msg_line,msg_line);
  py_bonuses(&blank_treasure,0);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void wizard_create()
{
  /*{ Wizard routine for creating objects                   -RAK-   }*/

  /* this routine was full of bugs.  worst off, missing support for several
     tvals.  next, failure to allow wizard to specify an item's tchar or
     level.  finally, asking for the data items in an order other than the
     order of treasure_type's members.  so, i added the missing tvals, added
     code to specify tchar and level, and re-ordered the input blocks to match
     treasure_type's structure.  this way it's much easier to create a one-off
     item from something in values.h; you just type in the parts you want to
     leave alone, in order, and change the one or two you want to mess with.
     2/5/00 JEB
  */

  integer    tmp_val;
  vtype      tmp_str;
  boolean    flag;
  char       tchar;

  msg_print("Warning: This routine can cause fatal error.");
  msg_print(" ");
  msg_flag = false;

  //with inven_temp->data do;
  prt("Name   : ",1,1);
  if (get_string(tmp_str,1,10,40)) {
    strcpy(inven_temp->data.name, tmp_str);
  } else {
    strcpy(inven_temp->data.name, "& Wizard Object!");
  }
  
  do {
    prt("Tval   : ",1,1);
    get_string(tmp_str,1,10,10);
    tmp_val = 0;
    sscanf(tmp_str,"%ld",&tmp_val);
    flag = true;
    
    switch (tmp_val) {

      // ordinary objects:  added several missing classes:
      // valuable_gems_wear, maul, sword, gem_helm, bracers, 
      // belt, junk_food, song_book.  --jb 2/5/00
    case 1: case 3: case 6: case 13: case 15:     tchar = '~';  break;
    case 4: case 5: case 7:                       tchar = '*';  break;
    case 2:                                       tchar = '&';  break;
    case 10: case 11: case 12:                    tchar = '{';  break;
    case 20:                                      tchar = '}';  break;
    case 21:                                      tchar = '/';  break;
    case 22: case 25: case 26:                    tchar = '\\'; break;
    case 23: case 24:                             tchar = '|';  break;
    case 29: case 30: case 31: case 33: case 37:  tchar = ']';  break;
    case 32: case 36: case 38:                    tchar = '(';  break;
    case 34:                                      tchar = ')';  break;
    case 35:                                      tchar = '[';  break;
    case 40:                                      tchar = '"';  break;
    case 45:                                      tchar = '=';  break;
    case 55:                                      tchar = '_';  break;
    case 60: case 65:                             tchar = '-';  break;
    case 70: case 71: case 90: case 91:           tchar = '?';  break;
    case 75: case 76: case 77:                    tchar = '!';  break;
    case 80: case 81:                             tchar = ',';  break;
    case 85: case 86: case 92: case 93:           tchar = '%';  break;

      /* traps.  yes, i know it's weird to be able to create a trap.
         but they're just special item types, and the wizard is already
         warned that they might crash the game, so let 'em...
         --jb, 2/5/2000
      */
    case 101:                                     tchar = '.';  break;
    case 102:                                     tchar = '^';  break;
    case 103:                                     tchar = ':';  break;
    case 104:                                     tchar = '\''; break;
    case 105:                                     tchar = '+';  break;
    case 107:                                     tchar = '<';  break;
    case 108:                                     tchar = '>';  break;
    case 109:                                     tchar = '#';  break;
    case 110:                                     tchar = '+';  break;
    case 111:                                     tchar = '<';  break;
    case 112:                                     tchar = '>';  break;
    case 113:                                     tchar = '`';  break;

    default:
      tchar = '?';
      flag  = false;
      break;
    }
  } while (!flag);

  prt("Tchar  : ",1,1);         // user might want to override the default.
  get_string(tmp_str,1,10,10);  // anything from ' ' on up is printable, so
  if(tmp_str[0] >= ' ') {       // we'll take it.
    tchar = tmp_str[0];         // --jb 2/5/2000
  }
  inven_temp->data.tchar = tchar;
  inven_temp->data.tval  = tmp_val;
  
  prt("Flags  (In HEX): ",1,1);
  inven_temp->data.flags = get_hex_value(1,18,8);
  
  prt("Flags2 (In HEX): ",1,1);
  inven_temp->data.flags2 = get_hex_value(1,18,8);
  
  prt("P1     : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.p1 = tmp_val;
  
  prt("Cost : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.cost = tmp_val;

  prt("Subval : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 1;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.subval = tmp_val;
  
  prt("Weight : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 1;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.weight = tmp_val;
  
  prt("Number : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 1;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.number = tmp_val;
  
  prt("+To hit: ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.tohit = tmp_val;
  
  prt("+To dam: ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.todam = tmp_val;
  
  prt("AC     : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.ac = tmp_val;
  
  prt("+To AC : ",1,1);
  get_string(tmp_str,1,10,10);
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  inven_temp->data.toac = tmp_val;
  
  prt("Damage : ",1,1);
  get_string(tmp_str,1,10,5);
  strcpy(inven_temp->data.damage, tmp_str);
  
  prt("Level  : ",1,1);                      // added code to specify item's
  get_string(tmp_str,1,10,10);               // level.  --jb 2/5/00
  tmp_val = 0;
  sscanf(tmp_str,"%ld",&tmp_val);
  if(tmp_val < 0) tmp_val = 0;
  inven_temp->data.level = tmp_val;
  
  if (get_com("Allocate? (Y/N)",&command)) {
    switch (command) {
    case 'y': case 'Y':
      popt(&tmp_val);
      t_list[tmp_val] = inven_temp->data;
      //with cave[char_row][char_col]. do;
      if (cave[char_row][char_col].tptr > 0) {
	delete_object(char_row,char_col);
      }
      cave[char_row][char_col].tptr = tmp_val;
      msg_print("Allocated...");
      break;
      
    default:
      msg_print("Aborted...");
      break;
    }
  }
  inven_temp->data = blank_treasure;
  move_char(5);
  creatures(false);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  wizard.c */
