/* rooms.c */
/* this all used to be in generate.pas */

#include "imoria.h"
#include "generate.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void gc__vault_trap(integer y,integer x,integer yd,integer xd, integer num)
{
  /*{ Place a trap with a given displacement of point	-RAK-	}*/

  integer    count,y1,x1,i1;
  boolean    flag;

  for (i1 = 1; i1 <= num; i1++) {
    flag = false;
    count = 0;
    
    do {
      y1 = y - yd - 1 + randint(2*yd+1);
      x1 = x - xd - 1 + randint(2*xd+1);
      //with cave[y1][x1]. do;
      if (is_in(cave[y1][x1].fval, floor_set)) {
	if (cave[y1][x1].tptr == 0) {
	  place_trap(y1,x1,1,randint(MAX_TRAPA));
	  flag = true;
	}
      }
      count++;
    } while (!((flag) || (count > 5)));
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__vault_monster(integer y,integer x,integer num)
{
  /*{ Place a trap with a given displacement of point	-RAK-	}*/
  integer    i1,y1,x1;

  for (i1 = 1; i1 <= num; i1++) {
    y1 = y;
    x1 = x;
    summon_land_monster(&y1,&x1,true);
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__build_room(integer yval, integer xval)
{
  /*{ Builds a room at a row,column coordinate		-RAK-	}*/

  integer     y_height,y_depth;
  integer     x_left,x_right;
  integer     i1,i2;
  floor_type  cur_floor;

  if (dun_level <= randint(25)) {
    cur_floor = lopen_floor;	/*{ Floor with light	}*/
  } else {
    cur_floor = dopen_floor;	/*{ Dark floor		}*/
  }

  y_height = yval - randint(4);
  y_depth  = yval + randint(3);
  x_left   = xval - randint(11);
  x_right  = xval + randint(11);

  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[i1][i2].fval  = cur_floor.ftval;
      cave[i1][i2].fopen = cur_floor.ftopen;
    }
  }

  for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
    cave[i1][x_left-1].fval   = rock_wall1.ftval;
    cave[i1][x_left-1].fopen  = rock_wall1.ftopen;
    cave[i1][x_right+1].fval  = rock_wall1.ftval;
    cave[i1][x_right+1].fopen = rock_wall1.ftopen;
  }

  for (i1 = x_left; i1 <= x_right; i1++) {
    cave[y_height-1][i1].fval  = rock_wall1.ftval;
    cave[y_height-1][i1].fopen = rock_wall1.ftopen;
    cave[y_depth+1][i1].fval   = rock_wall1.ftval;
    cave[y_depth+1][i1].fopen  = rock_wall1.ftopen;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__build_type1(integer yval, integer xval)
{
  /*	{ Builds a room at a row,column coordinate		-RAK-	}*/
  /*	{ Type 1 unusual rooms are several overlapping rectangular ones	}*/

  integer     y_height,y_depth;
  integer     x_left,x_right;
  integer     i0,i1,i2;
  floor_type  cur_floor;

  if (dun_level <= randint(25)) {
    cur_floor = lopen_floor;	/*{ Floor with light	}*/
  } else {
    cur_floor = dopen_floor;	/*{ Dark floor		}*/
  }

  for (i0 = 1; i0 <= (1 + randint(2)); i0++) {
    y_height = yval - randint(4);
    y_depth  = yval + randint(3);
    x_left   = xval - randint(11);
    x_right  = xval + randint(11);

    for (i1 = y_height; i1 <= y_depth; i1++) {
      for (i2 = x_left; i2 <= x_right; i2++) {
	cave[i1][i2].fval  = cur_floor.ftval;
	cave[i1][i2].fopen = cur_floor.ftopen;
      }
    }
    
    for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
      
      //with cave[i1][x_left-1]. do;
      if (cave[i1][x_left-1].fval != cur_floor.ftval) {
	cave[i1][x_left-1].fval  = rock_wall1.ftval;
	cave[i1][x_left-1].fopen = rock_wall1.ftopen;
      }
      
      //with cave[i1][x_right+1]. do;
      if (cave[i1][x_right+1].fval != cur_floor.ftval) {
	cave[i1][x_right+1].fval  = rock_wall1.ftval;
	cave[i1][x_right+1].fopen = rock_wall1.ftopen;
      }
    }
    
    for (i1 = x_left; i1 <= x_right; i1++) {
      
      //with cave[y_height-1][i1]. do;
      if (cave[y_height-1][i1].fval != cur_floor.ftval) {
	cave[y_height-1][i1].fval  = rock_wall1.ftval;
	cave[y_height-1][i1].fopen = rock_wall1.ftopen;
      }
      
      //with cave[y_depth+1][i1]. do;
      if (cave[y_depth+1][i1].fval != cur_floor.ftval) {
	cave[y_depth+1][i1].fval  = rock_wall1.ftval;
	cave[y_depth+1][i1].fopen = rock_wall1.ftopen;
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__build_type2(integer yval, integer xval)
{
  /*	{ Builds an unusual room at a row,column coordinate	-RAK-	}*/
  /*	{ Type 2 unusual rooms all have an inner room:			}*/
  /*	{   1 - Just an inner room with one door			}*/
  /*	{   2 - An inner room within an inner room			}*/
  /*	{   3 - An inner room with pillar(s)				}*/
  /*	{   4 - Inner room has a maze					}*/
  /*	{   5 - A set of four inner rooms				}*/

  integer      y_height,y_depth;
  integer      x_left,x_right;
  integer      i1,i2;
  floor_type   cur_floor;

  if (dun_level <= randint(30)) {
    cur_floor = lopen_floor;	/*{ Floor with light	}*/
  } else {
    cur_floor = dopen_floor;	/*{ Dark floor		}*/
  }

  y_height = yval - 4;
  y_depth  = yval + 4;
  x_left   = xval - 11;
  x_right  = xval + 11;

  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[i1][i2].fval  = cur_floor.ftval;
      cave[i1][i2].fopen = cur_floor.ftopen;
    }
  }

  for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
    cave[i1][x_left-1].fval   = rock_wall1.ftval;
    cave[i1][x_left-1].fopen  = rock_wall1.ftopen;
    cave[i1][x_right+1].fval  = rock_wall1.ftval;
    cave[i1][x_right+1].fopen = rock_wall1.ftopen;
  }

  for (i1 = x_left; i1 <= x_right; i1++) {
    cave[y_height-1][i1].fval  = rock_wall1.ftval;
    cave[y_height-1][i1].fopen = rock_wall1.ftopen;
    cave[y_depth+1][i1].fval   = rock_wall1.ftval;
    cave[y_depth+1][i1].fopen  = rock_wall1.ftopen;
  }

  /*{ The inner room		}*/
  y_height += 2;
  y_depth  -= 2;
  x_left   += 2;
  x_right  -= 2;

  for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
    cave[i1][x_left-1].fval   = 8;
    cave[i1][x_right+1].fval  = 8;
  }

  for (i1 = x_left; i1 <= x_right; i1++) {
    cave[y_height-1][i1].fval  = 8;
    cave[y_depth+1][i1].fval   = 8;
  }
  
  /*{ Inner room varitions		}*/
  switch (randint(5)) {
  case 1 :	/*	{ Just an inner room...	}*/
    switch (randint(4)) {	/*{ Place a door	}*/
    case 1 :	place_secret_door(y_height-1,xval);  break;
    case 2 :	place_secret_door(y_depth+1,xval);   break;
    case 3 :	place_secret_door(yval,x_left-1);    break;
    case 4 :	place_secret_door(yval,x_right+1);   break;
    }
    gc__vault_monster(yval,xval,1);
    break;
    
  case 2 :	/*	{ Treasure Vault	}*/
    switch (randint(4)) {	/*{ Place a door	}*/
    case 1 :	place_secret_door(y_height-1,xval);    break;
    case 2 :	place_secret_door(y_depth+1,xval);     break;
    case 3 :	place_secret_door(yval,x_left-1);      break;
    case 4 :	place_secret_door(yval,x_right+1);     break;
    }
    
    for (i1 = yval-1; i1 <= yval+1; i1++) {
      cave[i1][xval-1].fval   = 8;
      cave[i1][xval+1].fval   = 8;
    }
    
    cave[yval-1][xval].fval  = 8;
    cave[yval+1][xval].fval  = 8;
    
    switch (randint(4)) {	/*{ Place a door	}*/
    case 1 :	place_locked_door(yval-1,xval);   break;
    case 2 :	place_locked_door(yval+1,xval);   break;
    case 3 :	place_locked_door(yval,xval-1);   break;
    case 4 :	place_locked_door(yval,xval+1);   break;
    }
    
    /*{ Place an object in the treasure vault	}*/
    switch (randint(10)) {
    case 1 : place_a_staircase(yval,xval,up_staircase);   break;
    case 2 : place_a_staircase(yval,xval,down_staircase); break;
    default: place_object(yval,xval);                     break;
    }
    
    /*{ Guard the treasure well		}*/
    gc__vault_monster(yval,xval,2+randint(3));
    
    /*{ If the monsters don't get 'em...	}*/
    gc__vault_trap(yval,xval,4,10,2+randint(3));
    break;
    
  case 3 :	/*{ Inner pillar(s)...	}*/
    switch (randint(4)) {	/*{ Place a door	}*/
    case 1 :	place_secret_door(y_height-1,xval);   break;
    case 2 :	place_secret_door(y_depth+1,xval);    break;
    case 3 :	place_secret_door(yval,x_left-1);     break;
    case 4 :	place_secret_door(yval,x_right+1);    break;
    }
    
    for (i1 = yval-1; i1 <= yval+1; i1++) {
      for (i2 = xval-1; i2 <= xval+1; i2++) {
	cave[i1][i2].fval   = 8;
      }
    }

    if (randint(2) == 1) {
      switch (randint(2)) {
      case 1 :
	for (i1 = yval-1; i1 <= yval+1; i1++) {
	  for (i2 = xval-6; i2 <= xval-4; i2++) {
	    cave[i1][i2].fval   = 8;
	  }
	}
	
	for (i1 = yval-1; i1 <= yval+1; i1++) {
	  for (i2 = xval+4; i2 <= xval+6; i2++) {
	    cave[i1][i2].fval   = 8;
	  }
	}
	break;
	
      case 2 : 
	for (i1 = yval-1; i1 <= yval+1; i1++) {
	  for (i2 = xval-7; i2 <= xval-5; i2++) {
	    cave[i1][i2].fval   = 8;
	  }
	}
	
	for (i1 = yval-1; i1 <= yval+1; i1++) {
	  for (i2 = xval+5; i2 <= xval+7; i2++) {
	    cave[i1][i2].fval   = 8;
	  }
	}
	break;
      }

      if (randint(3) == 1) {  /*{ Inner rooms   }*/
	for (i1 = xval-5; i1 <= xval+5; i1++) {
	  cave[yval-1][i1].fval = 8;
	  cave[yval+1][i1].fval = 8;
	}

	switch (randint(2)) {
	  case 1 : place_secret_door(yval+1,xval-3); break;
	  case 2 : place_secret_door(yval-1,xval-3); break;
	}

	switch (randint(2)) {
	case 1 : place_secret_door(yval+1,xval+3); break;
	case 2 : place_secret_door(yval-1,xval+3); break;
	}

	if (randint(3) == 1) {
	  place_object(yval,xval-2);
	}

	if (randint(3) == 1) {
	  place_object(yval,xval+2);
	}

	gc__vault_monster(yval,xval-2,randint(2));
	gc__vault_monster(yval,xval+2,randint(2));
      }
    }
    break;

  case 4 :    /*  { Maze inside...        }*/
    switch (randint(4)) {    /*{ Place a door  }*/
    case 1 : place_secret_door(y_height-1,xval);  break;
    case 2 : place_secret_door(y_depth+1,xval);   break;
    case 3 : place_secret_door(yval,x_left-1);    break;
    case 4 : place_secret_door(yval,x_right+1);   break;
    }

    for (i1 = y_height; i1 <= y_depth; i1++) {
      for (i2 = x_left; i2 <= x_right; i2++) {
	if (((i2+i1) & 1) == 1) {
	  cave[i1][i2].fval = 8;
	}
      }
    }

    /*{ Monsters just love mazes...           }*/
    gc__vault_monster(yval,xval-5,randint(3));
    gc__vault_monster(yval,xval+5,randint(3));

    /*{ Traps make them entertaining...       }*/
    gc__vault_trap(yval,xval-3,2,8,randint(3));
    gc__vault_trap(yval,xval+3,2,8,randint(3));

    /*{ Mazes should have some treasure too.. }*/
    for (i1 = 1; i1 <= 3; i1++) {
      random_object(yval,xval,1);
    }
    break;

  case 5 :   /*   { Four small rooms...   }*/
    for (i1 = y_height; i1 <= y_depth; i1++) {
      cave[i1][xval].fval = 8;
    }

    for (i1 = x_left; i1 <= x_right; i1++) {
      cave[yval][i1].fval = 8;
    }

    switch (randint(2)) {
    case 1 : 
      i1 = randint(10);
      place_secret_door(y_height-1,xval-i1);
      place_secret_door(y_height-1,xval+i1);
      place_secret_door(y_depth+1,xval-i1);
      place_secret_door(y_depth+1,xval+i1);
      break;
      
    case 2 : 
      i1 = randint(3);
      place_secret_door(yval+i1,x_left-1);
      place_secret_door(yval-i1,x_left-1);
      place_secret_door(yval+i1,x_right+1);
      place_secret_door(yval-i1,x_right+1);
      break;
    }

    /*{ Treasure in each one...               }*/
    random_object(yval,xval,2+randint(2));

    /*{ Gotta have some monsters...           }*/
    gc__vault_monster(yval+2,xval-4,randint(2));
    gc__vault_monster(yval+2,xval+4,randint(2));
    gc__vault_monster(yval-2,xval-4,randint(2));
    gc__vault_monster(yval-2,xval+4,randint(2));
    break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void gc__build_type3(integer yval, integer xval)
{
  /*{ Builds a room at a row,column coordinate		-RAK-	}*/
  /*{ Type 3 unusual rooms are cross shaped			}*/

  integer      y_height,y_depth;
  integer      x_left,x_right;
  integer      i0,i1,i2;
  floor_type   cur_floor;

  if (dun_level <= randint(25)) {
    cur_floor = lopen_floor;	/*{ Floor with light	}*/
  } else {
    cur_floor = dopen_floor;	/*{ Dark floor		}*/
  }

  i0 = 2 + randint(2);
  y_height = yval - i0;
  y_depth  = yval + i0;
  x_left   = xval - 1;
  x_right  = xval + 1;

  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[i1][i2].fval  = cur_floor.ftval;
      cave[i1][i2].fopen = cur_floor.ftopen;
    }
  }

  for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
    cave[i1][x_left-1].fval   = rock_wall1.ftval;
    cave[i1][x_left-1].fopen  = rock_wall1.ftopen;

    cave[i1][x_right+1].fval  = rock_wall1.ftval;
    cave[i1][x_right+1].fopen = rock_wall1.ftopen;
  }

  for (i1 = x_left; i1 <= x_right; i1++) {
    cave[y_height-1][i1].fval  = rock_wall1.ftval;
    cave[y_height-1][i1].fopen = rock_wall1.ftopen;

    cave[y_depth+1][i1].fval   = rock_wall1.ftval;
    cave[y_depth+1][i1].fopen  = rock_wall1.ftopen;
  }

  i0 = 2 + randint(9);
  y_height = yval - 1;
  y_depth  = yval + 1;
  x_left   = xval - i0;
  x_right  = xval + i0;

  for (i1 = y_height; i1 <= y_depth; i1++) {
    for (i2 = x_left; i2 <= x_right; i2++) {
      cave[i1][i2].fval  = cur_floor.ftval;
      cave[i1][i2].fopen = cur_floor.ftopen;
    }
  }

  for (i1 = (y_height - 1); i1 <= (y_depth + 1); i1++) {
    if (cave[i1][x_left-1].fval != cur_floor.ftval) {
      cave[i1][x_left-1].fval  = rock_wall1.ftval;
      cave[i1][x_left-1].fopen = rock_wall1.ftopen;
    }

 
    if (cave[i1][x_right+1].fval != cur_floor.ftval) {
      cave[i1][x_right+1].fval  = rock_wall1.ftval;
      cave[i1][x_right+1].fopen = rock_wall1.ftopen;
    }
  }

  for (i1 = x_left; i1 <= x_right; i1++) {
    
    if (cave[y_height-1][i1].fval != cur_floor.ftval) {
      cave[y_height-1][i1].fval  = rock_wall1.ftval;
      cave[y_height-1][i1].fopen = rock_wall1.ftopen;
    }
    
    
    if (cave[y_depth+1][i1].fval != cur_floor.ftval) {
      cave[y_depth+1][i1].fval  = rock_wall1.ftval;
      cave[y_depth+1][i1].fopen = rock_wall1.ftopen;
    }
  }
  
  /*{ Special features...			}*/
  switch (randint(4)) {
  case 1 :	/*{ Large middle pillar		}*/
    for (i1 = yval-1; i1 <= yval+1; i1++) {
      for (i2 = xval-1; i2 <= xval+1; i2++) {
	cave[i1][i2].fval = 8;
      }
    }
    break;

  case 2 :		/*{ Inner treasure vault		}*/
    for (i1 = yval-1; i1 <= yval+1; i1++) {
      cave[i1][xval-1].fval   = 8;
      cave[i1][xval+1].fval   = 8;
    }

    cave[yval-1][xval].fval  = 8;
    cave[yval+1][xval].fval  = 8;

    switch (randint(4)) {	/*{ Place a door	}*/
    case 1 :	place_secret_door(yval-1,xval); break;
    case 2 :	place_secret_door(yval+1,xval); break; 
    case 3 :	place_secret_door(yval,xval-1); break; 
    case 4 :	place_secret_door(yval,xval+1); break; 
    }
    
    /*{ Place a treasure in the vault		}*/
    place_object(yval,xval);
    
    /*{ Let's gaurd the treasure well...	}*/
    gc__vault_monster(yval,xval,2+randint(2));
    
    /*{ Traps naturally			}*/
    gc__vault_trap(yval,xval,4,4,1+randint(3));
    break;
    
  case 3 :   
    if (randint(3) == 1) {
      cave[yval-1][xval-2].fval = 8;
      cave[yval+1][xval-2].fval = 8;
      cave[yval-1][xval+2].fval = 8;
      cave[yval-1][xval+2].fval = 8;
      cave[yval-2][xval-1].fval = 8;
      cave[yval-2][xval+1].fval = 8;
      cave[yval+2][xval-1].fval = 8;
      cave[yval+2][xval+1].fval = 8;
      if (randint(3) == 1) {
	place_secret_door(yval,xval-2);
	place_secret_door(yval,xval+2);
	place_secret_door(yval-2,xval);
	place_secret_door(yval+2,xval);
      }
    } else if (randint(3) == 1) {
      cave[yval][xval].fval   = 8;
      cave[yval-1][xval].fval = 8;
      cave[yval+1][xval].fval = 8;
      cave[yval][xval-1].fval = 8;
      cave[yval][xval+1].fval = 8;
    } else if (randint(3) == 1) {
      cave[yval][xval].fval = 8;
    }
    break;

  case 4 :   
    break;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* end rooms.c */
