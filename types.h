/* types.h */
/* The global types for imoria */

typedef unsigned char byteint;          // [byte] 0..255
typedef	signed char bytlint;            // [byte] -128..127;
typedef	unsigned short wordint;		// [word] 0..65535;
typedef	short worlint;		        // [word] -32768..32767;

typedef unsigned char   boolean;
typedef long            integer;
typedef float           real;

typedef struct quad_type
{
  unsigned short l0;
  unsigned short l1;
} quad_type;

typedef char atype[16+2];
typedef char btype[14+2];
typedef char ctype[26+2];
typedef char dtype[5+2];
typedef char etype[34+2];
typedef char htype[12+2];
typedef	char string[132+2];
typedef	char mtype[190+2];
typedef	char ntype[1024+2];
typedef	char ttype[68+2];
typedef	char vtype[80+2];
typedef char stat_type[6+2]; //	= packed array [1..6] of char;
typedef	char account_type[10+2];
typedef	byteint obj_set[MAX_OBJ_SET];       	// = set of 0..255;
//	char_set	= set of 'A'..'z';
typedef	char stat_set;	// = 0..5;
typedef	byteint stat_s_type[STAT_SET_MAX+1]; // = array [stat_set] of byteint;
//	ssn_type	= packed array [1..70] of char;
//	key_type	= record		{ For char saver	}
//		file_id : [key(0)] ssn_type;
//		seed	: integer;
//	end;

typedef	integer money_type[MITHRIL+1]; // = array[total$..mithril] of integer;

typedef struct game_time_type
{
	integer year;
	byteint month;
	byteint day;
	byteint hour;
	wordint secs;
} game_time_type;

typedef struct time_type
{
	wordint years;
	wordint months;
	wordint days;
	wordint hours;
	wordint minutes;
	wordint seconds;
	wordint hundredths;
} time_type;

typedef struct creature_type
{
	byteint         aaf;//: [bit(7),pos(0)] 0..127; { Area affect radius }
	byteint	        ac; //: [bit(7),pos(8)] 0..127;	{ AC	}
	ctype	        name;	 // { Descrip of creature	}
	unsigned long	cmove;	 //: unsigned;	// { Bit field		}
	unsigned long	spells;	 //: unsigned;	// { Creature spells	}
	unsigned long	cdefense;//: unsigned;	// { Bit field		}
	worlint	        sleep;                  // { Inactive counter	}
	integer      	mexp;	        	// { Exp value for kill	}
	bytlint	        speed;           	// { Movement speed	}
	char	        cchar;	        	// { Character rep.	}
	dtype	        hd;                     // { Creatures hit die	}
	etype           damage;         	// { Type attack and damage}
	bytlint 	level;           	// { Level of creature	}
	byteint 	mr;             	// { Magic Resistance      }
} creature_type;

typedef struct monster_type
{
	worlint	hp;		// { Hit points		}
	worlint	csleep;		// { Inactive counter	}
	worlint	cdis;		// { Cur dis from player	}
	wordint	mptr;		// { Pointer into creature	}
	wordint	nptr;		// { Pointer to next block	}
	bytlint	cspeed;		// { Movement speed	}

//			{ Note: FY and FX constrain dungeon size to 255	}
	byteint	fy;     	// { Y Pointer into map	}
	byteint	fx;      	// { X Pointer into map	}

	bytlint	stunned; // [bit(6),pos(104)] -32..31; // { Rounds stunned}
	boolean	ml;	 // [bit(1),pos(110)] boolean; // { On if shown   }
	boolean	confused;// [bit(1),pos(111)] boolean; // { On if confused}
	boolean	moved;	 // [bit(1),pos(112)] boolean; // { On if water-moved}
} monster_type;

typedef struct treasure_type
{
	ttype	name;	// { Object name		}
	byteint	tval;	// { Catagory number	}
	char	tchar;	// { Character representation}
	unsigned long	flags2;	//: unsigned;	// { MORE Special flags	}
	unsigned long	flags;	//: unsigned;	// { Special flags	}
	integer	p1;	// { Misc. use variable	}
	integer	cost;	// { Cost of item		}
	integer	subval;	// { Sub-catagory number	}
	wordint	weight;	// { Weight in gp's	}
	wordint	number;	// { Number of items	}
	worlint	tohit;	// { Pluses to hit		}
	worlint	todam;	// { Pluses to damage	}
	worlint	ac;	// { Normal AC		}
	worlint	toac;	// { Pluses to AC		}
	dtype	damage;	// { Damage when hits	}
	bytlint	level;	// { Level item found	}
} treasure_type;

