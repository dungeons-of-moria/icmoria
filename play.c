/* play.c */
/**/

#include "imoria.h"
#include "dungeon.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void p__druid_effects(integer effect)
{
  /*{ Songs....					}*/
  integer       i2,dir;
  integer       dumy,y_dumy,x_dumy;

  y_dumy = char_row;
  x_dumy = char_col;

  switch (effect+1) {
    
  case 1  :  /*{ Moon Beam }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      msg_print("A line of light appears!");
      light_line(dir,char_row,char_col,1);
    }
    break;
		  
  case 2  :  /*{ Detect Monster }*/
    detect_creatures(c_monster);
    break;
    
  case 3  :  /*{ Battle Song }*/
    bless(randint(12)+12);
    break;
    
  case 4  :  /*{ Light }*/
    light_area(char_row,char_col);
    break;
    
  case 5  :  /*{ Minor Cure }*/
    hp_player(damroll("5d3"),"a magic spell.");
    break;
    
  case 6  :  /*{ Find Safe Path }*/
    detect_sdoor();
    detect_trap();
    break;
		
  case 7  :  /*{ Magical Jig }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      zap_monster(dir,char_row,char_col,0,c_confuse);
    }
    break;
    
  case 8  :  /*{ Warp Wood }*/
    msg_print("The wood around you bends and warps...");
    td_destroy();
    break;
		
  case 9  :  /*{ Battle Dance }*/
    py.flags.hero += (randint(10) +5);
    bless(randint(20)+20);
    break;
		
  case 10 :  /*{ Cure Poison }*/
    cure_me(&py.flags.poisoned);
    break;
    
  case 11 :  /*{ Charm }*/
    sleep_monsters1(char_row,char_col);
    break;
    
  case 12 :  /*{ Detect Curse }*/
    detect_curse();
    break;
    
  case 13 :  /*{ Summon Insects }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_bolt(0,dir,char_row,char_col,
		damroll("1d6") + py.misc.lev div 3 * 2,"Insect Swarm");
    }
    break;
    
  case 14 :  /*{ Call Lightning }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_bolt(1,dir,char_row,char_col,
		damroll("2d8")+ py.misc.lev div 2,"Lightning Bolt");
    }
    break;
    
  case 15 :  /*{ Magic Res. }*/
    py.flags.magic_prot = randint(15) + 10; /* not cumulitive */
    break;
    
  case 16 :  /*{ Starlight }*/
    msg_print("A twinkling light appears.");
    starlite(char_row,char_col);
    break;
		
  case 17 :  /*{ Create Food }*/
    create_food(10,8,0,0,0);
    break;
    
  case 18 :  /*{ Remove Curse }*/
    for (i2 = Equipment_min; i2 <= EQUIP_MAX-1; i2++) {
      //with equipment[i2]. do;
      equipment[i2].flags &= 0x7FFFFFFF;
    }
    break;
    
  case 19 :  /*{ Infravision }*/
    py.flags.tim_infra += randint(30) + 30;
    break;
    
  case 20 :  /*{ Major Cure }*/
    hp_player(damroll("10d4"),"a magic spell. ");
    break;
    
  case 21 :  /*{ Resist Petrification }*/
    py.flags.resist_petri += randint(15) + 10;
    break;
    
  case 22 :  /*{ Transplant }*/
    msg_print("You step into a nearby patch of fungus...");
    teleport(py.misc.lev*6);
    break;
		
  case 23 :  /*{ Sunray }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_ball(10,dir,char_row,char_col,
		damroll("2d8"),"flash of sunlight");
    }
    break;
    
  case 24 :  /*{ Dispel Magic }*/
    //with py.flags do;
    cure_me(&PF.blind);
    cure_me(&PF.poisoned);
    cure_me(&PF.afraid);
    break;
		 
  case 25 :  /*{ Fire Stream }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_line(5,dir,char_row,char_col,
		damroll("3d4") + py.misc.lev div 2,"Stream of Fire");
    }
    break;

  case 26 :  /*{ Protection from Nature }*/
    //with py.flags do;
    PF.resist_heat += randint(15) +10;
    PF.resist_cold += randint(15) +10;
    PF.resist_lght += randint(15) +10;
    break;
		 
  case 27 :  /*{ Turn Stone to Mud }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      wall_to_mud(dir,char_row,char_col);
    }
    break;
    
  case 28 :  /*{ Goodberry }*/
    create_food(11,11,8,10,10);
    break;
    
  case 29 :  /*{ Creeping Doom }   */
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      creeping_doom(dir,char_row,char_col,py.misc.lev * 3,
		    py.misc.lev div 4,"Creeping Doom");
    }
    break;
    
  case 30 :  /*{ Pillar of Fire }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_bolt(5,dir,char_row,char_col,
		damroll("5d8")+py.misc.lev div 3,"Pillar of Fire");
    }
    break;
    
  case 31 :  /*{ Word of Recall }*/
    py.flags.word_recall = randint(20) + 20;
    break;
    
  case 32 :  /*{ Lightning Ball }*/
    if (d__get_dir("Which direction?",&dir,&dumy,&y_dumy,&x_dumy)) {
      fire_ball(1,dir,char_row,char_col,py.misc.lev,"Lightning Ball");
    }
    break;
    
  case 33 :  /*{ Word of Blindness }*/
    if (zap_area(0,0,c_confuse)) {
      msg_print("You blind the creatures!");
    }
    break;
		
  case 34 :  /*{ Protection from Monsters }*/
    py.flags.protmon = (randint(20)+py.misc.lev );
    break;
    
  case 35 :  /*{ Control Temperature }*/
    do {
      if (d__get_dir("Hotter(1) || Colder(2)?",&dir,&dumy,&y_dumy,&x_dumy)) {
	if (dir == 1) {
	  zap_area(0x0020,30,c_hp);
	} else if (dir == 2) {
	  zap_area(0x0010,30,c_hp);
	}
      } else {
	dir = 1;
      }
    } while (!((dir==1) || (dir==2)));
    break;
		
  case 36 :  /*{ Ring of Fire }*/
    py.flags.ring_fire += randint(4) + 1;
    break;
    
  case 37 :  /*{ Resist Charm }*/
    //with py.flags do;
    PF.free_time  += randint(10) + py.misc.lev;
    PF.magic_prot += randint(10) + py.misc.lev;
    break;
		
  case 38 :  /*{ Battle Frenzy }*/
    bless(randint(30)+30);         
    py.flags.shero = (randint(20)+20);  /* not cumulitive */
    break;
		
  case 39 :  /*{ Dispel Monster }*/
    zap_area(0x0002,3*py.misc.lev,c_hp);
    break;
    
  case 40 :  /*{ Note of Destruction }*/
    destroy_area(char_row,char_col);
    break;

  default:
    break;
  }
  /*{ End of songs...			       }*/

};
//////////////////////////////////////////////////////////////////////
void play()
{
  /*{ Play a Druid song					-CAPN/DMF-   }*/

  treas_ptr     i1,item_ptr;
  integer       choice,chance,i2;
  char          trash_char;
  boolean       redraw;
  obj_set       playable_things = {Instrument, 0};

  reset_flag = true;
  if (py.flags.hoarse > 0) {
    msg_print("You are too hoarse to sing!");
  } else if (py.flags.afraid > 0) {
    msg_print("You are too scared to play music!");
  } else if (py.flags.confused > 0) {
    msg_print("You are too confused...");
  } else if (class[py.misc.pclass].dspell) {
    if (inven_ctr > 0) {
      if (find_range(playable_things,false,&i1,&i2)) {
	
	redraw = false;
	if (get_item(&item_ptr,"Use which Instrument?",
		     &redraw,i2,&trash_char,false,false)) {
	  if (cast_spell("Play which song?",item_ptr,
			 &choice,&chance,&redraw)) {
	    //with magic_spell[py.misc.pclass][choice]. do;
	    reset_flag = false;
	    if (randint(100) < chance) {
	      switch (randint(5)) {
	      case 1  : msg_print("*Twang!*");  break;
	      case 2  : msg_print("*Boink!*");  break;
	      case 3  : msg_print("*Ding!*");   break;
	      case 4  : msg_print("*Plunk!*");  break;
	      case 5  : msg_print("*Clang!*");  break;
	      }; /*{ of the bad notes }*/
	      switch (randint(2)) {
	      case 1  : msg_print("You play a sour note!"); break;
	      case 2  : msg_print("You play an awful note!"); break;
	      }
	    } else {
	      p__druid_effects(choice);
	      if (!(reset_flag)) {
		//with py.misc do;
		PM.exp += magic_spell[py.misc.pclass][choice].sexp;
		prt_experience();
		magic_spell[py.misc.pclass][choice].sexp = 0;
	      }
	    }
	    
	    //with py.misc do;
	    if (!reset_flag) {
	      if (magic_spell[py.misc.pclass][choice].smana > PM.cmana) {
		msg_print("You lose your voice attempting the song!");
		py.flags.hoarse =
		  randint(5*(magic_spell[PM.pclass][choice].smana-PM.cmana));
		PM.cmana = 0;
		if (randint(3) == 1) {
		  lower_stat(CHR,"Your self-esteem is lowered!");
		}
	      } else {
		PM.cmana -= magic_spell[py.misc.pclass][choice].smana;
	      }
	      prt_mana();
	    }
	  } else {
	    if (redraw) {
	      draw_cave();
	    }
	  }
	}
      } else {
	msg_print("But you are not carrying any Instruments!");
      }
    } else {
      msg_print("But you are not carrying any Instruments!");
    }
  } else {
    switch (bard_adj()) {
    case 0 : msg_print("You utter a gutteral cry.");   break;
    case 1 : msg_print("You utter a gutteral cry.");   break;
    case 2 : msg_print("You attempt to sing.");        break;
    case 3 : msg_print("You attempt to sing.");        break;
    case 4 : msg_print("You sing a song.");            break;
    case 5 : msg_print("You sing a song.");            break;
    case 6 : msg_print("You sing a nice song.");       break;
    case 7 : msg_print("You sing a very nice song.");  break;
    }
  }
};
/* END FILE  play.c */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
