/* trade.h */
/* routines to handle the trading post */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define     T_DISPLAY_SIZE             12
#define     T_ACCEPTABLE_ITEM_PRICE    50
#define     T_PROFIT_FROM_BID          0.05
#define     T_PROFIT_FROM_SALE         0.25
#define     T_REFUND_ON_BID            (1.00 - T_PROFIT_FROM_BID)
#define     T_REFUND_ON_SALE           (1.00 - T_PROFIT_FROM_SALE)
#define     T_BID_INCREMENT_FACTOR     1.05
#define     T_TAKE_THE_MONEY_AND_RUN   0.90
#define     T_BID_WAIT_DAYS            0
#define     T_BID_WAIT_HOURS           6
#define     T_EXPIRE_TIME_DAYS         4
#define     T_EXPIRE_TIME_HOURS        0

#define     TT_PROFIT                  0
#define     TT_FOR_SALE                1
#define     TT_CASH                    2

typedef struct trade_account_type
{
  uid_t     uid;
  char      username[13];
  integer   master_id;
  integer   claim_check;
} trade_account_type;

typedef struct profit_record
{
  integer        trade_type;
  integer        time;
  integer        money;
} profit_record;

typedef struct for_sale_record
{
  integer              trade_type;
  integer              time;
  treasure_type        object;
  trade_account_type   seller;
  integer              bid_time;
  integer              best_bid;
  trade_account_type   best_bidder;
} for_sale_record;

typedef struct cash_record
{
  integer              trade_type;
  integer              time;
  integer              amount;
  trade_account_type   owner;
} cash_record;

typedef union trade_record_type
{
  struct profit_record    pr;
  struct for_sale_record  fsr;
  struct cash_record      cr;
} trade_record_type;

typedef struct pinven_record
{
  struct pinven_record  *prev;
  struct pinven_record  *next;
  trade_record_type      data;
} pinven_record;

typedef pinven_record *pinven_ptr;

#define ROUND(x) ((integer)((x)+.5))

extern boolean   trade_file_open(FILE **tf, boolean *busy, boolean create);
extern void      trade_file_close(FILE **tf);
extern void      enter_trading_post(); 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  trade.h */