typedef struct treas_rec
{
	treasure_type	data;
	boolean	        ok;
	wordint   	insides;
	boolean 	is_in;
	struct treas_rec * next; //	: ^treas_rec;
} treas_rec;

typedef  treas_rec *treas_ptr;

// various player fields

typedef struct p_misc
{
	integer	        xtr_wgt;	// { Extra weight limit	}
	integer	        account;	// { Money in the bank	}
	money_type      money;	        // { Money on person	}
	game_time_type  birth;          // {Date of char's birth}
	game_time_type  cur_age;        // {Current game date	}
	time_type       play_tm;	// { Time spent in game	}
	byteint	        diffic;  	// { Difficulty of game	}
//	ssn_type        ssn;   	// { Social Security Number}
	vtype		name;	// { Name of character	}
	vtype		race;	// { Race of character	}
	vtype		sex;	// { Sex of character	}
	vtype		title;	// { Character's title	}
	vtype		tclass;	// { Character's class	}
	integer		max_exp;	// { Max experience}
	integer		exp;	// { Cur experienc	}
	integer		rep;	// { XP from good creatures }
	integer		deaths;	// {Number of insured restores}
	integer		premium;	// {Base cost to restore }
	wordint		age;	// { Characters age}
	wordint		ht;	// { Height	}
	wordint		wt;	// { Weight	}
	wordint		lev;	// { Level		}
	wordint		max_lev;	// { Max level explored}
	worlint		srh;	// { Chance in search}
	worlint		fos;	// { Frenq of search}
	worlint		bth;	// { Base to hit	}
	worlint		bthb;	// { BTH with bows	}
	worlint		mana;	// { Mana points	}
	worlint		mhp;	// { Max hit pts	}
	worlint		ptohit;	// { Pluses to hit	}
	worlint		ptodam;	// { Pluses to dam	}
	worlint		pac;	// { Total AC	}
	worlint		ptoac;	// { Magical AC	}
	worlint		dis_th;	// { Display +ToHit}
	worlint		dis_td;	// { Display +ToDam}
	worlint		dis_ac;	// { Display +ToAC }
	worlint		dis_tac;	// { Display +ToTAC}
	worlint		disarm;	// { % to Disarm	}
	worlint		save;	// { Saving throw	}
	worlint		sc;	// { Social Class	}
	byteint		pclass;	// { # of class	}
	byteint		prace;	// { # of race	}
	byteint		hitdie;	// { Char hit die	}
	byteint		stl;	// { Stealth factor}
	real		expfact;		// { Experience factor}
	real		cmana;		// { Cur mana pts  }
	real		chp;		// { Cur hit pts	}
	vtype		history[5];// array [1..5] of vtype;//{ History record}
	boolean		cheated;   //{ gone into wizard or god mode}
	integer		mr;	   // { mag.res.lev.delta }
  	byteint	quests;    // { # completed } /*{FUBAR}*/
  	wordint	cur_quest; // { creature # of quest } /*{FUBAR}*/
        time_t          creation_time;  // used as key in master file
        integer         save_count;     // compared to master file value
        integer         claim_check;    // used to track trading post
} p_misc;

typedef struct p_stat
{
	byteint p[STAT_SET_MAX+1];// array[stat_set] of // {permanent}
	byteint c[STAT_SET_MAX+1];// array[stat_set] of // {current=p-l+m*10}
	bytlint m[STAT_SET_MAX+1];// array[stat_set] of // {net magical adj}
	byteint l[STAT_SET_MAX+1];// array[stat_set] of // {amt lost}
} p_stat;


