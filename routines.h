/* routines.h */
/**/


// { CASINO.PAS		}
 extern void enter_casino();


// { CREATE.PAS		}
 extern void put_character();
 extern void put_stats();
 extern void upd_stats(); 
 extern void put_misc1(); 
 extern void upd_misc1(); 
 extern void put_misc2(); 
 extern void put_misc3(); 
 extern void display_char(); 
 extern void get_name(); 
 extern void change_name(); 
 extern void create_character(); 
 extern void get_ssn(); 
 extern void set_gem_values(); 

// { CREATURE.PAS		}
 extern void load_monsters(); 
 extern void mon_name(); 
 extern integer find_mon(ctype virtual_name);
 extern void check_mon_lite(integer y, integer x); 
 extern void multiply_monster(integer y,integer x,integer z,boolean slp); 
 extern void creatures(boolean attack); 

	// debug routine
 extern void print_creature(creature_type *c, int c_num, int style);


// { DEATH.PAS		}
 extern void upon_death(); 
 extern void make_tomb(vtype dstr[]); 
 extern void replace_name(); 
 extern void write_tomb(vtype dstr[]); 
 extern void print_dead_character(); 
 extern void top_twenty(integer this_many); 


// { DESC.PAS		}
 extern void randes(); 
 extern void rantitle(char *title);	// : varying[a] of char); 
 extern void magic_init(unsigned long random_seed); 
 extern void known1(char *object_str);// : varying[a] of char); 
 extern void known2(char *object_str);// : varying[a] of char); 
 extern void unquote(char *object_str);// : varying[a] of char); 
 extern void identify(treasure_type *item); 
 extern void objdes(
		char *out_val,	//: varying[a] of char;
		treas_ptr ptr,	//	: treas_ptr;
		boolean pref);	//	: boolean); 


// { DUNGEON.PAS		}
 extern void move_rec(integer y1,integer x1,integer y2,integer x2);
 extern void update_stat(stat_set tstat);
 extern void change_stat(
		stat_set tstat,
		integer amount,
		integer factor);
 extern void change_speed(integer num); 
 extern void py_bonuses(
		treasure_type *tobj,
		integer	factor); 
 extern boolean get_panel(integer y, integer x, boolean forceit);
 extern void search(integer y,integer x,integer chance);
 extern void area_affect(integer dir,integer y,integer x);
 extern void carry(integer y,integer x);
 extern void move_light(integer y1,integer x1,integer y2,integer x2);
 extern void light_room(integer y,integer x);
 extern void lite_spot(integer y,integer x);
 extern void unlite_spot(integer y,integer x);
 extern boolean pick_dir(integer dir);
 extern void panel_bounds(); 
 extern boolean panel_contains(integer y,integer x);
 extern boolean no_light();
 extern void change_trap(integer y,integer x); 
 extern void kicked_out(); 
 extern void call_guards(vtype who); 
 extern void call_wizards(); 
 extern void beg_food(); 
 extern void beg_money(); 
 extern void invite_for_meal(); 
 extern void party(); 
 extern void spend_the_night(vtype who); 
 extern void worship(); 
 extern void battle_game(integer plus, vtype kb_str); 
 extern void brothel_game();
 extern void thief_games(); 
 extern integer react(integer x);
 extern void change_rep(integer amt); 
 extern boolean check_store_hours(integer st,integer sh);
 extern void check_store_hours_and_enter(integer st,integer sh,integer store_num);
 extern void hit_trap(integer *y,integer *x); 
 extern boolean minus_ac(integer typ_dam);
 extern void corrode_gas(vtype kb_str); 
 extern void poison_gas(integer dam, vtype kb_str); 
 extern void fire_dam(integer dam, vtype kb_str); 
 extern void acid_dam(integer dam, vtype kb_str); 
 extern void cold_dam(integer dam, vtype kb_str);
 extern void light_dam(integer dam, vtype kb_str);
 extern void monster_death(integer y,integer x, unsigned long flags); 
 extern integer mon_take_hit(integer monptr,integer dam);
 extern integer tot_dam(
		treasure_type *item,
		integer tdam,
		creature_type *monster);
 extern boolean py_attack(integer y,integer x);
 extern boolean find_range(
		obj_set		item_val,
		boolean		inner,
		treas_ptr	*first,
		integer		*count);
 extern boolean player_test_hit(integer bth,integer level,integer pth,
				integer ac, boolean was_fired);
 extern boolean test_hit(integer bth,integer level,integer pth,integer ac);
 extern void delete_monster(integer i2); 
 extern void summon_object(integer y,integer x,integer num,integer typ); 
 extern integer get_money_type(
		string		prompt,
		boolean		*back,
		boolean		no_check);
 extern boolean coin_stuff(char typ, integer *type_num);
 extern boolean set_money(char typ, integer coin_num);
						
 extern integer movement_rate(integer cspeed,integer mon);
 extern void get_player_move_rate(); 
 extern void xp_loss(integer amount); 
 extern boolean twall(integer y,integer x,integer t1,integer t2);
 extern void dungeon(); 


