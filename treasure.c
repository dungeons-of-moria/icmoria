/* treasure.c */
/**/

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///// magic_treasure code

void mt__ego_sword(integer x)
{
  //with t_list[x] do;
  switch (randint(5)) {

    case 1 : /*{Holy Avenger}*/
      
      strcat(t_list[x].name," (HA)");
      t_list[x].flags |= ( See_Invisible_worn_bit |
			   Sustain_Stat_worn_bit  |
			   Resist_Acid_worn_bit   |
			   Resist_Fire_worn_bit   |
			   Strength_worn_bit      |
			   Slay_Undead_worn_bit   |
			   Slay_Evil_worn_bit);

      t_list[x].tohit += 5;
      t_list[x].todam += 5;
      t_list[x].toac   = randint(4);
      t_list[x].p1     = randint(4) - 1;
      t_list[x].cost  += t_list[x].p1*50000;
      t_list[x].cost  += 1000000;
      break;

  case 2 : /*{Defender}*/
    
    strcat(t_list[x].name," [%P4] (DF)");
    t_list[x].flags |= (Feather_Fall_worn_bit      |
			See_Invisible_worn_bit     |
		        Resist_Lightning_worn_bit  |
			Free_Action_worn_bit       |
		        Resist_Cold_worn_bit       |
			Resist_Acid_worn_bit       |
			Resist_Fire_worn_bit       |
			Regeneration_worn_bit      |
			Stealth_worn_bit);
    
    t_list[x].tohit += 3;
    t_list[x].todam += 3;
    t_list[x].toac   = 5 + randint(5);
    t_list[x].p1     = randint(3);
    t_list[x].cost  += t_list[x].p1*50000;
    t_list[x].cost  += 750000;
    break;
    
  case 3 :  /*{Demon Bane}*/
    
    strcat(t_list[x].name," (DB)");
    t_list[x].flags  |= Resist_Fire_worn_bit;
    t_list[x].flags2 |= Slay_demon_worn_bit;
    t_list[x].tohit  += 3;
    t_list[x].todam  += 3;
    t_list[x].cost   += 500000;
    break;
    
  case 4 : /* {Soul Sword}*/
    
    strcat(t_list[x].name," (SS)");
    t_list[x].flags  |=  (Intelligence_worn_bit  |
			  Wisdom_worn_bit        |
			  Charisma_worn_bit      |
			  See_Invisible_worn_bit |
			  Regeneration_worn_bit);
    t_list[x].flags2 |=  (Soul_Sword_worn_bit    |
			  Bad_repute_worn_bit);
    
    t_list[x].tohit +=  5;
    t_list[x].todam += 10;
    t_list[x].p1     = -randint(3) - 2;
    t_list[x].cost  += 800000 + t_list[x].p1*40000;
    break;
    
  case 5 : /*{Vorpal Sword}*/
    
    strcat(t_list[x].name," (V)");
    t_list[x].flags  |= Sustain_Stat_worn_bit;
    t_list[x].flags2 |= Sharp_worn_bit;
    t_list[x].p1      = 1;
    t_list[x].tohit  += 5;
    t_list[x].todam  += 5;
    t_list[x].cost   += 750000;
    break;
    
  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
void mt__slaying_sword(integer x)
{
  //with t_list[x] do;
  switch (randint(4)) {
    
  case 1 : /* {Slay Monster}*/
    
    strcat(t_list[x].name," (SM)");
    t_list[x].flags |= (See_Invisible_worn_bit  |
			Slay_Monster_worn_bit);
    t_list[x].tohit += 3;
    t_list[x].todam += 3;
    t_list[x].cost  += 500000;
    break;
    
  case 2 : /* {Slay Dragon}*/

    strcat(t_list[x].name," (SD)");
    t_list[x].flags |= Slay_Dragon_worn_bit;
    t_list[x].tohit += 3;
    t_list[x].todam += 3;
    t_list[x].cost  += 400000;
    break;
    
  case 3 : /* {Slay Undead}*/

    strcat(t_list[x].name," (SU)");
    t_list[x].flags |= Slay_Undead_worn_bit;
    t_list[x].tohit += 2;
    t_list[x].todam += 2;
    t_list[x].cost  += 300000;
    break;
    
  case 4 : /* {Slay Regenerative}*/

    strcat(t_list[x].name," (SR)");
    t_list[x].flags2 |= Slay_regen_worn_bit;
    t_list[x].tohit  += 2;
    t_list[x].todam  += 2;
    t_list[x].cost   += 150000;
    break;

  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
void mt__magic_sword(integer x)
{
  //with t_list[x] do;
  switch (randint(4)) {

  case 1 : /* {Flame Tongue}*/
    
    strcat(t_list[x].name," (FT)");
    t_list[x].flags |= Flame_Brand_worn_bit;
    t_list[x].tohit += 1;
    t_list[x].todam += 3;
    t_list[x].cost  += 200000;
    break;
    
  case 2 : /* {Frost Brand}*/

    strcat(t_list[x].name," (FB)");
    t_list[x].flags |= Cold_Brand_worn_bit;
    t_list[x].tohit += 1;
    t_list[x].todam += 1;
    t_list[x].cost  += 120000;
    break;
    
  case 3 : /* {Wizards Blade}*/

    strcat(t_list[x].name," (WB)");
    t_list[x].flags2 |= Magic_proof_worn_bit;
    t_list[x].weight  = trunc(t_list[x].weight * 4 / 5);
    t_list[x].tval    = dagger;  /* let mages use it */
    t_list[x].tohit  += 3;
    t_list[x].todam  += 1;
    t_list[x].cost   += 80000;
    break;

  case 4 : /* {Blessed Blade}*/
    
    strcat(t_list[x].name," (BB)");
    t_list[x].flags2 |= Magic_proof_worn_bit;
    t_list[x].tval    = maul;  /* let priests use it */
    t_list[x].tohit  += 2;
    t_list[x].todam  += 4;
    t_list[x].cost   += 80000;
    break;

  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
boolean mt__magik(integer chance)
{
  /*{ Boolean : is object enchanted		  -RAK- }*/

  return (randint(150) <= chance); /*{ for deeper dungeon levels }*/
};
//////////////////////////////////////////////////////////////////////
integer mt__m_bonus(integer base,integer max_std,integer level,boolean forceit)
{
  /*{ Enchant a bonus based on degree desired -RAK- }*/
  integer  x,stand_dev;
  integer  return_value;

  if (forceit) {
//    base += 2;
  }

  stand_dev = trunc(obj_std_adj*level) + obj_std_min;

  if (stand_dev > max_std) {
    stand_dev = max_std;
  }

  x = trunc(abs(randnor(0,stand_dev))/10.0) + base;

  if (x < base) {
    return_value = base;
  } else {
    return_value = x;
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
void mt__gems(integer x,integer level,integer chance,
	      integer special,integer cursed, boolean forceit)
{
  integer   p1;

  switch (t_list[x].subval) {
  case 1 : p1 = randint(10) + 10;  break;
  case 2 : p1 = randint(5)  +  2;  break;
  case 3 : p1 = randint(8)  +  7;  break;
  case 4 : p1 = randint(3)  +  3;  break;
  case 5 : p1 = randint(10) + 10;  break;
  case 6 : p1 = randint(5)  +  5;  break;
  case 7 : p1 = randint(15) + 15;  break;
  case 8 : p1 = randint(3)  +  2;  break;
  case 9 : p1 = randint(5)  +  3;  break;
  case 10: p1 = randint(3)  +  2;  break;
  case 11: p1 = randint(6)  +  4;  break;
  default  :
#if DO_DEBUG
    MSG("Unknown subval in mt__gems");
    fprintf(debug_file, "   sub:  %ld  %s\n", t_list[x].subval,t_list[x].name);
    fflush(debug_file);
#endif	  
    p1 = 0;
    break;
  }

  t_list[x].p1 = p1;
};
//////////////////////////////////////////////////////////////////////
void mt__misc_usable(integer x,integer level,integer chance,
		     integer special,integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    if (mt__magik(special) || forceit) {
      switch (t_list[x].subval) {
	
      case 14   : /*{statues}*/
	
	switch (randint(3)) {
	case 1 : /*{summoning undead}*/
	  strcat(t_list[x].name," Major of Undead Summoning");
	  t_list[x].flags |= 0x00000100;
	  t_list[x].cost   = 0;
	  t_list[x].p1     = randint(4) + 2;
	  break;
	case 2 : /*{summon demon}*/
	  strcat(t_list[x].name," Major of Demon Summoning");
	  t_list[x].flags |= 0x00000200;
	  t_list[x].cost   = 0;
	  t_list[x].p1     = randint(3) + 1;
	  break;
	case 3 : /*{Life giving}*/
	  strcat(t_list[x].name," Life Giving");
	  t_list[x].flags |= 0x00000400;
	  t_list[x].cost   = 900000;
	  t_list[x].p1     = randint(5) + 3;
	  break;
	} /* end rand switch */
	break;
	
      case 15	    : /* teeth ? */
	
	switch (randint(4)) {
	case 1 :
	  strcat(t_list[x].name," from a Dragon");
	  t_list[x].p1     = randint(4) + 2;
	  t_list[x].cost  += t_list[x].p1*20000;
	  t_list[x].flags |= 0x20000000;
	  break;
	case 2 :
	  strcat(t_list[x].name," of a Demon");
	  t_list[x].flags |= 0x40000000;
	  t_list[x].p1     = randint(4) +2;
	  t_list[x].cost  += t_list[x].p1*20000;
	  break;
	case 3 :
	case 4 :
	  break;
	} /* end rand switch */
	break;
	
      case 16 : /*{crucifixes}*/
      case 17 :
      case 18 :
	
	switch (randint(4)) {
	case 1:
	case 2: 
	case 3:
	  strcat(t_list[x].name," of Turning");
	  t_list[x].flags |= 0x00000001;
	  t_list[x].p1     = randint(t_list[x].p1*2) + 2;
	  t_list[x].cost  += t_list[x].p1*20000;
	  break;
	case 4 :
	  strcat(t_list[x].name," of Demon Dispelling");
	  t_list[x].flags |= 0x00000002;
	  t_list[x].p1     = randint( trunc(t_list[x].subval/2));
	  t_list[x].cost  += t_list[x].p1 * 50000;
	} /* end rand switch */
	break;
	
      case 19 : 
	
	strcat(t_list[x].name," of Summon Undead");
	t_list[x].flags |= 0x00000004;
	t_list[x].cost   = 0;
	t_list[x].p1     = 2;
	break;
	
      case 20 : 
	
	strcat(t_list[x].name," of Demon Summoning");
	t_list[x].flags |= 0x00000008;
	t_list[x].cost   = 0;
	t_list[x].p1     = 2;
	break;
	
      case 21 : 
	
	switch (randint(3)) {
	case 1 :
	  strcat(t_list[x].name," containing a Djinni");
	  t_list[x].flags |= 0x00000010;
	  t_list[x].cost   = 200000;
	  t_list[x].p1     = 1;
	  break;
	case 2 :
	case 3 :
	  strcat(t_list[x].name," containing some Demons");
	  t_list[x].flags |= 0x00000020;
	  t_list[x].cost   = 0;
	  t_list[x].p1     = 1;
	  break;
	} /* end rand switch */
      } /* end subval switch */
    } /* end special */
  } /* end magic */
};
//////////////////////////////////////////////////////////////////////
void mt__armor_and_shields(integer x,integer level, integer chance,
			   integer special, integer cursed, boolean forceit)
{
  if ((t_list[x].tval == soft_armor) && (t_list[x].subval==6)) {
    /* Looks like soft_armor 6 is  Woven Cord Armor */
    
    if ((randint(4)==1) || forceit) {
      t_list[x]        = yums[11+randint(3)];   /* various non foods */
      t_list[x].weight *= WEIGHT_ADJ;
      t_list[x].cost   *= COST_ADJ;
    }
    
  } else if ((t_list[x].tval==hard_armor) && (t_list[x].subval==13)) {
    /* I think hard_armor 13 is Bronze Plate Mail */

    if (mt__magik(chance) || forceit || (randint(5)==1)) {
      if (mt__magik(special) || (randint(5)==1)) {
	if (randint(3)==1) {
	  t_list[x] = yums[17]; /* Mithril Plate Armor */
	} else {
	  t_list[x] = yums[16]; /* Mithril Chain Mail */
	}
      } else {
	t_list[x] = yums[15];   /* Elven Chain Mail */
      }
      
      t_list[x].weight *= WEIGHT_ADJ;
      t_list[x].cost    = trunc(t_list[x].cost*COST_ADJ);
      
    }
  } /* end else hard armor, 13 */
  
  if (mt__magik(chance) || forceit) {

    t_list[x].toac = mt__m_bonus(1,30,level,forceit);
    if (mt__magik(special)) {
      switch (randint(9)) {
	
      case 1     : /*{ Resist }*/
	strcat(t_list[x].name," (R)");
	t_list[x].flags |= (Resist_Lightning_worn_bit |
			    Resist_Cold_worn_bit      |
			    Resist_Acid_worn_bit      |
			    Resist_Fire_worn_bit);
	t_list[x].toac  += 5;
	t_list[x].cost  += 250000;
	break;
	
      case 2     : /*{ Resist Acid	}*/
	strcat(t_list[x].name," (RA)");
	t_list[x].flags |= Resist_Acid_worn_bit;
	t_list[x].cost  += 100000;
	break;
	
      case 3     : /*{ Resist Fire	}*/
      case 4     : 
	strcat(t_list[x].name," (RF)");
	t_list[x].flags |= Resist_Fire_worn_bit;
	t_list[x].cost  += 60000;
	break;
	
      case 5     : /*{ Resist Cold	}*/
      case 6     :	
	strcat(t_list[x].name," (RC)");
	t_list[x].flags |= Resist_Cold_worn_bit;
	t_list[x].cost  += 60000;
	break;
	
      case 7     : /*{ Resist Lightning}*/
      case 8     :
      case 9     :
	strcat(t_list[x].name," (RL)");
	t_list[x].flags |= Resist_Lightning_worn_bit;
	t_list[x].cost  += 50000;
	break;
      } /* end switch */
    }
  } else if (mt__magik(cursed)) {
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].toac   = -mt__m_bonus(1,40,level,forceit);
    t_list[x].cost   = 0;
  }
};
//////////////////////////////////////////////////////////////////////
void mt__weapons(integer x,integer level, integer chance,
		 integer special, integer cursed, boolean forceit)
{
  integer   wpn_type;

  if (mt__magik(chance) || forceit) {
    t_list[x].tohit = mt__m_bonus(0,40,level,forceit);
    t_list[x].todam = mt__m_bonus(0,40,level,forceit);
    if (mt__magik(special)) {
      if ((t_list[x].subval == 99) && (randint(5)==1)) { /* filthy rags? */
	strcat(t_list[x].name," of Trollkind");
	t_list[x].flags |= (Charisma_worn_bit     |
			    Searching_worn_bit    |
			    Stealth_worn_bit      |
			    Regeneration_worn_bit |
			    Resist_Acid_worn_bit  |
			    Resist_Cold_worn_bit);
	t_list[x].p1   = -5;
	t_list[x].cost = 120000;
	strcpy(t_list[x].damage, "3d4");
      } else {
	wpn_type = randint(100);
	if (wpn_type < 61) {
	  mt__magic_sword(x);     /* FT  FB  WB  BB     */
	} else if (wpn_type < 81) {
	  mt__slaying_sword(x);   /* SM  SD  SU  SR     */
	}  else if (wpn_type < 96) {
	  mt__ego_sword(x);       /* HA  DF  DB  SS  V  */
	} else {
	  mt__magic_sword(x);     /* FT  FB  WB  BB     */
	  mt__ego_sword(x);       /* HA  DF  DB  SS  V  */
	}
      }
    }
  } else if (mt__magik(cursed)) {
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].tohit  = -mt__m_bonus(1,55,level,forceit);
    t_list[x].todam  = -mt__m_bonus(1,55,level,forceit);
    t_list[x].cost   = 0;
  }
};
//////////////////////////////////////////////////////////////////////
void mt__bows_and_slings(integer x,integer level, integer chance,
			 integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    t_list[x].tohit = mt__m_bonus(1,30,level,forceit);
    if (mt__magik(special)) {
      strcat(t_list[x].name," of Criticals");
      t_list[x].flags2 |= Sharp_worn_bit;
      t_list[x].tohit  += 5;
      t_list[x].cost   += 300000;
    }
  } else if (mt__magik(cursed)) {
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].tohit  = -mt__m_bonus(1,50,level,forceit);
    t_list[x].cost   = 0;                     
  }
};
//////////////////////////////////////////////////////////////////////
void mt__pick_or_shovel(integer x,integer level, integer chance,
			integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    switch (randint(3)) {
    case 1:
    case 2:  /*{25}*/
      t_list[x].p1    = mt__m_bonus(2,25,level,forceit);
      t_list[x].cost += t_list[x].p1*10000;
      break;
    case 3:
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].p1     = -mt__m_bonus(1,30,level,forceit);
      t_list[x].cost   = 0;
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void mt__gloves_and_gauntlets(integer x,integer level, integer chance,
			      integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    t_list[x].toac = mt__m_bonus(1,20,level,forceit);
    if (mt__magik(special)) {
      if ((t_list[x].subval == 5) && (randint(10)==1)) {
	/* gloves_and_gauntlets 5 is "Set of Cloth Gloves" */
	strcat(t_list[x].name," of the Hive");
	t_list[x].flags |= Dexterity_worn_bit;
	t_list[x].p1     = 2;
	t_list[x].cost  += 50000;
      } else {
	switch (randint(5)) {
	case 1  :
	  strcat(t_list[x].name," of Free Action");
	  t_list[x].flags |= Free_Action_worn_bit;
	  t_list[x].cost  += 100000;
	  break;
	case 2  :
	  strcat(t_list[x].name," of Slaying");
	  t_list[x].tohit  = 1 + randint(3);
	  t_list[x].todam  = 1 + randint(3);
	  t_list[x].cost  += (t_list[x].tohit+t_list[x].todam)*25000;
	  break;
	case 3 :
	  strcat(t_list[x].name," of Thievery (%P1)");
	  t_list[x].flags2 |= Disarm_worn_bit;
	  t_list[x].flags  |= (Feather_Fall_worn_bit   |
			       See_Invisible_worn_bit);
	  t_list[x].p1      = mt__m_bonus(5,50,level,forceit);
	  t_list[x].cost   += 20000 + t_list[x].p1*5;
	  break;
	case 4 :
	case 5 :
	  strcat(t_list[x].name," of Ogre Power");
	  t_list[x].flags |= (Slow_Digestion_worn_bit  |
			      Strength_worn_bit);
	  t_list[x].p1     = randint(4);
	  t_list[x].cost  += 150000;
	  break;
	} /* end case */
      } /* end else */ 
    } /* end if special */
  } else if (mt__magik(cursed)) {
    if (mt__magik(special)) {
      switch (randint(3)) {
      case 1 :
	strcat(t_list[x].name," of Clumsiness");
	t_list[x].flags |= (Cursed_worn_bit      |
			    Dexterity_worn_bit);
	t_list[x].p1     = 1;
	break;
      case 2 : 
	strcat(t_list[x].name," of Weakness");
	t_list[x].flags |= (Cursed_worn_bit     |
			    Strength_worn_bit);
	t_list[x].p1     = 1;
	break;
      case 3 : 
	strcat(t_list[x].name," of Ogre Intelligence");
	t_list[x].flags |= (Cursed_worn_bit       |
			    Intelligence_worn_bit);
	t_list[x].p1     = 1;
	break;
      } /* end case*/
    } /* end if special cursed */
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].toac   = -mt__m_bonus(1,40,level,forceit);
    t_list[x].p1     = -mt__m_bonus(1,10,level,forceit);
    t_list[x].cost   = 0;
  } /* end if cursed */
};
//////////////////////////////////////////////////////////////////////
void mt__boots(integer x,integer level, integer chance,
	       integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    t_list[x].toac = mt__m_bonus(1,20,level,forceit);
    if (mt__magik(special)) {
      switch (randint(16)) {
      case 1 :
	strcat(t_list[x].name," of Speed");
	t_list[x].flags |= Speed_worn_bit;
	t_list[x].p1     = 1;
	t_list[x].cost  += 500000;
	break;
      case 2:
      case 3:
      case 4:
      case 5:
	strcat(t_list[x].name," of Stealth");
	t_list[x].flags |= Stealth_worn_bit;
	t_list[x].cost  += 50000;
	break;
      default:
	if ((t_list[x].subval == 4) && (randint(6)==1)) {/* Pair of Sandals */
	  strcat(t_list[x].name," of Dryadkind");
	  t_list[x].flags  |= (Charisma_worn_bit      |
			       Feather_Fall_worn_bit  |
			       See_Invisible_worn_bit |
			       Free_Action_worn_bit);
	  t_list[x].flags2 |= (Magic_proof_worn_bit);
	  t_list[x].p1      = 3;
	  t_list[x].cost    = 1; /*{see magi item}*/
	  break;
	}
      }/* end switch */
    } else {
      /* not special */
      strcat(t_list[x].name," of Slow descent");
      t_list[x].flags |= Feather_Fall_worn_bit;
      t_list[x].cost  += 25000;
    }
  } else if (mt__magik(cursed)) {
    
    t_list[x].cost = 0;
    t_list[x].ac   = -mt__m_bonus(2,45,level,forceit);
    
    switch (randint(3)) {
    case 1 :
      strcat(t_list[x].name," of Slowness");
      t_list[x].flags |= (Cursed_worn_bit  |
			  Speed_worn_bit);
      t_list[x].p1     = -1;
      break;
    case 2 : 
      strcat(t_list[x].name," of Noise");
      t_list[x].flags |= (Cursed_worn_bit        |
			  Aggravation_worn_bit);
      break;
    case 3 : 
      strcat(t_list[x].name," of Great Mass");
      t_list[x].flags  |= Cursed_worn_bit;
      t_list[x].weight *= 5;
      break;
    } /* end switch */
  } /* end if cursed */
};
//////////////////////////////////////////////////////////////////////
void mt__helms(integer x,integer level,integer chance,
	       integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    t_list[x].toac = mt__m_bonus(1,20,level,forceit);
    if (mt__magik(special))
      switch (t_list[x].subval) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
	switch (randint(3)) {
	case 1 : 
	  strcat(t_list[x].name," of Intelligence");
	  t_list[x].flags |= Intelligence_worn_bit;
	  t_list[x].p1     = randint(2);
	  t_list[x].cost  += t_list[x].p1*50000;
	  break;
	case 2 : 
	  strcat(t_list[x].name," of Wisdom");
	  t_list[x].flags |= Wisdom_worn_bit;
	  t_list[x].p1     = randint(2);
	  t_list[x].cost  += t_list[x].p1*50000;
	  break;
	case 3 :
	  strcat(t_list[x].name," of Infra-Vision");
	  t_list[x].flags |= Infra_Vision_worn_bit;
	  t_list[x].p1     = 1 + randint(4);
	  t_list[x].cost  += t_list[x].p1*25000;
	  break;
	} /* end rand switch */
	break;
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
	switch (randint(6)) {
	case 1 :
	  strcat(t_list[x].name," of Might");
	  t_list[x].flags |= (Free_Action_worn_bit  |
			      Constitution_worn_bit |
			      Strength_worn_bit     |
			      Dexterity_worn_bit);
	  t_list[x].p1     = randint(3);
	  t_list[x].cost  += 100000 + t_list[x].p1*50000;
	  break;
	case 2 :
	  strcat(t_list[x].name," of Lordliness");
	  t_list[x].flags |= (Wisdom_worn_bit       |
			      Charisma_worn_bit);
	  t_list[x].p1     = randint(3);
	  t_list[x].cost  += 100000 + t_list[x].p1*50000;
	  break;
	case 3 :
	  strcat(t_list[x].name," of the Magi");
	  t_list[x].flags |= (Free_Action_worn_bit |
			      Strength_worn_bit          |
			      Constitution_worn_bit      |
			      Dexterity_worn_bit);
	  t_list[x].p1     = randint(3);
	  t_list[x].cost  += 300000 + t_list[x].p1*50000;
	  break;
	case 4 :
	  strcat(t_list[x].name," of Beauty");
	  t_list[x].flags |= Charisma_worn_bit;
	  t_list[x].p1     = randint(3);
	  t_list[x].cost  += 75000;
	  break;
	case 5 :
	  strcat(t_list[x].name," of Seeing");
	  t_list[x].flags |= (See_Invisible_worn_bit   |
			      Searching_worn_bit);
	  t_list[x].p1     = 1 + randint(4);
	  t_list[x].cost  += 100000 + t_list[x].p1*10000;
	  break;
	case 6 :
	  strcat(t_list[x].name," of Regeneration");
	  t_list[x].flags |= Regeneration_worn_bit;
	  t_list[x].cost  += 150000;
	  break;
	} /* end rand switch */
	break;
      case 11 :
	strcat(t_list[x].name," of Hobbitkind");
	t_list[x].flags |= (Infra_Vision_worn_bit   |
			    See_Invisible_worn_bit  |
			    Free_Action_worn_bit    |
			    Searching_worn_bit);
	t_list[x].p1     = 5;
	t_list[x].cost  += 170000;
	break;
      } /* end switch subval */
  } else if (mt__magik(cursed)) {
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].toac   = -mt__m_bonus(1,45,level,forceit);
    t_list[x].cost   = 0;
    if (mt__magik(special)) {
      switch (randint(15)) {
      case 1:
      case 2: 
	strcat(t_list[x].name," of Stupidity");
	t_list[x].flags |= Intelligence_worn_bit;
	t_list[x].p1     = -1;
	break;
      case 3:
      case 4: 
	strcat(t_list[x].name," of Dullness");
	t_list[x].flags |= Wisdom_worn_bit;
	t_list[x].p1     = -1;
	break;
      case 5:
      case 6:
	strcat(t_list[x].name," of Blindness");
	t_list[x].flags |= Blindness_worn_bit;
	break;
      case 7:
      case 8: 
	strcat(t_list[x].name," of Timidness");
	t_list[x].flags |= Timidness_worn_bit;
	break;
      case 9:
      case 10: 
	strcat(t_list[x].name," of Weakness");
	t_list[x].flags |= Strength_worn_bit;
	t_list[x].p1     = -1;
	break;
      case 11:
      case 12:
	strcat(t_list[x].name," of Teleportation");
	t_list[x].flags |= Teleportation_worn_bit;
	break;
      case 13:
      case 14:
	strcat(t_list[x].name," of Ugliness");
	t_list[x].flags |= Charisma_worn_bit;
	t_list[x].p1     = -1;
	break;
      case 15 : 
	strcat(t_list[x].name," of **TOTAL DOOM**");
	t_list[x].flags  |= (Cursed_worn_bit         |
			     Strength_worn_bit       |
			     Dexterity_worn_bit      |
			     Constitution_worn_bit   |
			     Intelligence_worn_bit   |
			     Wisdom_worn_bit         |
			     Charisma_worn_bit       |
			     Stealth_worn_bit        |  
			     Aggravation_worn_bit    |
			     Teleportation_worn_bit  |
			     Blindness_worn_bit      |
			     Timidness_worn_bit);
	t_list[x].flags2 |= (Hunger_worn_bit         | 
			     Known_cursed_bit);
	t_list[x].p1      = -5;
	break;
      } /* end rand switch */
      t_list[x].p1 *= randint(5);
    }
  }
};
//////////////////////////////////////////////////////////////////////
void mt__belt(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    t_list[x].toac = mt__m_bonus(1,20,level,forceit);
    if (mt__magik(special)) {
      switch (t_list[x].subval) {

      case 1 : /* Girdle */
	t_list[x].flags2 |= Increase_carry_worn_bit;

	switch (randint(16)) {
	case 1 :
	  if (randint(3) == 1) {
	    strcat(t_list[x].name," of Titan Strength");
	    t_list[x].flags  |= (Resist_Lightning_worn_bit  |
				 Resist_Fire_worn_bit       |
				 Resist_Cold_worn_bit       |
				 Resist_Acid_worn_bit       |
				 Regeneration_worn_bit      |
				 Free_Action_worn_bit);
	    t_list[x].flags2 |= Magic_proof_worn_bit;
	    t_list[x].p1      = 7;
	    t_list[x].cost   += 7500000;
	  } else {
	    strcat(t_list[x].name," of Storm Giant Strength");
	    t_list[x].flags  |= (Resist_Lightning_worn_bit  |
				 Resist_Acid_worn_bit);
	    t_list[x].flags2 |= (Magic_proof_worn_bit);
	    t_list[x].p1      = 6;
	    t_list[x].cost   += 3500000;
	  }
	  break;
	case 2 :
	  strcat(t_list[x].name," of Cloud Giant Strength");
	  t_list[x].flags |= (Resist_Lightning_worn_bit  |
			      Resist_Acid_worn_bit);
	  t_list[x].p1     = 5;
	  t_list[x].cost  += 2000000;
	  break;
	case 3:
	case 4:
	  strcat(t_list[x].name," of Fire Giant Strength");
	  t_list[x].flags |= Resist_Fire_worn_bit;
	  t_list[x].p1     = 4;
	  t_list[x].cost  += 1750000;
	  break;
	case 5:
	case 6:
	case 7:
	  strcat(t_list[x].name," of Frost Giant Strength");
	  t_list[x].flags |= Resist_Cold_worn_bit;
	  t_list[x].p1     = 3;
	  t_list[x].cost  += 1250000;
	  break;
	case 8:
	case 9:
	case 10:
	case 11:
	  strcat(t_list[x].name," of Stone Giant Strength");
	  t_list[x].p1    = 2;
	  t_list[x].cost += 800000;
	  break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16: 
	  strcat(t_list[x].name," of Hill Giant Strength");
	  t_list[x].p1    = 1;
	  t_list[x].cost += 600000;
	  break;
	} /* end rand switch 16 */
	t_list[x].tohit = t_list[x].p1; 
	t_list[x].todam = t_list[x].p1;
	break;

      case 10:   /* Silver Belt Buckle, Gold Belt Buckle */
      case 11:
	switch (randint(2)) {
	case 1 :
	  strcat(t_list[x].name," of Deflection");
	  t_list[x].flags2 |= Magic_proof_worn_bit;
	  t_list[x].toac   += randint(5);
	  t_list[x].cost   += t_list[x].toac*20000;
	  break;
	case 2 :
	  strcat(t_list[x].name," of Improved Digestion");
	  t_list[x].flags |= (Sustain_Stat_worn_bit     |
			      Slow_Digestion_worn_bit);
	  t_list[x].p1     = 2;
	  t_list[x].cost  += 100000;
	  break;
	} /* end rand switch */
	break;

      case 13 : /* Leather Belt */
	strcat(t_list[x].name," of Dwarvenkind");
	t_list[x].flags  |= (Infra_Vision_worn_bit  |
			     Tunneling_worn_bit     |
			     Sustain_Stat_worn_bit);
	t_list[x].flags2 |= Magic_proof_worn_bit;
	t_list[x].p1      = 2;
	t_list[x].cost   += 70000;
	break;
      } /* end subval switch */
    }
  } else if (mt__magik(cursed)) {
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].toac   = -mt__m_bonus(1,45,level,forceit);
    t_list[x].cost   = 0;
    if (mt__magik(special)) {
      switch (t_list[x].subval) {
      case 1 : /* Girdle */
	switch (randint(2)) {
	case 1 :
	  strcat(t_list[x].name," of Sex Change");
	  t_list[x].flags |= Charisma_worn_bit;
	  t_list[x].p1     = -2;
	  break;
	case 2 :
	  strcat(t_list[x].name," of Weakness");
	  t_list[x].flags |= Strength_worn_bit;
	  t_list[x].p1     = -1;
	  break;
	} /* end rand switch */
	break;

      case 10: /* Silver Belt Buckle, Gold Belt Buckle */
      case 11:
	strcat(t_list[x].name," of Fear");
	t_list[x].flags |= (Cursed_worn_bit      |
			    Timidness_worn_bit);
	t_list[x].p1     = -1;
	break;
      case 13 : /* Leather Belt */
	strcat(t_list[x].name," of Hunger");
	t_list[x].flags  |= Cursed_worn_bit;
	t_list[x].flags2 |= Hunger_worn_bit;
	t_list[x].p1      = -1;
	break;
      } /* end switch */
      t_list[x].p1 *= randint(5);
    }
  }
};
//////////////////////////////////////////////////////////////////////
void mt__ring(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  switch (t_list[x].subval) {

  case 1: /* stat rings */
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].p1     = -mt__m_bonus(1,20,level,forceit);
      t_list[x].cost  *= -1;
    } else {
      t_list[x].p1     = mt__m_bonus(1,10,level,forceit);
      t_list[x].cost  += t_list[x].p1*10000;
    }
    break;

  case 7 : /* speed */
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].p1     = -randint(3);
      t_list[x].cost  *= -1;
    } else {
      t_list[x].p1     = 1;
      if (mt__magik(special)) {
	if (randint(100) == 1) {  /* added a rare, faster ring STK */
	  t_list[x].p1     = 2;
	  t_list[x].cost  *= 10;
	}
      }
    }
    break;

  case 8  :  /* searching */
    t_list[x].p1       = 5*mt__m_bonus(1,20,level,forceit);
    t_list[x].cost    += t_list[x].p1*10000;
    break;

  case 22 :/*	{ Increase damage	}*/
    t_list[x].todam    = mt__m_bonus(1,20,level,forceit);
    t_list[x].cost    += t_list[x].todam*10000;
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].todam *= -1;
      t_list[x].cost  *= -1;
    }
    break;

  case 23 :/*	{ Increase To-Hit	}*/
    t_list[x].tohit    = mt__m_bonus(1,20,level,forceit);
    t_list[x].cost    += t_list[x].todam*10000; /* XXXX seems like tohit... */

    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].tohit *= -1;
      t_list[x].cost  *= -1;
    }
    break;

  case 24 :/*	{ Protection		}*/
    t_list[x].toac     = mt__m_bonus(1,20,level,forceit);
    t_list[x].cost    += t_list[x].todam*10000; /* XXXX seems like toac... */
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].toac  *= -1;
      t_list[x].cost  *= -1;
    }
    break;

  case 33 :/*	{ Slaying	}*/
    t_list[x].todam  = mt__m_bonus(1,25,level,forceit);
    t_list[x].tohit  = mt__m_bonus(1,25,level,forceit);
    t_list[x].cost  += (t_list[x].tohit+t_list[x].todam)*10000;
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].tohit *= -1;
      t_list[x].todam *= -1;
      t_list[x].cost  *= -1;
    }
    break;

  case 35 :/*   { Speed -10 or worse }*/
    t_list[x].p1    = -(10+randint(10));
    t_list[x].cost += (1000000*t_list[x].p1);
    if ((0x80000000 & t_list[x].flags) != 0) { 
      t_list[x].flags &= 0x7FFFFFFF; /* XXXX what does this bit mean? */
    }
    break;

  default:
    break;
  }
};
//////////////////////////////////////////////////////////////////////
void mt__amulet(integer x,integer level,integer chance,
		integer special, integer cursed, boolean forceit)
{
  switch (t_list[x].subval) {

  case 1:            /* 1,2,3,4 not found, 5=wisdom, 6=charisma */
  case 2:            /* I assume the others are for other stats */
  case 3:
  case 4:
  case 5:
  case 6:  
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].p1     = -mt__m_bonus(1,20,level,forceit);
      t_list[x].cost  *= -1;
    } else {
      t_list[x].p1    = mt__m_bonus(1,10,level,forceit);
      t_list[x].cost += t_list[x].p1*10000;
    }
    break;

  case 7  :	/* Searching */
    t_list[x].p1 = 5*mt__m_bonus(1,25,level,forceit);
    if (mt__magik(cursed)) {
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].p1    *= -1;
      t_list[x].cost  *= -1;
    } else {
      t_list[x].cost  += 10000*t_list[x].p1;
    }
    break;

  default:
  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
