/* master.h */
/* routines to handle the master file, which keeps track of save files */
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include <gdbm.h>

extern boolean   master_file_open(GDBM_FILE *mf);
extern void      master_file_close(GDBM_FILE *mf);
extern boolean   master_file_read(GDBM_FILE mf, master_key *mkey,
				  master_entry *mentry);
extern boolean   master_file_write(GDBM_FILE mf, master_key *mkey,
				   master_entry *mentry);
extern boolean   master_file_write_new(GDBM_FILE mf, master_key *mkey,
				       master_entry *mentry);
extern boolean   master_file_delete(GDBM_FILE mf, master_key *mkey);
extern int       master_file_verify(GDBM_FILE mf, master_key *mkey);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  master.h */