// { FILES.PAS		}
 extern void intro(vtype finam, int argc, char *argv[]); 
 extern void print_map(); 
 extern void print_objects(); 
 extern void print_monsters(); 
 extern void file_character(); 
 extern boolean read_top_scores(FILE **f1, char *fnam, string list[],
			int max_high, int *n1, char *openerr);
 extern boolean write_top_scores(FILE **f1, string list[], int max_high);
 extern boolean close_top_scores(FILE **f1);
 extern char * format_top_score(vtype out_str, char *username, integer score,
                        int diffic, char *charname, int level, char *race,
                        char *class);
 extern FILE *priv_fopen( char *path, char *mode);
 extern void encrypt_file(vtype fnam);
 extern void decrypt_file(vtype fnam);

// { GENERATE.PAS		}
 extern void generate_cave(); 


// { HELP.PAS		}
 extern void ident_char(); 
 extern void help(); 
 extern void wizard_help(); 
 extern void moria_help(vtype help_level); 
 extern void enter_wizard_mode(boolean ask_for_pass);

// { INVEN.PAS		}
 extern integer change_all_ok_stats(boolean nok,boolean nin);
 extern char cur_char1();
 extern char cur_char2();
 extern char cur_insure();
 extern boolean inven_command(
		char		command,
		treas_ptr	*item_ptr,
		vtype		prompt);
 extern void delete_inven_item(treas_ptr ptr); 
 extern void inven_destroy(treas_ptr item_ptr); 
 extern void inven_drop(
		treas_ptr	item_ptr,
		integer		y,
		integer		x,
		boolean		mon); 
 extern integer inven_damage(
		obj_set		typ,
		integer		perc);
 extern boolean inven_check_weight(); 
 extern boolean inven_check_num(); 
 extern treas_ptr add_inven_item(treasure_type item);
 extern treas_ptr inven_carry();
 extern boolean get_item(
			treas_ptr	*com_ptr,
			vtype		pmt,
			boolean		*redraw,
			integer		count,
			char		*choice,
			boolean		mon,
			boolean		no_wait);//	: boolean := false);


// { IO.PAS		}
 extern void convert_time(
		unsigned long int_time,
		quad_type *bin_time); 
 extern void sleep_(unsigned long int_time); 
 extern void mini_sleep(unsigned long int_time); 
 extern void init_priv_switch();
 extern void priv_switch(integer switch_val); 
 extern void no_controly(); 
 extern void controly(); 
 extern void exit_game(); 
 extern void init_channel(); 
/* extern void inkey(char *getchar); */
 extern char inkey();
 extern void msg_record(vtype message, boolean save); 
 extern void inkey_delay(
		char *getchar,
		integer delay); 
 extern void flush(); 
 extern void inkey_flush(char *x); 
 extern void get_message(); 
 extern void set_the_trap(); 
 extern void disable_the_trap(); 
/* use erase_line */
 extern void Erase_Line(
		integer	row,
		integer col);
 extern void clear_rc(integer row,integer col);
