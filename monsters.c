/* monsters.c */
/**/

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void place_monster(integer y,integer x,integer z, boolean slp)
{
  /*{ Places a monster at given location			-RAK-	}*/
	
  integer    cur_pos;

//  printf ("\n     Enter place_monster: %d (%d,%d)",z,x,y);  fflush(stdout);

  popm(&cur_pos);

//  printf ("\n     got cur_pos: %d",cur_pos);  fflush(stdout);

  //with m_list[cur_pos] do;
  m_list[cur_pos].fy    = y;
  m_list[cur_pos].fx    = x;
  m_list[cur_pos].mptr  = z;
  m_list[cur_pos].nptr  = muptr;
  muptr = cur_pos;

  if ((c_list[z].cdefense & 0x4000) != 0) {
    m_list[cur_pos].hp = max_hp(c_list[z].hd);
  } else {
    m_list[cur_pos].hp = damroll(c_list[z].hd);
  }
//  printf ("\n     hp: %d)",m_list[cur_pos].hp);  fflush(stdout);

  m_list[cur_pos].cdis    = distance(char_row,char_col,y,x);
//  printf ("\n     cdis: %d)",m_list[cur_pos].cdis);  fflush(stdout);
  m_list[cur_pos].cspeed  = c_list[z].speed + py.flags.speed;
  m_list[cur_pos].stunned = 0;

  if (slp) {
//    printf ("\n     set sleep: %d",c_list[z].sleep);  fflush(stdout);
    m_list[cur_pos].csleep = (c_list[z].sleep/5.0)+randint(c_list[z].sleep);
//    printf ("\n     sleep: %d",m_list[cur_pos].csleep);  fflush(stdout);
  } else {
//    printf ("\n     set no sleep:");  fflush(stdout);
    m_list[cur_pos].csleep = 0;
  }

  cave[y][x].cptr = cur_pos;

//  printf ("\n     Exit place_monster: %d (%d,%d)",z,x,y);  fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void alloc_land_monster(obj_set alloc_set, integer num, integer dis,
			boolean slp, boolean water)
{
  /*{ Allocates a random land monster			-RAK-	}*/

  integer    y,x,i1,i2,i3,count;
  integer    count2;
  boolean    flag;

//  printf ("\n         Enter alloc land monster\n");  fflush(stdout);

  for (i1 = 0; i1 < num; i1++) {
//    printf ("\n             Doing a monster");  fflush(stdout);
    flag   = false;
    count  = 0;
    count2 = 0;
    do {
      y = randint(cur_height-2)+1;
      x = randint(cur_width-2)+1;
      count2++;
    } while (!(((is_in(cave[y][x].fval, alloc_set))  &&
	       (cave[y][x].cptr == 0)           &&
	       (cave[y][x].fopen)               &&
	       (distance(y,x,char_row,char_col) > dis)) 
	       || (count2 > 7500)));
    
//    printf ("\n             got me coords (%d, %d)",x,y);  fflush(stdout);

    do {
      if (dun_level == 0) {
//printf ("\n             doing dun_level: %d",m_level[0]);  fflush(stdout);
	i2 = randint(m_level[0]);
      } else if (dun_level > MAX_MONS_LEVEL) {
	i2 = randint(m_level[MAX_MONS_LEVEL]) + m_level[0];
      } else if (randint(mon_nasty) == 1) {
	i2 = dun_level + abs(randnor(0,4)) + 1;
	if (i2 > MAX_MONS_LEVEL) {
	  i2 = MAX_MONS_LEVEL;
	}
	i3 = m_level[i2] - m_level[i2-1];
	i2 = randint(i3) + m_level[i2-1];
      } else {
	i2 = randint(m_level[dun_level]) + m_level[0];
      }
      
//      printf ("\n             got me i2 (%d)",i2);  fflush(stdout);

      if (!water) {
	flag = (((c_list[i2].cmove & 0x00008000) == 0) &&
		(((c_list[i2].cmove & 0x00000010) == 0) ||
		 ((c_list[i2].cmove & 0x00000040) == 0) ||
		 ((c_list[i2].cmove & 0x00800000) != 0))) ;
      } else {
	flag = (((c_list[i2].cmove & 0x00008000) == 0) &&
		((c_list[i2].cmove & 0x00000010) != 0));
      }

//      printf ("\n             got me flag (%d)",flag);  fflush(stdout);
	
      if (flag) {
	if (count2 < 7500) {
//      printf ("\n             placing monster");  fflush(stdout);
	  place_monster(y,x,i2,slp);
	  flag = true;
//      printf ("\n             placed monster");  fflush(stdout);
	}
      }


      count++;
    } while (!((flag) || (count > 10)));
  } /* end for */

//  printf ("\n         Exit alloc land monster\n");  fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  monsters.c */
