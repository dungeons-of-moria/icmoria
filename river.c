/* river.c */
/* this all used to be in generate.pas */

#include "imoria.h"
#include "generate.h"


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean r__move_this(river_args *a, integer dir, coords *this, coords *that)
{
  /*{ returns position of (this + dir) in gup or this if out of bounds }*/

  boolean  return_value = false;

  ENTER("r__move_this", "r");

  that->y = this->y + dy_of[dir];
  that->x = this->x + dx_of[dir];

  if ((that->y >= 1) && (that->y <= RIVER_SIZE_Y) &&
      (that->x >= 1) && (that->x <= RIVER_SIZE_X)) {
    return_value = true;
  } else {
    /*{reset to legal value}*/
    *that = *this;
  }

  RETURN("r__move_this", "r",'b',"moved",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void r__remove_this(river_args *a, coords *this)
{
  /*{make gup[this] unavailable (for later selection), decrement Num_left}*/

  s_l_type last;

  ENTER("r__remove_this", "r");

  //with Gup[this->y][this->x]. do;
  if (Gup[this->y][this->x].pos <= Num_left) {
    /*{if gup[this].pos is still available}*/
    last = S_list[Num_left];  /*{switch gup[this].pos with top elmt}*/
    S_list[Num_left] = S_list[Gup[this->y][this->x].pos];
    S_list[Gup[this->y][this->x].pos] = last;

    Gup[last.loc.y][last.loc.x].pos = Gup[this->y][this->x].pos;
    Gup[this->y][this->x].pos = Num_left;
    Num_left--;  /*{pop gup[this].pos}*/
  }

  LEAVE("r__remove_this", "r");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void r__plot_water(integer y, integer x, integer font, integer tdir)
{
  integer num_dots;
  coords  dots[6];		/*: array [1..5] of coords;*/
  integer i1;

  ENTER("r__plot_water", "r");

  dots[1].y = y;
  dots[1].x = x;

  switch (font) {
  case 0 : 
    num_dots = 1;
    break;

  case 1 : 
    num_dots = 2;
    dots[2].y = y + dx_of[tdir];
    dots[2].x = x - dy_of[tdir];
    break;

  default :
    num_dots = 5;
    for (i1 = 1; i1 <= 4; i1++) {
      dots[i1+1].y = y + dy_of[2*i1];
      dots[i1+1].x = x + dx_of[2*i1];
    }
    break;
  } /* end switch font */

  for (i1 = 1; i1 <= num_dots; i1++) {
    if (in_bounds(dots[i1].y,dots[i1].x)) {
      //with cave[dots[i1].y][dots[i1].x]. do;
      if ((cave[dots[i1].y][dots[i1].x].fval == dopen_floor.ftval) ||
	  (cave[dots[i1].y][dots[i1].x].fval == lopen_floor.ftval)) {
	cave[dots[i1].y][dots[i1].x].fval  = water2.ftval;
	cave[dots[i1].y][dots[i1].x].fopen = water2.ftopen;
      } else {
	cave[dots[i1].y][dots[i1].x].fval  = water1.ftval;
	cave[dots[i1].y][dots[i1].x].fopen = water1.ftopen;
      }

      cave[dots[i1].y][dots[i1].x].h2o = 1;

      if ((cave[dots[i1].y][dots[i1].x].tptr != 0) &&
	  (t_list[cave[dots[i1].y][dots[i1].x].tptr].tval > valuable_metal)) {
	pusht(cave[dots[i1].y][dots[i1].x].tptr);
	cave[dots[i1].y][dots[i1].x].tptr = 0;
      }

    }
  }

  LEAVE("r__plot_water", "r");
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer pr__figure_out_path_of_water(integer y, integer x,
				     integer oy, integer ox)
{
  integer    target_dy,target_dx,dist_squared;
  integer    i1,dot_product,rand_num,chance;
  integer    start[9];	/*: array [0..8] of integer;*/
  boolean    flag;
  obj_set    odds = {1, 3, 5, 7, 0};
  integer    return_value;

  ENTER("pr__figure_out_path_of_water", "r");

  target_dy    = y - oy;
  target_dx    = x - ox;
  dist_squared = target_dy * target_dy + target_dx * target_dx;
  start[0]     = 1;

  for (i1 = 0; i1 <= 7; i1++) {	/*{octant number}*/
    dot_product = target_dy*dy_of[key_of[i1]] + target_dx*dx_of[key_of[i1]];
    
    /*{formula subtracts dist_squared to keep stream semi-normal}*/
    /*{diagonals give root2 inflated dot_products}*/
    
    if (dot_product > 0) {
      /*if (i1 in [1,3,5,7]) {*/
      if (is_in(i1, odds)) {
	chance = dot_product * dot_product * 2 - dist_squared;
      } else {
	chance = dot_product * dot_product * 4 - dist_squared;
      }
    } else {
      chance = 0;
    }
    
    if (chance > 0) {
      start[i1+1] = start[i1] + chance;
    } else {
      start[i1+1] = start[i1];
    }
  } /* end for i1 */
  
  /*{choose random directions; chances partitioned by start[]}*/
  rand_num = randint(start[8] - 1);
  
  flag = false;
  i1 = -1;
  do {
    i1++;
    flag = (start[i1 + 1] > rand_num);
  } while (!flag);
  
  return_value = key_of[i1];
  
  RETURN("pr__figure_out_path_of_water", "r",'d',"key_of[i1]",&return_value);
  return return_value;
}
//////////////////////////////////////////////////////////////////////
void r__place_river(river_args *a, integer dir, integer next_dir,
		 coords this, coords wiggle)
{

  /*{ A recursive procedure, starting at river mouth and moving upstream; 
    connects the dots laid out by chart_river. }*/

  integer   i1,i2,y,x,oy,ox;
  integer   temp_dir,done_first; /*{ compute next direction }*/
  coords    up1,up2;
  integer   tflow;
  integer   overflow;
  obj_set   even_nums = {2, 4, 6, 8, 0};

  ENTER("r__place_river","r");

  r__move_this(a,dir,&this,&up1);     /*{up1 is upstream end of segment}*/
  r__move_this(a,next_dir,&up1,&up2); /*{up2 is upstream end of next segment}*/

  tflow = (Gup[up2.y][up2.x].flow - 1) div 2;     /*{river size}*/

  /*{aim (y,x) toward upstream end of segment, randomize slightly}*/
  oy = RIVER_SEGMENT_SIZE * this.y + wiggle.y;
  ox = RIVER_SEGMENT_SIZE * this.x + wiggle.x;
  
  if (dir != next_dir) {
    i1 = oct_of[next_dir] - oct_of[dir]; /*{ (1=left, -1 = right) mod 8}*/
    if ((oct_of[dir] % 2) == 0) {
      i2 = rotate_dir(next_dir,i1);
    } else {
      i2 = rotate_dir(next_dir,2*i1);
    }
    wiggle.y = dy_of[i2] + (randint(3) - 2);
    wiggle.x = dx_of[i2] + (randint(3) - 2);
  }

  i1 = 0;

  /*{y,x=(upstream) destination of river}*/
  y = RIVER_SEGMENT_SIZE*up1.y+wiggle.y; 
  x = RIVER_SEGMENT_SIZE*up1.x+wiggle.x;

  overflow = 0;
  for ( ; ((oy != y) || (ox != x)) && (overflow++ < 5000); ) {
    temp_dir = pr__figure_out_path_of_water(y, x, oy, ox);
    /*if ((temp_dir) in [2,4,6,8]) {*/
    if (is_in(temp_dir, even_nums)) {
      move_dir(temp_dir,&oy,&ox);
      r__plot_water(oy,ox,tflow,temp_dir);
    } else {
      if (randint(2) == 1) {
	done_first = 1;
      } else {
	done_first = -1;
      }
      move_dir(rotate_dir(temp_dir,done_first),&oy,&ox);
      r__plot_water(oy,ox,tflow,temp_dir);
      move_dir(rotate_dir(temp_dir,-done_first),&oy,&ox);
      r__plot_water(oy,ox,tflow,temp_dir);
    }
  }
  
  /*{branch rivers 1 move early to make branching more gradual}*/
  //with Gup[up2.y][up2.x]. do;
  if (Gup[up2.y][up2.x].in1 != 5) {
    r__place_river(a,next_dir,Gup[up2.y][up2.x].in1,up1,wiggle);
  }
  if (Gup[up2.y][up2.x].in2 != 5) {
    r__place_river(a,next_dir,Gup[up2.y][up2.x].in2,up1,wiggle);
  }
  
  LEAVE("r__place_river","r");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
integer cr__choose_stream_dirs(river_args *a, coords *this, integer dir,
			       integer that_dir[], boolean that_ok[],
			       coords that[], boolean that_chosen[])
{
  /*{determines next point(s) upstream depending on coordinates (this),
    previous direction (Gup[this].out), and available positions. outputs
    # of branches}*/

  integer    i1;
  boolean    done;
  integer    return_value = 0;

  ENTER("cr__choose_stream_dirs", "r");

  *this = S_list[S_l_top].loc;
  dir = Gup[this->y][this->x].out;

  for (i1 = 1; i1 <= 3; i1++) {       /*{left,straight,right}*/
    that_dir[i1] = rotate_dir(dir,2-i1);
    that_ok[i1]  = r__move_this(a,that_dir[i1],this,&(that[i1]));
    if (that_ok[i1]) {
      that_ok[i1] = Gup[that[i1].y][that[i1].x].pos <= Num_left;
    }
    that_chosen[i1] = false;
  }

  done = false;

  if ((randint(3*Gup[this->y][this->x].flow) == 1) ||
      ! (that_ok[1] || that_ok[2] || that_ok[3])) {
    /*{end stream if blocked or small river and random}*/
    done = true;
    return_value = 0;
  } else if (((randint(5) == 1) || ! (that_ok[1] || that_ok[3]))
	     && that_ok[2]) {
    /*{straight stream (1/5 and ok) or sides blocked}*/
    done = true;
    that_chosen[2] = true;
    return_value = 1;
  } else if ((randint(5) == 1) && (that_ok[1] && that_ok[3])) {
    /*{fork 1/5 and both sides ok}*/
    done = true;
    that_chosen[1] = true;
    that_chosen[3] = true;
    return_value = 2;
  }

  if (!done) {        /*{ 1 or 3 must be open } */
    /*{check 1 side first; if it fails, second must be true}*/
    i1 = 2*randint(2) - 1;
    that_chosen[i1] = that_ok[i1];
    that_chosen[4-i1] = !that_chosen[i1];
    return_value = 1;
  }
  /*{no rivers adjacent each other (except connected segments)}*/

  RETURN("cr__choose_stream_dirs", "r",'d',"branches",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
boolean cr__dequeue_s_list(river_args *a)
{
  /*{get highest unresolved river segment; S_l_top points to new segment
    if any is found. }*/

  boolean  return_value;

  ENTER("cr__dequeue_s_list", "r");

  for( ; ((S_l_top > Num_left) && (! S_list[S_l_top].is_active)) ; ) {
    S_l_top--;
  }

  if (S_l_top > Num_left) {
    S_list[S_l_top].is_active = false;
    return_value = true;
  } else {
    return_value = false;
  }

  RETURN("cr__dequeue_s_list", "r",'b',"some left",&return_value);
  return return_value;
};
//////////////////////////////////////////////////////////////////////
void r__chart_river(river_args *a)
{
  /*{ recursively charts basic path of stream upstream }*/

  integer    i1,i2,dir,branches;
  integer    out_flow,in_flow;
  coords     this,thing;
  coords     that[4];       /*     : array [1..3] of coords;*/
  integer    that_dir[4];   /*     : array [1..3] of integer;*/
  boolean    that_ok[4];    /*     : array [1..3] of boolean;*/
  boolean    that_chosen[4];
  boolean    starting_river = true;

  ENTER("r__chart_river", "r");

  r__remove_this(a,&(S_list[randint(Num_left)].loc)); 
  /*{element is now S_l_top}*/

  S_list[S_l_top].is_active = true;
  this = S_list[S_l_top].loc;
  Gup[this.y][this.x].flow = 4+randint(3);
  River_mouth = this;

  for (i1 = 1; i1 <= 3; i1++) {
    that_chosen[i1] = false;
  }
  i1 = 0;

  do {              /*{ choose initial heading, in streams }*/
    dir = randint(8);
    if (dir == 5) {
      dir = 9;
    }
    i1++;
    if (r__move_this(a,dir,&this,&(that[2]))) {
      that_chosen[2] = Gup[that[2].y][that[2].x].pos <= Num_left;
    }
  } while (!((that_chosen[2]) || (i1 >= 10)));


  that_dir[2] = dir;
  that_ok[2] = true;
  branches = 1;

  for ( ; cr__dequeue_s_list(a) ; ) {     /*{loop until river stops}*/
    if (starting_river) {
      starting_river = false;
    } else {
      branches = cr__choose_stream_dirs(a, &this, dir, that_dir, that_ok,
					that, that_chosen);
    }
    
    for (i1 = 1; i1 <= 9; i1++) { 
      if (r__move_this(a,i1,&this,&thing)) {
	r__remove_this(a, &thing);
      }
    }
    
    if (that_chosen[1]) { /*{ No sharp left turns }*/
      r__move_this(a,rotate_dir(dir,1),&this,&thing);
      if (r__move_this(a,rotate_dir(dir,2),&thing,&thing)) {
	r__remove_this(a, &thing);
      }
    }
    
    if (that_chosen[3]) {  /*{ No sharp right turns }*/
      r__move_this(a,rotate_dir(dir,-1),&this,&thing);
      if (r__move_this(a,rotate_dir(dir,-2),&thing,&thing)) {
	r__remove_this(a, &thing);
      }
    }
    
    out_flow = Gup[this.y][this.x].flow;
    i2 = 1;
    
    for (i1 = 1; i1 <= 3; i1++) {
      if (that_chosen[i1] && (RIVER_TOTAL_SIZE-Num_left<Max_wet)) {
	if (branches == 1) {
	  in_flow = out_flow;
	} else {
	  in_flow = out_flow - randint(2);
	}
	
	if (in_flow > 0) {
	  //with Gup[that[i1].y][that[i1].x]. do;
	  if (i2 == 1) {
	    Gup[this.y][this.x].in1 = that_dir[i1];
	  } else {
	    Gup[this.y][this.x].in2 = that_dir[i1];
	  }
	  S_list[Gup[that[i1].y][that[i1].x].pos].is_active = true;
	  Gup[that[i1].y][that[i1].x].out = that_dir[i1];
	  Gup[that[i1].y][that[i1].x].flow = in_flow;
	}
	i2++;
      }
    } /* end for i1 */
  } /* end for dequeue_s_list */

  LEAVE("r__chart_river", "r");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void r__draw_river(river_args *a)
{
  integer    first_dir;
  coords     wiggle,that;

  ENTER("r__draw_river", "r");

  wiggle.y = randint(3) - 2;
  wiggle.x = randint(3) - 2;

  /*{XXX place whirlpool at RIVER_SEGMENT_SIZE*river + wiggle}*/

  first_dir = Gup[River_mouth.y][River_mouth.x].in1;
  r__move_this(a,first_dir,&(River_mouth),&that);

  //with Gup[that.y][that.x]. do;
  if (Gup[that.y][that.x].in1 != 5) {
    r__place_river(a,first_dir,Gup[that.y][that.x].in1,River_mouth,wiggle);
  }
  if (Gup[that.y][that.x].in2 != 5) {
    r__place_river(a,first_dir,Gup[that.y][that.x].in2,River_mouth,wiggle);
  }

  LEAVE("r__draw_river", "r");
};
//////////////////////////////////////////////////////////////////////
void all_the_river_stuff()
{
  river_args     a;
  integer        i1,i2;

  ENTER("all_the_river_stuff", "r");

  a.max_wet = randint(RIVER_TOTAL_SIZE) - 50;

  if (a.max_wet < 0) {
    a.max_wet = 0;
  }
  a.num_left = 0;

  for (i1 = 1; i1 <= RIVER_SIZE_Y; i1++) {
    for (i2 = 1; i2 <= RIVER_SIZE_X; i2++) {
      a.num_left++;

      //with gup[i1][i2]. do;
      a.gup[i1][i2].in1  = 5;
      a.gup[i1][i2].in2  = 5;
      a.gup[i1][i2].out  = 5;
      a.gup[i1][i2].flow = 0;
      a.gup[i1][i2].pos  = a.num_left;

      //with s_list[num_left]. do;
      a.s_list[a.num_left].loc.y     = i1;
      a.s_list[a.num_left].loc.x     = i2;
      a.s_list[a.num_left].is_active = false;
    }
  }

  /*{remove borders of map}*/
  for (i1 = 1; i1 <= a.num_left; i1++) {  
    //with s_list[i1]. do;
    if ((a.s_list[i1].loc.y == 1) || (a.s_list[i1].loc.y == RIVER_SIZE_Y) || 
	(a.s_list[i1].loc.x == 1) || (a.s_list[i1].loc.x == RIVER_SIZE_X)) {
      r__remove_this(&a, &(a.s_list[i1].loc));
    }
  }

  a.s_l_top = a.num_left;
  
  for ( ; (RIVER_TOTAL_SIZE - a.num_left < a.max_wet) ; ) {
    r__chart_river(&a);
    r__draw_river(&a);
  }

  LEAVE("all_the_river_stuff", "r");

}; /* end all_the_river_stuff */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* end river.c */