typedef struct p_flags
{
	boolean	insured;	// { Character insured   }
	boolean	dead;	        // { Currently restored  }
unsigned long	status;	        // { Status of player    }
	integer	rest;	        // { Rest counter	 }
	integer	blind;	        // { Blindness counter   }
	integer	paralysis;	// { Paralysis counter   }
	integer	confused;	// { Confusion counter   }
	integer	foodc;	        // { Food counter        } (was just food)
	integer	food_digested;	// { Food per round      }
	integer	protection;	// { Protection fr. evil }
	integer	speed;	        // { Cur speed adjust    }
	integer	speed_paral;	// { Slow speed adjust   }
	boolean	speed_flag;	// { On if reset speed   }
	integer	paral_init;	// { Init val for slow   }
	integer	move_rate;	// { move_rate	         }
	integer	swim;	        // { Cur swim adjust     }
	integer	fast;	        // { Temp speed change   }
	integer	slow;	        // { Temp speed change   }
	integer	petrification;	// { Amount Petrified    }
	integer	afraid;	        // { Fear                }
	integer	poisoned;	// { Poisoned            }
	integer	image;	        // { Hallucinate         }
	integer	protevil;	// { Protect VS evil     }
	integer	invuln;	        // { Increases AC        }
	integer	hero;	        // { Heroism	         }
	integer	shero;	        // { Super Heroism	 }
	integer	blessed;	// { Blessed	         }
	integer	resist_heat;	// { Timed heat resist   }
	integer	resist_cold;	// { Timed cold resist   }
	integer	detect_inv;	// { Timed see invisible }
	integer	word_recall;	// { Timed teleport level}
	integer	see_infra;	// { See warm creatures  }
	integer	tim_infra;	// { Timed infra vision  }
	boolean	see_inv;	// { Can see invisible   }
	boolean	teleport;	// { Random teleportation}
	boolean	free_act;	// { Never paralyzed     }
	boolean	slow_digest;	// { Lower food needs    }
	boolean	aggravate;	// { Agravate monsters   }
	boolean	fire_resist;	// { Resistance to fire  }
	boolean	cold_resist;	// { Resistance to cold  }
	boolean	acid_resist;	// { Resistance to acid  }
	boolean	hunger_item;	// { Resets food counter }
	boolean	regenerate;	// { Regenerate hit pts  }
	boolean	lght_resist;	// { Resistance to light }
	boolean	ffall;	        // { No damage falling   }
	boolean sustain[STAT_SET_MAX+1];      // { keep characteristic }
		//sustain		: array [0..5] of boolean;
	boolean	confuse_monster;// { Glowing hands...    }
	integer	resist_lght;	// { Timed lighting rst  }
	integer	free_time;	// { Timed free action   }
	integer	ring_fire;	// { Timed fire spell    }
	integer	protmon;	// { Timed monst prot    }
	integer	hoarse;	        // { Timed no-bard spells}
	integer	magic_prot;	// { Timed magic prot    }
	integer	ring_ice;	// { Timed cold spell    }
	integer	temp_stealth;	// { Timed stealth       }
	integer	resist_petri;	// { Timed resist petrify}
	integer	blade_ring;	// { Timed blade spell   }
	boolean	petri_resist;	// { Resist Petrification}
        boolean	quested;	// { Performing a Quest  } /*{FUBAR}*/
        boolean light_on;       // { Light source is active }
        boolean resting_till_full;
} p_flags;

typedef struct player_type
{
	p_misc	misc;
        p_stat  stat;
        p_flags flags;
} player_type;

typedef struct spell_type
{
	ctype	sname;
	byteint	slevel;
	byteint	smana;
	wordint	sexp;
	byteint	sfail;
	boolean	learned;
} spell_type;

typedef struct spl_rec
{
	integer	splnum;
	integer	splchn;
} spl_rec;

typedef spl_rec spl_type[MAX_SPELLS]; // array [1..max_spells] of spl_rec;

typedef struct race_type // = packed record
{
	vtype	trace;   // { Type of race		}
	bytlint	adj[STAT_SET_MAX+1];//	: array [stat_set] of bytlint;
	wordint	b_age; // { Base age of character	}
	wordint	m_age; // { Maximum age of character}
	wordint	m_b_ht; // { base height for males	}
	wordint	m_m_ht; // { mod height for males	}
	wordint	m_b_wt; // { base weight for males	}
	wordint	m_m_wt; // { mod weight for males	}
	wordint	f_b_ht; // { base height females	}
	wordint	f_m_ht; // { mod height for females	}
	wordint	f_b_wt; // { base weight for female	}
	wordint	f_m_wt; // { mod weight for females	}
	real	b_exp;	   // { Base experience factor	}
	bytlint	b_dis; // { base chance to disarm	}
	bytlint	srh; // { base chance for search	}
	bytlint	stl; // { Stealth of character	}
	bytlint	fos; // { frequency of auto search} 
	bytlint	bth; // { adj base chance to hit	}
	bytlint	bthb; // { adj base to hit with bows}
	bytlint	bsav; // { Race base for saving throw}
	bytlint	bhitdie; // { Base hit points for race}
	bytlint	infra; // { See infra-red		}
	bytlint	swim; // { Race base for swim speed}
	unsigned long tclass; // { Bit field for class types}
} race_type;

