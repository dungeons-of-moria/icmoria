/* constants.h */
/* The global constants for imoria */

//	{Note to the Wizard:						}
//	{	Tweaking these constants can *GREATLY* change the game. }
//	{	Two years of constant tuning have generated these	}
//	{	values.	 Minor adjustments are encouraged, but you must }
//	{	be very careful not to unbalance the game.  Moria was	}
//	{	meant to be challenging, not a give away.  Many		}
//	{	adjustments can cause the game to act strangely, or even}
//	{	cause errors.						}

//	{ QIOW constants, see $IODEF in STARLET.MLB			}
//	IO$MOR_OUTPUT = IO$_WRITEVBLK;
//	IO$MOR_INPUT  = IO$_TTYREADALL + IO$M_NOECHO;
//	IO$MOR_DELAY  = IO$MOR_INPUT   + IO$M_NOWAIT;
//	IO$MOR_IPURGE = IO$MOR_DELAY   + IO$M_PURGE;
  
//	{ Wizard accounts (can use wizard/god modes)	      -DCJ/KRC-
//	  Please document additions to this list.  Current wizards are:
//		games/arcana	: GAMES
//		Tom Bunch       : 8630891
//		Ken Case	: 9EBB07D3, C8518
//              Robert Christ   : 8725262
//		Mary Conner	: 26F45030, 8221984
//		Brad DeGrazia   : 8830329
//		Rob DeLoura	: 8536399
//				: DD7CB31C
//		Jimbo Harris    : 8727288
//		Jonathan Sari	: 8728277
//		Raoul St.Pierre : 8832780
//		And Dean Yasuda as the MBI!
//	}
//	wizards = ':GAMES:9EBB07D3:C8518:26F45030:8727288:8536399:DD7CB31C:8728277:8221984:8725262:8832780:8830329:';
 
#define wizards "goohead"

//	{ Encryption constants						}
//	{ Note: These numbers were pulled from the air, and can be	}
//	{	changed.  If changed, characters produced from other	}
//	{	versions will fail to restore.				}
#define	ENCRYPT_SEED1  1175191
#define	ENCRYPT_SEED2  99755171
 
//	{ Screen size parameters					}
#define	SCREEN_HEIGHT  22
#define	SCREEN_WIDTH   66
 
//	{ Screen placement parameters					}
#define	STAT_COLUMN	 1
#define	RACE_ROW	 2
#define	CLASS_ROW	 (RACE_ROW + 1)
#define	TITLE_ROW	 (CLASS_ROW + 1)
#define	STR_ROW		 (TITLE_ROW + 2)
#define	INT_ROW		 (STR_ROW + 1)
#define	WIS_ROW		 (INT_ROW + 1)
#define	DEX_ROW		 (WIS_ROW + 1)
#define	CON_ROW		 (DEX_ROW + 1)
#define	CHA_ROW		 (CON_ROW + 1)
#define	LEVEL_ROW	 (CHA_ROW + 2)
#define	EXP_ROW		 (LEVEL_ROW + 1)
#define	MANA_ROW	 (EXP_ROW + 1)
#define	HP_ROW		 (MANA_ROW + 1)
#define	QUEST_ROW	 (HP_ROW + 1)
#define	AC_ROW		 (QUEST_ROW + 1)
#define	GOLD_ROW	 (AC_ROW + 1)
#define	WEIGHT_ROW	 (GOLD_ROW + 1)
#define	TIME_ROW	 (WEIGHT_ROW + 2)
#define	WINNER_ROW	 23
#define	WINNER_COLUMN	 1
 
#define	STATUS_ROW	 24
#define		HUNGER_COLUMN		 1 
#define		BLIND_COLUMN		 9 
#define		CONFUSED_COLUMN		 16
#define		AFRAID_COLUMN		 26
#define		POISONED_COLUMN		 34
#define		SEARCHING_COLUMN	 44
#define		RESTING_COLUMN		 44
#define		QUESTED_COLUMN		 53 /*{FUBAR}*/
#define		DEPTH_COLUMN		 62 
#define		LIGHT_ON_COLUMN		 62 
 
