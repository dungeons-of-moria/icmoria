/* variables.h */
/* Ever feel the need for more global vars? */

extern	treas_ptr	cur_inven;	// { Current inven page  }
extern	boolean		is_magii;	// { True if has mana    }
extern	time_t   	start_time;	// { Time started playing}
extern	boolean		is_from_file;	// { True if restored    }
extern	money_type	bank;		// { Bank's money	 }
extern	money_type	coin_value;	// { Copy of money values}
extern	integer		player_max_exp; // { Max exp possible    }
extern	unsigned long	seed;           // { Contains seed #     }
extern	unsigned long	randes_seed;    // { For encoding colors }
extern	unsigned long	town_seed;      // { Seed for town genera}
extern	integer		channel;        // { I/O channel #       }
extern	unsigned long	pasteb;         // { Pasteboard id       }
extern	quad_type	io_bin_pause;   // { I/O pause time      }
extern	integer		cur_height;	// { Cur dungeon size    }
extern	integer		cur_width;	
extern	integer		dun_level;      // { Cur dungeon level   }
extern	integer		missle_ctr;     // { Counter for missles }
extern	integer		msg_line;       // { Contains message txt}
extern	boolean		msg_flag;       // { Set with first msg  }
extern	vtype		msg_prev[MAX_MESSAGES+1];
extern	integer		quest[NUM_QUESTS+1]; // {quest data}
extern	vtype		old_msg;        // { Last message	      }
extern	boolean		want_trap;      // { True = trap messages}
extern	boolean		want_warn;      // { True = water warning}
extern	message_ptr	caught_message; // { Message from other  }
extern	message_ptr	old_message;	// { Past messages	      }
extern	integer		old_mess_count; // { Count of old mess's }
extern	integer		max_mess_keep;  // { Max old to keep     }
extern	message_ptr	cur_message;	// { Pointer to add mess }
extern	message_ptr	message_cursor; // { Pointer to read mess}
extern	integer		caught_count; //	{ # of mesgs waiting  }
extern	integer		max_score; //	{ # of scores to list }
extern	boolean		generate; //	{ Generate next level }
extern	boolean		death; //	{ True if died	      }
extern	vtype		died_from; //	{ What killed him     }
extern	integer		turn_counter; //	{ Turns ellapsed      }
extern	boolean		find_flag; //	{ Used in MORIA	      }
extern	boolean		cave_flag; //	{ Used in GET_PANEL   }
extern	boolean		light_flag; //	{ Used in MOVE_LIGHT  }
extern	boolean		redraw; //	{ For redraw screen   }
extern	unsigned long	print_stat; //	{ Flag for stats      }
extern	integer		turn; //	{ Cur trun of game    }
extern	boolean		wizard1; //	{ Wizard flag	      }
extern	boolean		wizard2; //	{ Wizard flag	      }
extern	boolean		used_line[24]; // array [2..23] of boolean;
extern	char		password1[13];
extern	char		password2[13];
extern  boolean         became_wizard;
extern	unsigned long	wdata[2][13]; //  array [1..2,0..12] of unsigned;
extern	char		days[7][30];
extern	integer		closing_flag;   // { Used for closing   }
extern	boolean		uw_id; //	{ Is this a UW node? }
//{neatness arrays}
extern	byteint		key_of[9];	//  array [0..8] of byteint;
extern	byteint		oct_of[10];	//  array [1..9] of byteint;
extern	bytlint		dx_of[10];	//  array [1..9] of bytlint;
extern	bytlint		dy_of[10];	//  array [1..9] of bytlint;

//	{ Bit testing array						}
extern	unsigned long	bit_array[33];  //  array [1..32] of unsigned;

//	{ External file names; are all located in directory with image	}
extern	vtype		MORIA_HOU;
extern	vtype		MORIA_MOR;
extern	vtype		MORIA_MAS;
extern	vtype		MORIA_TOP;
extern	vtype		MORIA_TRD;
extern	vtype		MORIA_HLP;
extern	vtype		MORIA_LCK;
extern	vtype		MORIA_DTH;
extern	vtype		MORIA_MON;
extern	vtype		MORIA_CST;
extern	vtype		MORIA_GCST;