typedef struct class_type
{
	vtype	title;     // { type of class		}
	real	m_exp;	   // { Class experience factor	}
	bytlint	adj_hd;    // { Adjust hit points	}
	bytlint	mdis;      // { mod disarming traps	}
	bytlint	msrh;      // { modifier to searching	}
	bytlint	mstl;      // { modifier to stealth	}
	bytlint	mfos;      // { modifier to freq-of-search	}
	bytlint	mbth;      // { modifier to base to hit/2 levels }
	bytlint	mbthb;     //{modifier to base to hit - bows/2 levels}
	bytlint	msav;      // { Class modifier to save	}
	bytlint	madj[STAT_SET_MAX+1];//	: array[stat_set] of bytlint;
	boolean	pspell;    // { class use priest spells	}
	boolean	mspell;    // { class use mage spells	}
	boolean	dspell;    // { class use druid spells	}
	boolean	bspell;    // { class use bardic spells	}
	boolean	mental;    // { class use Monk disciplines }
	integer	mr;        // { mag.res.class.delta        }
} class_type;

typedef struct	background_type
{
	vtype	info;      // { History information	}
	byteint	roll;      // { Die roll needed for history}
	byteint	chart;     // { Table number		}
	bytlint	next;      // { Pointer to next table	}
	bytlint	bonus;     // { Bonus to the Social Class	}
} background_type;

typedef struct	floor_type
{
	byteint	ftval;	//: [bit(7),pos(0)] 0..127;
	boolean	ftopen; //: [bit(1),pos(7)] boolean;
} floor_type;

typedef struct	cave_type
{
	byteint	cptr; //	: byteint;
	byteint	tptr; //	: byteint;
	byteint	fval; //	: [bit(7),pos(16)] 0..127;
	boolean	fopen;//	: [bit(1),pos(23)] boolean;
	boolean	fm;   //	: [bit(1),pos(24)] boolean;
	boolean	pl;   //	: [bit(1),pos(25)] boolean;
	boolean	tl;   //	: [bit(1),pos(26)] boolean;
	boolean	moved;//	: [bit(1),pos(27)] boolean;
	byteint	oct;  //	: [bit(3),pos(28)] 0..7; //{ octant direction }
	byteint	h2o;  //	: [bit(4),pos(31)] 0..15;
} cave_type;

typedef cave_type row_floor[MAX_WIDTH+1];//= array [1..max_width] of cave_type;

typedef struct	owner_type
{
	vtype	owner_name;
	integer	max_cost;
	real	max_inflate;
	real	min_inflate;
	real	haggle_per;
	byteint	owner_race;
	byteint	insult_max;
} owner_type;

typedef struct	inven_record
{
	integer	        scost;
	treasure_type	sitem;
} inven_record;

typedef struct store_type
{
	game_time_type	store_open;
	byteint	        owner;
	bytlint	        insult_cur;
	byteint	        store_ctr;
	inven_record	store_inven[STORE_INVEN_MAX+1];
	                 // : array [1..store_inven_max] of inven_record;
} store_type;

typedef	struct message_record
{
  string   data;
  struct   message_record  *next;
} message_record;

typedef	message_record *message_ptr; // = ^message_record;

typedef struct list_elem
{
  treasure_type      data;
  struct list_elem  *next;
} list_elem;

typedef	list_elem *list_elem_ptr;    // = ^list_elem;

/* needs to be a multiple of 8 */
#define ENCRYPT_STAT_BUF_SIZE 1024
typedef struct encrypt_state
{
  byteint     des_key[8];
  byteint     des_ivec[8];
  boolean     doit;

  boolean     got_eof;                 /* out of bytes and hit eof         */
  int         buf_pos;                 /* for read/write, current position */
  int         buf_size;                /* for reading, bytes in buffer     */
  char        data_buf[ENCRYPT_STAT_BUF_SIZE+8];
} encrypt_state;

typedef struct master_key
{
  time_t      creation_time;
} master_key;

typedef struct master_entry
{
  integer     save_count;
  integer     deaths;
  time_t      updated;
} master_entry;

/* END FILE  types.h */
