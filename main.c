/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: main.c
*  DESC: all public SRS functions are located here
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

#include <stdio.h>          /* stdin / stdout */
#include <string.h>         /* memcpy, sprintf, etc.. */
#include <strings.h>        /* strcasecmp etc..  */
#include <sys/types.h>      /* typdefs */
#include <time.h>           /* time */
#include <sys/timeb.h>      /* getting time in ms */

#include "srs.h"            /* libsrs structures / defines */
#include "util.h"           /* libsrs utility */
#include "crypto.h"         /* libsrs crypto */
#include "error.h"          /* libsrs error handling */
#include "base.h"           /* libsrs base encoding */
#include "main.h"           /* libsrs public function calls */

srs_config_t  conf;   /* global config structure */

/* SRS_new
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Allocates memory for an srs_t structure, populates it with the
*  passed values and returns the structure.  Free memory in use by this
*  structure by calling SRS_del(srs_t).  Upon error (unable to allocate
*  memory), we die.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
srs_t *SRS_new(const char *secret, const int32_t age,
  const int16_t hash_len, const int16_t hash_min)
{
  srs_t  *srs;      /* srs storage container */

  if (secret == NULL)
  {
    xeprintf("Unable to proceed without a secret.\n");
    return(NULL);
  }    

  xprintf("Called with:\n"
    "\tsecret:\t\t(%s)\n\tage:\t\t(%i)\n\thash len\t(%i)\n\thash min\t(%i)\n",
     secret, age, hash_len, hash_min);

  srs               = xmalloc(SIZEOF(srs_t));

  srs->secret       = xstrndup(secret, MAX_SECRET);
  srs->s_len        = strlen(secret);
  srs->hash_len     = hash_len;
  srs->hash_min     = hash_min;
  srs->age          = age; 

  return(srs);
}


/* SRS_del
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Takes an srs structure and free's any memory in use,
*
*/
srs_t *SRS_del(srs_t *srs)
{
  xprintf("Destroying srs structure (%s) %u : %u\n",
     srs->secret, srs->s_len, srs->age);
    
  /* abunai! kono sekurito ha totemo oishii desuyo! */
  memset(srs->secret, '\0', srs->s_len);
  xfree(srs->secret);
  srs->s_len      = 0;
  srs->hash_len   = 0;
  srs->hash_min   = 0;
  srs->age        = 0;
  xfree(srs);
  
  return(NULL);
}


/* SRS_forward
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Allocates memory for an srs_t structure, populates it with the
*  passed values and returns the structure.  Free memory in use by this
*  structure by calling SRS_del(srs_t).  Upon error (unable to allocate
*  memory), we die.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*/
char *SRS_forward(srs_t *srs, char *sender, char *alias)
{
  srs_addr_t    *addr;    /* srs address structure */
  char          *buf;     /* remporary buffer */
  char          *ret;     /* return buffer */
  char          *tmp;     /* utility pointer */
  
  xprintf("Called (%s) (%s)\n",
    sender, alias);
  
  if ((strstr(sender, "@")) == NULL)
    ERROR("(%s) has no '@'\n", sender);

  /* split the address into its local-part and domain.tld */
  addr = UTIL_split_addr(sender);
  
  if ((strstr(addr->user, "@" )) != NULL)
    ERROR("%s) has '@'\n", addr->user);

  /* skip over local-part if any, its not required */
  if ((tmp = strstr(alias, "@")) != NULL)
  {
    tmp++;
  }
  
  if (strncmp(addr->user, "SRS1", 4) == 0)
  {
    buf = strdup(addr->user);
  }
  else
  {
    buf = SRS_compile(srs, addr);
  }
  
  xprintf("Compile returns: (%s)\n", buf);
  
  ret = xmalloc((UTIL_varlen(2, buf, tmp) + 2));
  snprintf(ret, MAX_RWA, "%s@%s", buf, tmp);
  
  xfree(buf);
  xfree(addr->host);
  xfree(addr->user);
  free(addr);
  
  return(ret);
}


/* SRS_reverse
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          A wrapper function for SRS_parse which decompiles an RWA one
*  level at a time returning the reversely rewritten address.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/ 
char *SRS_reverse(srs_t *srs, char *addr)
{
  char *buf;    /* return buffer */

  if (strstr(addr, "@") == NULL)
    ERROR("(%s) has no '@'\n", addr);
  
  return((buf = SRS_parse(srs, addr)));
}