void mt__lamp_or_torch(integer x,integer level,integer chance,
		       integer special, integer cursed, boolean forceit)
{
  /*{ Subval should be even for store, odd for dungeon}*/
  /*{ Dungeon found ones will be partially charged    }*/

  if ((t_list[x].subval & 1) == 1) {
    t_list[x].p1 = randint(t_list[x].p1);
  }
};
//////////////////////////////////////////////////////////////////////
void mt__wand(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  integer   p1;

  switch (t_list[x].subval) {
  case 1   :    p1 = randint(10) +  6;  break;
  case 2   :    p1 = randint(8)  +  6;  break;
  case 3   :    p1 = randint(5)  +  6;  break;
  case 4   :    p1 = randint(8)  +  6;  break;
  case 5   :    p1 = randint(4)  +  3;  break;
  case 6   :    p1 = randint(8)  +  6;  break;
  case 7   :    p1 = randint(20) + 12;  break;
  case 8   :    p1 = randint(20) + 12;  break;
  case 9   :    p1 = randint(10) +  6;  break;
  case 10  :    p1 = randint(12) +  6;  break;
  case 11  :    p1 = randint(10) + 12;  break;
  case 12  :    p1 = randint(3)  +  3;  break;
  case 13  :    p1 = randint(8)  +  6;  break;
  case 14  :    p1 = randint(10) +  6;  break;
  case 15  :    p1 = randint(5)  +  3;  break;
  case 16  :    p1 = randint(5)  +  3;  break;
  case 17  :    p1 = randint(5)  +  6;  break;
  case 18  :    p1 = randint(5)  +  4;  break;
  case 19  :    p1 = randint(8)  +  4;  break;
  case 20  :    p1 = randint(6)  +  2;  break;
  case 21  :    p1 = randint(4)  +  2;  break;
  case 22  :    p1 = randint(8)  +  6;  break;
  case 23  :    p1 = randint(5)  +  2;  break;
  case 24  :    p1 = randint(12) + 12;  break;
  case 25  :    p1 = randint(20) + 10;  break;
  default  :
    MSG("Unknown subval in mt__wand");
    p1 = 0;
    break;
  }

  if (forceit) {
    p1 += 8;
  }
  t_list[x].p1 = p1;

};
//////////////////////////////////////////////////////////////////////
void mt__staff(integer x,integer level,integer chance,
	       integer special, integer cursed, boolean forceit)
{
  integer   p1;

  switch (t_list[x].subval) {
  case 1   :    p1 = randint(20) + 12;  break;
  case 2   :    p1 = randint(8)  +  6;  break;
  case 3   :    p1 = randint(5)  +  6;  break;
  case 4   :    p1 = randint(20) + 12;  break;
  case 5   :    p1 = randint(15) +  6;  break;
  case 6   :    p1 = randint(4)  +  5;  break;
  case 7   :    p1 = randint(5)  +  3;  break;
  case 8   :    p1 = randint(3)  +  1;  break;
  case 9   :    p1 = randint(3)  +  1;  break;
  case 10  :    p1 = randint(3)  +  1;  break;
  case 11  :    p1 = randint(5)  +  6;  break;
  case 12  :    p1 = randint(10) + 12;  break;
  case 13  :    p1 = randint(5)  +  6;  break;
  case 14  :    p1 = randint(5)  +  6;  break;
  case 15  :    p1 = randint(5)  +  6;  break;
  case 16  :    p1 = randint(10) + 12;  break;
  case 17  :    p1 = randint(3)  +  4;  break;
  case 18  :    p1 = randint(5)  +  6;  break;
  case 19  :    p1 = randint(5)  +  6;  break;
  case 20  :    p1 = randint(3)  +  4;  break;
  case 21  :    p1 = randint(10) + 12;  break;
  case 22  :    p1 = randint(3)  +  4;  break;
  case 23  :    p1 = randint(3)  +  4;  break;
  case 24  :    p1 = randint(3)  +  1;  break;
  case 25  :    p1 = randint(10) +  6;  break;
  case 26  :    p1 = randint(6)  +  6;  break;
  default  :
    MSG("Unknown subval in mt__staff");
    p1 = 0;
    break;
  }

  if (forceit) {
    p1 += 5;
  }
  t_list[x].p1 = p1;
};
//////////////////////////////////////////////////////////////////////
void mt__chime(integer x,integer level,integer chance,
	       integer special, integer cursed, boolean forceit)
{
  integer   p1;

  switch (t_list[x].subval) {
  case 1   :   p1 = randint(20) + 12;  break;
  case 2   :   p1 = randint(8)  +  6;  break;
  case 3   :   p1 = randint(5)  +  6;  break;
  case 4   :   p1 = randint(4)  +  5;  break;
  case 5   :   p1 = randint(5)  +  3;  break;
  case 6   :   p1 = randint(3)  +  1;  break;
  case 7   :   p1 = randint(10) + 10;  break;
  case 8   :   p1 = randint(10) + 12;  break;
  case 9   :   p1 = randint(5)  +  6;  break;
  case 10  :   p1 = randint(5)  +  6;  break;
  case 11  :   p1 = randint(5)  +  6;  break;
  case 12  :   p1 = randint(5)  +  6;  break;
  case 13  :   p1 = randint(3)  +  4;  break;
  case 14  :   p1 = randint(3)  +  4;  break;
  case 15  :   p1 = randint(3)  +  4;  break;
  case 16  :   p1 = randint(10) +  6;  break;
  default  :
    MSG("Unknown subval in mt__chime");
    p1 = 0;
    break;
  }

  if (forceit) {
    p1 += 5;
  }
  t_list[x].p1 = p1;
};
//////////////////////////////////////////////////////////////////////
void mt__horn(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  integer   p1;

  switch (t_list[x].subval) {
  case 1   :   p1 = randint(10) + 6;  break;
  case 2   :   p1 = randint(6)  + 3;  break;
  case 3   :   p1 = randint(5)  + 6;  break;
  case 4   :   p1 = randint(3)  + 1;  break;
  case 5   :   p1 = randint(3)  + 4;  break;
  case 6   :   p1 = randint(3)  + 4;  break;
  case 7   :   p1 = randint(3)  + 4;  break;
  case 8   :   p1 = randint(10) + 3;  break;
  case 9   :   p1 = randint(5)  + 1;  break;
  case 10  :   p1 = randint(3)  + 1;  break;
  case 11  :   p1 = randint(3)  + 4;  break;
  case 12  :   p1 = randint(3)  + 4;  break;
  case 13  :   p1 = randint(8)  + 1;  break;
  default  :   
    MSG("Unknown subval in mt__horn");
    p1 = 0;
    break;
  }

  if (forceit) {
    p1 += 5;
  }
  t_list[x].p1 = p1;
};
//////////////////////////////////////////////////////////////////////
void mt__cloak(integer x,integer level,integer chance,
	       integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    if (mt__magik(special)) {
      switch (randint(9)) {
      case 1:
      case 2:
      case 3:
      case 4: 
	strcat(t_list[x].name," of Protection");
	t_list[x].toac  = mt__m_bonus(2,40,level,forceit);
	t_list[x].cost += 25000 + t_list[x].toac*10000;
	break;
      case 5:
      case 6:
      case 7:
      case 8: 
	strcat(t_list[x].name," of Stealth (%P1)");
	t_list[x].flags |= Stealth_worn_bit;
	t_list[x].toac   = mt__m_bonus(1,20,level,forceit);
	t_list[x].p1     = randint(3);
	t_list[x].cost  += t_list[x].p1*50000 + t_list[x].toac*10000;
	break;
      case 9: 
	strcat(t_list[x].name," of Elvenkind");
	t_list[x].flags |= (See_Invisible_worn_bit |
			    Sustain_Stat_worn_bit  |
			    Stealth_worn_bit       |
			    Charisma_worn_bit);
	t_list[x].p1     = 2; 
	t_list[x].cost  += 200000;
	break;
      } /* end switch */
    } else {
      /* not special */
      t_list[x].toac  = mt__m_bonus(1,20,level,forceit);
      t_list[x].cost += t_list[x].toac+10000;
    }
  } else if (mt__magik(cursed)) {
    switch (randint(3)) {
    case 1 : 
      strcat(t_list[x].name," of Irritation");
      t_list[x].flags |= (Cursed_worn_bit       |
			  Aggravation_worn_bit);
      t_list[x].ac     =  0;
      t_list[x].toac   = -mt__m_bonus(1,10,level,forceit);
      t_list[x].tohit  = -mt__m_bonus(1,10,level,forceit);
      t_list[x].todam  = -mt__m_bonus(1,10,level,forceit);
      t_list[x].cost   =  0;
      break;
    case 2 :
      strcat(t_list[x].name," of Vulnerability");
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].ac     = 0;
      t_list[x].toac   = -mt__m_bonus(10,100,level+50,forceit);
      t_list[x].cost   = 0;
      break;
    case 3 : 
      strcat(t_list[x].name," of Enveloping");
      t_list[x].flags |= Cursed_worn_bit;
      t_list[x].toac   = -mt__m_bonus(1,10,level,forceit);
      t_list[x].tohit  = -mt__m_bonus(2,40,level+10,forceit);
      t_list[x].todam  = -mt__m_bonus(2,40,level+10,forceit);
      t_list[x].cost   = 0;
      break;
    } /* end switch */
  }
};
//////////////////////////////////////////////////////////////////////
void mt__chest(integer x,integer level,integer chance,
	       integer special, integer cursed, boolean forceit)
{
  if (t_list[x].subval == 5) {
    strcat(t_list[x].name,"^ (Looted)");   /* dead human body */
  } else {
    switch (randint(level)+4) {
    case 1		:
      strcat(t_list[x].name,"^ (Empty)");
      break;
    case 2		:
      strcat(t_list[x].name,"^ (Locked)");
      t_list[x].flags |= 0x00000001;
      break;
    case 3:
    case 4: 
      strcat(t_list[x].name,"^ (Poison Needle)");
      t_list[x].flags |= 0x00000011;
      break;
    case 5:
    case 6: 
      strcat(t_list[x].name,"^ (Poison Needle)");
      t_list[x].flags |= 0x00000021;
      break;
    case 7:
    case 8:
    case 9: 
      strcat(t_list[x].name,"^ (Gas Trap)");
      t_list[x].flags |= 0x00000041;
      break;
    case 10:
    case 11: 
      strcat(t_list[x].name,"^ (Explosion Device)");
      t_list[x].flags |= 0x00000081;
      break;
    case 12:
    case 13:
    case 14: 
      strcat(t_list[x].name,"^ (Summoning Runes)");
      t_list[x].flags |= 0x00000101;
      break;
    case 15:
    case 16:
    case 17: 
      strcat(t_list[x].name,"^ (Multiple Traps)");
      t_list[x].flags |= 0x00000071;
      break;
    default:
      strcat(t_list[x].name,"^ (Multiple Traps)");
      t_list[x].flags |= 0x00000181;
      break;
    }
  }
};
//////////////////////////////////////////////////////////////////////
void mt__ammo(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  integer   i1;

  if ((t_list[x].tval == bolt) || (t_list[x].tval == arrow)) {
    if (mt__magik(chance) || forceit) {
      t_list[x].tohit = mt__m_bonus(1,35,level,forceit);
      t_list[x].todam = mt__m_bonus(1,35,level,forceit);

      if (mt__magik(special)) {
	switch (t_list[x].tval) {

	case sling_ammo:
	  break;

	case spike:
	  break;

	case arrow:
	case bolt:  
	  switch (randint(10)) {
	    
	  case 1:
	  case 2:
	  case 3:
	    strcat(t_list[x].name," of Slaying");
	    t_list[x].tohit += 5;
	    t_list[x].todam += 5;
	    t_list[x].cost  += 2000;
	    break;
	  case 4:
	  case 5:
	    strcat(t_list[x].name," of Fire");
	    t_list[x].flags |= Flame_Brand_worn_bit;
	    t_list[x].tohit += 2;
	    t_list[x].todam += 4;
	    t_list[x].cost  += 2500;
	    break;
	  case 6:
	  case 7:
	    strcat(t_list[x].name," of Slay Evil");
	    t_list[x].flags |= Slay_Evil_worn_bit;
	    t_list[x].tohit += 3;
	    t_list[x].todam += 3;
	    t_list[x].cost  += 2500;
	    break;
	  case 8:
	  case 9:
	    strcat(t_list[x].name," of Slay Monster");
	    t_list[x].flags |= Slay_Monster_worn_bit;
	    t_list[x].tohit += 2;
	    t_list[x].todam += 2;
	    t_list[x].cost  += 3000;
	    break;
	  case 10:
	    strcat(t_list[x].name," of Dragon Slaying");
	    t_list[x].flags |= Slay_Dragon_worn_bit;
	    t_list[x].tohit += 10;
	    t_list[x].todam += 10;
	    t_list[x].cost  += 3500;
	    break;
	  } /* end switch randint */
	  
	default:  break;
	} /* end switch tval */
      } /* end special */
    } /* end chance */	
  } else if (mt__magik(cursed)) { /* end if chance */
    t_list[x].flags |= Cursed_worn_bit;
    t_list[x].tohit  = -mt__m_bonus(5,55,level,forceit);
    t_list[x].todam  = -mt__m_bonus(5,55,level,forceit);
    t_list[x].cost   = 0;
  }

  t_list[x].number = forceit ? 15 : 0;
  for (i1 = 0; i1 < 7; i1++) { 
    t_list[x].number += randint(6);
  }

  missle_ctr++;
  if (missle_ctr > 65534) {
    missle_ctr = 1;
  }

  t_list[x].subval = missle_ctr + 512;
};
//////////////////////////////////////////////////////////////////////
void mt__food(integer x,integer level,integer chance,
	      integer special, integer cursed, boolean forceit)
{
  if (mt__magik(chance) || forceit) {
    if (mt__magik(special)) {
      if (mt__magik(cursed)) {
	if (t_list[x].subval == 319) {  /* Eyeballs of Unhealth */
	  t_list[x] = yums[18];         /* Eyeballs of Drong */
	  t_list[x].weight *= WEIGHT_ADJ;
	  t_list[x].cost   *= COST_ADJ;
	}
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////
void mt__get_chances(integer level, integer *chance,
		     integer *special, integer *cursed, boolean forceit)
{
  /*{ Chance of treasure having magic abilities		-RAK-	}*/
  /*{ Chance increases with each dungeon level			}*/
  
  *chance = obj_base_magic +(level*(obj_base_max-obj_base_magic)) div 100;
  
  if (*chance > obj_base_max) {
    *chance = obj_base_max;
  }

  *special = trunc(*chance/obj_div_special);
  *cursed  = forceit ? 0 : trunc(*chance/obj_div_cursed);

};
//////////////////////////////////////////////////////////////////////
void magic_treasure(integer x,integer level, boolean forceit)
{
  integer    chance, special, cursed;

  mt__get_chances(level, &chance, &special, &cursed, forceit);

  /*{ Depending on treasure type, it can have certain magical properties}*/

  switch (t_list[x].tval) {

    /*{ Money money everywhere }*/
  case valuable_gems :
    mt__gems(x, level, chance, special, cursed, forceit);
    break;

    /*{ Miscellaneous Objects }*/
  case misc_usable :
    mt__misc_usable(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Armor and shields }*/
  case shield:
  case hard_armor:
  case soft_armor:
    mt__armor_and_shields(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Weapons }*/
  case hafted_weapon:
  case pole_arm:
  case sword:
  case dagger:
  case maul:
    mt__weapons(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Bows, crossbows, and slings }*/
  case bow_crossbow_or_sling:
    mt__bows_and_slings(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Digging tools }*/
  case pick_or_shovel :
    mt__pick_or_shovel(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Gloves and Gauntlets }*/
  case gloves_and_gauntlets :
    mt__gloves_and_gauntlets(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Boots }*/
  case boots :
    mt__boots(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Helms }*/
  case helm :
    mt__helms(x, level, chance, special, cursed, forceit);
    break;
    
    /*{girdles, belts and buckles}*/
  case belt :
    mt__belt(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Rings }*/
  case ring :
    mt__ring(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Amulets }*/
  case amulet :
    mt__amulet(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Lamps and torches }*/
  case lamp_or_torch :
    mt__lamp_or_torch(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Wands }*/
  case wand :
    mt__wand(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Staffs }*/
  case staff : 
    mt__staff(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Chimes }*/
  case chime :
    mt__chime(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Horns }*/
  case horn :
    mt__horn(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Cloaks }*/
  case cloak :
    mt__cloak(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Chests }*/
  case chest :
    mt__chest(x, level, chance, special, cursed, forceit);
    break;
    
    /*{ Arrows, bolts, ammo, and spikes }*/
  case sling_ammo:
  case arrow:
  case bolt:
  case spike:
    mt__ammo(x, level, chance, special, cursed, forceit);
    break;

  case Food:
    mt__food(x, level, chance, special, cursed, forceit);
    break;

  } /* end switch */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
