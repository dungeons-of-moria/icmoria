/* generate.c */
/**/

#include "imoria.h"
#include "generate.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__correct_dir(integer *rdir, integer *cdir, 
		     integer y1, integer x1, integer y2, integer x2)
{
  /*{ Always picks a correct direction		}*/

  if (y1 < y2) {
    *rdir =  1;
  } else if (y1 == y2) {
    *rdir =  0;
  } else {
    *rdir = -1;
  }

  if (x1 < x2) {
    *cdir =  1;
  } else if (x1 == x2) {
    *cdir =  0;
  } else {
    *cdir = -1;
  }

  if ((rdir != 0) && (cdir != 0)) {
    if (randint(2) == 1) {
      *rdir = 0;
    } else {
      *cdir = 0;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__rand_dir(integer *rdir,integer *cdir, 
		  integer y1, integer x1, integer y2, integer x2,
		  integer chance)
{
  /*{ Chance of wandering direction			}*/

  switch (randint(chance)) {
  case 1 :
    *rdir = -1;    *cdir =  0;    break;
  case 2 :
    *rdir =  1;    *cdir =  0;    break;
  case 3 :
    *rdir =  0;    *cdir = -1;    break;
  case 4 :
    *rdir =  0;    *cdir =  1;    break;
  default :
    gc__correct_dir(rdir,cdir,y1,x1,y2,x2); break;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__blank_cave()
{
  /*{ Blanks out entire cave				-RAK-	}*/

  integer   i1,i2;

  for (i1 = 0; i1 <= MAX_HEIGHT; i1++) {
    for (i2 = 0; i2 <= MAX_WIDTH; i2++) {
      cave[i1][i2] = blank_floor;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__fill_cave(floor_type fill)
{
  /*{ Fills in empty spots with desired rock		-RAK-	}*/
  /*{ Note: 9 is a temporary value.				}*/

  integer  i1,i2;

  for (i1 = 2; i1 <= cur_height - 1 ; i1++) {
    for (i2 = 2; i2 <= cur_width - 1 ; i2++) {
      if (is_in(cave[i1][i2].fval, blank_floor_set)) {
	cave[i1][i2].fval  = fill.ftval;
	cave[i1][i2].fopen = fill.ftopen;
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__place_boundry()
{
  /*{ Places indestructable rock around edges of dungeon	-RAK-	}*/

  integer   i1;

  for (i1 = 1; i1 <= cur_height; i1++) {
    cave[i1][1].fval          = boundry_wall.ftval;
    cave[i1][1].fopen         = boundry_wall.ftopen;
    cave[i1][cur_width].fval  = boundry_wall.ftval;
    cave[i1][cur_width].fopen = boundry_wall.ftopen;
  }

  for (i1 = 1; i1 <= cur_width; i1++) {
    cave[1][i1].fval           = boundry_wall.ftval;
    cave[1][i1].fopen          = boundry_wall.ftopen;
    cave[cur_height][i1].fval  = boundry_wall.ftval;
    cave[cur_height][i1].fopen = boundry_wall.ftopen;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__place_streamer(floor_type rock, integer treas_chance)
{
  /*{ Places "streamers" of rock through dungeon		-RAK-	}*/

  integer   i1,y,x,dir,ty,tx,t1,t2;
  boolean   flag;

  /*{ Choose starting point and direction		}*/
  y = trunc(cur_height/2.0) + 11 - randint(23);
  x = trunc(cur_width/2.0)  + 16 - randint(33);

  dir = randint(8);	/*{ Number 1-4, 6-9	}*/
  if (dir > 4) {
    dir++;
  }

  /*{ Place streamer into dungeon			}*/
  flag = false;	/*{ Set to true when y,x are out-of-bounds}*/
  t1 = 2*DUN_STR_RNG + 1;	/*{ Constants	}*/
  t2 =   DUN_STR_RNG + 1;

  do {
    for (i1 = 1; i1 < DUN_STR_DEN; i1++) {
      ty = y + randint(t1) - t2;
      tx = x + randint(t1) - t2;
      if (in_bounds(ty,tx)) {
	if (cave[ty][tx].fval == rock_wall1.ftval) {
	  cave[ty][tx].fval  = rock.ftval;
	  cave[ty][tx].fopen = rock.ftopen;
	  if (randint(treas_chance) == 1) {
	    place_gold(ty,tx);
	  }
	}
      }
    }
    if (!(move_dir(dir,&y,&x))) {
      flag = true;
    }
  } while (!flag);
};
//////////////////////////////////////////////////////////////////////
//insert gen.c
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void gc__tunnel(integer row1,integer col1,integer row2,integer col2,
		integer *doorptr, coords *doorstk)
{
  /*{ Constructs a tunnel between two points		}*/

  integer   tmp_row,tmp_col;
  integer   row_dir,col_dir;
  integer   i1,i2;
  coords    tunstk[1001];
  coords    wallstk[1001];
  integer   tunptr;
  integer   wallptr;
  boolean   stop_flag,door_flag;

  /*{ Note: 9 is a temporary value		}*/

  stop_flag = false;
  door_flag = false;
  tunptr    = 0;
  wallptr   = 0;
  gc__correct_dir(&row_dir,&col_dir,row1,col1,row2,col2);

  do {
    if (randint(100) > DUN_TUN_CHG) {
      gc__rand_dir(&row_dir,&col_dir,row1,col1,row2,col2,DUN_TUN_RND);
    }

    tmp_row = row1 + row_dir;
    tmp_col = col1 + col_dir;

    for (;!(in_bounds(tmp_row,tmp_col));) {
      gc__rand_dir(&row_dir,&col_dir,row1,col1,row2,col2,DUN_TUN_RND);
      tmp_row = row1 + row_dir;
      tmp_col = col1 + col_dir;
    }

    //with cave[tmp_row][tmp_col]. do;
    if (cave[tmp_row][tmp_col].fval == rock_wall1.ftval) {
      row1 = tmp_row;
      col1 = tmp_col;
      if (wallptr < 1000) {
	wallptr++;
      }
      wallstk[wallptr].y = row1;
      wallstk[wallptr].x = col1;
      for (i1 = row1-1; i1 <= row1+1; i1++) {
	for (i2 = col1-1; i2 <= col1+1; i2++) {
	  if (in_bounds(i1,i2)) {
	    //with cave[i1][i2]. do;
	    if (is_in(cave[i1][i2].fval, wall_set)) {
	      cave[i1][i2].fval = 9;
	    }
	  }
	}
      }

    } else if (cave[tmp_row][tmp_col].fval == corr_floor1.ftval) {
      row1 = tmp_row;
      col1 = tmp_col;
      if (!(door_flag)) {
	if (*doorptr <= 100) {
	  (*doorptr)++;
	  doorstk[*doorptr].y = row1;
	  doorstk[*doorptr].x = col1;
	}
	door_flag = true;
      }
      if (randint(100) > DUN_TUN_CON) {
	stop_flag = true;
      }

    } else if (cave[tmp_row][tmp_col].fval == 0) {
      row1 = tmp_row;
      col1 = tmp_col;
      if (tunptr < 1000) {
	tunptr++;
      }
      tunstk[tunptr].y = row1;
      tunstk[tunptr].x = col1;
      door_flag = false;

    } else if (cave[tmp_row][tmp_col].fval != 9) {
      row1 = tmp_row;
      col1 = tmp_col;
    }
  } while (! (((row1 == row2) && (col1 == col2)) || (stop_flag)));

  for (i1 = 1; i1 <= tunptr; i1++) {
    cave[tunstk[i1].y][tunstk[i1].x].fval  = corr_floor1.ftval;
    cave[tunstk[i1].y][tunstk[i1].x].fopen = corr_floor1.ftopen;
  }

  for (i1 = 1; i1 <= wallptr; i1++) {
    //with cave[wallstk[i1].y][wallstk[i1].x]. do;
    if (cave[wallstk[i1].y][wallstk[i1].x].fval == 9) {
      if (randint(100) < DUN_TUN_PEN) {
	place_door(wallstk[i1].y, wallstk[i1].x);
      } else {
	cave[wallstk[i1].y][wallstk[i1].x].fval  = corr_floor2.ftval;
	cave[wallstk[i1].y][wallstk[i1].x].fopen = corr_floor2.ftopen;
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
boolean gc__next_to(integer y, integer x)
{
  obj_set fourfivesix = {4, 5, 6, 0};
  boolean next_to = false;

  if (next_to8(y,x,fourfivesix) > 2) {
    if (is_in(cave[y-1][x].fval, wall_set) &&
	is_in(cave[y+1][x].fval, wall_set)) {
      next_to = true;
    } else if (is_in(cave[y][x-1].fval, wall_set) &&
	       is_in(cave[y][x+1].fval, wall_set)) {
      next_to = true;
    } else {
      next_to = false;
    }
  } else {
    next_to = false;
  }

  return next_to;
};
//////////////////////////////////////////////////////////////////////
void gc__try_door(integer y, integer x)
{
  /*{ Places door at y,x position if at least 2 walls found	}*/

  if (randint(100) > DUN_TUN_JCT) {
    if (cave[y][x].fval == corr_floor1.ftval) {
      if (gc__next_to(y,x)) {
	place_door(y,x);
      }
    }
  }

};
//////////////////////////////////////////////////////////////////////
void gc__place_pool(floor_type water)
{
/*{ Place a pool of water, and rough up the edges		-DMF-	}*/

  integer   y,x;

  y = (integer)(cur_height/2.0) + 11 - randint(23);
  x = (integer)(cur_width/2.0)  + 16 - randint(33);

  /* XXXX place_pool does nothing useful */

};
//////////////////////////////////////////////////////////////////////
void gc__all_the_river_stuff()
{
  all_the_river_stuff(); /* in river.c */
};
//////////////////////////////////////////////////////////////////////
void gc__cave_gen()
{
  /*{ Cave logic flow for generation of new dungeon		}*/
  /*
      type
	spot_type = record
		endx	: integer;
		endy	: integer;
	end;
	room_type = array [1..20,1..20] of boolean;
*/
  coords    doorstk[101];
  boolean   room_map[21][21];		/*: room_type;*/
  integer   i1,i2,i3,i4;
  integer   y1,x1,y2,x2;
  integer   pick1,pick2;
  integer   row_rooms,col_rooms;
  integer   doorptr = 0;
  integer   alloc_level;
  integer   roomCount;
  worlint   yloc[401];			/*: array [1..400] of worlint;*/
  worlint   xloc[401];			/*: array [1..400] of worlint;*/

  obj_set allocSet1 = {1,2,0};          /* land mosnters */
  obj_set allocSet2 = {16,17,18,0};     /* land mosnters */
  obj_set allocSet3 = {4,0};            /* treasure things */
  obj_set allocSet4 = {1,2,0};          /* treasure things */
  obj_set allocSet5 = {1,2,4,0};        /* treasure things */

  seed = get_seed();
  i3   = 0;

  row_rooms = 2*(integer)(cur_height/SCREEN_HEIGHT);
  col_rooms = 2*(integer)(cur_width /SCREEN_WIDTH);

  for (i1 = 1; i1 <= row_rooms; i1++) {
    for (i2 = 1; i2 <= col_rooms; i2++) {
      room_map[i1][i2] = false;
    }
  }

  if (1) {
    for (i1 = 1, roomCount = randnor(DUN_ROO_MEA,2); i1 <= roomCount; i1++) {
      room_map[randint(row_rooms)][randint(col_rooms)] = true;
    }
  } else {
    char_row = 17;
    char_col = 50;
    room_map[2][2] = true;
    room_map[2][3] = true;
  }

  for (i1 = 1; i1 <= row_rooms; i1++) {
    for (i2 = 1; i2 <= col_rooms; i2++) {
      if (room_map[i1][i2]) {
	i3++;
	yloc[i3] = (i1-1)*(quart_height*2 + 1) + quart_height + 1;
	xloc[i3] = (i2-1)*(quart_width*2  + 1) + quart_width  + 1;
	if (dun_level > randint(dun_unusual)) {
	  switch (randint(3)) {
	  case 1 : gc__build_type1(yloc[i3],xloc[i3]); break;
	  case 2 : gc__build_type2(yloc[i3],xloc[i3]); break;
	  case 3 : gc__build_type3(yloc[i3],xloc[i3]); break;
	  }
	} else {
	  gc__build_room(yloc[i3],xloc[i3]);
	}
      }
    }
  }

  for (i4 = 1; i4 <= i3; i4++) {
    pick1 = randint(i3);
    pick2 = randint(i3);
    y1 = yloc[pick1];
    x1 = xloc[pick1];
    yloc[pick1] = yloc[pick2];
    xloc[pick1] = xloc[pick2];
    yloc[pick2] = y1;
    xloc[pick2] = x1;
  }

  for (i4 = 1; i4 < i3; i4++) {
    y1 = yloc[i4];
    x1 = xloc[i4];
    y2 = yloc[i4+1];
    x2 = xloc[i4+1];
    gc__tunnel(y2,x2,y1,x1,&doorptr,doorstk);
  }

  gc__fill_cave(rock_wall1);

  for (i1 = 1; i1 <= DUN_STR_MAG; i1++) {
    gc__place_streamer(rock_wall2,dun_str_mc);
  }
  for (i1 = 1; i1 <= DUN_STR_QUA; i1++) {
    gc__place_streamer(rock_wall3,dun_str_qc);
  }

  gc__place_boundry();
  gc__all_the_river_stuff();

  for (i1 = 1; i1 <= DUN_POOLS; i1++) {
    gc__place_pool(water1);
  }

  /*{ Place intersection doors	}*/
  for (i1 = 1; i1 <= doorptr; i1++) {
    gc__try_door(doorstk[i1].y,   doorstk[i1].x-1);
    gc__try_door(doorstk[i1].y,   doorstk[i1].x+1);
    gc__try_door(doorstk[i1].y-1, doorstk[i1].x);
    gc__try_door(doorstk[i1].y+1, doorstk[i1].x);
  }

  alloc_level = trunc(dun_level/3);
  if (alloc_level < 2) {
    alloc_level = 2;
  } else if (alloc_level > 10) {
    alloc_level = 10;
  }

  place_stairs(up_staircase,randint(2),3);
  place_stairs(down_staircase,randint(2)+2,3);
  place_stairs(up_steep_staircase,1,3);
  place_stairs(down_steep_staircase,1,3);


  alloc_land_monster(allocSet1,
		     (randint(8)+MIN_MALLOC_LEVEL+alloc_level),0,true,false);
  alloc_land_monster(allocSet2,
		     (randint(8)+MIN_MALLOC_LEVEL+alloc_level)/3,0,true,true);

  alloc_object(allocSet3, 3, randint(alloc_level));
  alloc_object(allocSet4, 5, randnor(treas_room_alloc,3));
  alloc_object(allocSet5, 5, randnor(treas_any_alloc,3));
  alloc_object(allocSet5, 4, randnor(treas_gold_alloc,3));
  alloc_object(allocSet5, 1, randint(alloc_level));

  if (dun_level >= WIN_MON_APPEAR) {
    place_win_monster();
  }

  gc__place_boundry();                           /* just to make sure */
};
//////////////////////////////////////////////////////////////////////
void gc__make_door(integer y, integer x,
		    integer *cur_pos, integer store_num, integer house_type)
{
  void *old_seed;

  cave[y][x].fval  = corr_floor3.ftval;
  cave[y][x].fopen = corr_floor3.ftopen;
  popt(cur_pos);
  cave[y][x].tptr  = *cur_pos;

  if (store_num < TOT_STORES) {
    t_list[*cur_pos] = store_door[store_num];
  } else {
    t_list[*cur_pos] = store_door[TOT_STORES];
  }

  mini_sleep(5);

  old_seed = save_rand_state(NULL);
  set_seed(get_seed() ^ randint(9999999));
  if (store_num >= TOT_STORES) {
    //with t_list[cur_pos] do;
    switch (house_type) {
    case 1 : t_list[*cur_pos].p1 = 8 + randint(4);   break; /*  9..12  */
    case 2 : t_list[*cur_pos].p1 = randint(10);      break; /*  1..10  */
    case 3 : t_list[*cur_pos].p1 = 3 + randint(6);   break; /*  4..9   */
    case 4 : t_list[*cur_pos].p1 = randint(7);       break; /*  1..7   */
    case 5 : t_list[*cur_pos].p1 = 1;                break; /*  1      */
    default:    	                             break;
    }
  }
  restore_rand_state(old_seed);
};
//////////////////////////////////////////////////////////////////////
void dr_castle(integer yval,integer xval,integer dy,integer dx, floor_type ft)
{
  /*{ for castle--changes all in both lines of symmetry }*/

  dx = abs(dx);
  dy = abs(dy);

  do {
    dy = -dy;
    if (dy <= 0) {
      dx = -dx;
    }
    cave[yval+dy][xval+dx].fopen = ft.ftopen;
    cave[yval+dy][xval+dx].fval  = ft.ftval;
  } while (! ((dy >= 0) && (dx >= 0)));
};
//////////////////////////////////////////////////////////////////////
void gc__blank_square(integer dy, integer dx)
{
  cave[dy][dx].fopen = dopen_floor.ftopen;
  cave[dy][dx].fval  = dopen_floor.ftval;
};
//////////////////////////////////////////////////////////////////////
void gc__build_store(integer store_num,integer where)
{
  /*{ Builds a building at a row,column coordinate, and	}*/
  /*{ set up the initial contents by setting p1 to	}*/
  /*{ whatever inside type is desired			}*/

  integer        yval,y_height,y_depth;
  integer        xval,x_left,x_right;
  integer        i1,i2,q1,q2,cur_pos,house_type;
  
  yval = 10*(where div 9)+6;
  xval = 14*(where % 9)+11;
  
  if (store_num == S_FORTRESS) {
    house_type = 5;
    house_type = 1;
  } else if (store_num == S_BLACK_MARKET) {
    house_type = randint(2);
  } else if (store_num >= TOT_STORES) {
    house_type = store_num - TOT_STORES + 1;
  } else {
    house_type = 0;
  }
  
  if ((house_type == 1) || (house_type == 3)) {
    y_height = yval - 1 - randint(2);
    y_depth  = yval + 1 + randint(3);
    x_left   = xval - 1 - randint(4);
    x_right  = xval + 2 + randint(3);
  } else if (house_type == 2) {
    yval     = yval - 2 + randint(3);
    xval     = xval - 3 + randint(4);
    y_height = yval - randint(2);
    y_depth  = yval + randint(3);
    x_left   = xval - randint(3);
    x_right  = xval + randint(4);
  } else if (house_type == 5) {
    yval     = yval + 5;
    y_height = yval - 3;
    y_depth  = yval + 3;
    x_left   = xval - 5;
    x_right  = xval + 5;
  } else {
    y_height = yval - 3;
    y_depth  = yval + 3;
    x_left   = xval - 5;
    x_right  = xval + 5;
  }
  
  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[i1][i2].fval  = boundry_wall.ftval;
      cave[i1][i2].fopen = boundry_wall.ftopen;
    }
  }
  
  if (house_type == 4) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[yval][i2].fval  = dopen_floor.ftval;
      cave[yval][i2].fopen = dopen_floor.ftopen;
    }
  }
  
  if (house_type == 5) {
    dr_castle(yval,xval,0,5,water1);
    dr_castle(yval,xval,1,5,water1);
    dr_castle(yval,xval,1,6,water1);
    dr_castle(yval,xval,2,6,water1);
    dr_castle(yval,xval,2,7,water1);
    dr_castle(yval,xval,3,7,water1);
    dr_castle(yval,xval,4,7,water1);
    dr_castle(yval,xval,4,6,water1);
    dr_castle(yval,xval,5,6,water1);
    dr_castle(yval,xval,5,5,water1);
    dr_castle(yval,xval,5,4,water1);
    dr_castle(yval,xval,5,3,water1);
    dr_castle(yval,xval,4,3,water1);
    dr_castle(yval,xval,4,2,water1);
    dr_castle(yval,xval,3,2,water1);
    dr_castle(yval,xval,3,1,water1);
    dr_castle(yval,xval,3,0,water1);
    dr_castle(yval,xval,3,6,boundry_wall);
    dr_castle(yval,xval,4,5,boundry_wall);
    dr_castle(yval,xval,4,4,boundry_wall);
  }
  
  if (house_type == 3) {
    gc__make_door(y_height,xval-2+randint(4),&cur_pos,store_num,house_type);
    gc__make_door(y_depth,xval-2+randint(4),&cur_pos,store_num,house_type);
    gc__make_door(yval-2+randint(3),x_left,&cur_pos,store_num,house_type);
    gc__make_door(yval-2+randint(3),x_right,&cur_pos,store_num,house_type);
  } else if (house_type == 4) {
    gc__make_door(yval-1,xval-4,&cur_pos,store_num,house_type);
    gc__make_door(yval+1,xval-4,&cur_pos,store_num,house_type);
    gc__make_door(yval-1,xval+4,&cur_pos,store_num,house_type);
    gc__make_door(yval+1,xval+4,&cur_pos,store_num,house_type);
    gc__make_door(yval-1,xval,&cur_pos,store_num,house_type);
    gc__make_door(yval+1,xval,&cur_pos,store_num,house_type);
  } else if (house_type == 5) {
    i1 = 2*randint(2)-3;
    gc__make_door(yval+2*i1,xval-1,&cur_pos,store_num,house_type);
    gc__make_door(yval+2*i1,xval,&cur_pos,store_num,house_type);
    gc__make_door(yval+2*i1,xval+1,&cur_pos,store_num,house_type);
    gc__blank_square(yval+3*i1,xval-1);
    gc__blank_square(yval+3*i1,xval+0);
    gc__blank_square(yval+3*i1,xval+1);
  } else {
    switch (randint(4)) {
    case 1 :
      i1 = randint(y_depth-y_height) + y_height - 1;
      i2 = x_left;
      q1 = randint(y_depth-y_height) + y_height - 1;
      q2 = x_right;
      break;
    case 2 :
      i1 = randint(y_depth-y_height) + y_height - 1;
      i2 = x_right;
      q1 = randint(y_depth-y_height) + y_height - 1;
      q2 = x_left;
      break;
    case 3 :
      i1 = y_depth;
      i2 = randint(x_right-x_left) + x_left - 1;
      q1 = y_height;
      q2 = randint(x_right-x_left) + x_left - 1;
      break;
    case 4 :
      i1 = y_height;
      i2 = randint(x_right-x_left) + x_left - 1;
      q1 = y_depth;
      q2 = randint(x_right-x_left) + x_left - 1;
      break;
    default:
      MSG("randint returned something other than 1-4 in gc__build_store");
      i1=i2=q1=q2=0;
      break;
    }
    gc__make_door(i1,i2,&cur_pos,store_num,house_type);
    if (store_num == S_BANK) {
      gc__make_door(q1,q2,&cur_pos,S_INSURANCE,house_type);
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__build_house(integer house_num, integer where)
{
  gc__build_store(house_num+TOT_STORES-1,where);
};
//////////////////////////////////////////////////////////////////////
void gc__build_fountain (integer where)
{
  /*{ Build a fountain at row, column coordinate	-dmf-	}*/
  
  integer        yval,y_height,y_depth;
  integer        xval,x_left,x_right;
  integer        i1,i2;
  integer        count;
  integer        flr[36];	/*: array [1..35] of integer;*/
  void          *old_seed;
  
  yval = (10*(int)(where div 9))+4+randint(3);
  xval = 14*(where % 9)+9+randint(3);
  
  for (i1 = 1; i1 <= 35; i1++) {
    flr[i1] = 2;
  }
  flr[1] = 1;
  flr[7] = 1;
  for (i1 = 10; i1 <= 12; i1++) {
    flr[i1] = 3;
  }
  for (i1 = 16; i1 <= 17; i1++) {
    flr[i1] = 3;
  }
  for (i1 = 19; i1 <= 20; i1++) {
    flr[i1] = 3;
  }
  for (i1 = 24; i1 <= 26; i1++) {
    flr[i1] = 3;
  }
  flr[29] = 1;
  flr[35] = 1;
  
  y_height = yval - 2;
  y_depth  = yval + 2;
  x_left   = xval - 3;
  x_right  = xval + 3;

  count = 0;
  do {
    i1 = randint(35);
  } while (!(flr[i1] == 2) && (i1 != 18));

  flr[i1] = 4;
  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      count++;
      switch(flr[count]) {
      case 1 :
	cave[i1][i2].fval  = dopen_floor.ftval;
	cave[i1][i2].fopen = dopen_floor.ftopen;
	break;
      case 2 :
	cave[i1][i2].fval  = boundry_wall.ftval;
	cave[i1][i2].fopen = boundry_wall.ftopen;
	break;
      case 3 :
	cave[i1][i2].fval  = water1.ftval;
	cave[i1][i2].fopen = water1.ftopen;
	mini_sleep(5);
	old_seed = save_rand_state(NULL);
	set_seed(get_seed() ^ randint(9999999));
	if (randint(12) == 1) {
	  place_gold(i1,i2);
	}
	restore_rand_state(old_seed);
	break;
      case 4 :
	cave[i1][i2].fval  = rock_wall2.ftval;
	cave[i1][i2].fopen = rock_wall2.ftopen;
	break;
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void gc__mixem(integer rooms[], integer num)
{
  /*{ randomize array[0..num-1] }*/

  integer   i1,i2,i3;

  for (i1 = 0; i1 < num; i1++) {
    i2 = i1-1+randint(num-i1);
    i3 = rooms[i1];
    rooms[i1] = rooms[i2];
    rooms[i2] = i3;
  }
};
//////////////////////////////////////////////////////////////////////
void gc__town_gen()
{
  integer   i1,i2,i3;
  integer   rooms[36];    /* array [0..35] of integer;*/
  boolean   roomdone[36]; /* array [0..35] of boolean;*/
  integer   center;

  obj_set allocSet1 = {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  obj_set allocSet2 = {16,17,18,0,0,0,0,0,0,0,0,0,0,0,0,0};

//  for (i1 = 0; i1 < MAX_MONS_LEVEL+1; i1++) {
//    printf ("\n m_level[%d] : %d",i1,m_level[i1]);  fflush(stdout);
//  }

  set_seed(town_seed);
  for (i1 = 0; i1 < 36; i1++) {
    roomdone[i1] = false;
  }

  center = 10 + randint(5);
  i3 = 0;
  for (i1 = -2; i1 <= 2; i1++) {
    for (i2 = -1; i2 <= 2; i2++) {
      if (((i1 < 2) && (i1 > -2)) || ((i2 > -1) && (i2 < 2))) {
	roomdone[center+i1+i2*9] = true;
	if ((i1!=0) || (i2==-1) || (i2==2)) {	/*{not castle}*/
	  rooms[i3] = center+i1+i2*9;
	  i3++;
	}
      }
    }
  }

  gc__mixem(rooms, i3);
  gc__build_store(S_TEMPLE,rooms[0]);   /*  3 */
  gc__build_store(S_ALCHEMY,rooms[1]);  /*  4 */
  gc__build_store(S_MAGIC,rooms[2]);    /*  5 */
  gc__build_store(S_LIBRARY,rooms[3]);  /*  7 */
  gc__build_store(S_MUSIC,rooms[4]);    /*  8 */
  gc__build_store(S_GEM,rooms[5]);      /*  9 */
  gc__build_store(S_BANK,rooms[6]);     /* 14 */
  gc__build_store(S_FORTRESS,rooms[7]); /* 17 */
  gc__build_fountain(rooms[8]);
  gc__build_fountain(rooms[9]);
  gc__build_store(S_BLACK_MARKET,rooms[10]); /* 11 */
  
  for (i1 = 2; i1 <= MAX_HOUSE1; i1++) {
    gc__build_house(1,rooms[9+i1]);
  }
  
  i3 = 0;
  for (i1 = 0; i1 < 36; i1++) {
    if (!roomdone[i1]) {
      rooms[i3] = i1;
      i3++;
    }
  }
  
  gc__mixem(rooms, i3);
  gc__build_store(S_GENERAL,rooms[0]);    /*  0 */
  gc__build_store(S_ARMORY,rooms[1]);     /*  1 */
  gc__build_store(S_WEAPONS,rooms[2]);    /*  2 */
  gc__build_store(S_INN,rooms[3]);        /*  6 */
  gc__build_store(S_TRADE_POST,rooms[4]); /* 12 */
  gc__build_store(S_CASINO,rooms[5]);     /* 16 */
  gc__build_store(S_DELI,rooms[6]);       /* 10 */
  gc__build_fountain(rooms[8]);
  gc__build_fountain(rooms[9]);
  gc__build_house(4,rooms[10]);
  
  for (i1 = 1; i1 <= MAX_HOUSE2; i1++) {
    gc__build_house(2,rooms[10+i1]);
  }
  
  gc__fill_cave(dopen_floor);
  
  do {
    i1 = randint(4);
    i2 = randint(4);
  } while (i1 == i2);
  
  gc__place_boundry();
  
  if ((py.misc.cur_age.hour > 17) || (py.misc.cur_age.hour < 6)) {
    /*{ Night	}*/
    
    mugging_chance = NIGHT_MUGGING;
    
    for (i1 = 1; i1 <= cur_height; i1++) {
      for (i2 = 1; i2 <= cur_width; i2++) {
	if (cave[i1][i2].fval != dopen_floor.ftval) {
	  cave[i1][i2].pl = true;
	}
      }
    }
    
    place_stairs(down_staircase,2,0);
    place_stairs(down_steep_staircase,1,0);
    set_seed(get_seed());

    alloc_land_monster(allocSet1,MIN_MALLOC_TN,3,true,false);
    alloc_land_monster(allocSet2,7,0,true,true);
    store_maint();
    
  } else {
    /*{ Day	}*/
    
    mugging_chance = DAY_MUGGING;
    for (i1 = 1; i1 <= cur_height; i1++) {
      for (i2 = 1; i2 <= cur_width; i2++) {
	cave[i1][i2].pl = true;
      }
    }
    
    place_stairs(down_staircase,2,0);
    place_stairs(down_steep_staircase,1,0);
    set_seed(get_seed());

    alloc_land_monster(allocSet1,MIN_MALLOC_TD,3,true,false);
    alloc_land_monster(allocSet2,7,0,true,true);
    store_maint();
  }

  gc__place_boundry();                     /* just to make sure */
};
//////////////////////////////////////////////////////////////////////
void generate_cave()
{
  /*{ Generates a random dungeon level			-RAK-	}*/


  panel_row_min	    =  0;
  panel_row_max	    =  0;
  panel_col_min	    =  0;
  panel_col_max	    =  0;
  char_row	    = -1;
  char_col	    = -1;


  tlink();
  mlink();

  gc__blank_cave();

//  for (i1 = 0; i1 < MAX_MONS_LEVEL+1; i1++) {
//    printf ("\n m_level[%d] : %d",i1,m_level[i1]);  fflush(stdout);
//  }

  if (dun_level == 0) {
    cur_height      =  SCREEN_HEIGHT * 2;
    cur_width       =  SCREEN_WIDTH * 2;
    max_panel_rows  =  trunc(cur_height/SCREEN_HEIGHT)*2 - 2;
    max_panel_cols  =  trunc(cur_width /SCREEN_WIDTH )*2 - 2;
    panel_row       =  0;
    panel_col       =  0;
    gc__town_gen();
  } else {
    cur_height      =  MAX_HEIGHT;
    cur_width       =  MAX_WIDTH;
    max_panel_rows  =  trunc(cur_height/SCREEN_HEIGHT)*2 - 2;
    max_panel_cols  =  trunc(cur_width /SCREEN_WIDTH )*2 - 2;
    panel_row       =  max_panel_rows;
    panel_col       =  max_panel_cols;
    gc__cave_gen();
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  generate.c */