//	{  following are calculated from max dungeon sizes		}
extern	integer		max_panel_rows, max_panel_cols;
extern	integer		quart_height, quart_width;
extern	integer		panel_row, panel_col;
extern	integer		panel_row_min, panel_row_max;
extern	integer		panel_col_min, panel_col_max;
extern	integer		panel_col_prt, panel_row_prt;

//	{  Following are all floor definitions				}
extern	row_floor	cave[MAX_HEIGHT+1];
extern	cave_type	blank_floor;
extern	floor_type	dopen_floor;
extern	floor_type	lopen_floor;
extern	floor_type	corr_floor1;
extern	floor_type	corr_floor2;
extern	floor_type	corr_floor3;
extern	floor_type	corr_floor4;
extern	floor_type	rock_wall1;
extern	floor_type	rock_wall2;
extern	floor_type	rock_wall3;
extern	floor_type	water1;
extern	floor_type	water2;
extern	floor_type	water3;
extern	floor_type	boundry_wall;

//	{  Following are set definitions				}
extern	obj_set		floor_set;
extern	obj_set		wall_set;
extern	obj_set		pwall_set;
extern	obj_set		corr_set;
extern	obj_set		trap_set;
extern	obj_set		light_set;
extern	obj_set		water_set;
extern	obj_set		earth_set;
extern	obj_set		float_set;
extern	obj_set		slow_set;
extern	obj_set		stable_set;

//	{ Following are player variables				}
extern	player_type	py;

//	{ Class titles for different levels				}

extern	btype	player_title[MAX_CLASS][MAX_PLAYER_LEVEL+1];
//				  array [1..max_class] of
//				  array [1..max_player_level] of btype;

extern	integer		player_exp[MAX_PLAYER_LEVEL+1];
extern	real		acc_exp;   //{ Accumulator for fractional exp}
extern	dtype		bare_hands;
extern	boolean		msg_terse;
extern	byteint		record_ctr;
extern	integer		char_row;
extern	integer		char_col;
extern	integer		com_val;
extern	integer		pclass;
extern	vtype		sex_type;
extern	race_type	race[MAX_RACES];
extern	background_type	background[MAX_BACKGROUND];
extern	real		rgold_adj[MAX_RACES][MAX_RACES];
extern	class_type	class[MAX_CLASS];
extern	spell_type	magic_spell[MAX_CLASS][MAX_SPELLS];
extern	treasure_type	yums[NUM_YUM+1];
extern	treasure_type	monk_book;
extern	byteint		player_init[MAX_CLASS][5];
extern	boolean		total_winner;

//	{ Following are store definitions				}
extern	owner_type	owners[MAX_OWNERS];
extern	store_type	stores[MAX_STORES+1];
extern	treasure_type	store_door[MAX_STORES+MAX_UNNAMED+5+1];
extern	integer		store_choice[MAX_STORES][STORE_CHOICES];
extern	obj_set		store_buy[MAX_STORES];
			//	  array [1..max_stores] of obj_set;
extern	htype		store_hours[MAX_STORES+MAX_UNNAMED][7];
extern	integer		store_bribe[MAX_STORES+MAX_UNNAMED];
extern	integer		mugging_chance;		// { Chance page gets mugged}

//	{ Following are treasure arrays	and variables			}
extern	treasure_type	object_list[MAX_OBJECTS+1];
extern	boolean		object_ident[MAX_OBJECTS+1];
extern	integer		t_level[MAX_OBJ_LEVEL+1];
extern	treasure_type	gold_list[MAX_GOLD];
extern	treasure_type	t_list[MAX_TALLOC+1];
extern	treasure_type	equipment[EQUIP_MAX];
extern	treas_ptr	inventory_list;
extern	treas_ptr	inven_temp;
extern	treasure_type	inventory_init[INVEN_INIT_MAX+1];
extern	treasure_type	blank_treasure;
extern	integer		inven_ctr;	// { Total different obj's	}
extern	integer		inven_weight;	// { Cur carried weight	}
extern	integer		equip_ctr;	// { Cur equipment ctr	}
extern	integer		tcptr;		// { Cur treasure heap ptr	}