//	sr = 0;	iq = 1;	ws = 2;	dx = 3;	cn = 4;	ca = 5;
#define STR 0
#define INT 1
#define WIS 2
#define DEX 3
#define CON 4
#define CHR 5
 
//	{ Spell Effect Constants! }
#define	c_null 0
#define	c_lightning 1
#define c_gas 2
#define c_acid 3
#define c_cold 4
#define c_fire 5
#define	c_good 6
#define c_evil 7
#define c_petrify 8
#define c_sunray 9
#define c_illusion 10
#define c_probe 11
#define	c_sleep 12
#define c_confuse 13
#define c_hp 14
#define c_drain 15
#define c_speed 16
#define c_hold 17
#define	c_turn 18
#define c_poly 19
#define c_joke 20
#define c_monster 21
#define c_invisible 22
#define	c_creature 23
#define c_object 24
#define c_treasure 25
#define c_hp_undead 26
#define	c_thunder 27
#define c_holy_word 28

#define	c_sc_hold 1
#define c_sc_mental 2


//	{ # of numbered objects for easy summoning (ie. create food) }
#define NUM_YUM 18

//	{ Dungeon size parameters					}
#define	MAX_HEIGHT     66	// { Multiple of 11; >= 22 }
#define	MAX_WIDTH      198	// { Multiple of 33; >= 66 }
 
//	{ Output dungeon section sizes					}
#define	OUTPAGE_HEIGHT 44	// { 44 lines of dungeon per section	}
#define	OUTPAGE_WIDTH  99	// { 100 columns of dungeon per section	}
 
//	{ Dungeon generation values					}
//	{ Note: The entire design of dungeon can be changed by only	}
//	{	slight adjustments here.				}
#define	DAY_LENGTH	 9600  //{ Turns in a day			}
#define	DUN_TUN_RND	 36    //{ Random direction (4 is min)		}
#define	DUN_TUN_CHG	 70    //{ Chance of changing direction (99 max)}
#define	DUN_TUN_FND	 12    //{ Distance for auto find to kick in	}
#define	DUN_TUN_CON	 15    //{ Chance of extra tunneling		}
#define	DUN_ROO_MEA	 32    //{ Mean of # of rooms, standard dev=2	}
#define	DUN_TUN_PEN	 25    //{ % chance of room doors		}
#define	DUN_TUN_JCT	 15    //{ % chance of doors at tunnel junctons	}
#define	DUN_STR_DEN	 5     //{ Density of streamers			}
#define	DUN_STR_RNG	 2     //{ Width of streamers			}
#define	DUN_STR_MAG	 3     //{ Number of magma streamers		}
#define	DUN_STR_QUA	 2     //{ Number of quartz streamers		}
#define	DUN_WTR_DEN	 5     //{ Density of water			}
#define	DUN_WTR_WIDTH	 4     //{ Width of river			}
#define	DUN_RIVERS	 3     //{ Number of rivers			}
#define	DUN_RIV_LEN	 35    //{ Maximum river length			}
#define	DUN_POOLS	 3     //{ Number of pools			}
 
//	{ Store constants						}
#define	MAX_OWNER_KIND	  4    //{ Number of different keepers for a shop}
#define	MAX_STORES	 12    //{ Number of different stores		}
#define	MAX_OWNERS	 (MAX_STORES * MAX_OWNER_KIND)
			       //{ Number of owners to choose from	}
