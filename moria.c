/* moria.c

Port of imoria from VMS in pascal to linux in C.  Joy.  Here is the
copyright notice from the sources I started with (although trying to
maintain a copyright on something you put into the public domain seems
wrong somehow):


{ Moria Version 4.8	COPYRIGHT (c) Robert Alan Koeneke		}
{                       Public Domain                                   }
{                                                                       }
{       I lovingly dedicate this game to hackers and adventurers        }
{       everywhere...                                                   }
{                                                                       }
{                                                                       }
{       Designer and Programmer : Robert Alan Koeneke                   }
{                                 University of Oklahoma                }
{                                                                       }
{       Assitant Programmers    : Jimmey Wayne Todd                     }
{                                 University of Oklahoma                }
{                                                                       }
{                                 Gary D. McAdoo                        }
{                                 University of Oklahoma                }
{                                                                       }
{       Moria may be copied and modified freely as long as the above    }
{       credits are retained.  No one who-so-ever may sell or market    }
{       this software in any form without the expressed written consent }
{       of the author Robert Alan Koeneke.                              }
{                                                                       }
*/

#define __MORIA_C__
#include "imoria.h"

/***********************************************************************/

void	main(int argc, char *argv[], char *envp[])
{
  /*// { SYSPRV stays off except when needed...	}*/
  game_state = GS_LOADING;
  init_priv_switch();
  priv_switch(0);

  ENTER("main", "")

    //init_signals();

	// { Get the time player entered game		}
    start_time = time(NULL);

	// { Get the directory location of the image	}
    get_paths();
	
	// { Here comes the monsters....                   }
    load_monsters();

	// { Check to see if an update is in progress		-DMF-	}
    if (check_kickout()) {
	writeln("Imoria is locked . . . Try playing conquest.");
	writeln("Who knows *how* long this might take?");
	exit_game();
    }

	// { Some necessary initializations		}
    msg_line               = 1;
    quart_height           = trunc(SCREEN_HEIGHT/4);
    quart_width            = trunc(SCREEN_WIDTH /4);
    dun_level              = 0;
    inven_temp = (treas_ptr)safe_malloc(sizeof(treas_rec),"inven_temp");
    inven_temp->data       = blank_treasure;
    inven_temp->ok         = false;
    inven_temp->insides    = 0;
    inven_temp->next       = nil;
    inven_temp->is_in      = false;
    caught_message         = nil;
    old_message            = nil;
    old_mess_count         = 0;
    turn_counter           = 100000;

	// { Grab a random seed from the clock		}
    seed = get_seed();

	// { Sort the objects by level			}
    sort_objects();

	// { Init monster and treasure levels for allocate }
    init_m_level();
    init_t_level();

	// { Init the store inventories			}
    store_init();
    if (COST_ADJ   != 1.00) price_adjust();
    if (WEIGHT_ADJ != 1)    item_weight_adjust();
    bank_init();

	// { Build the secret wizard and god passwords	}
    bpswd();

	// { Check operating hours 			}
	// { If not wizard then No_Control_Y               }
	// { Check or create hours.dat, print message	}
    intro(finam, argc, argv);

        // { Init an IO channel for QIO			}
    /* init_channel(); */

    clear_screen();

	// { Generate a character, or retrieve old one...	}
    if (strlen(finam) > 0) {
        // { Retrieve character    }
	game_state = GS_IGNORE_CTRL_C;
	generate = get_char(finam,true);
	py.flags.dead = true;
	is_from_file = true;
	save_char(false);
	change_name();
	magic_init(randes_seed);
    } else {
	// { Create character      }
	is_from_file = false;
	strncpy(finam,"moriachar.sav",sizeof(vtype));
	create_character();

	char_inven_init();

	if (class[py.misc.pclass].mspell) {
            // { Magic realm   }
	    learn_spell(&msg_flag);
	    gain_mana(spell_adj(INT));
	} else if (class[py.misc.pclass].pspell) {
	    // { Clerical realm}
	    learn_prayer();
	    gain_mana(spell_adj(WIS));
	} else if (class[py.misc.pclass].dspell) {
 	    // { Druidical realm }
	    learn_druid();
	    gain_mana(druid_adj());
	} else if (class[py.misc.pclass].bspell) {
	    // { Bardic realm }
	    learn_song(&msg_flag);
	    gain_mana(bard_adj());
	}
	py.misc.cmana = py.misc.mana;
	randes_seed = seed;            // { Description seed      }
	town_seed   = seed;            // { Town generation seed  }
	magic_init(randes_seed);
	generate = true;

    } /* end if (load/create character) */

      if (py.misc.pclass == C_MONK) {
	strcpy(bare_hands, "2d2");
      }

      if (class[py.misc.pclass].mspell || class[py.misc.pclass].pspell ||
	  class[py.misc.pclass].dspell || class[py.misc.pclass].bspell ||
	  class[py.misc.pclass].mental )
	{
	  is_magii = true;
	} else {
	  is_magii = false;
	}

	// { Begin the game				}
      replace_name();
      set_gem_values();

      set_difficulty(py.misc.diffic);	// { Set difficulty of game	}
      player_max_exp = (integer)(player_exp[MAX_PLAYER_LEVEL-1]
				 * py.misc.expfact);
      clear_from(1);
      prt_stat_block();

	// { Turn on message trapping, if requested	}
//    if (want_trap) set_the_trap();

    // { Loop till dead, or exit			}
    do {
      //generate = true;
      if (generate) generate_cave();            // { New level     }
      dungeon();                                // { Dungeon logic }
      generate = true;
    } while (!death);

    upon_death();                         // { Character gets buried }
  
    LEAVE("main", "")
}  /* end main */

/***********************************************************************/