/* SRS_compile
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Compiles or builds RWA's (Re-written Addresses) using the passed
*  variables.  There are essentially three types of outputs.  The first being
*  the creation of an SRS address, which includes the generate of both a
*  timestamp and an HMAC.  The second being when called with an existing RWA,
*  the RWA is re-written as an SRS1 RWA and the hostname is the appended to
*  the RWA TYPE (SRS1+host..) and the rest of the original string following
*  suit.  The third type is an SRS1 address which has already been "wrapped",
*  in this case, this function doesn't acutally handle this, unlike Mail::SRS.
*  See the calling function.  For more information about the way in which 
*  RWA's are created, you MUST read the srs.pdf available from Shevek's site
*  or none of this will make any sense. 
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
char *SRS_compile(srs_t *srs, srs_addr_t *addr)
{
  int32_t     n;            /* length of return buffer */
  char        *buf;         /* return buffer */

  int32_t     d_len;        /* length of data */
  u_char      *data;        /* data to hash */

  char        *rtype;       /* rewrite address type */
  
  char        *timestamp;   /* timestamp */
  u_char      *hash;        /* hash / digest */
  
  char        *rwa;         /* rwa string use this var to avoid confusion */
  
  xprintf("User: (%s) Host: (%s)\n",
     addr->user, addr->host);

  if (strstr(addr->user, SRS_TAG))
  {
    /* jump past SRS0= */
    rtype   = UTIL_get_rtype_sep(addr->user);
    /*rtype = strstr(addr->user, "=");*/
    rtype++;
    rwa = rtype;
  
    n = (UTIL_varlen(3, SRS_WRAP, addr->host, rwa) + 3);
    buf = xmalloc(n);
    snprintf(buf, MAX_RWA, "%s=%s=%s", SRS_WRAP, addr->host, rwa);
    
    xprintf("TAG! Return: (%s) : %u\n",
       buf, n);
      
    return(buf);
  }

  timestamp = SRS_timestamp_create((time_t)0);
  
  xprintf("SRS_compile :: ts: (%s) host: (%s) user: (%s)\n",
     timestamp, addr->host, addr->user);
    
  /* concatenate timestamp+host+user */
  data    = UTIL_create_hashdata(timestamp, addr->host, addr->user);
  d_len   = strlen((char *)data);
  
  if ((hash = SRS_hash_create(srs->secret, srs->s_len, data, d_len)) == NULL)
  {
    xfree(data);
    xeprintf("Can't crate HMAC hash, aiya!\n");
  }

  /* truncate hash to specified length */
  *(hash + srs->hash_len) = '\0';
  
  xvprintf("Assembled data: (%s) len: %u\n",
     data, d_len);

  /* + 4 for each '=', + 1 for the '\0' */
  n = (UTIL_varlen(5, SRS_TAG, hash, timestamp, addr->host,
    addr->user) + 4 + 1);
    
  buf = xmalloc(n);
  snprintf(buf, MAX_RWA, "%s=%s=%s=%s=%s", SRS_TAG, hash,
    timestamp, addr->host, addr->user);    

  xfree(timestamp);
  xfree(hash);
  xfree((char *)data);
   
  xprintf("Return: (%s) : %u\n",
     buf, (d_len + 1));

  return(buf);
}