#define	MAX_UNNAMED	  6    //{ # of 'odd' shops (Post, etc)	}
#define	TOT_STORES	 (MAX_STORES+MAX_UNNAMED)
#define	MAX_FOUNTAINS	  2    //{ # of fountains on town level	}
#define	MAX_HOUSE1	 4     //{ # of generic houses in town	}
#define	MAX_HOUSE2	 8     //{ # of small houses in town }
#define	MAX_HOUSE3	 2     //{ # apartments }
#define	MAX_HOUSE4	 2     //{ # small apartments }
#define	DAY_MUGGING	 50    //{ 1/x chance that page gets mugged (day)}
#define	NIGHT_MUGGING	 15    //{ 1/x chance that page gets mugged (night)}
#define	BANK_SKIM	 0.95  //{ Percent of money that really gets deposited}
#define	STORE_INVEN_MAX  24    //{ Max number of discrete objs in inven	}
#define	STORE_CHOICES	 50    //{ Number of items to choice stock from	}
#define	STORE_MAX_INVEN  20    //{ Max diff objs in stock before auto sell}
#define	STORE_MIN_INVEN  14    //{ Min diff objs in stock before auto buy}
#define	STORE_TURN_AROUND 3    //{ Amount of buying and selling normally }
#define	INVEN_INIT_MAX	 157   //{ Size of store init array		}
#define	COST_ADJ	 240.00//{ FLOAT(Gold$value)--adjusts store costs }
#define	WEIGHT_ADJ	 100   //{ Adjust weights of items 		}
#define	QUEST_DELAY	 800  //{ # of turns to wait before char can get quest}
 
//	{ Treasure constants						}
#define	EQUIP_MAX      15      //{ Size of equipment array		}
#define	MAX_OBJ_LEVEL  256     //{ Maximum level of magic in dungeon	}
#define	MAX_OBJECTS    473     //{ Number of objects for universe       }
/* if MAX_OBJECTS goes over 1024 then save will break */
#define	MAX_GOLD       25      //{ Number of different types of gold	}
#define	MAX_TALLOC     225     //{ Max objects per level		}
 
//	{ Money constants, for those of us who can't remember '12480'   }
#define	MITHRIL_VALUE	  1248 //{ Iron Pieces				}
#define	PLATINUM_VALUE	   960
#define	GOLD_VALUE	   240
#define	SILVER_VALUE	    20
#define	COPPER_VALUE	     4
#define	IRON_VALUE	     1

//	{ Constants describing limits of certain objects		}
#define	OBJ_LAMP_MAX	15000  //{ Maximum amount that lamp can be filled}
#define	OBJ_BOLT_RANGE	   18  //{ Maximum range of bolts and balls	}
#define	OBJ_RUNE_PROT	 3000  //{ Rune of protection resistance	}
 
//	{ Creature contants						}
#define	MAX_CREATURES	  415 //{ Number of creatures defined for univ	}
#define	MAX_MALLOC   (100 +1) //{ Max that can be allocated	        }
#define	MAX_MALLOC_CHANCE 160 //{ 1/x chance of new monster each round	}
#define	MAX_MONS_LEVEL	  100 //{ Maximum level of creatures		}
#define	MAX_SIGHT	   20 //{ Maximum dis a creature can be seen	}
#define	MAX_SPELL_DIS	   18 //{ Maximum dis creat. spell can be cast	}
#define	MAX_MON_MULT	   75 //{ Maximum reproductions on a level	}
#define	MIN_MALLOC_LEVEL   14 //{ Minimum number of monsters/level	}
#define	MIN_MALLOC_TD	    8 //{ Number of people on town level (day)	}
#define	MIN_MALLOC_TN	   12 //{ Number of people on town level (night)}
#define	WIN_MON_TOT	    2 //{ Total number of "win" creatures	}
#define	WIN_MON_APPEAR	  100 //{ Level where winning creatures begin	}
#define	OUT_OF_ENV_DAM	   32 //{ max HP lost/rnd/2^move in wrong env	}
#define	MON_SUMMON_ADJ	    2 //{ Adjust level of summoned creatures	}
#define	MON_DRAIN_LIFE	    5 //{ Percent of player exp drained per hit }
 
//	{ Trap constants						}
#define	MAX_TRAPA	  19   //{ Number of defined traps		}
#define	MAX_TRAPB	  20   //{ Includes secret doors		}
 
//	{ Descriptive constants						}
#define	MAX_COLORS     68      //{ Used with potions	}
#define	MAX_MUSH       29      //{ Used with mushrooms	}
#define	MAX_WOODS      41      //{ Used with staffs	}
#define	MAX_METALS     32      //{ Used with wands	}
#define	MAX_HORNS      13      //{ Used with horns	}
#define	MAX_ROCKS      53      //{ Used with rings	}
#define	MAX_CLOTHS      7      //{ Used with bags/sacks	}
#define	MAX_AMULETS    39      //{ Used with amulets	}
#define	MAX_SYLLABLES  156     //{ Used with scrolls	}
 
