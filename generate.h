typedef struct coords
{
  integer   y;
  integer   x;
} coords;

typedef struct river_deal
{
  integer in1,in2,out; /*{ (keypad) directions; in is upstream }*/
  integer flow;        /*{ water flow out of this river spot }*/
  integer pos;         /*{ in array of s_l_type; if > num_left then }*/
                       /*{ spot is no longer available }*/
} river_deal;

typedef struct s_l_type
{
  coords  loc;         /*{ cross-ref back to river_deal }*/
  boolean is_active;   /*{ is still an unresolved river source}*/
} s_l_type;

#define RIVER_SIZE_Y        10
#define RIVER_SIZE_X        31
#define RIVER_TOTAL_SIZE   310
#define RIVER_SEGMENT_SIZE   6

typedef struct river_args
{
  river_deal     gup[RIVER_SIZE_Y+1][RIVER_SIZE_X+1];
  /*: array [1..RIVER_SIZE_Y] of array [1..RIVER_SIZE_X] of river_deal;*/
  
  s_l_type       s_list[RIVER_TOTAL_SIZE+1];
  /*: array [1..RIVER_TOTAL_SIZE] of s_l_type;*/
  
  integer        max_wet;                /*{ # of river or next-to-river }*/
  integer        num_left,s_l_top;
  coords         river_mouth;
  
} river_args;

#define Gup          a->gup
#define S_list       a->s_list
#define Max_wet      a->max_wet
#define Num_left     a->num_left
#define S_l_top      a->s_l_top
#define River_mouth  a->river_mouth

/* end generate.h */
