/* encrypt.c */
/* routines to handle encrypting and decrypting save files */

#include "imoria.h"

#if ENABLE_DES
#include <des_crypt.h>
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void encrypt_init(encrypt_state *state, byteint key[], boolean doit)
{
  /* called by before using encrypt_write or read_decrypt */

#if ENABLE_DES
  int i1;

  memcpy(state->des_key, key, 8);
  des_setparity(state->des_key);
 
  for (i1 = 0 ; i1 < 8 ; i1++) {
    state->des_ivec[i1] = 0;
  }

  state->doit     = doit;
#else
  state->doit     = doit;
#endif

  state->got_eof  = false;
  state->buf_pos  = 0;
  state->buf_size = 0;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if ENABLE_DES
void des_encrypt_write(FILE *f1, encrypt_state *state)
{
  /* called by encrypt_write to encode a block of plaintext and write it */

    int   result;

    if (state->buf_pos != 0) {
      /* round up to 8 bytes */
      state->buf_pos += 7;
      state->buf_pos &= 0xfffffff8;
      
      result = cbc_crypt(state->des_key, state->data_buf, state->buf_pos,
			 DES_ENCRYPT | DES_SW, state->des_ivec);
      
      if (result != DESERR_NONE) {
	prt("Error calling cbc_crypt to encrypt.", 1, 1);
	put_qio();
      }
      
      if (write((int)fileno(f1), state->data_buf, state->buf_pos)
	  != state->buf_pos) {
	prt("Error writing line to file.", 1, 1);
	put_qio();
      }

      state->buf_pos = 0;
    }
};
#else
void rand_encrypt_write(FILE *f1, encrypt_state *state)
{
  /* called by encrypt_write to encode a block of plaintext and write it */
  
  if (state->buf_pos != 0) {
    
    if (write((int)fileno(f1), state->data_buf, state->buf_pos)
	!= state->buf_pos) {
      prt("Error writing line to file.", 1, 1);
      put_qio();
    }
    
    state->buf_pos = 0;
  }
};
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if ENABLE_DES
void des_read_decrypt(FILE *f1, encrypt_state *state)
{
  /* called by read_decrypt to get next block of plaintext */

  int   result;
  
  state->buf_pos     = 0;
  state->buf_size    = 0;
  state->data_buf[0] = 0;
  
  if (feof(f1)) {
    state->got_eof  = true;
  } else {
    
    state->buf_size = read((int)fileno(f1), state->data_buf,
			   ENCRYPT_STAT_BUF_SIZE);
    if (state->buf_size == -1) {
      prt("Error reading line from file.", 1, 1);
      state->got_eof  = true;
    } else {
      
      if (state->buf_size == 0) {
	state->got_eof  = true;
      } else if (state->buf_size & 0x07) {
	prt("Error read not a multiple of 8 bytes.", 1, 1);
	put_qio();
	state->got_eof  = true;
	state->buf_size = 0;
      } else {	
	result = cbc_crypt(state->des_key, state->data_buf, state->buf_size,
			   DES_DECRYPT | DES_SW, state->des_ivec);
	
	if (result != DESERR_NONE) {
	  prt("Error calling cbc_crypt to decrypt.", 1, 1);
	  put_qio();
	}
      }
    }
  }
};
#else
void rand_read_decrypt(FILE *f1, encrypt_state *state)
{
  /* called by read_decrypt to get next block of plaintext */

  state->buf_pos     = 0;
  state->buf_size    = 0;
  state->data_buf[0] = 0;
  
  if (feof(f1)) {
    state->got_eof  = true;
  } else {
    
    state->buf_size = read((int)fileno(f1), state->data_buf,
			   ENCRYPT_STAT_BUF_SIZE);
    if (state->buf_size == -1) {
      prt("Error reading line from file.", 1, 1);
      state->got_eof  = true;
    } else {
      
      if (state->buf_size == 0) {
	state->got_eof  = true;
      }
    }
  }
};
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void encrypt_flush(FILE *f1, encrypt_state *state)
{
  /* file is about to close, flush out the rest of the bytes */
  /* this will add a null and pad out to a multiple of 8 bytes */

  /* we do not care about overflow because the buffer is 8 bytes too big */

  if (state->doit) {
    state->data_buf[state->buf_pos++] = 0;
#if ENABLE_DES
    state->data_buf[state->buf_pos++] = randint(256)-1;
    des_encrypt_write(f1, state);
#else
    rand_encrypt_write(f1, state);
#endif
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void encrypt_write (FILE *f1, encrypt_state *state, ntype line)
{
  /* this is ment to encrypt ascii, when decrypting a null in the
     plaintext marks the end of the file.  so, you can use it with
     binary data as long as there are no nulls!  also, newlines are
     bad since decrypt breaks things into lines                      */

  int i1;

  //  printf("writeline: ***%s***\n", line); fflush(stdout);

  if (!(state->doit)) {
    fprintf(f1,"%s\n",line);
  } else {

    for (i1 = 0 ; line[i1] ; i1++) {
      state->data_buf[state->buf_pos++] = line[i1] ^ (randint(256)-1);

      if (state->buf_pos == ENCRYPT_STAT_BUF_SIZE) {
#if ENABLE_DES
	des_encrypt_write(f1, state);
#else
	rand_encrypt_write(f1, state);
#endif
      }
    }

    state->data_buf[state->buf_pos++] =  '\n' ^ (randint(256)-1);

    if (state->buf_pos == ENCRYPT_STAT_BUF_SIZE) {
#if ENABLE_DES
      des_encrypt_write(f1, state);
#else
      rand_encrypt_write(f1, state);
#endif
    }
  }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void read_decrypt (FILE *f1, encrypt_state *state, ntype line,boolean *got_eof)
{
  /* this is ment to decrypt ascii, a null in the plaintext marks the
     end of the file.  so, you can use it with binary data as long as there
     are no nulls!  (or newlines, since we break things into lines)    */

  int      i1;
  byteint  i2;
  boolean  exit;

  line[0] = 0;

  if (!(state->doit)) {
    /* file is not encrypted */
    
    if ((fgets(line, sizeof(ntype), f1)) == NULL) {
      *got_eof = true;
      //printf("Yikes!  EOF on f1: %p\n",f1); fflush(stdout);
    } else {
      line[strlen(line)-1] = 0;  /* strip newline */
    }
    
  } else {
    
    if ((state->buf_size != 0) || (!state->got_eof)) {

      for (i1 = 0, exit = false ; (i1 < 1000) && !exit ; ) {
	if (state->buf_pos >= state->buf_size) {
#if ENABLE_DES
	  des_read_decrypt(f1, state);
#else
	  rand_read_decrypt(f1, state);
#endif
	}
	
	if (state->buf_size == 0) {
	  exit = true;
	  i2   = 0;
	} else {
	  i2 = state->data_buf[state->buf_pos++] ^ (randint(256)-1);
	  
	  if (i2 == 0) {
	    /* end of file */
	    state->buf_size = 0;
	    exit            = true;
	  } else if (i2 == '\n') {
	    /* end of line */
	    exit = true;
	  } else {
	    line[i1++] = i2;
	  }
	}
	
      }
      line[i1] = 0;

      if (state->buf_size == 0) {
	state->got_eof = true;
	*got_eof       = true;
      }
      
    }
  }    
    //printf("readline: ***%s***\n", line); fflush(stdout);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* END FILE  encrypt.c */