//	{ Player constants						}
#define	MAX_QUESTS	  35   //{ Maximum number of quests per game	}
#define	MAX_SPELLS	  40   //{ Maximum number of spells per class	}
#define	MAX_PLAYER_LEVEL  40   //{ Maximum possible character level	}
#define	MAX_RACES	  10   //{ Number of defined races	        }
#define	MAX_CLASS	  10   //{ Number of defined classes		}
#define	MAX_MESSAGES	  50   //{ Previous messages that in buffer	}
#define	USE_DEVICE	  3    //{ x> Harder devices x< Easier devices	}
#define	MAX_BACKGROUND	  177  //{ Number of types of histories for univ }
#define	NUM_QUESTS	  5    //{ # of possible quests to select from	}
#define BASE_FOOD_DIGESTED 2   // food digested per turn
#define	PLAYER_FOOD_FULL 10000 //{ Getting full				}
#define	PLAYER_FOOD_MAX	 15000 //{ Maximum food value, beyond is wasted	}
#define	PLAYER_FOOD_FAINT  300 //{ Character begins fainting		}
#define	PLAYER_FOOD_WEAK  1000 //{ Warn player that he is getting very low}
#define	PLAYER_FOOD_ALERT 2000 //{ Warn player that he is getting low	}
#define	PLAYER_REGEN_FAINT   0.0005    //{ Regen factor when fainting	}
#define	PLAYER_REGEN_WEAK    0.0015    //{ Regen factor when weak	}
#define	PLAYER_REGEN_NORMAL  0.0030    //{ Regen factor when full	}
#define	PLAYER_REGEN_HPBASE  0.0220    //{ Min amount hp regen		}
#define	PLAYER_REGEN_MNBASE  0.0080    //{ Min amount mana regen	}
#define	PLAYER_WEIGHT_CAP 13   //{ "#"*(1/10 pounds) per strength point	}
#define	PLAYER_EXIT_PAUSE 0    //{ Pause time before player can re-roll	}
#define	MAX_HIGH_SCORES  100   //{ Maximum number of high scores to keep }
#define	FEMALE	 1 
#define	MALE	 2

//	{ Base to hit constants						}
#define	BTH_LEV_ADJ	 3     //{ Adjust BTH per level			}
#define	BTH_PLUS_ADJ	 3     //{ Adjust BTH per plus-to-hit		}
#define	BTH_HIT		 12    //{ Automatic hit; 1/bth_hit		}
 
//	{ Misc								}
#define	NULL_CHAR  0           // null = chr(0);
 
#define	Nothing_flag				0x00000000
#define	nothing_flag				0x00000000
#define	Everything_flag				0xFFFFFFFF
#define	everything_flag				0xFFFFFFFF
 
//	{ Flags for items that can be worn or wielded		-DCJ-	}
#define	Strength_worn_bit			0x00000001
#define	Dexterity_worn_bit			0x00000002
#define	Constitution_worn_bit			0x00000004
#define	Intelligence_worn_bit			0x00000008
#define	Wisdom_worn_bit				0x00000010
#define	Charisma_worn_bit			0x00000020
#define	Searching_worn_bit			0x00000040
#define	Slow_Digestion_worn_bit			0x00000080
#define	Stealth_worn_bit			0x00000100
#define	Aggravation_worn_bit			0x00000200
#define	Teleportation_worn_bit			0x00000400
#define	Regeneration_worn_bit			0x00000800
#define	Speed_worn_bit				0x00001000
#define	Slay_Dragon_worn_bit			0x00002000
#define	Slay_Monster_worn_bit			0x00004000
#define	Slay_Evil_worn_bit			0x00008000
#define	Slay_Undead_worn_bit			0x00010000
#define	Cold_Brand_worn_bit			0x00020000
#define	Flame_Brand_worn_bit			0x00040000
#define	Resist_Fire_worn_bit			0x00080000
#define	Resist_Acid_worn_bit			0x00100000
#define	Resist_Cold_worn_bit			0x00200000
#define	Sustain_Stat_worn_bit			0x00400000
#define	Free_Action_worn_bit			0x00800000
#define	See_Invisible_worn_bit			0x01000000
#define	Resist_Lightning_worn_bit		0x02000000
#define	Feather_Fall_worn_bit			0x04000000
#define	Blindness_worn_bit			0x08000000
#define	Timidness_worn_bit			0x10000000
#define	Tunneling_worn_bit			0x20000000
#define	Infra_Vision_worn_bit			0x40000000
#define	Cursed_worn_bit				0x80000000
 