/* use print */
 extern void Print(
		char	ch, //	: varying[a] of char;
		int	row,
		int     col); 
 extern void print_str(
		char	*str_buff, //	: varying[a] of char;
		int	row,
		int     col); 

/* use prt */
 extern void Prt(
		char	*str_buff, //	: varying[a] of char;
		int	row,
		int     col); 
 extern void prt2(
		char	*str_buff1, //	: varying[a] of char;
		char	*str_buff2,
		int	row,
		int     col); 

 extern boolean msg_print(char *str_buff); // : varying[a] of char);
 extern boolean get_com(
		char *prompt, //	: varying[a] of char;
		char *command);
 extern boolean get_yes_no(char *prompt); // : varying[a] of char);
/* use get_string */
 extern boolean Get_String(
		char *in_str,	// : varying[a] of char;
		int row,int column,int slen);
 extern integer get_hex_value(integer row,integer col,integer slen);
 extern void print_hex_value(integer num,integer row,integer col); 
 extern void pause_game(integer prt_line); 
/* use pause_exit */
 extern void Pause_Exit(
		int	prt_line,
		int	delay); 
 extern void get_paths(); 


// { PLAYER.PAS		}
 extern void find_off(); 
 extern void search_off(); 
 extern void search_on(); 
 extern void rest_off(); 
 extern void take_hit(integer damage, vtype hit_from); 
 extern void regenhp(real percent); 
 extern void regenmana(real percent); 

// { QUEST.PAS		}
 extern void enter_fortress(); 
 extern itos(integer i, ctype *result); // used to return ctype

// { SAVE.PAS		}
// extern void coder(ntype line); 
// extern void encrypt_line(ntype line); 
// extern void decrypt_line(ntype line);
 extern void data_exception(); 
 extern boolean save_char(boolean quick);
 extern boolean get_char(vtype fnam, boolean prop);
 extern void restore_char(vtype fnam,boolean present, boolean undead); 

// { encrypt.c }
 extern void encrypt_init(encrypt_state *state, byteint key[], boolean doit);
 extern void encrypt_write(FILE *f1, encrypt_state *state, ntype line);
 extern void encrypt_flush(FILE *f1, encrypt_state *state);
 extern void read_decrypt(FILE *f1, encrypt_state *state, ntype line,
                          boolean *got_eof);

// { SCREEN.PAS		}
 extern void prt_map(); 
 extern void prt_stat(
		vtype 	stat_name,
		byteint	stat,
		integer row,
		integer column); 
 extern void prt_field(vtype info, integer row, integer column); 
 extern void prt_num(
		vtype header,
		integer num,integer row,integer column); 
 extern void prt_title(); 
 extern void prt_a_stat(stat_set tstat); 
 extern void prt_level(); 
 extern void prt_mana(); 
 extern void prt_hp(); 
 extern void prt_pac(); 
 extern void prt_gold(); 
 extern void prt_weight(); 
 extern void prt_time(); 
 extern void prt_depth(); 
 extern void prt_light_on(); 
 extern void prt_hunger(); 
 extern void prt_blind(); 
 extern void prt_confused(); 
 extern void prt_afraid(); 
 extern void prt_poisoned(); 
 extern void prt_search(); 
 extern void prt_rest(); 
 extern void prt_quested(); 
 extern void prt_winner(); 
 extern void prt_experience(); 
 extern void prt_6_stats(stat_s_type p, byteint row, byteint col); 
 extern void prt_stat_block(); 
 extern void draw_cave(); 