/* SRS_parse
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Called when reversing a re-written address, this function is
*  effectively the opposite of SRS_compile.  The RWA (Re-written Address)
*  is walked and tokenized and rewritten accordingly.  For more information
*  on how this works, one should read the srs.pdf available for download
*  from Shevek's site.
*
*  Note:
*          Please ignore the verbose code here, there are a couple of 
*  functions which shall be written shortly which will drastically trim down
*  the amount of repeated code seen here.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/    
char *SRS_parse(srs_t *srs, char *addr)
{
  int32_t     n;            /* length of return buffer */
  char        *buf;         /* return buffer */
  int32_t     d_len;        /* length of data buffer */
  u_char      *data;        /* data buffer for hash compare */
  
  /* used during rewrite address parsing */
  char        *sep;         /* primary separator used for plusaddresses -,+,= */
  char        *rtype;       /* rewrite address type */
  char        *relay;       /* relay host (used only in SRS1..+n */
  char        *rwa;         /* host to forward to's domain.tld  */
  char        *host;        /* sendhost (domain.tld) */
  char        *user;        /* senduser (local-part) */
  char        *timestamp;   /* timestamp */
  char        *hash;        /* hash / digest */
  
  char        *tmp;         /* utility pointer */
  
  sep = (addr + 4);
  xvprintf("Address: (%s) with originating separator: %c\n", addr, sep);

  if (strstr(addr, SRS_WRAP)/* && (*sep == '-' || *sep == '+' || *sep == '=')*/)
  {
    /* jump past SRS1= */
    rtype   = UTIL_get_rtype_sep(addr);
    /*rtype = strstr(addr, "=");*/
    rtype++;
    
    /* truncate the existing domain.tld */
    tmp     = strstr(rtype, "@");
    *tmp    = '\0';

    /* this mess retreives the relay host */
    tmp     = strstr(rtype, "=");
    *tmp    = '\0';
    relay   = strdup(rtype);
    *tmp    = '=';
      
    /* move forward past the relay host */
    rwa   = strstr(rtype, "=");
    rwa++;
      
    n = (UTIL_varlen(3, SRS_WRAP, rwa, relay) + 3);
    buf = xmalloc(n);
      
    /* reversing from 4th to 3rd, so we stay as SRS1 */
    snprintf(buf, MAX_RWA, "%s=%s@%s", SRS_TAG, rwa, relay);

    xprintf("WRAP! Return: (%s) : %u\n",
       buf, n);
    
    xfree(relay); 
    return(buf);
  }
  else if (strstr(addr, SRS_TAG)/* && (*tmp == '-' || *tmp == '+' || *tmp ==
  '=')*/)
  {
    /* jump past SRS0= */
    rtype     = UTIL_get_rtype_sep(addr);
    /*rtype = strstr(addr, "=");*/
    rwa       = rtype++;

    /* this mess retreives the digest */
    tmp       = strstr(rtype, "=");
    *tmp      = '\0';
    hash      = strdup(rtype);
    *tmp      = '=';
    rtype     += (strlen(hash) + 1);
    
    /* this mess retreives the time stamp */
    tmp       = strstr(rtype, "=");
    *tmp      = '\0';
    timestamp = strdup(rtype);
    *tmp      = '=';
    rtype     += (strlen(timestamp) + 1);
    
     /* this mess retreives the host (domain.tld) */
    tmp       = strstr(rtype, "=");
    *tmp      = '\0';
    host      = strdup(rtype);
    *tmp      = '=';
    rtype     += (strlen(host) + 1);
 
    /* this mess retreives the local-part */
    tmp       = strstr(rtype, "@");
    *tmp      = '\0';
    user      = strdup(rtype);
    *tmp      = '@';
    
    xprintf("hash (%s) timestamp (%s) user (%s) "
      "host (%s)\n", hash, timestamp, user, host);
      
    /* +1 for '@' and +1 for '\0' */    
    n         = (UTIL_varlen(2, user, host) + 2);
    buf       = xmalloc(n);

    if (SRS_timestamp_check(srs, timestamp) == FALSE)
    {
      ERROR("Invalid timestamp (%s)\n", timestamp);
    }

    /*timestamp = TIME_create((time_t)0);*/
  
    xvprintf("ts: (%s) host: (%s) user: (%s)\n",
       timestamp, host, user);
    
    /* concatenate timestamp+host+user */
    data    = UTIL_create_hashdata(timestamp, host, user);
    d_len   = strlen((char *)data);

    if (SRS_hash_verify(srs->secret, srs->s_len, 
      data, d_len, (u_char *)hash, srs->hash_min, srs->hash_len) == FALSE)
    {
      ERROR("Invalid hash (%s).\n", hash);
    }
  
    xprintf("Bounce message HMAC (%s) verified!\n", hash);
      
    snprintf(buf, MAX_RWA, "%s@%s", user, host);

    xfree(timestamp);
    xfree(hash);
    xfree(host);
    xfree(user);
    xfree(data);
   
    xprintf("Return: (%s) : %u\n",
       buf, (n + 1));

    return(buf); 
  }
  else
  {
    xprintf("Invalid wrapped SRS address %s\n", addr);
  }

  return(NULL);
}


