/* libsrs - Sender Rewriting Scheme library
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: util.c
*  DESC: utility functions
*
*  License:
*
*  The libspf Software License, Version 1.0
*
*  Copyright (c) 2004 James Couzens.  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
*  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
*  ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
*  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
*  SUCH DAMAGE.
*
*/

#include "util.h"

/* _printf_dbg
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   12/25/03
*
*  Desc:
*          Tied to a compile time switch this can instantly and at
*  little to no real expense enable a discreet debugging with out
*  hoards of #ifdefs all over the place.
*
*/
void _printf_dbg(u_int8_t level, const char *function, const char *format,...)
{
  char     _dbg_output[MAX_DEBUG_LEN];
  
  FILE     *fp      = NULL;
  va_list  argptr;

  va_start(argptr, format);
  vsnprintf(_dbg_output, SIZEOF(_dbg_output), format, argptr);
  va_end(argptr);

  if (f_bit_set(conf.level, level))
  {
    printf("%s :: %s", function, _dbg_output);
    fflush(stdout);

    if ((fp = fopen(DEBUG_LOG_FILE, "a")) != NULL)
    {
        fprintf(fp, "%s", _dbg_output);
        fclose(fp);
    }
  }

  return;
}


/* dummy_debug
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   12/25/03
*
*  Desc:
*          dummy function thats used instead of the _printf_dbg
*  function when compiling without debugging
*
*/
void dummy_debug(const u_int8_t level, const char *function, const char *s,...)
{
  return;
}


/* UTIL_malloc
*
*  Author: Travis Anderson <travis@anthrax.ca>
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   02/17/04
*
*  Desc:
*         Wrapper for malloc.  Upon success, behaves as malloc does.  
*  Wrapper functionality is to print an error message and exit upon failure.
*
*/
void *UTIL_malloc(const int32_t n, const char *file, int32_t line,
  const char *func)
{
    void *x = malloc(n);
    
    if (x == NULL)
    {
        xprintf("Unable to allocate %i bytes at %s:%i "
          "in %s\n", n, file, line, func);
          
        exit(0);        
    }

    return(x);
}


/* UTIL_realloc
*
*  Author: Travis Anderson <travis@anthrax.ca>
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   02/17/04
*
*  Desc:
*         Wrapper for realloc.  If 'p' is NULL, allocates memory via a call to 
*  UTIL_malloc, otherwise if 'x' is assigned successfully, the behaviour is
*  identical to realloc.  Wrapper functionality is to print an error message 
*  and exit upon failure.
*
*/
void *UTIL_realloc(void *p, const int32_t n, const char *file, const int32_t line,
  const char *func)
{
    void *x = NULL;

    if (p == NULL)
    {
      return(UTIL_malloc(n, file, line, func));
    }
    
    x = realloc(p, n);
    if (x == NULL)
    {
        xprintf("Unable to reallocate %i bytes at %s:%i in %s; original address "
          "0x%x\n", n, file, line, func, (long)p);
          
        exit(0);        
    }

    return(x);
}


/* UTIL_free
*
*  Author: Travis Anderson <travis@anthrax.ca>
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   02/17/04
*
*  Desc:
*         Wrapper for free.  Upon success, behaves as free does.
*  Wrapper functionality is to print an error message and exit upon
*  failure.
*
*/
void UTIL_free(void *p, const char *file, const int32_t line, const char *func)
{
    if (p == NULL)
    {
        xprintf("Unable to free() on NULL pointer at %s:%i in %s; address "
          "0x%x.", file, line, func, (long)p);
        return;
    }
    
    xvprintf("Free address 0x%x by %s on line %i (%s)\n",
      (long)p, func, line, file);
       
    free(p);
    return;
}


/* UTIL_strndup 
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   12/25/03
*
*  Desc:   
*          n bytes are allocated and then filled with \0 chars.  Char s
*  is copied over to the allocated memory writing n -1 bytes leaving the
*  new string NULL terminated.  This new string is returned upon success
*  and NULL upon failure. 
*  
*/
char *UTIL_strndup(const char *s, const size_t n)
{
  char *ret_ptr = NULL;

  if (s == NULL)
  {
    xeprintf("Passed string is NULL.  Abort!.\n");
    return(FALSE);
  }
  
  xvprintf("Called with (%s) and max length of %u\n", s, n); 

  ret_ptr = xmalloc(n);
  xvprintf("Allocated %u bytes of memory.\n", n);
    
  memset(ret_ptr, '\0', n);
  strncpy(ret_ptr, s, n);

  xvprintf("Returning string: (%s)\n", ret_ptr); 
  
  return(ret_ptr);   
}