//	{ Flags for items that can be worn or wielded (flags2)		}
#define	Slay_demon_worn_bit			0x00000001
#define	Soul_Sword_worn_bit			0x00000002
#define	Slay_regen_worn_bit   			0x00000004
#define	Swimming_worn_bit			0x00000008
#define	Magic_proof_worn_bit			0x00000010
#define	Disarm_worn_bit				0x00000020
#define	Sharp_worn_bit				0x00000040
//{	Unused_bit				0x00000080		}
//{	Unused_bit_set				0x000FFF00		}
//{	Unused_bit				0x00100000		}
#define	Bad_repute_worn_bit			0x00200000
#define	Hunger_worn_bit				0x00400000
#define	Negative_gem_bit			0x00800000
#define	Increase_carry_worn_bit			0x01000000
//{	Unused_bit				0x02000000		}
#define	Holding_bit				0x04000000
#define	Swallowing_bit				0x08000000
#define	Sharp_bit				0x10000000            
#define	Blackmarket_bit				0x20000000
#define	Insured_bit				0x40000000
#define	Known_cursed_bit			0x80000000
 
 
//	{ Constants for equipment inventory items		-KRC-	}
 
#define	Equipment_min			0
#define	Equipment_primary		(Equipment_min)             /*  0 */
#define	Equipment_helm			(Equipment_primary    + 1)  /*  1 */
#define	Equipment_amulet		(Equipment_helm       + 1)  /*  2 */
#define	Equipment_armor			(Equipment_amulet     + 1)  /*  3 */
#define	Equipment_belt			(Equipment_armor      + 1)  /*  4 */
#define	Equipment_shield		(Equipment_belt       + 1)  /*  5 */
#define	Equipment_gloves		(Equipment_shield     + 1)  /*  6 */
#define	Equipment_bracers		(Equipment_gloves     + 1)  /*  7 */
#define	Equipment_right_ring		(Equipment_bracers    + 1)  /*  8 */
#define	Equipment_left_ring		(Equipment_right_ring + 1)  /*  9 */
#define	Equipment_boots			(Equipment_left_ring  + 1)  /* 10 */
#define	Equipment_cloak			(Equipment_boots      + 1)  /* 11 */
#define	Equipment_light			(Equipment_cloak      + 1)  /* 12 */
#define	Equipment_secondary		(Equipment_light      + 1)  /* 13 */
 
//	{ tval constants					-DCJ-	}
 