// { STORE.PAS		}
 extern void prt_comment1(); 
 extern void prt_comment2(integer offer,integer asking,integer final); 
 extern void prt_comment3(integer offer,integer asking,integer final); 
 extern void prt_comment4(); 
 extern void prt_comment5(); 
 extern void prt_comment6(); 
 extern void display_commands(); 
 extern void haggle_commands(integer typ); 
 extern void display_inventory(integer store_num,integer start); 
 extern void display_cost(integer store_num,integer pos); 
 extern void store_prt_gold(); 
 extern void display_store(integer store_num,integer cur_top); 
 extern boolean get_store_item(
		integer	*com_val,
		vtype	pmt,
		integer i1,integer i2);
 extern void shut_store(integer store_num); 
 extern boolean increase_insults(integer store_num);
 extern void decrease_insults(integer store_num); 
 extern boolean haggle_insults(integer store_num);
 extern integer receive_offer(
		integer			store_num,
		vtype			comment,
		integer			*new_offer,
		integer			last_offer,
		integer			factor);
 extern integer purchase_haggle(
		integer		store_num,
		integer		*price,
		treasure_type	*item,
		boolean		blitz);
 extern integer sell_haggle(
		integer		store_num,
		integer		*price,
		treasure_type	*item,
		boolean		blitz);
 extern void spend_time(integer days_spent, vtype place,
					boolean whole_days); 
 extern boolean store_purchase(
		integer		store_num,
		integer		*cur_top,
		boolean		blitz);
 extern boolean store_sell(
		integer		store_num,
		integer		cur_top,
		boolean		blitz);
 extern void enter_store(integer store_num); 
 extern integer item_value(treasure_type *item);
 extern integer sell_price (
		integer		snum,
		integer		*max_sell,
		integer		*min_sell,
		treasure_type	*item);
 extern boolean store_check_num(integer store_num);
 extern void store_carry(
		integer		store_num,
		integer		*ipos); 
 extern void store_destroy(
		integer store_num,integer item_val,
		boolean one_of); 
 extern void store_init(); 
 extern void bank_init(); 
 extern void store_create(integer store_num); 
 extern void store_maint(); 


// { TERMDEF.PAS		}
 extern void termdef();