/* UTIL_varlen
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   02/10/04
*
*  Desc:
*          Calculates the length of all passed strings and returns the
*  ammount to the calling function.
*
*/
int32_t UTIL_varlen(int32_t count, ...)
{
  va_list     argptr;       /* arg pointer list */
  int32_t     i = 0;        /* utility */
  int32_t     j = 0;        /* utility */
  int32_t     n = 0;        /* length of all passed vars */
  char       *myarray[12];  /* pointers to each passed argument */

  if (count > 11)
  {
    count = 11;
  }
  
  va_start(argptr, count);
  while (i < count)
  {
    if ((myarray[i] = va_arg(argptr, char *)) == NULL)
    {
      return(0);
    }
    
    j = strlen(myarray[i]);
    xvprintf("var: (%s) : %i\n", myarray[i], j);
    i++;
    n += j;
  }
  va_end(argptr);

  xvprintf("Returning length: %u\n", n);

  return(n);
}


/* UTIL_split_addr
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:
*          Takes the contents of the 'alias' string within the passed
*  srs structure, and splits it into user and host, which are stored
*  within the internal 'data' structure using memory allocated by this
*  function which is cleaned by calling UTIL_fini_srs.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
srs_addr_t *UTIL_split_addr(const char *sender)
{
  srs_addr_t  *addr;    /* address structure */
  char        *copy;    /* copy of sender */
  char        *cp;      /* working pointer on copy */

  if (sender == NULL)
  {
    xeprintf("Called with NULL pointer\n");
    return(NULL);
  }
  
  xvprintf("Called with (%s)\n", sender);

  addr = xmalloc(SIZEOF(srs_addr_t));

  cp = copy = strdup(sender);
  while (*cp)
  {
    if (*cp == '@')
    {
      addr->host   = strdup(cp + 1);
      addr->h_len  = strlen(addr->host);
      break;
    }
    cp++;
  }

  *cp = '\0';
  addr->user       = strdup(copy);
  addr->u_len      = strlen(addr->user);
  *cp = '@';

  xvprintf("user: (%s) (%i) host: (%s) (%i)\n",
     addr->user, addr->u_len, addr->host, addr->h_len, sender);

  xfree(copy);

  return(addr);
}


/* UTIL_create_hashdata
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/09/04
*
*  Desc:
*          Called with local-part, domain.tld, and a 2 char base64 timestamp.
*  These values are concatenated together and returned.  The returned string is used
*  what is used to create the hash.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
u_char *UTIL_create_hashdata(const char *timestamp, const char *host,
  const char *user)
{
  u_char    *buf;   /* return buffer */
  int32_t   len;    /* lenfth of return buffer */

  if (host == NULL || user == NULL)
  {
    xeprintf("Called with NULL pointer\n");
    return(NULL);
  }

  xvprintf("Called with (%s) (%s) (%s)\n",
     timestamp, host, user);

  len = (UTIL_varlen(3, timestamp, host, user) + 1);

  buf = malloc(len);
  snprintf((char *)buf, MAX_DIGEST, "%s%s%s", timestamp, host, user);

  xvprintf("Returning (%s) len: %u\n",
     buf, len);

  return(buf);
}


/* UTIL_get_rtype_sep
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   02/18/04
*
*  Desc:
*          Returns a pointer to the first separator/delimiter in 's' if one can
*  be located ('=', '-', '+').  Returns NULL upon failure.
*
*/
char *UTIL_get_rtype_sep(char *s)
{
  char *sep;    /* separator */

  if (s == NULL)
  {
    xeprintf("Called with NULL pointer\n");
    return(0);
  }
  
  xvprintf("Called with: (%s)\n", s);
  sep = (s + 4);
  xvprintf("Working with separator: (%c)\n", *sep);
  
  if (*sep == '=' || *sep == '-' || *sep == '+')
  {
    return(sep);
  }

  return(NULL);
}

/* end of util.c */
