/* save.c */
/* code for saving or loading a character */

#include "imoria.h"
#include "master.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void data_exception()
{

/*   Data Corruption means character is dead, or save file was -RAK-
     screwed with.  Keep them guessing as to what is actually wrong.*/

  clear_from(1);
  prt("Data Corruption Error.",1,1);
  prt(" ",2,1);
  exit_game();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//void encrypt_line(ntype line)
//{
//  /*{ Encrypts a line of text, complete with a data-check sum-RAK-  }*/
//  /*{ (original by JWT)                                             }*/
//
//  integer   i1,i2;
//  ntype     temp;
//
//  i2 = 0;
//  for (i1 = 0; i1 < strlen(line); i1++) {
//    i2 += line[i1] + i1;
//  }
//  strcpy(temp, line);
//  sprintf(line,"%ld %s",i2,temp);
//
//  coder(line);
//};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//void coder(ntype line)
//{
//  /*{ Uses XOR function to encode data			-RAK-	}*/
//
//  integer        i1, x;
//  unsigned long  i2,i3,i4;
//
//  x = strlen(line);
//  for (i1 = 0; i1 < x; i1++) {
//    i2 = uint(ord(line[i1]));
//    i3 = uint(randint(256)-1);
//    i4 = uxor(i2,i3);
//    line[i1] = i4;
//  }
//};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sc__open_save_file(FILE **f1, vtype fnam, boolean quick, boolean *flag)
{
  vtype    out_val;

  if (py.flags.dead || quick) {
    *flag = true;
    if (is_from_file) {
      *f1 = (FILE *)fopen(finam,"w");
      /*open(f1,file_name:=finam,record_length:=1024,history:=old);*/
    } else {
      *f1 = (FILE *)fopen(finam,"w");
      /*open(f1,file_name:=finam,record_length:=1024,history:=new);*/
    }
  } else {
    prt("Enter Filename:",1,1);
    *flag = false;

    /*{ Open the user's save file                             -JWT-   }*/
    if (get_string(fnam,1,17,60)) {
      if (strlen(fnam) == 0) {
	strcpy(fnam, finam);
      }

      if ((is_from_file) && (strcmp(fnam, finam) == 0)) {
	*f1 = (FILE *)fopen(fnam,"w");
	/*open (f1,FILE_NAME:=fnam,record_length:=1024,history:=old);*/
      } else {
	*f1 = (FILE *)fopen(fnam,"w");
	/*open (f1,FILE_NAME:=fnam,record_length:=1024,history:=new);*/
      }
  
      if (*f1 == NULL) {
	sprintf(out_val, "Error creating> %s", fnam);
	msg_print(out_val);
      } else {
	*flag = true;
      }
    } else {
      erase_line(1,1);
    }/* endif else get_string */
  } /*endif dead or quick*/

};
//////////////////////////////////////////////////////////////////////
void sc__open_master(FILE *f1, GDBM_FILE *f2, boolean *flag)
{
  /*{ Make an attempt to open the MASTER file               -JWT-   }*/

  if (!master_file_open(f2)) {
    msg_print("Error saving character (open MASTER), contact MORIA Wizard.");
    *flag = true;
  }
  
};
//////////////////////////////////////////////////////////////////////
void sc__write_master(GDBM_FILE f2, boolean *flag)
{
  /*{ Write key to MASTER		-KRC-	}*/

  int           i2;
  master_key    mkey;
  master_entry  mentry;
  boolean       result;

  mkey.creation_time = PM.creation_time;
  mentry.save_count  = PM.save_count + 1;
  mentry.deaths      = PM.deaths;

  if (PM.save_count == 0) {
    result = master_file_write_new(f2, &mkey, &mentry);
    PM.creation_time = mkey.creation_time;
    PM.save_count++;
  } else {
    i2 = master_file_verify(f2, &mkey);
    if (i2 == MF_CHAR_MISMATCH) {
      result = false;
    } else {
      result = master_file_write(f2, &mkey, &mentry);
      PM.save_count++;
    }
  }

  if (!result) {
    msg_print("Error in writing to MASTER.");
  }

  *flag = result;
};
//////////////////////////////////////////////////////////////////////
void sc__display_status(boolean quick, vtype fnam,ntype out_rec)
{
    /*{ Message to player on what is happening}*/
    if (!py.flags.dead) {
      clear_from(1);
      if (!quick) {
	put_qio();
      } else {
	strcpy(fnam, finam);
      }
      sprintf(out_rec, "Saving character in %s...",fnam);
      prt(out_rec,1,1);
    }
};
//////////////////////////////////////////////////////////////////////
void sc__write_player_record(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
  /*{ Write out the player record.	}*/
  
  integer        i1, inven_ctr;
  //time_type      tim;
  treas_ptr      curse;
  time_t         current_time, delta_time;
  //quad_type      current_time,delta_time;
  
  /*with py.misc do*/

  sprintf(out_rec, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %d",
	  PM.xtr_wgt,PM.account,PM.money[0],PM.money[6],PM.money[5],
	  PM.money[4],PM.money[3],PM.money[2],PM.money[1],(int)PM.diffic);
  encrypt_write(f1, cf_state, out_rec);
  
    /*with py.misc.birth do*/
  sprintf(out_rec, "%ld %d %d %d %d",
	  PM.birth.year,(int)PM.birth.month,(int)PM.birth.day,
	  (int)PM.birth.hour,(int)PM.birth.secs);
  encrypt_write(f1, cf_state, out_rec);
    
    /*with py.misc.cur_age do*/
  sprintf(out_rec, "%ld %d %d %d %d",
	  PM.cur_age.year,(int)PM.cur_age.month,(int)PM.cur_age.day,
	  (int)PM.cur_age.hour,(int)PM.cur_age.secs);
  encrypt_write(f1, cf_state, out_rec);
    
    /*{FUBAR modification for quests}*/
  sprintf(out_rec, "%d %d %d %ld %d", (int)PF.quested, (int)PM.cur_quest, (int)PM.quests, PM.claim_check, (int)PF.light_on);
  encrypt_write(f1, cf_state, out_rec);
    
//    sys_gettim(&current_time);
//    sub_quadtime(current_time,start_time,&delta_time);
//    sys_numtim(&tim,&delta_time);
//    add_play_time(&tim,py.misc.play_tm);

    /*with tim do*/
//  sprintf(out_rec, "%ld %ld %ld %ld %ld %ld %ld",
//	  (int)tim.years,(int)tim.months,(int)tim.days,(int)tim.hours,
//	  (int)tim.minutes,(int)tim.seconds,(int)tim.hundredths);
//  encrypt_write(f1, out_rec);
    
  current_time = time(NULL);
  delta_time   = current_time - start_time;
  sprintf(out_rec,"%ld",convert_time_to_seconds(&py.misc.play_tm)+delta_time);
  encrypt_write(f1, cf_state, out_rec);

  sprintf(out_rec,"%s",py.misc.name);
  encrypt_write(f1, cf_state, out_rec);
	
  sprintf(out_rec,"%s",py.misc.race);
  encrypt_write(f1, cf_state, out_rec);
      
  sprintf(out_rec,"%s",py.misc.sex);
  encrypt_write(f1, cf_state, out_rec);

  sprintf(out_rec,"%s",py.misc.tclass);
  encrypt_write(f1, cf_state, out_rec);
	
  sprintf(out_rec,"%s",py.misc.title);
  encrypt_write(f1, cf_state, out_rec);

  for (i1 = 0; i1 < 5; i1++) {
    strcpy(out_rec,py.misc.history[i1]);
    encrypt_write(f1, cf_state, out_rec);
  }
    
  sprintf(out_rec,"%d",(int)py.misc.cheated);
  encrypt_write(f1, cf_state, out_rec);

  sprintf(out_rec, "%ld %ld %d %d %d %d %d %d %d %ld %ld %ld %d %d %f",
	  char_row,char_col,(int)PM.pclass,(int)PM.prace,
	  (int)PM.age,(int)PM.ht,(int)PM.wt,(int)PM.sc,(int)PM.max_exp,
	  PM.exp,PM.rep,PM.premium,(int)PM.lev,
	  (int)PM.max_lev,PM.expfact);
  encrypt_write(f1, cf_state, out_rec);
    
  sprintf(out_rec, "%d %d %d %d %d %d %f %d %f %d %d %d %d %d %d %d %d %d %d %d",
	  (int)PM.srh,(int)PM.fos,(int)PM.stl,(int)PM.bth,
	  (int)PM.bthb,
	  (int)PM.mana,PM.cmana,(int)PM.mhp,PM.chp,
	  (int)PM.ptohit,(int)PM.ptodam,(int)PM.pac,(int)PM.ptoac,
	  (int)PM.dis_th,(int)PM.dis_td,(int)PM.dis_ac,(int)PM.dis_tac,
	  (int)PM.disarm,(int)PM.save,(int)PM.hitdie);
  encrypt_write(f1, cf_state, out_rec);

    /*change by Dean--inven_ctr calculated from scratch to*/
    /*(hopefully) solve some of the get-after-EOF save bugs*/
    inven_ctr=0;
    curse = inventory_list;
    while (curse != NULL) {
      curse = curse->next;
      inven_ctr++;
    }
//    sprintf(out_rec, "DBG: got some inventory %ld", inven_ctr);
//    prt(out_rec,10,10);

    sprintf(out_rec, "%ld %ld %ld %ld %ld %ld %ld %lu",
	    inven_ctr,inven_weight,equip_ctr,dun_level,
	    missle_ctr,mon_tot_mult,uand(0xF,turn),
	    randes_seed);
    encrypt_write(f1, cf_state, out_rec);
    /*end with py.misc*/
    
    /*with py.flags do*/
    sprintf(out_rec, "%d %d",(int)PF.insured, (int)PF.dead);
    encrypt_write(f1, cf_state, out_rec);
};
//////////////////////////////////////////////////////////////////////
void sc__write_inventory(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
    /*{ Write out the inventory records.	}*/
  treas_ptr      curse;
  
  curse = inventory_list;
  while (curse != NULL) {

    sprintf(out_rec,"%c %s",curse->data.tchar,curse->data.name);
    encrypt_write(f1, cf_state, out_rec);
    
    sprintf(out_rec,"%d %d",(int)curse->is_in,(int)curse->insides);
    encrypt_write(f1, cf_state, out_rec);
    
    sprintf(out_rec,"%s",curse->data.damage);
    encrypt_write(f1, cf_state, out_rec);
    
    //with curse->data do;
      sprintf(out_rec,"%d %d %d %d %d %d %d %d %d %ld %ld %d %ld",
	      (int)curse->data.tval,(int)curse->data.subval,
	      (int)curse->data.weight,(int)curse->data.number,
	      (int)curse->data.tohit,(int)curse->data.todam,
	      (int)curse->data.ac,(int)curse->data.toac,
	      (int)curse->data.p1,
	      curse->data.flags,curse->data.flags2,
	      (int)curse->data.level,curse->data.cost);
    encrypt_write(f1, cf_state, out_rec);
    curse = curse->next;

    } /* end while curse */
};
//////////////////////////////////////////////////////////////////////
void sc__write_equipment(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
    /*{ Write out the equipment records.	}*/

  integer    i1;
    for (i1 = Equipment_min; i1 < EQUIP_MAX; i1++) {

      sprintf(out_rec,"%c %s",equipment[i1].tchar,equipment[i1].name);
      encrypt_write(f1, cf_state, out_rec);

      sprintf(out_rec,"%s",equipment[i1].damage);
      encrypt_write(f1, cf_state, out_rec);

      //with equipment[i1] do;
      sprintf(out_rec,"%d %d %d %d %d %d %d %d %d %ld %ld %d %ld",
	      (int)equipment[i1].tval,(int)equipment[i1].subval,
	      (int)equipment[i1].weight,(int)equipment[i1].number,
	      (int)equipment[i1].tohit,(int)equipment[i1].todam,
	      (int)equipment[i1].ac,(int)equipment[i1].toac,
	      (int)equipment[i1].p1,
	      equipment[i1].flags,equipment[i1].flags2,
	      (int)equipment[i1].level,equipment[i1].cost);
      encrypt_write(f1, cf_state, out_rec);

    } /* end for i1 */
};
//////////////////////////////////////////////////////////////////////
void sc__write_stats_and_flags(FILE *f1, encrypt_state *cf_state,ntype out_rec)
{
    //with py.stat do
      sprintf(out_rec,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	      (int)PS.p[STR],(int)PS.c[STR],(int)PS.m[STR],(int)PS.l[STR],
	      (int)PS.p[INT],(int)PS.c[INT],(int)PS.m[INT],(int)PS.l[INT],
	      (int)PS.p[WIS],(int)PS.c[WIS],(int)PS.m[WIS],(int)PS.l[WIS],
	      (int)PS.p[DEX],(int)PS.c[DEX],(int)PS.m[DEX],(int)PS.l[DEX],
	      (int)PS.p[CON],(int)PS.c[CON],(int)PS.m[CON],(int)PS.l[CON],
	      (int)PS.p[CHR],(int)PS.c[CHR],(int)PS.m[CHR],(int)PS.l[CHR]);
    encrypt_write(f1, cf_state, out_rec);
    
    
    //with py.flags do
      sprintf(out_rec,"%lu %ld %ld %ld %ld %ld %ld %ld %ld %d",
	      PF.status,PF.blind,PF.confused,
	      PF.foodc,PF.food_digested,PF.protection,
	      PF.speed,PF.afraid,
	      PF.poisoned,(int)PF.see_inv);
    encrypt_write(f1, cf_state, out_rec);
    
    sprintf(out_rec,"%ld %ld %ld %d %d %d %ld",
	    PF.fast,PF.slow,PF.protevil,
	    (int)PF.teleport,(int)PF.free_act,(int)PF.slow_digest,
	    PF.petrification);
    encrypt_write(f1, cf_state, out_rec);

    sprintf(out_rec,"%d %d %d %d %d %d %d",
	    (int)PF.aggravate,
	    (int)PF.sustain[STR],(int)PF.sustain[INT],
	    (int)PF.sustain[WIS],(int)PF.sustain[CON],
	    (int)PF.sustain[DEX],(int)PF.sustain[CHR]);
    encrypt_write(f1, cf_state, out_rec);

    sprintf(out_rec,"%d %d %d %d %d %d %d",
	    (int)PF.fire_resist,(int)PF.cold_resist,
	    (int)PF.acid_resist,(int)PF.regenerate,
	    (int)PF.lght_resist,(int)PF.ffall,
	    (int)PF.confuse_monster);
    encrypt_write(f1, cf_state, out_rec);
    
    sprintf(out_rec,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
	    PF.image,PF.invuln,PF.hero,
	    PF.shero,PF.blessed,
	    PF.resist_heat,PF.resist_cold,PF.detect_inv,
	    PF.word_recall,PF.see_infra,PF.tim_infra);
  encrypt_write(f1, cf_state, out_rec);
    
    sprintf(out_rec,"%d %ld %ld %ld %ld %ld %ld %ld %ld %ld",
	    (int)PF.resist_lght,PF.free_time,PF.ring_fire,
	    PF.protmon,PF.hoarse,PF.magic_prot,
	    PF.ring_ice,PF.temp_stealth,PF.resist_petri,
	    PF.blade_ring);
  encrypt_write(f1, cf_state, out_rec);
    /* end with py.flags */
    
};
//////////////////////////////////////////////////////////////////////
void sc__write_magic(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
  integer i1;
  for (i1 = 0; i1 < MAX_SPELLS; i1++) {
    //with magic_spell[py.misc.pclass,i1] do
    sprintf(out_rec,"%d %d",(int)PSPELL(i1).learned,PSPELL(i1).sexp);
  encrypt_write(f1, cf_state, out_rec);
  }
};
//////////////////////////////////////////////////////////////////////
void sc__write_dungeon(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
    /*{ Write the important dungeon info and floor	-RAK-	}*/

  integer        i1, i2, tot_treasure, tptr, count = 0;
  unsigned long  xfloor, prevFloor = 999999;

  sprintf(out_rec,"%ld %ld %ld %ld",cur_height,cur_width,
	  max_panel_rows,max_panel_cols);
  encrypt_write(f1, cf_state, out_rec);
  
  /*{ Save the floor	}*/
  
  tot_treasure = 0;
  for (i1 = 1; i1 <= cur_height; i1++) {
    //out_rec = pad(' ',' ',cur_width);
    for (i2 = 1; i2 <= cur_width; i2++) {
      //with cave[i1][i2]. do;
      xfloor = cave[i1][i2].fval;
      if (cave[i1][i2].fopen) {
	xfloor |= 0x20;
      }
      if (cave[i1][i2].pl) {
	xfloor |= 0x40;
      }
      if (cave[i1][i2].fm) {
	xfloor |= 0x80;
      }
      if (cave[i1][i2].tptr > 0) {
	tot_treasure++;
      }
      
      /* run length encoding the floor cuts about 30k from the save file */
      if (xfloor == prevFloor) {
	count++;
      } else {
	if (count != 0) {
	  if (count == 1) {
	    sprintf(out_rec, "%ld", prevFloor);
	  } else {
	    sprintf(out_rec, "%ld %ld", prevFloor, count);
	  }      
	  encrypt_write(f1, cf_state, out_rec);
	}
	prevFloor = xfloor;
	count = 1;
      }

    } /* end for i2 */
  } /* end for i1 */
  
  if (count == 1) {
    sprintf(out_rec, "%ld", prevFloor);
  } else {
    sprintf(out_rec, "%ld %ld", prevFloor, count);
  }      
  encrypt_write(f1, cf_state, out_rec);

  /*{ Save the Treasure List		}*/
  sprintf(out_rec,"%ld",tot_treasure);
  encrypt_write(f1, cf_state, out_rec);
  
  for (i1 = 1; i1 <= cur_height; i1++) {
    for (i2 = 1; i2 <= cur_width; i2++) {
      if (cave[i1][i2].tptr > 0) {
	tptr = cave[i1][i2].tptr;
	//with t_list[tptr]. do;
	sprintf(out_rec,"%ld %ld",i1,i2);
	encrypt_write(f1, cf_state, out_rec);
  
	sprintf(out_rec,"%c %s",t_list[tptr].tchar,t_list[tptr].name);
	encrypt_write(f1, cf_state, out_rec);
	
	sprintf(out_rec,"%s",t_list[tptr].damage);
	encrypt_write(f1, cf_state, out_rec);
	
	sprintf(out_rec,"%d %ld %d %d %d %d %d %d %ld %ld %ld %d %ld",
		t_list[tptr].tval,t_list[tptr].subval,t_list[tptr].weight,
		t_list[tptr].number,t_list[tptr].tohit,t_list[tptr].todam,
		t_list[tptr].ac,t_list[tptr].toac,t_list[tptr].p1,
		t_list[tptr].flags,t_list[tptr].flags2,t_list[tptr].level,
		t_list[tptr].cost);
	encrypt_write(f1, cf_state, out_rec);
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void sc__write_identified(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
  /*{ Save identified list			}*/
  integer i1;

  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {
    if (object_ident[i1]) {
      out_rec[i1-1] = 'T';
    } else {
      out_rec[i1-1] = 'F';
    }
  }
  out_rec[MAX_OBJECTS] = 0;
  encrypt_write(f1, cf_state, out_rec);
};
//////////////////////////////////////////////////////////////////////
void sc__write_monsters(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
  /*{ Save the Monster List 		}*/
  integer    i1, tot_monsters;
  
  for (i1=muptr,tot_monsters = 0; i1 > 0; i1 = m_list[i1].nptr) {
    tot_monsters++;
  }
  
  sprintf(out_rec,"%ld",tot_monsters);
  encrypt_write(f1, cf_state, out_rec);
  
  for (i1 = muptr; i1 > 0; i1 = m_list[i1].nptr) {
    
    // with m_list[i1] do;
    sprintf(out_rec,"%d %d %d %d %d %d %d %d %d",
	    m_list[i1].fy,m_list[i1].fx,m_list[i1].mptr,m_list[i1].hp,
	    m_list[i1].cspeed,m_list[i1].csleep,m_list[i1].cdis,m_list[i1].ml,
	    m_list[i1].confused);
  encrypt_write(f1, cf_state, out_rec);

  }
  
};
//////////////////////////////////////////////////////////////////////
void sc__write_town(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
      /*{ Save the town level stores		}*/

  integer          i1,i2;
  game_time_type   st;

  sprintf(out_rec,"%ld",town_seed);
  encrypt_write(f1, cf_state, out_rec);
  
  sprintf(out_rec,"%ld %ld %ld %ld %ld %ld %ld",
	  bank[0],bank[6],bank[5],
	  bank[4],bank[3],bank[2],bank[1]);
  encrypt_write(f1, cf_state, out_rec);
  
  for (i1 = 0; i1 < MAX_STORES; i1++) {
    //with stores[i1]. do;
    /*{ Save items...                 }*/
    sprintf(out_rec,"%d",stores[i1].store_ctr);
  encrypt_write(f1, cf_state, out_rec);
    
    for (i2 = 1; i2 <= stores[i1].store_ctr; i2++) {
      //with stores[i1].store_inven[i2].sitem do;
      sprintf(out_rec,"%ld",stores[i1].store_inven[i2].scost);
      encrypt_write(f1, cf_state, out_rec);
      
      sprintf(out_rec,"%c %s",
	      stores[i1].store_inven[i2].sitem.tchar,
	      stores[i1].store_inven[i2].sitem.name);
      encrypt_write(f1, cf_state, out_rec);
      
      sprintf(out_rec,"%s",stores[i1].store_inven[i2].sitem.damage);
      encrypt_write(f1, cf_state, out_rec);
      
      sprintf(out_rec,"%d %ld %d %d %d %d %d %d %ld %ld %ld %d %ld",
	      stores[i1].store_inven[i2].sitem.tval,
	      stores[i1].store_inven[i2].sitem.subval,
	      stores[i1].store_inven[i2].sitem.weight,
	      stores[i1].store_inven[i2].sitem.number,
	      stores[i1].store_inven[i2].sitem.tohit,
	      stores[i1].store_inven[i2].sitem.todam,
	      stores[i1].store_inven[i2].sitem.ac,
	      stores[i1].store_inven[i2].sitem.toac,
	      stores[i1].store_inven[i2].sitem.p1,
	      stores[i1].store_inven[i2].sitem.flags,
	      stores[i1].store_inven[i2].sitem.flags2,
	      stores[i1].store_inven[i2].sitem.level,
	      stores[i1].store_inven[i2].sitem.cost);
      encrypt_write(f1, cf_state, out_rec);
      //end with store inven;
    } //end for i2;
    
    //with stores[i1].store_inven[i2].store_open. do;
    //with py.misc do;
    st = stores[i1].store_open;
    if ((PM.cur_age.year > st.year) ||
	((PM.cur_age.year == st.year) &&
	 ((PM.cur_age.month > st.month) ||
	  ((PM.cur_age.month == st.month) &&
	   ((PM.cur_age.day > st.day) ||
	    ((PM.cur_age.day == st.day) &&
	     ((PM.cur_age.hour > st.hour) ||
	      ((PM.cur_age.hour == st.hour) ||
	       ((PM.cur_age.secs > st.secs)))))))))) {
      st.year  = 0;
      st.month = 0;
      st.day   = 0;
      st.hour  = 0;
      st.secs  = 0;
      stores[i1].store_open = st;
    }
    
    //with store_open do;
    sprintf(out_rec,"%d %d %ld %d %d %d %d",
	    stores[i1].owner,stores[i1].insult_cur,st.year,
	    st.month,st.day,st.hour,st.secs);
      encrypt_write(f1, cf_state, out_rec);
  } //end for i1;
  
};
//////////////////////////////////////////////////////////////////////
void sc__write_version(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
    /*{ Version number of Moria               }*/
    sprintf(out_rec,"%3.2f",CUR_VERSION);
    encrypt_write(f1, cf_state, out_rec);
};
//////////////////////////////////////////////////////////////////////
void sc__write_seeds(FILE *f1, encrypt_state *cf_state, ntype out_rec)
{
  /* creation_time, save_count and deaths are in the master file, be sure
     to fix up char_restore if you move more onto this line               */

  unsigned long  save_seed;

    save_seed  = get_seed();
    save_seed ^= randint(9999999);
    sprintf(out_rec,"%lu %ld %ld %ld",save_seed,py.misc.creation_time,
	    py.misc.save_count,py.misc.deaths);
    //sprintf(title2,"%lu %s",save_seed,py.misc.ssn); 
    set_seed(ENCRYPT_SEED2);
    encrypt_write(f1, cf_state, out_rec);
    set_seed(save_seed);
};
//////////////////////////////////////////////////////////////////////
boolean save_char(boolean quick)
{
  /*	{ Actual save procedure 			-RAK- & -JWT-	}*/
  
  vtype          fnam;
  boolean        flag;
  ntype          out_rec;
  FILE          *f1 = NULL;  //: text;
  encrypt_state  cf_state;
  GDBM_FILE      f2;
  
  sc__open_save_file(&f1, fnam, quick, &flag);
  encrypt_init(&cf_state, saveFileKey, saveFilesAreEncrypted);
  
  //unlink(fnam); /* I don't think this is needed... */

  if (flag) {
    sc__open_master(f1, &f2, &flag);
  }
  if (flag) {
    sc__write_master(f2, &flag);
    master_file_close(&f2);
  }
  
  /*{ If ID was written to MASTER, continue saving          -RAK-   }*/
  if (flag) {
    sc__write_seeds(f1, &cf_state, out_rec);
    sc__display_status(quick, fnam, out_rec);
    
    sc__write_version(f1, &cf_state, out_rec);
    sc__write_player_record(f1, &cf_state, out_rec);
//    printf("\tgot past player record: %ld\n", flag); fflush(stdout);
    sc__write_inventory(f1, &cf_state, out_rec);
    sc__write_equipment(f1, &cf_state, out_rec);
    sc__write_stats_and_flags(f1, &cf_state, out_rec);
    sc__write_magic(f1, &cf_state, out_rec);
    sc__write_dungeon(f1, &cf_state, out_rec);
    sc__write_identified(f1, &cf_state, out_rec);
    sc__write_monsters(f1, &cf_state, out_rec);
    sc__write_town(f1, &cf_state, out_rec);
    
    sprintf(out_rec, "%ld", PM.creation_time);
    encrypt_write(f1, &cf_state, out_rec);
    
    encrypt_flush(f1, &cf_state);
    fclose(f1);
    
  } /* endif flag */
  
  if (flag && !py.flags.dead) {
    sprintf(out_rec,"Character saved. [Moria Version %3.2f]\n",CUR_VERSION);
    prt(out_rec,2,1);
    exit_game();
  }
  
  set_seed(get_seed());

  return flag;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__add_item(treas_ptr *cur_bag)
{
  treas_ptr      ptr,curse;
  

  /* {Extensive clarifications and bug fixes here by Dean}*/
  ptr = (treas_ptr)safe_malloc(sizeof(treas_rec),"gc__add_item");

  if (inventory_list == nil) {
    inventory_list = ptr;
  } else {
    curse = inventory_list;
    while (curse->next != nil) {
      curse = curse->next;
    }
    curse->next = ptr;
  }

  ptr->data = inven_temp->data;
  ptr->is_in = inven_temp->is_in;
  ptr->insides = inven_temp->insides;
  ptr->ok = false;
  ptr->next = nil;

  if (ptr->data.tval == bag_or_sack) {
    *cur_bag = ptr;
  }

  if (ptr->is_in && (*cur_bag != nil)) {
    ((*cur_bag)->insides)++;
  }

//    printf("\n\tgot item: >>>%s<<<\n", ptr->data.name); fflush(stdout);
}
//////////////////////////////////////////////////////////////////////
void gc__open_save_file(FILE **f1, vtype fnam, boolean *paniced)
{
  vtype   out_str;

  *f1 = (FILE *)fopen(fnam,"r");
  if (*f1 == NULL) {
    sprintf(out_str, "Error Opening> %s", fnam);
    prt(out_str,1,1);
    prt("",2,1);
    *paniced = true;
  } else {
    rewind(*f1);
  }
};
//////////////////////////////////////////////////////////////////////
void gc__open_master(FILE *f1, GDBM_FILE *f2, boolean *paniced)
{
  if (!master_file_open(f2)) {
    prt("ERROR opening file MASTER.",1,1);
    *paniced = true;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_master(GDBM_FILE f2, boolean *paniced)
{
  int           result;
  master_key    mkey;

  mkey.creation_time = PM.creation_time;

  result = master_file_verify(f2, &mkey);

  if (result != MF_CHAR_OK) {
    printf("\n\r--------------------------------------------------------\n\r");
    printf(    "| There was a data corruption error with the character |\n\r");
    printf(    "| Terminating IMORIA now.                              |\n\r");
    printf(    "--------------------------------------------------------\n\r");
//    exit_game();

    *paniced = true;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_seeds(FILE *f1, encrypt_state *cf_state, ntype in_rec,
		    boolean *paniced)
{
  /* creation_time, save_count and deaths are in the master file, be sure
     to fix up char_restore if you move more onto this line               */

  unsigned long  save_seed;

  set_seed(ENCRYPT_SEED2);
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%lu %ld %ld %ld",&save_seed, &py.misc.creation_time,
	     &py.misc.save_count, &py.misc.deaths) != 4) {
    *paniced = true;
  }

//  strcpy(temp,in_rec+13);
//  py.misc.ssn = temp;
  
//  set_seed(ENCRYPT_SEED1);
//  coder(temp);
//  temp_id = temp;

    set_seed(save_seed);
};
//////////////////////////////////////////////////////////////////////
void gc__display_status(vtype fnam, ntype in_rec)
{
  prt("Restoring Character...",1,1);
  put_qio();
};
//////////////////////////////////////////////////////////////////////
void gc__read_version(FILE *f1, encrypt_state *cf_state,ntype in_rec,
		      boolean *paniced,real *save_version)
{
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%f",save_version) != 1) {
    *paniced = true;
  }

  if ( (int)(*save_version*100.0) != (int)(CUR_VERSION*100.0)) {
    prt("Save file is incompatible with this version.",2,1);
    sprintf(in_rec,"  [Save file version %3.2f]",*save_version);
    prt(in_rec,3,1);
    sprintf(in_rec,"  [Moria version     %3.2f]",CUR_VERSION);
    prt(in_rec,4,1);

    exit_game();

    prt("Updating character for newer version...",5,1);
    pause_game(24);
  }

};
//////////////////////////////////////////////////////////////////////
void gc__read_player_record(FILE *f1, encrypt_state *cf_state, ntype in_rec,
			    boolean *paniced, boolean prop, boolean *was_dead)
{
  int       x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18;
  integer   i1;
  time_t    old_time;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %d",
	 &PM.xtr_wgt,&PM.account,&PM.money[0],&PM.money[6],&PM.money[5],
	     &PM.money[4],&PM.money[3],&PM.money[2],&PM.money[1],&x1) != 10) {
    *paniced = true;
  }
  PM.diffic = x1;

  /*with py.misc.birth do*/
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld %d %d %d %d",
	     &PM.birth.year,&x1,&x2,&x3,&x4) != 5) {
    *paniced = true;
  }
  PM.birth.month = x1;
  PM.birth.day   = x2;
  PM.birth.hour  = x3;
  PM.birth.secs  = x4;
    
    /*with py.misc.cur_age do*/
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld %d %d %d %d",
	     &PM.cur_age.year,&x1,&x2,&x3,&x4) != 5) {
    *paniced = true;
  }
  PM.cur_age.month = x1;
  PM.cur_age.day   = x2;
  PM.cur_age.hour  = x3;
  PM.cur_age.secs  = x4;
    
    /*{FUBAR modification for quests}*/
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%d %d %d %ld %d", &x1, &x2, &x3, &PM.claim_check, &x4) 
      !=5 ) {
    *paniced = true;
  }
  PF.quested   = x1;
  PM.cur_quest = x2;
  PM.quests    = x3;
  PF.light_on  = x4;
    
//    sys_gettim(&current_time);
//    sub_quadtime(current_time,start_time,&delta_time);
//    sys_numtim(&tim,&delta_time);
//    add_play_time(&tim,py.misc.play_tm);

    /*with tim do*/
//  read_decrypt(f1, cf_state, in_rec, paniced);
//  sscanf(in_rec, "%ld %ld %ld %ld %ld %ld %ld",
//	  (int)tim.years,(int)tim.months,(int)tim.days,(int)tim.hours,
//	  (int)tim.minutes,(int)tim.seconds,(int)tim.hundredths);

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld", &old_time) != 1) {
    *paniced = true;
  }
  convert_seconds_to_time(old_time, &py.misc.play_tm);
  start_time = time(NULL);
    
  read_decrypt(f1, cf_state, in_rec, paniced);
  strncpy(py.misc.name, in_rec, sizeof(vtype));
	
  read_decrypt(f1, cf_state, in_rec, paniced);
  strncpy(py.misc.race, in_rec, sizeof(vtype));
      
  read_decrypt(f1, cf_state, in_rec, paniced);
  strncpy(py.misc.sex, in_rec, sizeof(vtype));

  read_decrypt(f1, cf_state, in_rec, paniced);
  strncpy(py.misc.tclass, in_rec, sizeof(vtype));
	
  read_decrypt(f1, cf_state, in_rec, paniced);
  strncpy(py.misc.title, in_rec, sizeof(vtype));

  for (i1 = 0; i1 < 5; i1++) {
    read_decrypt(f1, cf_state, in_rec, paniced);
    strncpy(py.misc.history[i1], in_rec, sizeof(vtype));
  }
    
  read_decrypt(f1, cf_state, in_rec, paniced);
  if(sscanf(in_rec,"%d",&x1) != 1) {
    *paniced = true;
  }
  py.misc.cheated |= x1;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld %ld %d %d %d %d %d %d %d %ld %ld %ld %d %d %f",
	     &char_row,&char_col,&x1,&x2,
	     &x3,&x4,&x5,&x6,&x7,
	     &PM.exp,&PM.rep,&PM.premium,&x8,
	     &x9,&PM.expfact) != 15) {
    *paniced = true;
  }

  PM.pclass    = x1;
  PM.prace     = x2;
  PM.age       = x3;
  PM.ht        = x4;
  PM.wt        = x5;
  PM.sc        = x6;
  PM.max_exp   = x7;
  PM.lev       = x8;
  PM.max_lev   = x9;

  if (PM.wt > max_allowable_weight()) {
    PM.wt = ( 0.9*max_allowable_weight());
  } else if (PM.wt < min_allowable_weight()) {
    PM.wt = ( 1.10*min_allowable_weight()) ;
  }

  if (py.misc.pclass == C_WARRIOR) {
    py.misc.mr = -10;
  } else if ((py.misc.pclass == C_MAGE) || (py.misc.pclass == C_PRIEST)) {
    py.misc.mr = 0;
  } else {
    py.misc.mr = -5;
  }

  read_decrypt(f1, cf_state, in_rec, paniced);
  //  sscanf(in_rec, "%ld %ld %f %ld %f %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
	     //	  &(PM.srh),&(PM.fos),&(PM.stl),&(PM.bth),
	     //	  &x1,
	     //	  &x2, &(PM.cmana), &x3, &(PM.chp),
	     //	  &x4, &x5, &x7, &x7, 
	     //	  &x8, &x9, &x10, &x11, 
	     //	  &x12, &x13, &x14);
  if (sscanf(in_rec, "%d %d %d %d %d %d %f %d %f %d %d %d %d %d %d %d %d %d %d %d",
	     &x1,&x2,&x3,&x4,
	     &x5,
	     &x6, &(PM.cmana), &x7, &(PM.chp),
	     &x8, &x9, &x10, &x11, 
	     &x12, &x13, &x14, &x15, 
	     &x16, &x17, &x18) != 20) {
    *paniced = true;
  }
  PM.srh     = x1;
  PM.fos     = x2;
  PM.stl     = x3;
  PM.bth     = x4;
  PM.bthb    = x5;
  PM.mana    = x6;
  PM.mhp     = x7;
  PM.ptohit  = x8;
  PM.ptodam  = x9;
  PM.pac     = x10;
  PM.ptoac   = x11;
  PM.dis_th  = x12;
  PM.dis_td  = x13;
  PM.dis_ac  = x14;
  PM.dis_tac = x15;
  PM.disarm  = x16;
  PM.save    = x17;
  PM.hitdie  = x18;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld %ld %ld %ld %ld %ld %ld %lu",
	     &inven_ctr,&inven_weight,&equip_ctr,&dun_level,
	     &missle_ctr,&mon_tot_mult,&turn,
	     &randes_seed) != 8) {
    *paniced = true;
  }
  /*end with py.misc*/
    
  /*with py.flags do*/
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%d %d",&x1, &x2) != 2) {
    *paniced = true;
  }
  PF.insured = x1;
  PF.dead    = x2;

  *was_dead = false;

  if (PF.dead && prop) {
    msg_print("Hmmm, it would appear that you are dead.");
    if (PF.insured) {
      msg_print("Luckily, your insurance is paid up!");
      py.misc.deaths++;
      PF.insured = false;
    } else {
      msg_print("Unfortunately, you did not pay for insurance.");
      put_qio();
      printf("\n\r\n\r");
      exit_game();
    }
    *was_dead = true;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_inventory(FILE *f1, encrypt_state *cf_state, ntype in_rec,
			boolean *paniced, boolean *was_dead)
{
  /* { Read in the inventory records.	}*/

  integer      lost_inven_count;
  boolean      bag_lost;
  treas_ptr    ptr,cur_bag;
  integer      i1;
  char         trash_char;
  int          x1,x2,x3,x4,x5,x6,x7,x8,x9,x10;

  inventory_list = nil;
  lost_inven_count = 0;
  bag_lost = false;
  cur_bag = nil;

  for (i1 = 0; i1 < inven_ctr; i1++) {
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec, "%c", &(inven_temp->data.tchar)) != 1) {
      *paniced = true;
    }
    if (strlen(in_rec) > 2) {
      strncpy(inven_temp->data.name, in_rec+2, sizeof(ttype));
    } else {
      strncpy(inven_temp->data.name, "unknown object", sizeof(ttype));
    }
    
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec,"%c %d",&trash_char, &x1) != 2) {
      *paniced = true;
    }
    inven_temp->is_in = (trash_char == 'T');
    inven_temp->insides = x1;

    read_decrypt(f1, cf_state, in_rec, paniced);
    strncpy(inven_temp->data.damage, in_rec, sizeof(dtype));
    
    read_decrypt(f1, cf_state, in_rec, paniced);
    //with inven_temp->data do;
    if (sscanf(in_rec,"%d %d %d %d %d %d %d %d %d %ld %ld %d %ld",
	       &x1, &x2,
	       &x3, &x4,
	       &x5, &x6,
	       &x7, &x8,
	       &x9,
	       &(inven_temp->data.flags),&(inven_temp->data.flags2),
	       &x10,&(inven_temp->data.cost)) != 13) {
      *paniced = true;
    }

      inven_temp->data.tval    = x1;
      inven_temp->data.subval  = x2;
      inven_temp->data.weight  = x3;
      inven_temp->data.number  = x4;
      inven_temp->data.tohit   = x5;
      inven_temp->data.todam   = x6;
      inven_temp->data.ac      = x7;
      inven_temp->data.toac    = x8;
      inven_temp->data.p1      = x9;
      inven_temp->data.level   = x10;


    if ((*was_dead) &&
	((inven_temp->data.flags2 & Insured_bit) == 0)) {
      if (inven_temp->data.tval == bag_or_sack) {
	bag_lost = true;
      }
      lost_inven_count++;
      inven_weight = inven_weight - (inven_temp->data.number *
				     inven_temp->data.weight);
    } else if (bag_lost && inven_temp->is_in) {
      lost_inven_count++;
    } else {
      if (*was_dead) {
	inven_temp->data.flags2 &= ~Insured_bit;
	inven_temp->data.flags2 |=  Blackmarket_bit;
      }
      gc__add_item(&cur_bag);
      bag_lost = false;
    }
  } /* end for i1 */

  for (inven_ctr = 0, ptr = inventory_list; ptr != nil; ) {
    ptr = ptr->next;
    inven_ctr++;
  }

  if (lost_inven_count == 1) {
    msg_print("You lost an item that wasn't insured.");
  } else if (lost_inven_count > 1) {
    msg_print("You lost several items that weren't insured.");
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_equipment(FILE *f1, encrypt_state *cf_state, ntype in_rec,
			boolean *paniced, boolean *was_dead)
{
  /*{ Read in the equipment records.	}*/

  integer      i1;
  integer      lost_equip_count;
  int          x1,x2,x3,x4,x5,x6,x7,x8,x9,x10;

  lost_equip_count = 0;

  for (i1 = Equipment_min; i1 < EQUIP_MAX; i1++) {
    //with inven_temp->data do;
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (strlen(in_rec) > 0) {
      sscanf(in_rec, "%c", &(inven_temp->data.tchar));
    } else {
      inven_temp->data.tchar = 0;
    }
    if (strlen(in_rec) > 2) {
      strncpy(inven_temp->data.name, in_rec+2, sizeof(ttype));
    } else {
      inven_temp->data.name[0] = 0;
    }

    read_decrypt(f1, cf_state, in_rec, paniced);
    strncpy(inven_temp->data.damage, in_rec, sizeof(dtype));
    
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec,"%d %d %d %d %d %d %d %d %d %ld %ld %d %ld",
	       &x1, &x2,
	       &x3, &x4,
	       &x5, &x6,
	       &x7, &x8,
	       &x9,
	       &(inven_temp->data.flags),&(inven_temp->data.flags2),
	       &x10,&(inven_temp->data.cost)) != 13) {
      *paniced = true;
    }

    inven_temp->data.tval    = x1;
    inven_temp->data.subval  = x2;
    inven_temp->data.weight  = x3;
    inven_temp->data.number  = x4;
    inven_temp->data.tohit   = x5;
    inven_temp->data.todam   = x6;
    inven_temp->data.ac      = x7;
    inven_temp->data.toac    = x8;
    inven_temp->data.p1      = x9;
    inven_temp->data.level   = x10;
    
    if ((*was_dead) && (inven_temp->data.tval > 0) &&
	(uand(inven_temp->data.flags2,Insured_bit) == 0)) {
      lost_equip_count++;
      equipment[i1] = blank_treasure;
      inven_weight -= inven_temp->data.number *	inven_temp->data.weight;
      if (i1 != EQUIP_MAX-1) {
	py_bonuses(&(inven_temp->data),-1);
      }
    } else {
      if (*was_dead) {
	inven_temp->data.flags2 = uand(inven_temp->data.flags2,
					0xBFFFFFFF);
      }
      equipment[i1] = inven_temp->data;
    }
  } /* end for f1 */

  equip_ctr -= lost_equip_count;
  if (lost_equip_count == 1) {
    msg_print("You lost a piece of equipment that wasn't insured.");
  } else if (lost_equip_count > 1) {
    msg_print("You lost several pieces of equipment that weren't insured.");
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_stats_and_flags(FILE *f1, encrypt_state *cf_state, ntype in_rec,
			      boolean *paniced)
{
  int    x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12;
  int    x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if(sscanf(in_rec,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	    &x1,  &x2,  &x3,  &x4,
	    &x5,  &x6,  &x7,  &x8,
	    &x9,  &x10, &x11, &x12,
	    &x13, &x14, &x15, &x16,
	    &x17, &x18, &x19, &x20,
	    &x21, &x22, &x23, &x24) != 24) {
    *paniced = true;
  }

  PS.p[STR] =  x1; PS.c[STR] =  x2; PS.m[STR] =  x3; PS.l[STR] =  x4;
  PS.p[INT] =  x5; PS.c[INT] =  x6; PS.m[INT] =  x7; PS.l[INT] =  x8;
  PS.p[WIS] =  x9; PS.c[WIS] = x10; PS.m[WIS] = x11; PS.l[WIS] = x12;
  PS.p[DEX] = x13; PS.c[DEX] = x14; PS.m[DEX] = x15; PS.l[DEX] = x16;
  PS.p[CON] = x17; PS.c[CON] = x18; PS.m[CON] = x19; PS.l[CON] = x20;
  PS.p[CHR] = x21; PS.c[CHR] = x22; PS.m[CHR] = x23; PS.l[CHR] = x24;    

  //with py.flags do;
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%lu %ld %ld %ld %ld %ld %ld %ld %ld %d",
	 &(PF.status),&(PF.blind),&(PF.confused),
	 &(PF.foodc),&(PF.food_digested),&(PF.protection),
	 &(PF.speed),&(PF.afraid),
	 &(PF.poisoned), &x1) != 10) {
    *paniced = true;
  }
  PF.see_inv = x1;
    
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld %ld %ld %d %d %d %ld",
	 &(PF.fast),&(PF.slow),&(PF.protevil),
	 &x1, &x2, &x3,
	 &(PF.petrification)) != 7) {
    *paniced = true;
  }
  PF.teleport    = x1;
  PF.free_act    = x2;
  PF.slow_digest = x3;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%d %d %d %d %d %d %d",
	 &x1, &x2, &x3, &x4, &x5, &x6, &x7) != 7) {
    *paniced = true;
  }
  PF.aggravate    = x1;
  PF.sustain[STR] = x2;
  PF.sustain[INT] = x3;
  PF.sustain[WIS] = x4;
  PF.sustain[CON] = x5;
  PF.sustain[DEX] = x6;
  PF.sustain[CHR] = x7;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%d %d %d %d %d %d %d",
	 &x1, &x2, &x3, &x4, &x5, &x6, &x7) != 7) {
    *paniced = true;
  }
  PF.fire_resist      = x1;
  PF.cold_resist      = x2;
  PF.acid_resist      = x3;
  PF.regenerate       = x4;
  PF.lght_resist      = x5;
  PF.ffall            = x6;
  PF.confuse_monster  = x7;
  
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
	 &(PF.image),&(PF.invuln),&(PF.hero),
	 &(PF.shero),&(PF.blessed),
	 &(PF.resist_heat),&(PF.resist_cold),&(PF.detect_inv),
	 &(PF.word_recall),&(PF.see_infra),&(PF.tim_infra)) != 11) {
    *paniced = true;
  }
  
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
	 &(PF.resist_lght),&(PF.free_time),&(PF.ring_fire),
	 &(PF.protmon),&(PF.hoarse),&(PF.magic_prot),
	 &(PF.ring_ice),&(PF.temp_stealth),&(PF.resist_petri),
	 &(PF.blade_ring)) != 10) {
    *paniced = true;
  }
  
    /* end with py.flags */
};
//////////////////////////////////////////////////////////////////////
void gc__read_magic(FILE *f1, encrypt_state *cf_state, ntype in_rec,
		    boolean *paniced)
{
  integer i1;
  int     x1, x2;

  for (i1 = 0; i1 < MAX_SPELLS; i1++) {
    //with magic_spell[py.misc.pclass,i1] do;
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec,"%d %d", &x1, &x2) != 2) {
      *paniced = true;
    }
    PSPELL(i1).learned = x1;
    PSPELL(i1).sexp    = x2;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_dungeon(FILE *f1, encrypt_state *cf_state, ntype in_rec,
		      boolean *paniced)
{
  integer   i1,i2,i3,i4,tot_treasure;
  integer   x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13;
  integer   count = 0;
  unsigned long  xfloor;

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld %ld %ld %ld",&cur_height,&cur_width,
	 &max_panel_rows,&max_panel_cols) != 4) {
    *paniced = true;
  }
  
  if (cur_height > MAX_HEIGHT) { 
    *paniced   = true;
  } else if (cur_width > MAX_WIDTH) { 
    *paniced   = true;
  }

  /*{ Read the floor	}*/
  
  if (!(*paniced)) {
    for (i1 = 1; i1 <= cur_height; i1++) {
      //read_decrypt(f1, cf_state, in_rec, paniced);
      for (i2 = 1; i2 <= cur_width; i2++) {

	if (count == 0) {
	  read_decrypt(f1, cf_state, in_rec, paniced);
	  if (sscanf(in_rec, "%ld %ld", &xfloor, &count) == 1) {	  
	    count = 1;
	  }
	}
	count--;
	
	//with cave[i1][i2]. do;
	cave[i1][i2].fval = (xfloor & 0x1f);
	if (xfloor & 0x20) {
	  cave[i1][i2].fopen = true;
	}
	if (xfloor & 0x40) {
	  cave[i1][i2].pl = true;
	}
	if (xfloor & 0x80) {
	  cave[i1][i2].fm = true;
	}
	cave[i1][i2].tl   = false;
	cave[i1][i2].tptr = 0;
	cave[i1][i2].cptr = 0;
	
      } /* end for i2 */
    } /* end for i1 */
  }  

  /*{ Read the Treasure List		}*/
  tlink();
  if (!(*paniced)) {
    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec,"%ld",&tot_treasure) != 1) {
      *paniced = true;
    }
  
    for (i1 = 1; i1 <= tot_treasure; i1++) {
      popt(&i2);
      //with t_list[i2] do;
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      if (sscanf(in_rec,"%ld %ld",&i3,&i4) != 2) {
	*paniced = true;
      }
      cave[i3][i4].tptr = i2;
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      t_list[i2].tchar = in_rec[0];
      strcpy(t_list[i2].name, &in_rec[2]);
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      strcpy(t_list[i2].damage, in_rec);
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
		 &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8, &x9, 
		 &x10, &x11, &x12, &x13) != 13) {
	*paniced = true;
      }
      
      t_list[i2].tval    = x1;
      t_list[i2].subval  = x2;
      t_list[i2].weight  = x3;
      t_list[i2].number  = x4;
      t_list[i2].tohit   = x5;
      t_list[i2].todam   = x6;
      t_list[i2].ac      = x7;
      t_list[i2].toac    = x8;
      t_list[i2].p1      = x9;
      t_list[i2].flags   = x10;
      t_list[i2].flags2  = x11;
      t_list[i2].level   = x12;
      t_list[i2].cost    = x13;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__read_identified(FILE *f1, encrypt_state *cf_state, ntype in_rec,
			 boolean *paniced)
{
  integer i1;

  read_decrypt(f1, cf_state, in_rec, paniced);
  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {
    if (in_rec[i1-1] == 'T') {
      identify(&(object_list[i1]));
    } else if (in_rec[i1-1] == 'F') {
      object_ident[i1] = false;
    } else {
      *paniced = true;
    }
  } 
};
//////////////////////////////////////////////////////////////////////
void gc__read_monsters(FILE *f1, encrypt_state *cf_state, ntype in_rec,
		       boolean *paniced)
{
  int       x1,x2,x3,x4,x5,x6,x7,x8,x9;
  integer   i1,i2,i3, tot_monsters;
  
  muptr = 0;
  mlink();

  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld",&tot_monsters) != 1) {
    *paniced = true;
  }

  i3 = 0;
  for (i1 = 1 ; i1 <= tot_monsters ; i1++) {
    read_decrypt(f1, cf_state, in_rec, paniced);
    popm(&i2);
    
    // with m_list[i2] do;
    if (sscanf(in_rec,"%d %d %d %d %d %d %d %d %d",
	   &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8, &x9) != 9) {
      *paniced = true;
    }
    m_list[i2].fy       = x1;
    m_list[i2].fx       = x2;
    m_list[i2].mptr     = x3;
    m_list[i2].hp       = x4;
    m_list[i2].cspeed   = x5;
    m_list[i2].csleep   = x6;
    m_list[i2].cdis     = x7;
    m_list[i2].ml       = x8;
    m_list[i2].confused = x9;

    if (x1 > MAX_HEIGHT) { 
      *paniced   = true;
    } else if (x2 > MAX_WIDTH) { 
      *paniced   = true;
    }

    if (!(*paniced)) {
      cave[x1][x2].cptr = i2;
      if (muptr == 0) {
	muptr = i2;
      } else {
	m_list[i3].nptr = i2;
      }
      m_list[i2].nptr = 0;
      i3 = i2;
    }
  }
  
};
//////////////////////////////////////////////////////////////////////
void gc__read_town(FILE *f1, encrypt_state *cf_state, ntype in_rec,
		   boolean *paniced)
{
  integer     x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13;
  integer     i1, i2, i3;
  game_time_type   st;

  /*{ Restore the town level stores 	}*/
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec, "%ld", &town_seed) != 1) {
    *paniced = true;
  }
  
  read_decrypt(f1, cf_state, in_rec, paniced);
  if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld",
	    &bank[0],&bank[6],&bank[5],
	    &bank[4],&bank[3],&bank[2],&bank[1]) != 7) {
    *paniced = true;
  }
  
  for (i1 = 0; i1 < MAX_STORES; i1++) {
    //with stores[i1]. do;
    read_decrypt(f1, cf_state, in_rec, paniced);
    sscanf(in_rec,"%ld",&i2);
    stores[i1].store_ctr = i2;
    for (i3 = 1; i3 <= i2; i3++) {
      //with stores[i1].store_inven[i3].sitem do;
      read_decrypt(f1, cf_state, in_rec, paniced);
      if (sscanf(in_rec,"%ld",&stores[i1].store_inven[i3].scost) != 1) {
	*paniced = true;
      }
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      stores[i1].store_inven[i3].sitem.tchar = in_rec[0];
      strcpy(stores[i1].store_inven[i3].sitem.name, &in_rec[2]);
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      strcpy(stores[i1].store_inven[i3].sitem.damage, in_rec);
      
      read_decrypt(f1, cf_state, in_rec, paniced);
      if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
		 &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8, &x9,
		 &x10, &x11, &x12, &x13) != 13) {
	*paniced = true;
      }
      
      stores[i1].store_inven[i3].sitem.tval    = x1;
      stores[i1].store_inven[i3].sitem.subval  = x2;
      stores[i1].store_inven[i3].sitem.weight  = x3;
      stores[i1].store_inven[i3].sitem.number  = x4;
      stores[i1].store_inven[i3].sitem.tohit   = x5;
      stores[i1].store_inven[i3].sitem.todam   = x6;
      stores[i1].store_inven[i3].sitem.ac      = x7;
      stores[i1].store_inven[i3].sitem.toac    = x8;
      stores[i1].store_inven[i3].sitem.p1      = x9;
      stores[i1].store_inven[i3].sitem.flags   = x10;
      stores[i1].store_inven[i3].sitem.flags2  = x11;
      stores[i1].store_inven[i3].sitem.level   = x12;
      stores[i1].store_inven[i3].sitem.cost    = x13;
    }
    
    /*{ If not current version then re-outfit the stores      }*/

    read_decrypt(f1, cf_state, in_rec, paniced);
    if (sscanf(in_rec,"%ld %ld %ld %ld %ld %ld %ld",
	   &x1,&x2,&x3,&x4,&x5,&x6,&x7) != 7) {
      *paniced = true;
    }
    stores[i1].owner      = x1;
    stores[i1].insult_cur = x2;
    st.year               = x3;
    st.month              = x4;
    st.day                = x5;
    st.hour               = x6;
    st.secs               = x7;
    stores[i1].store_open = st;
  }

};
//////////////////////////////////////////////////////////////////////
boolean get_char(vtype fnam, boolean prop)
{
  /*{ Restore a saved game				-RAK- & -JWT-	}*/
  
  real           save_version;
  ntype          in_rec;
  FILE          *f1 = NULL;
  GDBM_FILE      f2 = NULL;
  boolean        dun_flag;
  boolean        was_dead,paniced;
  encrypt_state  cf_state;
  integer        check_time;
  
  dun_flag = false;
  paniced = false;
// clear_from(1);

  gc__open_save_file(&f1, fnam, &paniced);
  encrypt_init(&cf_state, saveFileKey, saveFilesAreEncrypted);

  if (paniced) {
    exit_game();
  } else {
    gc__read_seeds(f1, &cf_state, in_rec, &paniced);
    if (!paniced) { 
      gc__display_status(fnam, in_rec);
    }
    if (!paniced) {
      gc__read_version(f1, &cf_state, in_rec, &paniced, &save_version);
    }
    if (!paniced) {
      gc__read_player_record(f1, &cf_state, in_rec, &paniced, prop, &was_dead);
    }
    if (!paniced) { 
      gc__read_inventory(f1, &cf_state, in_rec, &paniced, &was_dead);
    }
    if (!paniced) { 
      gc__read_equipment(f1, &cf_state, in_rec, &paniced, &was_dead);
    }

    if (was_dead) {
      /* nuke claim_check entry, they are lucky to be alive */
      PM.claim_check = 0;

      msg_print(" ");
    }

    if (!paniced) { 
      gc__read_stats_and_flags(f1, &cf_state, in_rec, &paniced);
    }
    if (!paniced) { 
      gc__read_magic(f1, &cf_state, in_rec, &paniced);
    }
    if (!paniced) { 
      gc__read_dungeon(f1, &cf_state, in_rec, &paniced);
    }
    if (!paniced) { 
      gc__read_identified(f1, &cf_state, in_rec, &paniced);
    }
    if (!paniced) { 
      gc__read_monsters(f1, &cf_state, in_rec, &paniced);
    }
    if (!paniced) { 
      gc__read_town(f1, &cf_state, in_rec, &paniced);
    }

    /* odds are we would have paniced by this time if an 
       encrypted file has been tampered with, but just in case... */
    if (!paniced) {
      read_decrypt(f1, &cf_state, in_rec, &paniced);
      sscanf(in_rec, "%ld", &check_time);
      if (PM.creation_time != check_time) {
	paniced = true;
      }
    }

    fclose(f1);
    
  } /* endif !paniced */
  
  if (!paniced) {
    seed = get_seed();
  }

  /*{ See if character is should be allowed to load }*/

  if (!paniced) { 
    gc__open_master(f1, &f2, &paniced);
  }

  if (!paniced) {
    gc__read_master(f2, &paniced);
    master_file_close(&f2);
  }

  if (paniced) { 
    data_exception();
    //exit_game();
  }

  return dun_flag;
} /* end get_char */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void restore_char(vtype fnam, boolean present, boolean undead)
{
  /*{ Wizard command for restoring character                -RAK-   }*/

  ntype      in_rec;
  vtype      tfnam;
//ssn_type   temp_id;
  FILE      *f1;
  GDBM_FILE  f2;
  boolean    flag,bleah_flag,exit_flag,paniced;
  char       command;

  encrypt_state   cf_state;
  unsigned long   save_seed;
  time_t          creation_time;
  integer         save_count;
  integer	  deaths;
  master_key      mkey;
  master_entry    mentry;

  exit_flag  = false;
  bleah_flag = true;
  paniced    = false;

  strcpy(tfnam, fnam);

  if (!present) {
    if (!undead) {

      do {
	msg_print("What kind of restore? (<d>eath-flag, <m>aster-file) ");
	if (get_com("",&command)) {
	  switch (command) {
	  case 'd' : 
	    exit_flag = true;
	    undead    = true;
	    break;
	    
	  case 'm' : 
	    exit_flag = true;
	    undead    = false;
	    break;
	  }
	} else {
	  bleah_flag = false;
	}
      } while (!exit_flag);
    }
    
    if (bleah_flag) {
      prt("Name of file to be restored: ",1,1);
      flag = get_string(tfnam,1,30,48);
    } else {
      flag = false;
    }
    
  } else {
    flag = true;
  }

  if (flag) {
    if (tfnam[0] == 0) {
      strcpy(tfnam, finam);
    }
    
    f1 = (FILE *)fopen(tfnam,"r");
    
    if (f1 == NULL) {
      sprintf(in_rec, "Error Opening> %s", tfnam);
      prt(in_rec,1,1);
      prt(" ",2,1);
      paniced = true;
    } else {
      //rewind(f1);
      encrypt_init(&cf_state, saveFileKey, saveFilesAreEncrypted);
      set_seed(ENCRYPT_SEED2);
      read_decrypt(f1, &cf_state, in_rec, &paniced);
      sscanf(in_rec,"%lu %ld %ld %ld",&save_seed, &creation_time,
	     &save_count, &deaths);

//      if (!paniced) {
//	for ( ; !paniced ; ) {
//	  read_decrypt(f1, &cf_state, in_rec, &paniced);
//	}	
//	paniced = false;
//      }
        fclose(f1);
    }      

    if (!paniced) {
      /*{ Check to see if master is openable   -JPS- }*/

      if (!master_file_open(&f2)) {
	msg_print("MASTER could not be opened.");
      } else {
	/*{ Reset the character in the master file.  -JPS- }*/

	mkey.creation_time = creation_time;
	mentry.save_count  = save_count;
	mentry.deaths      = deaths;
	
	if (master_file_write(f2, &mkey, &mentry)) {
	  msg_print("Character restored...");
	} else {
	  msg_print("Could not write ID in MASTER.");
	}

	master_file_close(&f2);
      }

    }

    //seed := get_seed;
    //priv_switch(0);
  }


  if (undead && !paniced) {
    get_char(tfnam,false);
    py.flags.dead = false;
    strcpy(finam, tfnam);
    save_char(false);        /* this probably will not return */
  }

  put_qio();
  printf("\n\r\n\r");
  exit_game();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  save.c */