/* SRS_timestamp_create
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Creates a two character base32 encoded timestamp representing today. 
*  If no timestamp is passed, the aforementioned takes place.  If a timestamp is
*  passed, the passed timestamp is used instead.  This affords the luxury of
*  specifying an expiry date without ever having to store them.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
char *SRS_timestamp_create(time_t timestamp)
{
  time_t    curtime;          /* time */
  time_t    p_time;           /* (epoch / PRECISION) */
  
  char      t1;               /* base64(epoch / PRECISION & 63) */
  char      t2;               /* base64(time & 64) */
  char      *buf;             /* return buffer */

  /* we were passed an expiry date, lets use it */
  if (timestamp > 0)
  {
    xvprintf("Expiry supplied: (%lu)\n", timestamp);
      
    p_time = time(&timestamp);
  }

  xprintf("Called with timestamp %lu\n", timestamp);

  p_time = time(&curtime);
  p_time = (p_time / PRECISION);
  t1 = BASE_32[p_time & 31];
  xvprintf("%lu (e/p) %c (t&31)\n", p_time, t1);
  
  p_time = (p_time >> 5);
  t2 = BASE_32[p_time & 31];
  xvprintf("%lu (t>>5) %c (t&31)\n", p_time, t2);

  if ((buf = malloc(4)) == NULL)
    ERROR(ERR_NOMEM, "SRS_timestamp_create", 4);
  
  snprintf(buf, 4, "%c%c", t2, t1);
  xvprintf("returning: (%c%c)\n", t2, t1);
  
  return(buf); 
}


/* SRS_timestamp_check
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Compares timestamps to see if the timestamp found within an
*  envelope is within the range of days specified (age).  Returns TRUE
*  upon a match within range, and FALSE when not.
*
*/
SRS_BOOL SRS_timestamp_check(srs_t *srs, const char *timestamp)
{
  time_t    curtime = 0;    /* utility */
  time_t    p_time  = 0;     /* time / PRECISION */
  time_t    c_time  = 0;     /* time to check against */
  
  xvprintf("Called with time: (%s)\n", timestamp);

  curtime = 0;
  c_time  = (c_time * 64 + BASE_32_10_c(*timestamp));
  xvprintf("Time 0 decoded: (%ul)\n", c_time);
  
  c_time  = (c_time * 64 + BASE_32_10_c(*timestamp++));
  xvprintf("Time 1 decoded: (%ul)\n", c_time);

  p_time  = (time(&curtime) / PRECISION);
  
  do
  {
    p_time += TICKSLOTS;
  } while (p_time < c_time);
  
  if (p_time <= (c_time + srs->age))
  {
    return(TRUE);
  }
  
  return(FALSE); 
}


/* SRS_hash_create
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Using the passed variables an SHA1 HMAC encoded in base64 to the
*  length of 22 characters is created and then truncated to srs->hash_len
*  bytes.  Returns the hash/digest upon success, and NULL upon failure.
*
*  Memory: IIWFO dyn memory is allocated.  Caller should free.
*
*/
u_char *SRS_hash_create(const char *secret, const int32_t s_len,
  const u_char *data, const int32_t d_len)
{
  u_char *buf;        /* digest buffer */
  
  return((buf = CRYPTO_hmac_sha1(secret, s_len, data, d_len)));
}


/* SRS_hash_verify
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Using the passed digest, a new digest is created using the other
*  passed variables, and then the two digests area compared.  If they match,
*  returns TRUE, if not, returns FALSE.
*
*/
SRS_BOOL SRS_hash_verify(const char *secret, const int32_t s_len, 
  const u_char *data, const int32_t d_len, const u_char *digest, 
  const int16_t hash_min, const int16_t hash_len)
{
  size_t  len;      /* length of digest */
  u_char  *buf;     /* digest buffer */
  
  len = strlen((char *)digest);
  
  if (len < hash_min)
  {
    xprintf("Digest (%s) is too short!  "
      "Len: %u Min: %u\n", digest, len, hash_min);
        
    return(FALSE);
  }
  buf = CRYPTO_hmac_sha1(secret, s_len, data, d_len);
  *(buf + hash_len) = '\0';
  
  xprintf("Called with: (%s) : %u (%s) : %u "
    "(%s)\n", secret, s_len, data, d_len, digest);
    
  if (strcmp((char *)buf, (char *)digest) == 0)
  {
    xprintf("Verification success: (%s) matches (%s)\n", buf, digest);
      
    free(buf);
    return(TRUE);
  }

  xprintf("Verification failure: (%s) does not match (%s)\n", buf, digest);
    
  free(buf);
  return(FALSE);
}


/* SRS_set_secret
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*         Useless function as far as this library is concerned at the moment 
*  since we only support one secret, and no retention.  This is merely here to 
*  satisfy the API and will likely be functional in the very near future.
*
*/
SRS_BOOL SRS_set_secret(srs_t *srs, const char *s)
{
  xfree(srs->secret);
  srs->secret = xstrndup(s, MAX_SECRET);

  xprintf("Secret (%s) successfully set\n", s);
  
  return(TRUE);
}

/* end srs.c */