// { WIZARD.PAS		}
 extern void game_version(); 
 extern void bpswd(); 
 extern boolean check_pswd(
		string passw,
		boolean present);
 extern void wizard_light(); 
 extern void monster_summon_by_name(
		integer y,integer x,
		ctype name,
		boolean present,
		boolean sleepy);
 extern boolean summon_item(
		integer y,integer x,
		ttype name1,
		ttype name2,
		integer count,
		boolean present);
 extern void change_character(); 
 extern void edit_score_file(); 
 extern void wizard_create(); 

 extern void py_bonuses(
		treasure_type *tobj,
		integer factor); 
 extern boolean delete_object(integer y,integer x);
 extern char *center(string str, integer len, string result); // was func
 extern void eat(); 
 extern void cast(); 
 extern void discipline(); 
 extern void move_char(integer dir); 
 extern void quaff(); 
 extern void pray(); 
 extern void play(); 
 extern void sing();
 extern void read_scroll(); 
 extern void use_staff(); 
 extern boolean mon_resists(byteint a_cptr);
 extern boolean mon_save(integer a_cptr,integer bonus,integer spell_class);
 extern boolean sleep_monsters1(integer y,integer x);
 extern boolean detect_item(integer typ);
 extern boolean detect_trap();
 extern boolean detect_sdoor();
 extern boolean light_area(integer y,integer x);
 extern boolean unlight_area(integer y,integer x);
 extern boolean map_area();
 extern boolean ident_spell();
 extern boolean aggravate_monster(integer dis_affect);
 extern boolean trap_creation();
 extern boolean door_creation();
 extern boolean td_destroy();
 extern boolean light_line(integer dir,integer y,integer x,integer power);
 extern boolean starlite(integer y,integer x);
 extern boolean disarm_all(integer dir,integer y,integer x);
 extern boolean detect_curse();
 extern void get_flags (
		integer typ,
		integer *weapon_type,
		integer *harm_type,
		obj_set **destroy);
 extern boolean detect_magic();
 extern boolean purify();
 extern boolean lore_spell();
 extern boolean fire_bolt(
		integer typ,integer dir,integer y,integer x,integer dam,
		ctype bolt_typ);
 extern boolean fire_ball(
		integer typ,integer dir,integer y,integer x,integer dam_hp,
		ctype descrip);
 extern boolean creeping_doom(
		integer dir,integer y,integer x,integer dam_hp,integer range,
		ctype ddesc);
 extern boolean fire_line(
		integer typ,integer dir,integer y,integer x,integer dam_hp,
		ctype descrip);
 extern boolean breath(
		integer typ,integer y,integer x,integer dam_hp,
		vtype ddesc);
 extern boolean recharge(integer num);
 extern boolean zap_monster(integer dir,integer y,integer x,integer aux,
			integer zaptype);
 extern boolean wall_to_mud(integer dir,integer y,integer x);
 extern boolean td_destroy2(integer dir,integer y,integer x);
 extern boolean poly_monster(integer dir,integer y,integer x);
 extern boolean build_wall(integer dir,integer y,integer x);
 extern boolean clone_monster(integer dir,integer y,integer x);
 extern boolean teleport_away(integer monptr,integer dis);
 extern boolean teleport_to(integer ny,integer nx);
 extern boolean teleport_monster(integer dir,integer y,integer x);
 extern boolean mass_genocide();
 extern boolean genocide();
 extern boolean mass_poly();
 extern boolean detect_creatures(integer typ);
 extern boolean hp_player(integer num, vtype kind);
 extern boolean cure_me(integer *what_flag);
 extern boolean earthquake();
 extern boolean protect_evil();
 extern boolean create_food(integer t0,integer t1,integer 
		t2,integer t3,integer t4);
 extern boolean zap_area(integer cflag,integer dmge,integer typ);
 extern boolean warding_glyph();
 extern void lower_stat(stat_set tstat, vtype msg1); 
 extern boolean lose_stat(stat_set tstat,
		vtype msg1, vtype msg2);
 extern boolean restore_stat(stat_set tstat,
		vtype msg1);
 extern boolean gain_stat(stat_set tstat,
		vtype msg1);
 extern void lose_exp(integer amount); 
 extern boolean slow_poison();
 extern boolean bless(integer amount);
 extern boolean detect_inv2(integer amount);
 extern boolean destroy_area(integer y,integer x);
 extern boolean enchant(worlint *pluses);
 extern boolean remove_curse();
 extern boolean restore_level();
 extern void use(); 
 extern void aim_wand(); 
 extern integer randint(integer maxval);
 extern integer rand_rep(
		integer num,
		integer die);
 extern integer maxmin(
		integer	x,
		integer	y,
		integer	z);
 extern integer minmax(
		integer	x,
		integer	y,
		integer	z);
 extern integer bit_pos(unsigned long *test);
 extern integer bit_pos64(
		unsigned long	*high,
		unsigned long	*low);
 extern integer distance(
		integer y1,
		integer x1,
		integer y2,
		integer x2);
 extern void insert_str(
		char	*object_str, // : varying[a] of char;
		char	*mtc_str,    //	: varying[b] of char;
		char 	*insert_str);// : varying[c] of char); 
 extern void shell_out(
//		command_str	: integer := %immed 0;
//		input_file	: integer := %immed 0;
//		output_file	: integer := %immed 0;
//		flags		: integer := %immed 0;
//		process_name	: integer := %immed 0;
//		process_id	: integer := %immed 0;
//		comp_status	: integer := %immed 0;
//		comp_efn	: integer := %immed 0;
//		comp_astadr	: integer := %immed 0;
//		comp_astprm	: integer := %immed 0
		);
 extern integer users();
 extern integer max_users();
 extern boolean get_uw_id();		// was uw_id()
 extern boolean net_trade();
 extern void get_account (char *account); 
 extern boolean already_playing();
 extern void sys_gettim(quad_type *bin_time);
 extern void sys_numtim(
		time_type *weird_num,
		quad_type *bin_time);
 extern void sys_asctim(
		wordint   *timlin,	//: [reference] wordint := %immed 0;
		char      *timbuf,	//: [class_s] packed array [$l1..$u1:integer] of char;
		quad_type *timadr,	//: [reference] quad_type := %immed 0;
		integer   *cvtflg);	//: [reference] integer := %immed 0); 