#define	Miscellaneous_Object		1
#define	miscellaneous_object		1
#define	chest				2
#define	misc_usable			3
#define	valuable_jewelry		4
#define	valuable_gems			5
#define	bag_or_sack			6
#define	valuable_gems_wear		7
#define	sling_ammo			10
#define	bolt				11
#define	arrow				12
#define	spike				13
#define	Lamp_or_Torch			15
#define	lamp_or_torch			15
#define	bow_crossbow_or_sling		20
#define	hafted_weapon			21
#define	pole_arm			22
#define	dagger				23
#define	sword				24
#define	pick_or_shovel			25
#define	maul				26
#define	gem_helm			29
#define	boots				30
#define	gloves_and_gauntlets		31
#define	Cloak				32
#define	cloak				32
#define	helm				33
#define	shield				34
#define	hard_armor			35
#define	soft_armor			36
#define	bracers				37
#define	belt				38
#define	amulet				40
#define	ring				45
#define	staff				55
#define	rod				60
#define	wand				65
#define	scroll1				70
#define	scroll2				71
#define	potion1				75
#define	potion2				76
#define	flask_of_oil			77
#define	Food				80
#define	Junk_food		 	81
#define	junk_food		 	81
#define	chime				85
#define	horn				86
#define	Magic_Book			90
#define	magic_book			90
#define	Prayer_Book			91
#define	prayer_book			91
#define	Instrument			92
#define	instrument			92
#define	Song_Book			93
#define	song_book			93
#define	valuable_metal			100 /* look in detect_item for limit */
#define	Unseen_trap			101
#define	unseen_trap			101
#define	Seen_trap			102
#define	seen_trap			102
#define	Rubble				103
#define	rubble				103
#define	Open_door			104
#define	open_door			104
#define	Closed_door			105
#define	closed_door			105
#define	Up_staircase			107
#define	up_staircase			107
#define	Down_staircase			108
#define	down_staircase			108
#define	Secret_door			109
#define	secret_door			109
#define	Entrance_to_store		110
#define	entrance_to_store		110
#define	Up_steep_staircase		111
#define	up_steep_staircase		111
#define	Down_steep_staircase		112    
#define	down_steep_staircase		112    
#define	Whirlpool			113
#define	whirlpool			113

/* fields in py.flags.status */
#define IS_HUNGERY             0x00000001
#define IS_WEAK                0x00000002
#define IS_BLIND               0x00000004
#define IS_CONFUSED            0x00000008
#define IS_AFRAID              0x00000010
#define IS_POISONED            0x00000020
#define IS_HASTED              0x00000040
#define IS_SLOW                0x00000080
#define IS_SEARCHING           0x00000100
#define IS_RESTING             0x00000200
#define IS_aaa        0x00000400
#define IS_bbb        0x00000800
#define IS_INVULNERABLE        0x00001000
#define IS_HERO                0x00002000
#define IS_SUPER_HERO          0x00004000
#define IS_BLESSED             0x00008000
#define IS_ABLE_TO_SEE_INVIS   0x00010000
#define IS_ABLE_TO_SEE_HEAT    0x00020000
#define IS_ccc        0x00040000
#define IS_ddd        0x00080000
#define IS_eee        0x00100000
#define IS_fff        0x00200000
#define IS_ggg        0x00400000
#define IS_CHARM_PROOF         0x00800000
#define IS_hhh        0x01000000
#define IS_iii        0x02000000
#define IS_jjj        0x04000000
#define IS_kkk        0x08000000
#define IS_lll        0x10000000
#define IS_STEALTHY            0x20000000
#define IS_MAGIC_PROTECED      0x40000000
#define IS_mmm        0x80000000


#define	MITHRIL_POS			(MAX_GOLD)
#define	PLATINUM_POS			(MAX_GOLD-1)
#define	GOLD_POS			(MAX_GOLD-2)
#define	SILVER_POS			 4
#define	COPPER_POS			 3
#define	IRON_POS			 2

#define	TOTAL_   0
#define	IRON     1
#define	COPPER   2
#define	SILVER   3
#define	GOLD     4
#define	PLATINUM 5
#define	MITHRIL  6

#define	COIN_WEIGHT  5

#define	CTRL_A			  1 // chr(1);
#define	CTRL_B			  2 // chr(2);
#define	CTRL_C			  3 // chr(3);
#define	CTRL_D			  4 // chr(4);
#define	CTRL_E			  5 // chr(5);
#define	CTRL_F			  6 // chr(6);
#define	CTRL_G			  7 // chr(7);
#define	CTRL_H			  8 // chr(8);
#define	CTRL_I			  9 // chr(9);
#define	CTRL_J			 10 // chr(10);
#define	CTRL_K			 11 // chr(11);
#define	CTRL_L			 12 // chr(12);
#define	CTRL_M			 13 // chr(13);
#define	CTRL_N			 14 // chr(14);
#define	CTRL_O			 15 // chr(15);
#define	CTRL_P			 16 // chr(16);
#define	CTRL_Q			 17 // chr(17);
#define	CTRL_R			 18 // chr(18);
#define	CTRL_S			 19 // chr(19);
#define	CTRL_T			 20 // chr(20);
#define	CTRL_U			 21 // chr(21);
#define	CTRL_V			 22 // chr(22);
#define	CTRL_W			 23 // chr(23);
#define	CTRL_X			 24 // chr(24);
#define	CTRL_Y			 25 // chr(25);
#define	CTRL_Z			 26 // chr(26);

