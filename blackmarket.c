/* blackmarket.c */
/**/

#include "imoria.h"
#include "dungeon.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bm__add_tohit(integer a,integer b,integer c,integer i,integer *r)
{
  *r = randint(a) + randint(b) + randint(c);
  t_list[i].tohit = *r;
};
//////////////////////////////////////////////////////////////////////
void bm__add_todam(integer a,integer b,integer c,integer i,integer *r)
{
  *r = randint(a) + randint(b) + randint(c);
  t_list[i].todam = *r;
};
//////////////////////////////////////////////////////////////////////
void bm__add_ac(integer a,integer b,integer c,integer i,integer *r)
{
  *r = randint(a) + randint(b) + randint(c);
  t_list[i].ac = *r;
};
//////////////////////////////////////////////////////////////////////
void bm__add_toac(integer a,integer b,integer c,integer i,integer *r)
{
  *r = randint(a) + randint(b) + randint(c);
  t_list[i].toac = *r;
};
//////////////////////////////////////////////////////////////////////
void bm__add_p1(integer a,integer b,integer c,integer i,integer *r)
{
  *r = a + randint(b) + randint(c);
  t_list[i].p1 = *r;
};
//////////////////////////////////////////////////////////////////////
void bm__add_p1_rods(integer a,integer b,integer c,integer i,integer *r)
{
  *r = a + randint(b) + c;
  t_list[i].p1 = *r;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void soup_up_black_market(integer indx)
{
  /*{ Soup-up the Black Market inventory - mwk}*/

  integer    r1,r2,r3,r4,r5;

  //with t_list[indx]. do;
  switch (t_list[indx].tval) {

  case sling_ammo:
  case arrow:
  case bolt:  
    switch (randint(10)) {
    case 1: case 2: case 3:
      strcat(t_list[indx].name," of Slaying");
      bm__add_tohit(5,5,1,indx,&r1);
      bm__add_todam(5,5,1,indx,&r2);
      t_list[indx].cost += 1250*r1 + 1250*r2;
      break;
      
    case 4: case 5:
      t_list[indx].flags |= 0x00040000;
      strcat(t_list[indx].name," of Fire");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      break;
      
    case 6: case 7:
      t_list[indx].flags |= 0x00008000;
      strcat(t_list[indx].name," of Slay Evil");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      break;
      
    case 8: case 9:
      t_list[indx].flags |= 0x00004000;
      strcat(t_list[indx].name," of Slay Monster");
      bm__add_tohit(3,2,1,indx,&r1);
      bm__add_todam(3,2,1,indx,&r2);
      t_list[indx].cost += 900*r1 + 900*r1;
      break;
      
    case 10:
      t_list[indx].flags |= 0x00002000;
      strcat(t_list[indx].name," of Dragon Slaying");
      bm__add_tohit(8,8,1,indx,&r1);
      bm__add_todam(8,8,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      break;
      
    default:
      break;
    };
    
    t_list[indx].number = 0;
    for (r1 = 1; r1 <= 7; r1++) { 
      t_list[indx].number += randint(12);
    }
    missle_ctr++;
    if (missle_ctr > 65534) {
      missle_ctr = 1;
    }
    t_list[indx].subval = missle_ctr + 512;
    break;
    
  case bow_crossbow_or_sling:
    bm__add_tohit(5,5,1,indx,&r1);
    t_list[indx].cost += 1000*r1 + 10000;
    break;
    
  case hafted_weapon:
  case pole_arm:
  case dagger:
  case sword:
    switch (randint(16)) {
      case 1: case 9:
	t_list[indx].flags |= 0x01418001;
	strcat(t_list[indx].name," (HA)");
	bm__add_tohit(6,6,1,indx,&r1);
	bm__add_todam(6,6,1,indx,&r2);
	bm__add_toac(5,1,1,indx,&r4);
	bm__add_p1(1,4,1,indx,&r5);   /*{for sustain stat}*/
	t_list[indx].cost += 1000*r1 + 1000*r2;
	t_list[indx].cost += 2000*r4;
	t_list[indx].cost += 20000;
	break;

    case 2: case 10:
      t_list[indx].flags |= 0x07B80900;
      strcat(t_list[indx].name," [%P4] (DF)");
      bm__add_tohit(6,6,1,indx,&r1);
      bm__add_todam(6,6,1,indx,&r2);
      bm__add_toac(5,5,8,indx,&r4);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 2000*r4;
      t_list[indx].cost += 25000;
      break;

    case 3: case 11:
      t_list[indx].flags |= 0x01004000;
      strcat(t_list[indx].name, " (SM)");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 15000;
      break;

    case 4: case 12:
      t_list[indx].flags |= 0x00002000;
      strcat(t_list[indx].name," (SD)");
      bm__add_tohit(5,5,1,indx,&r1);
      bm__add_todam(5,5,1,indx,&r2);
      bm__add_toac(10,1,1,indx,&r4);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 5000*r4;
      t_list[indx].cost += 15000;
      break;

    case 5: case 13:
      t_list[indx].flags |= 0x01008000;
      strcat(t_list[indx].name," (SE)");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 15000;
      break;

    case 6: case 14:
      t_list[indx].flags |= 0x01010000;
      strcat(t_list[indx].name," (SU)");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 15000;
      break;

    case 7: case 15:
      t_list[indx].flags |= 0x00040000;
      strcat(t_list[indx].name," (FT)");
      bm__add_tohit(4,4,1,indx,&r1);
      bm__add_todam(4,4,1,indx,&r2);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 15000;
      break;

    case 8: case 16:
      t_list[indx].flags |= 0x00002000;
      strcat(t_list[indx].name," (SD)");
      bm__add_tohit(5,5,1,indx,&r1);
      bm__add_todam(5,5,1,indx,&r2);
      bm__add_toac(10,1,1,indx,&r4);
      t_list[indx].cost += 1000*r1 + 1000*r2;
      t_list[indx].cost += 5000*r4;
      t_list[indx].cost += 15000;
      break;

    default:
      break;
    }
    break;

  case pick_or_shovel:
    bm__add_p1(0,3,3,indx,&r5);
    bm__add_tohit(4,4,1,indx,&r1);
    bm__add_todam(4,4,1,indx,&r2);
    t_list[indx].cost += 1000*r1 + 1000*r2 + 1000*r5;
    t_list[indx].cost += 15000;
    break;

  case gloves_and_gauntlets:
    switch (randint(2)) {
    case 1:
      t_list[indx].flags |=0x00800000;
      strcat(t_list[indx].name," of Free Action");
      bm__add_ac(4,4,1,indx,&r3);
      bm__add_toac(4,4,1,indx,&r4);
      t_list[indx].cost += 1000*r3 + 1000*r4;
      t_list[indx].cost += 15000;
      break;

    case 2:
      strcat(t_list[indx].name," of Slaying");
      bm__add_tohit(6,6,1,indx,&r1);
      bm__add_todam(6,6,1,indx,&r2);
      bm__add_ac(4,4,1,indx,&r3);
      bm__add_toac(4,4,1,indx,&r4);
      t_list[indx].cost += 5000*r1 + 6000*r2;
      t_list[indx].cost += 1000*r3 + 1000*r4;
      t_list[indx].cost += 15000;
      break;

    default:
      break;
    }
    break;

  case cloak:
    switch (randint(2)) {
    case 1:
      strcat(t_list[indx].name," of Protection");
      bm__add_ac(4,4,1,indx,&r3);
      bm__add_toac(4,4,1,indx,&r4);
      t_list[indx].cost += 5000*r3 + 5000*r4;
      break;

    case 2:
      t_list[indx].flags |= 0x00000100;
      strcat(t_list[indx].name," of Stealth (%P1)");
      bm__add_p1(0,2,2,indx,&r5);
      bm__add_ac(4,4,1,indx,&r3);
      bm__add_toac(4,4,1,indx,&r4);
      t_list[indx].cost += 10000*r5 + 5000*r3 + 5000*r4;
      break;

    default:
      break;
    }
    break;

  case helm:
    switch (t_list[indx].subval) {
      case 1: case 2: case 3: case 4: case 5:
	switch (randint(3)) {
	case 1:
	  t_list[indx].flags |= 0x00000008;
	  strcat(t_list[indx].name," of Intelligence");
	  bm__add_p1(0,2,2,indx,&r5);
	  bm__add_ac(4,4,1,indx,&r3);
	  bm__add_toac(4,4,1,indx,&r4);
	  t_list[indx].cost += 10000*r5;
	  t_list[indx].cost += 5000*r3 + 5000*r4;
	  break;
	  
	case 2:
	  t_list[indx].flags |= 0x00000010;
	  strcat(t_list[indx].name," of Wisdom");
	  bm__add_p1(0,2,2,indx,&r5);
	  bm__add_ac(4,4,1,indx,&r3);
	  bm__add_toac(4,4,1,indx,&r4);
	  t_list[indx].cost += 10000*r5;
	  t_list[indx].cost += 5000*r3 + 5000*r4;
	  break;

	case 3:
	  t_list[indx].flags |= 0x40000000;
	  strcat(t_list[indx].name," of Infra-Vision");
	  bm__add_p1(0,4,4,indx,&r5);
	  bm__add_ac(4,4,1,indx,&r3);
	  bm__add_toac(4,4,1,indx,&r4);
	  t_list[indx].cost += 10000*r5;
	  t_list[indx].cost += 5000*r3 + 5000*r4;
	  break;

	default:
	  break;
	}

    case 6: case 7: case 8:
      switch (randint(6)) {
      case 1:
	t_list[indx].flags |= 0x00800007;
	strcat(t_list[indx].name," of Might");
	bm__add_p1(0,4,4,indx,&r5);
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 10000*r5;
	t_list[indx].cost += 5000*r3 + 5000*r4;
        break;
	
      case 2:
	t_list[indx].flags |= 0x00000030;
	strcat(t_list[indx].name," of Lordliness");
	bm__add_p1(0,4,4,indx,&r5);
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 10000*r5;
	t_list[indx].cost += 5000*r3 + 5000*r4;
	break;

      case 3:
	t_list[indx].flags |= 0x01380008;
	strcat(t_list[indx].name," of the Magi");
	bm__add_p1(0,4,4,indx,&r5);
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 10000*r5;
	t_list[indx].cost += 5000*r3 + 5000*r4;
	break;

      case 4:
	t_list[indx].flags |= 0x00000020;
	strcat(t_list[indx].name," of Beauty");
	bm__add_p1(0,4,4,indx,&r5);
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 10000*r5;
	t_list[indx].cost += 5000*r3 + 5000*r4;
        break;

      case 5:
	t_list[indx].flags |= 0x01000040;
	strcat(t_list[indx].name," of Seeing");
	bm__add_p1(0,4,4,indx,&r5);
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 10000*r5;
	t_list[indx].cost += 5000*r3 + 5000*r4;
	break;

      case 6:
	t_list[indx].flags |= 0x00000800;
	strcat(t_list[indx].name," of Regeneration");
	bm__add_ac(4,4,1,indx,&r3);
	bm__add_toac(4,4,1,indx,&r4);
	t_list[indx].cost += 5000*r3 + 5000*r4;
	break;

      default:
	break;
      }
      break;

    default:
      break;
    };

  case shield:
  case hard_armor:
    switch (randint(10)) {
    case 1: case 6:
      t_list[indx].flags |= 0x02380000;
      strcat(t_list[indx].name," (R)");
      bm__add_toac(8,8,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      t_list[indx].cost += 50000;
      break;

    case 2: case 7:
      t_list[indx].flags |= 0x00100000;
      strcat(t_list[indx].name," (RA)");
      bm__add_toac(8,8,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      t_list[indx].cost += 40000;
      break;

    case 3: case 8:
      t_list[indx].flags |= 0x00080000;
      strcat(t_list[indx].name," (RF)");
      bm__add_toac(8,8,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      t_list[indx].cost += 40000;
      break;

    case 4: case 9:
      t_list[indx].flags |= 0x00200000;
      strcat(t_list[indx].name," (RC)");
      bm__add_toac(8,8,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      t_list[indx].cost += 40000;
      break;

    case 5: case 10:
      t_list[indx].flags |= 0x02000000;
      strcat(t_list[indx].name," (RL)");
      bm__add_toac(8,8,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      t_list[indx].cost += 30000;
      break;

    default:
      break;
    }
    break;
 
  case amulet:
    switch (t_list[indx].subval) {
    case 1: case 2:
      bm__add_p1(0,2,2,indx,&r5);
      t_list[indx].cost += 10000*r5;
      break;

    case 3:
      bm__add_p1(8,15,15,indx,&r5);
      t_list[indx].cost += 10000*r5;
      break;

    case 5:
      bm__add_toac(4,4,0,indx,&r4);
      t_list[indx].cost += 10000*r4;
      break;

    default:
      break;
    }
     
  case ring:
    switch (t_list[indx].subval) {
    case 1: case 2: case 3: case 4: /*{Gain stat}*/
      bm__add_p1(0,2,2,indx,&r5);
      t_list[indx].cost += 10000*r5;
      break;

    case 7:    /*{Speed}*/
      bm__add_p1(1,2,1,indx,&r5);
      t_list[indx].cost += 500000*r5;
      break;

    case 8:   /*{Searching}*/
      bm__add_p1(8,15,15,indx,&r5);
      t_list[indx].cost += 10000*r5;
      break;

    case 10: case 13: case 26: /*{Slow Digestion, Feather-Falling, See-Invis}*/
      bm__add_toac(2,2,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      break;

    case 17: case 18: case 19: /*{Lordly Protection}*/
      bm__add_toac(10,8,5,indx,&r4);
      t_list[indx].cost += 10000*r4;
      break;

    case 22:                   /*{Increase Damage}*/
      bm__add_todam(3,3,3,indx,&r2);
      t_list[indx].cost += 10000*r2;
      break;

    case 23:                   /*{Increase To-hit}*/
      bm__add_tohit(3,3,3,indx,&r1);
      t_list[indx].cost += 10000*r1;
      break;

    case 24:                 /*{Protection}*/
      bm__add_toac(4,4,1,indx,&r4);
      t_list[indx].cost += 10000*r4;
      break;

    case 33:                 /*{Slaying}*/
      bm__add_tohit(3,3,3,indx,&r1);
      bm__add_todam(3,3,3,indx,&r2);
      t_list[indx].cost += 10000*r2 + 10000*r1;
      break;

    default:
      break;
    }
    break;

  case staff:
    switch (t_list[indx].subval) {
    case 1:
      bm__add_p1_rods(32,20,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 2:
      bm__add_p1_rods(14,8,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 3:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 4:
      bm__add_p1_rods(32,20,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 5:
      bm__add_p1_rods(21,15,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 6:
      bm__add_p1_rods(9,4,5,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 7:
      bm__add_p1_rods(8,5,3,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 8:
      bm__add_p1_rods(4,3,1,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 9:
      bm__add_p1_rods(4,3,1,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 10:
      bm__add_p1_rods(4,3,1,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 11:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 13:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 14:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 16:
      bm__add_p1_rods(22,10,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 17:
      bm__add_p1_rods(7,3,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 19:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;
      
    case 20:
      bm__add_p1_rods(7,3,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 21:
      bm__add_p1_rods(22,10,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 22:
      bm__add_p1_rods(7,3,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 23:
      bm__add_p1_rods(7,3,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 24:
      bm__add_p1_rods(7,3,1,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    default:
      break;
    }
    break;

  case wand:
    switch (t_list[indx].subval) {
    case 1:
      bm__add_p1_rods(16,10,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 2:
      bm__add_p1_rods(14,8,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 3:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 4:
      bm__add_p1_rods(14,8,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 5:
      bm__add_p1_rods(7,4,3,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 6:
      bm__add_p1_rods(14,8,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 9:
      bm__add_p1_rods(16,10,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 10:
      bm__add_p1_rods(18,12,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 11:
      bm__add_p1_rods(22,10,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 12:
      bm__add_p1_rods(6,3,3,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 13:
      bm__add_p1_rods(14,8,6,indx,&r5);
      t_list[indx].cost += 5000*r5;      
      break;

    case 14:
      bm__add_p1_rods(16,10,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 15:
      bm__add_p1_rods(8,5,3,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 16:
      bm__add_p1_rods(8,5,3,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 17:
      bm__add_p1_rods(11,5,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 18:
      bm__add_p1_rods(9,5,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;
      
    case 19:
      bm__add_p1_rods(12,8,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;
      
    case 20:
      bm__add_p1_rods(12,8,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 21:
      bm__add_p1_rods(12,8,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 22:
      bm__add_p1_rods(36,24,12,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 23:
      bm__add_p1_rods(12,8,4,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    case 24:
      bm__add_p1_rods(16,10,6,indx,&r5);
      t_list[indx].cost += 5000*r5;
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  /*
  scost = -t_list[indx].cost;
  insert_num(t_list[indx].name,"%P1",t_list[indx].p1,true);
  insert_num(t_list[indx].name,"%P2",t_list[indx].tohit,true);
  insert_num(t_list[indx].name,"%P3",t_list[indx].todam,true);
  insert_num(t_list[indx].name,"%P4",t_list[indx].toac,true);
  insert_num(t_list[indx].name,"%P5",t_list[indx].p1,false);
  insert_num(t_list[indx].name,"%P6",t_list[indx].ac,false);
  */
};                                                                    

#if 0
extern void store_carry( integer store_num, integer *ipos);

/*{Add Black Market items to the inventory}*/

void do_black_market()
{
  integer    indx,iptr,inven_num;

   inven_num = 0;
   store[S_BLACK_MARKET].store_ctr = 0;
   //while (inven_num < STORE_MIN_INVEN) {
     inven_num = 5 + randint(STORE_INVEN_MAX);
   //}
   if (inven_num > STORE_INVEN_MAX) {
     inven_num = STORE_INVEN_MAX;
   }
   for (indx = 1; indx <= inven_num; indx++) {
     //with store[S_BLACK_MARKET].store_inven[indx] do;
     iptr      = randint(BLK_MKT_MAX);   
     BMI.sitem = blk_mkt_init[iptr];
     soup_up_black_market(indx);
     inventory[inven_max] = BMI.sitem;
     store_carry(S_BLACK_MARKET,iptr);
   }

   for (indx = inven_num; indx <= STORE_INVEN_MAX; indx++) {
     //with store[S_BLACK_MARKET].store_inven[indx] do;
     BMI.sitem = blank_treasure;
   }
 
  store[S_BLACK_MARKET].store_ctr--;
};
                                 
void Opusii_vomit(integer percentage)
{
  if (randint(100) < percentage) {
    //with py.flags do;
    PF.foodc     -= 500;
    PF.paralysis  = 4;
    msg_print("You feel your insides heave!");
  }
};
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  store.c */