// { MISC.PAS		}
 extern unsigned long get_seed();
 extern void set_seed(unsigned long the_seed);
 extern void *save_rand_state(void *randState);
 extern void restore_rand_state(void *randState);
 extern void init_m_level(); 
 extern void init_t_level(); 
 extern void price_adjust(); 
 extern void item_weight_adjust(); 
 extern void set_difficulty(integer diff); 
 extern integer day_num();
 extern integer hour_num();
 extern boolean check_kickout();
 extern boolean check_time();
 extern integer randnor(integer mean,integer stand);
 extern boolean in_bounds(integer y,integer x);
 extern integer next_to4(
		integer y,integer x,
		obj_set group_set);
 extern integer next_to8(
		integer y,integer x,
		obj_set group_set);
 extern integer rotate_dir(integer dir,integer rot);
 extern integer get_hexdecant(integer dy,integer dx);
 extern void tlink(); 
 extern void mlink(); 
 extern integer damroll(dtype dice);
 extern boolean los(integer y1,integer x1,integer y2,integer x2);
 extern char get_loc_symbol(integer y, integer x);
 extern char loc_symbol(integer y,integer x); 
 extern boolean test_light(integer y,integer x);
 extern void compact_monsters(); 
 extern void popm(integer *x); 
 extern void pushm(integer x); 
 extern integer max_hp(dtype hp_str);
 extern void place_monster(integer y,integer x,integer z, boolean slp); 
 extern void place_win_monster(); 
 extern void alloc_land_monster(obj_set alloc_set,
		integer num,integer dis,
		boolean slp,
		boolean water); 

 extern boolean summon_land_monster(
		integer *y, integer *x,
		boolean slp);
 extern boolean summon_water_monster(
		integer *y, integer *x,
		boolean slp);
 extern boolean summon_undead(integer *y,integer *x);
 extern boolean summon_demon(integer *y,integer *x);
 extern boolean summon_breed(integer *y,integer *x);
 extern void petrify(integer amt); 
 extern void compact_objects(); 
 extern void popt(integer *x); 
 extern void pusht(integer x); 
 extern	void sort_objects(); 
 extern void magic_treasure(integer x,integer level, boolean forceit); 
 extern void place_trap(integer y,integer x,integer typ,integer subval); 
 extern void place_rubble(integer y,integer x);
 extern void place_open_door(integer y,integer x);
 extern void place_broken_door(integer y,integer x);
 extern void place_closed_door(integer y,integer x);
 extern void place_locked_door(integer y,integer x);
 extern void place_stuck_door(integer y,integer x);
 extern void place_secret_door(integer y,integer x);
 extern void place_door(integer y,integer x);
 extern void place_a_staircase(integer y,integer x,integer typ); 
 extern void place_stairs(integer typ,integer num,integer walls); 
 extern void place_gold(integer y,integer x);
 extern integer get_obj_num(integer level, integer tries);
 extern void place_object(integer y,integer x);
 extern void alloc_object(
		obj_set alloc_set,
		integer typ,integer num); 
 extern void random_object(integer y,integer x,integer num); 
 extern void cnv_stat(
		byteint		stat,
		stat_type	out_val); 
 extern integer spell_adj(stat_set attr);
 extern integer bard_adj();
 extern integer druid_adj();
 extern integer monk_adj();
 extern real chr_adj();
 extern integer con_adj();
 extern integer get_hitdie();
 extern char * place_string(integer num, string result); // was func
 extern char * day_of_week_string(		       // was func
		integer day,
		integer wid,
		string  result);
 extern char * month_string(integer mon, string result); // was func
 extern char * time_string(
		integer hour,
		integer sec,
		string  result);			// was func
 extern void time_diff(
		game_time_type	a,
		game_time_type	b,
		game_time_type	*c);
 extern void add_days(
		game_time_type	*ti,
		integer d); 
 extern char *full_date_string(game_time_type time, string result); // was func
 extern void adv_time(boolean flag); 
 extern char *play_time(time_type *t, string result);	// was func
 extern void add_play_time(
		time_type	*res,
		time_type	add); 
 extern time_type *convert_seconds_to_time(time_t seconds, time_type *tim);
 extern time_t convert_time_to_seconds(time_type *tim);
 extern char *show_char_age(string result);		// was func
 extern char *show_current_time(string result);		// was func
 extern char *show_play_time(string result);		// was func
 extern char *bag_descrip(treas_ptr bag, string result);// was func
 extern byteint squish_stat(integer this);
 extern byteint in_statp(byteint stat);
 extern byteint de_statp(byteint stat);
 extern integer tohit_adj();
 extern integer toac_adj();
 extern integer todis_adj();
 extern integer todam_adj();
 extern char *likert(integer x,integer y, btype result);	// was func
 extern byteint  characters_sex();
 extern wordint  max_allowable_weight();
 extern wordint  min_allowable_weight();
 extern integer weight_limit();
 extern treas_ptr money_carry();
 extern char * cost_str(integer amt, string result);	// was func
 extern void reset_total_cash(); 
 extern void add_money(integer amount); 
 extern void subtract_money(
		integer amount,
		boolean make_change); 
 extern boolean send_page(integer to_bank);
 extern void spell_chance(spl_rec *spell); 
 extern void print_new_spells(
		spl_type	spell,
		integer		num,
		boolean		*redraw); 
 extern boolean get_spell(spl_type spell, integer num,
		integer *sn,integer *sc, vtype prompt,
		boolean *redraw);
 extern integer num_new_spells(integer smarts);
 extern boolean learn_spell(boolean *redraw);
 extern boolean learn_prayer();
 extern boolean learn_song(boolean *redraw);
 extern boolean learn_druid();
 extern void gain_mana(integer amount); 
 extern void gain_level(); 
 extern void insert_num(
		char		*object_str, //	: varying[a] of char;
		char		*mtc_str,    //	: varying[b] of char;
		integer		number,
		boolean		show_sign); 
 extern integer attack_blows(
		integer weight,
		integer *wtohit);
 extern integer critical_blow(
		integer weight,integer plus,
		boolean cs_sharp,boolean is_fired);
 extern boolean move_dir(integer dir, integer *y, integer *x); /* was move */
 extern boolean player_saves(integer adjust);
 extern boolean player_spell_saves();
 extern void char_inven_init(); 
 extern void find_monster_name(
		vtype 		m_name,
		integer		ptr,
		boolean		begin_sentence); 
 extern void check_kickout_time(integer num,integer check); 