#define STAT_SET_MAX 5   // type stat_set is supposed to be 0..5
#define OBJ_SET_MAX  255 // type obj_set is supposed to be 0..255
#define MAX_OBJ_SET  25  // max items allowed in an obj_set[]

#define R_HUMAN	     0
#define R_HALF_ELF   1
#define R_ELF	     2
#define R_HALFLING   3
#define R_GNOME	     4
#define R_DWARF	     5
#define R_HALF_ORC   6
#define R_HALF_TROLL 7
#define R_PHRAINT    8
#define R_DRYAD	     9

#define C_WARRIOR    0
#define C_MAGE       1
#define C_PRIEST     2
#define C_ROGUE      3
#define C_RANGER     4
#define C_PALADIN    5
#define C_DRUID      6
#define C_BARD       7 
#define C_ADVENTURER 8
#define C_MONK       9

#define S_GENERAL       0
#define S_ARMORY        1
#define S_WEAPONS       2
#define S_TEMPLE        3
#define S_ALCHEMY       4
#define S_MAGIC         5
#define S_INN           6
#define S_LIBRARY       7
#define S_MUSIC         8
#define S_GEM           9
#define S_DELI         10
#define S_BLACK_MARKET 11
#define S_TRADE_POST   12
#define S_INSURANCE    13
#define S_BANK         14
#define S_CHANGER      15  /* money changer */
#define S_CASINO       16
#define S_FORTRESS     17
#define S_GENERIC_1    18
#define S_GENERIC_2    19
#define S_GENERIC_3    20
#define S_GENERIC_4    21
#define S_GENERIC_5    22

/*
 1  'the entrance to the General Store'
 2  'the entrance to the Armory'		
 3  'the entrance to the Weapon Smiths'	
 4  'the entrance to the Temple'		
 5  'the entrance to the Alchemy Shop'	
 6  'the entrance to the Magic Shop'	
 7  'the entrance to the Inn'		
 8  'the entrance to the Trading Post'	
 9  'the entrance to the Library'		
10  'the entrance to the Music Shop'	
11  'the entrance to the Gem Store'	
12  'the entrance to the Insurance Shop'	
13  'the entrance to the Bank'		
14  'the entrance to the Money Exchange'	
15  'the entrance to the Casino'		
16  'the entrance to the All-Nite Deli'	
17  'the entrance to a strange building' 	
18  'the entrance to a building'		
19  'the entrance to a building'		
20  'the entrance to a building'		
21  'the entrance to a building'		
22  'the entrance to a building'		
*/


#define PLACE_OBJECT_TRIES  3  /* pick best object out of this many */

/* return values for master_file_verify */
#define MF_CHAR_OK          0
#define MF_CHAR_NOT_FOUND   1
#define MF_CHAR_MISMATCH    2

/* values for game_state */
#define GS_IGNORE_CTRL_C    0
#define GS_ALLOW_CTRL_C     1
#define GS_LOADING          2
#define GS_GET_COMMAND      3
#define GS_HELP             4


/* the unix code I took starts at 0,0 and the vms code starts at 1,1 */
#define prt(s,r,c)          Prt((s),(r)-1,(c)-1)
#define put_buffer(s,r,c)   Put_Buffer((s),(r)-1,(c)-1)
#define erase_line(r,c)     Erase_Line((r)-1,(c)-1)
#define clear_from(r)       Clear_From((r)-1)
#define print(s,r,c)        Print((s),(r)-1,(c)-1)
#define get_string(s,r,c,l) Get_String((s),(r)-1,(c)-1,(l))
#define pause_line(r)       Pause_Line((r)-1)
#define pause_exit(r,d)     Pause_Exit((r)-1,(d))
#define print_null(r,c) (move_cursor((r)-panel_row_prt-1,(c)-panel_col_prt-1))

/* END FILE  constants.h */
