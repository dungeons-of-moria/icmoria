/* desc.c */
/**/

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void magic_init(unsigned long random_seed)
{
  /*	{ Initialize all potions, wands, staves, scrolls, etc...	}*/

  integer i1,tmpv;
  vtype   tmps;

  set_seed(random_seed);
  randes();

  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {

    /*
     * The arrays of the object materals all start at 0.
     * Object subvals start at 1.  When doing the lookup
     * subtract one from subval!
     */
    tmpv = (0xFF & object_list[i1].subval);

    switch (object_list[i1].tval) {
      
    case potion1: case potion2: 
      if (tmpv <= MAX_COLORS) {
	insert_str(object_list[i1].name,"%C",colors[tmpv-1]);
      }
      break;
      
    case scroll1: case scroll2:
      rantitle(tmps);
      insert_str(object_list[i1].name,"%T",tmps);
      break;
      
    case ring: 
      if (tmpv <= MAX_ROCKS) {
	insert_str(object_list[i1].name,"%R",rocks[tmpv-1]);
      }
      break;
      
    case valuable_gems:
      if (tmpv <= MAX_ROCKS) {
	insert_str(object_list[i1].name,"%R",rocks[tmpv-1]);
      }
      break;
      
    case valuable_gems_wear:
      if (tmpv <= MAX_ROCKS) {
	insert_str(object_list[i1].name,"%R",rocks[tmpv-1]);
      }
      break;
      
    case amulet:
      if (tmpv <= MAX_AMULETS) {
	insert_str(object_list[i1].name,"%A",amulets[tmpv-1]);
      }
      break;
      
    case wand:
      if (tmpv <= MAX_METALS) {
	insert_str(object_list[i1].name,"%M",metals[tmpv-1]);
      }
      break;
      
    case chime:
      if (tmpv <= MAX_METALS) {
	insert_str(object_list[i1].name,"%M",metals[tmpv-1]);
      }
      break;
      
    case horn:
      if (tmpv <= MAX_HORNS) {
	insert_str(object_list[i1].name,"%H",horns[tmpv-1]);
      }
      break;
      
    case staff:
      if (tmpv <= MAX_WOODS) {
	insert_str(object_list[i1].name,"%W",woods[tmpv-1]);
      }
      break;
      
    case Food:
      if (tmpv <= MAX_MUSH) {
	insert_str(object_list[i1].name,"%M",mushrooms[tmpv-1]);
      }
      break;
      
    case rod :
      /* what happened to the rods? */
      /*
      if (tmpv <= MAX_RODS) {
	insert_str(object_list[i1].name,"%D",rods[tmpv-1]);
      }
      */
      break;
      
    case bag_or_sack:
      if (tmpv <= MAX_CLOTHS) {
	insert_str(object_list[i1].name,"%N",cloths[tmpv-1]);
      }
      break;
      
    case misc_usable:
      if (tmpv <= MAX_ROCKS) {
	insert_str(object_list[i1].name,"%R",rocks[tmpv-1]);
      }
      if (tmpv <= MAX_WOODS) {
	insert_str(object_list[i1].name,"%W",woods[tmpv-1]);
      }
      if (tmpv <= MAX_METALS) {
	insert_str(object_list[i1].name,"%M",metals[tmpv-1]);
      }
      if (tmpv <= MAX_AMULETS) { 
	insert_str(object_list[i1].name,"%A",amulets[tmpv-1]);
      }
      break;
    default:
      break;
    } /* end switch */
  } /* end for */

#if DO_DEBUG && 0
  for (i1 = 1; i1 <= MAX_OBJECTS; i1++) {
    fprintf(debug_file,": object_list[%ld] = %s\n", 
	    i1, object_list[i1].name);
  }
  fflush(debug_file);
#endif
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void r__randdesloop(atype str_array[], int count)
{
  integer   i1,i2;
  vtype     tmp;

  for (i1 = 0; i1 < count; i1++) {
	  i2 = randint(count)-1;
	  if (i1 != i2) {
#if DO_DEBUG && 0
	    fprintf(debug_file,"%2ld:%2ld \"%s\" swapped with \"%s\"\n",
		    i1,i2,str_array[i1],str_array[i2]);
#endif
	    strcpy(tmp, str_array[i1]);
	    strcpy(str_array[i1], str_array[i2]);
	    strcpy(str_array[i2], tmp);
	  }
  }
  
};

//////////////////////////////////////////////////////////////////////

void randes()
{
  /*{ Randomize colors, woods, and metals				}*/

  r__randdesloop( colors,    MAX_COLORS);
  r__randdesloop( woods,     MAX_WOODS);
  r__randdesloop( metals,    MAX_METALS);
  r__randdesloop( horns,     MAX_HORNS);
  r__randdesloop( rocks,     MAX_ROCKS);
  r__randdesloop( amulets,   MAX_AMULETS);
  r__randdesloop( mushrooms, MAX_MUSH);
  r__randdesloop( cloths,    MAX_CLOTHS);

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void rantitle(char *title)
{
  /*{ Return random title						}*/

  integer i1,i2,i3,i4;

  i3 = randint(2) + 1;             /* two or three words */
  strcpy(title, "Titled \"");

  for (i1 = 0; i1 < i3; i1++) {
    i4 = randint(2);               /* one or two syllables each */
    for (i2 = 0; i2 < i4; i2++) {
      strcat(title, syllables[randint(MAX_SYLLABLES)-1]);
    }

    if (i1 != i3-1) {
      strcat(title," ");
    }
  }
  strcat(title,"\"");

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void identify(treasure_type *item)
{
  /*{ Something has been identified					}*/
  
  integer    i1,x1,x2;
  treas_ptr  curse;
  
  x1 = item->tval;
  x2 = item->subval;
  
  if (strstr(item->name,"|") != NULL) {
    for (i1 = 0; i1 < MAX_TALLOC; i1++) {
      //with t_list[i1] do;
      if ((t_list[i1].tval == x1) && (t_list[i1].subval == x2)) {
	unquote(t_list[i1].name);
	known1(t_list[i1].name);
      }
    }
    for (i1 = Equipment_min; i1 <= Equipment_secondary; i1++) {
      //with equipment[i1] do;
      if ((equipment[i1].tval == x1) && (equipment[i1].subval == x2)) {
	unquote(equipment[i1].name);
	known1(equipment[i1].name);
      }
    }
    
    for(curse = inventory_list; curse != nil; curse = curse->next) {
      //with curse^.data do;
      if ((curse->data.tval == x1) && (curse->data.subval == x2)) {
	unquote(curse->data.name);
	known1(curse->data.name);
      }
    }
    
    i1 = 0;
    do {
      i1++;
      //with object_list[i1] do;
      if ((object_list[i1].tval == x1) && 
	  (object_list[i1].subval == x2)) {
	if (strstr(object_list[i1].name,"%T") != NULL) {
	  insert_str(object_list[i1].name," %T|","");
	  object_ident[i1] = true;
	} else {
	  unquote(object_list[i1].name);
	  known1(object_list[i1].name);
	  object_ident[i1] = true;
	}
      }
    } while (i1 != MAX_OBJECTS);
  } /* end if | */
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void known1(char *object_str)
{
  /*{ Remove 'Secret' symbol for identity of object			}*/

  insert_str(object_str, "|", "");

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void known2(char *object_str)
{
  /*{ Remove 'Secret' symbol for identity of pluses			}*/

  insert_str(object_str, "^", "");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void unquote(char *object_str)
{
  /*	{ Return string without quoted portion				}*/

  integer   pos0,pos1,pos2,olen;
  vtype     str1,str2;

  pos0 = pindex(object_str,'"');
  if (pos0 > 0) {
    pos1 = pindex(object_str,'~');
    pos2 = pindex(object_str,'|');
    olen = strlen(object_str);
    strncpy(str1,object_str,pos1); str1[pos1] = 0;
    strncpy(str2,&(object_str[pos2]),olen-pos2); str2[olen-pos2] = 0;
    sprintf(object_str,"%s%s",str1,str2);
  }

};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void objdes(char *out_val,treas_ptr ptr,boolean pref)
{
  /*{ Returns a description of item for inventory			}*/

  char       *cpos;
  vtype      tmp_val;

  ENTER("objdes","i");

  //with ptr->data. do;
  strcpy(tmp_val, ptr->data.name);

//  fprintf(debug_file,": obj start: >%s<\n",tmp_val);

  cpos = strstr(tmp_val,"|");
  if (cpos != NULL) {
    *cpos = 0;
  }

//  fprintf(debug_file,": obj thmp1: >%s<\n",tmp_val);

  cpos = strstr(tmp_val,"^");
  if (cpos != NULL) {
    *cpos = 0;
  }

//  fprintf(debug_file,": obj thmp2: >%s<\n",tmp_val);

  if (!(pref)) {
    cpos = strstr(tmp_val," (");
    if (cpos != NULL) {
      *cpos = 0;
    }
  }

//  fprintf(debug_file,": obj thmp3: >%s<\n",tmp_val);

  insert_num(tmp_val,"%P1",ptr->data.p1,true);
  insert_num(tmp_val,"%P2",ptr->data.tohit,true);
  insert_num(tmp_val,"%P3",ptr->data.todam,true);
  insert_num(tmp_val,"%P4",ptr->data.toac,true);
  insert_num(tmp_val,"%P5",ptr->data.p1,false);
  insert_num(tmp_val,"%P6",ptr->data.ac,false);

//  fprintf(debug_file,": obj thmp4: >%s<\n",tmp_val);

  if (ptr->data.number != 1) {
    insert_str(tmp_val,"ch~","ches");
    insert_str(tmp_val,"y~","ies");
    insert_str(tmp_val,"~","s");
  } else {
    insert_str(tmp_val,"~","");
  }

//  fprintf(debug_file,": obj thmp5: >%s<\n",tmp_val);

  if (pref) {
    if (strstr(tmp_val,"&") != NULL) {
      insert_str(tmp_val,"&","");

      if (ptr->data.number > 1) {
	sprintf(out_val,"%d%s",ptr->data.number,tmp_val);
      } else if (ptr->data.number < 1) {
	sprintf(out_val,"no more%s",tmp_val);
      } else if (is_vowel(tmp_val[1])) {
	sprintf(out_val,"an%s",tmp_val);
      } else {
	sprintf(out_val,"a%s",tmp_val);
      }

    } else {
      if (ptr->data.number > 0 ) {
	strcpy(out_val, tmp_val);
      } else {
	sprintf(out_val,"no more %s",tmp_val);
      }
    }

  } else {
    insert_str(tmp_val,"& ","");
    strcpy(out_val, tmp_val);
  }

#if DO_DEBUG
  fprintf(debug_file,": obj final: >%s<\n",out_val);
#endif

  LEAVE("objdes","i");
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char *bag_descrip(treas_ptr bag, string result)  // was func
{
 /*{ Return description about the contents of a bag	-DMF-	}*/
 
 integer    count,wgt;
 treas_ptr  ptr;
 
 if ((bag->next == nil) || (bag->next->is_in == false)) {
   sprintf(result," (empty)");
 } else {
   count = 0;
   wgt = 0;
   
   for (ptr = bag->next;(ptr != nil) && (ptr->is_in);ptr = ptr->next) {
     count += ptr->data.number;
     wgt   += ptr->data.weight * ptr->data.number;
   }
   
   sprintf(result," (%ld%% full, containing %ld item%s)",
	   trunc(wgt * 100 / bag->data.p1),count,
	   ((count != 1) ? "s" : ""));
 }
 return result;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/* END FILE  desc.c */
