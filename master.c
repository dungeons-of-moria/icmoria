/* master.c */
/* routines to handle the master file, which keeps track of save files */

#include "imoria.h"
#include "master.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean master_file_open(GDBM_FILE *mf)
{
  /* open the master file db, returns true on success */

  int trys;

  *mf = NULL;
  for (trys = 0 ; (trys < 10) && (*mf == NULL) ; trys++) {
    priv_switch(1);
    *mf = gdbm_open(MORIA_MAS, 1024, GDBM_WRCREAT, 0, 0);
    priv_switch(0);
    if ((*mf == NULL)) {
      sleep(1);
    }
  }

  return (*mf != NULL);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void master_file_close(GDBM_FILE *mf)
{
  /* save changes and close the master file db */

  if (*mf != NULL) {
    gdbm_close(*mf);
    *mf = NULL;
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean master_file_read(GDBM_FILE mf, master_key *mkey, master_entry *mentry)
{
  /* fetch a db entry, returns true if the entry was found */

  boolean  return_value = false;
  datum    key, data;
  
  key.dptr   = (char *)mkey;
  key.dsize  = sizeof(master_key);

  if (mf != NULL) {
    data = gdbm_fetch(mf, key);

    if (data.dptr != NULL) {
      /* Will read from data that is bigger than expected, but not smaller. */
      /* If more data is added to entries without fixing the database then  */
      /* handle that case here!                                             */
      if (data.dsize >= sizeof(master_entry)) {
	memcpy(mentry, data.dptr, sizeof(master_entry));
	return_value = true;
      }
      free(data.dptr);
    }
  } else {
    /* no open db file, that isn't good */
  }
  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean master_file_write(GDBM_FILE mf, master_key *mkey, master_entry *mentry)
{
  datum    key, data;
  int      result;
  
  key.dptr    = (char *)mkey;
  key.dsize   = sizeof(master_key);
  data.dptr   = (char *)mentry;
  data.dsize  = sizeof(master_entry);

  if (mf != NULL) {
    mentry->updated = time(0);
    result = gdbm_store(mf, key, data, GDBM_REPLACE);
  }

  return true;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean master_file_delete(GDBM_FILE mf, master_key *mkey)
{
  datum    key;
  int      result = -1;
  
  key.dptr    = (char *)mkey;
  key.dsize   = sizeof(master_key);

  if (mf != NULL) {
    result = gdbm_delete(mf, key);   /* 0 if deleted, -1 if not found */
  }

  return (result == 0) ? true : false;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int master_file_verify(GDBM_FILE mf, master_key *mkey)
{
  boolean         foundit;
  master_entry    mentry;
  int             return_value = MF_CHAR_NOT_FOUND;

  if (mf != NULL) {
    foundit = master_file_read(mf, mkey, &mentry);

    if (foundit) {
      if (py.misc.save_count == mentry.save_count) {
	return_value = MF_CHAR_OK;
      } else {
	return_value = MF_CHAR_MISMATCH;
      }
    }
  }

//  if (return_value != MF_CHAR_OK) {
//    msg_print("Verify mismatch.");
//    msg_print(" ");
//  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
boolean master_file_write_new(GDBM_FILE mf, master_key *mkey,
			      master_entry *mentry)
{
  /* insert a new key, if key already exists then try new keys for a while */

  int      trys;
  datum    key;
  boolean  return_value = false;

  key.dptr    = (char *)mkey;
  key.dsize   = sizeof(master_key);

  if (mf != NULL) {
    for (trys = 0 ; (trys < 500) && !return_value ; trys++) {
      if (!gdbm_exists(mf, key)) {
	master_file_write(mf, mkey, mentry);
	return_value = true;
      } else {
	mkey->creation_time += randint(40);
      }
    }
  }

  return return_value;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  master.c */