//	{ Following are variables that change with level of difficulty	}
//	{ 1/x chance of treasure per magma		}
extern	integer		dun_str_mc;
//	{ 1/x chance of treasure per quartz		}
extern	integer		dun_str_qc;
//	{ Level/x chance of unusual room		}
extern	integer		dun_unusual;
//	{ Amount of objects for rooms			}
extern	integer		treas_room_alloc;
//	{ Amount of objects for corridors		}
extern	integer		treas_any_alloc;
//	{ Amount of gold (and gems)			}
extern	integer		treas_gold_alloc;
//	{ 1/n Chance of item being a Great Item 	}
extern	integer		obj_great;
//	{ Adjust STD per level				}
extern	real		obj_std_adj;
//	{ Minimum STD					}
extern	integer		obj_std_min;
//	{ Town object generation level			}
extern	integer		obj_town_level;
//	{ Base amount of magic				}
extern	integer		obj_base_magic;
//	{ Max amount of magic				}
extern	integer		obj_base_max;
//	{ magic_chance/# = special magic		}
extern	integer		obj_div_special;
//	{ magic_chance/# = cursed items			}
extern	real		obj_div_cursed;
//	{ High value slows multiplication		}
extern	integer		mon_mult_adj;
//	{ Dun_level/x chance of high level creature	}
extern	integer		mon_nasty;

//	{ Following are feature objects defined for dungeon		}
extern	treasure_type	trap_lista[MAX_TRAPA+1];
extern	treasure_type	trap_listb[MAX_TRAPB+1];
extern	treasure_type	scare_monster;		// { Special trap	}
extern	treasure_type	some_rubble;
extern	treasure_type	door_list[3];
extern	treasure_type	up_stair;
extern	treasure_type	down_stair;
extern	treasure_type	up_steep;
extern	treasure_type	down_steep;

//	{ Following are creature arrays and variables			}
extern	creature_type	c_list[MAX_CREATURES+1];
extern	monster_type	m_list[MAX_MALLOC+1];
extern	integer		m_level[MAX_MONS_LEVEL+1];
extern	monster_type	blank_monster;	// { Blank monster values	}
extern	integer		muptr;		// { Cur used monster ptr	}
extern	integer		mfptr;		// { Cur free monster ptr	}
extern	integer		mon_tot_mult;	// { # of repro's of creature	}

//	{ Following are arrays for descriptive pieces			}
extern	atype		colors[MAX_COLORS];
extern	atype		mushrooms[MAX_MUSH];
extern	atype		woods[MAX_WOODS];
extern	atype		metals[MAX_METALS];
extern	atype		horns[MAX_HORNS];
extern	atype		rocks[MAX_ROCKS];
extern	atype		amulets[MAX_AMULETS];
extern	atype		cloths[MAX_CLOTHS];
extern	atype		syllables[MAX_SYLLABLES];
//	vowel_set		: 
//				  char_set;

//	{ Following are variables for the Save Character Routines	}
extern	vtype		finam;
//	key_type	key_rec;

//	{ Cursor variables, used for cursor positioning			}
extern	char		cursor_r[25][11]; //array [1..24] of varying[10] of 
extern	integer		curlen_r;
extern	char		cursor_c[81][11]; //array [1..80] of varying[10] of 
extern	integer		curlen_c;
extern	integer		cursor_l;
extern	boolean		row_first;
extern	char		cursor_erl[11]; //	:  varying[10] of char;
extern	char		cursor_erp[11];	//	:  varying[10] of char;

// new stuff
extern	integer malloc_calls;
extern	integer malloc_bytes;
extern	integer	free_calls;
extern	integer	free_bytes;
extern	vtype	coin_name[MITHRIL+1];
extern  obj_set blank_floor_set;

extern	obj_set null_obj_set;
extern	obj_set destroyed_by_lightning;
extern	obj_set destroyed_by_acid;
extern	obj_set destroyed_by_cold;
extern	obj_set destroyed_by_fire;
extern	obj_set destroyed_by_petrify;
extern	obj_set destroyed_by_sunray;

extern  gid_t   games_gid;
extern  boolean scoresAreEncrypted;
extern  boolean saveFilesAreEncrypted;

extern  byteint highScoreKey[8];
extern  byteint saveFileKey[8];

extern  int game_state;

/* END FILE  variables.h */