// { BANK.INC		}
extern	void enter_bank();

// { INSURANCE.INC	}
extern  void buy_insurance();

// { river.c }
extern void all_the_river_stuff();

// { rooms.c }
extern void gc__build_room(integer yval, integer xval);
extern void gc__build_type1(integer yval, integer xval);
extern void gc__build_type2(integer yval, integer xval);
extern void gc__build_type3(integer yval, integer xval);


// { port.c		}
extern	void * safe_malloc(int size, char *message);
extern	void dispose(void *ptr, int size, char *message);
extern  char * chomp(char *input_line);
extern  integer min3(integer i1, integer i2, integer i3);

extern	void ignore_signals();
extern	void restore_signals();
extern	void default_signals();


// { term.c		}
extern	void init_curses();

/* use put_buffer */
extern void Put_Buffer(
		char	*out_str,//	: varying [a] of char;
		integer row,
		integer	col);
extern void put_buffer_attr(
		char	*out_str,//	: varying [a] of char;
		integer row,
		integer	col,
		int     attrs);
extern void put_qio();
/* use clear_from */
extern void Clear_From (int row);
extern void move_cursor_relative(int row, int col);
/* use pause_line */
extern void Pause_Line(int prt_line);
extern void move_cursor(int row, int col);
extern unsigned sleep();
extern void screen_map();
extern void show_location();

// { unix.c		}
extern void user_name(char *buf);
extern int check_input(int microsec);

//extern long time(long *);
//extern char * ctime(long *);


/* END FILE  routines.h */
