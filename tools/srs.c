/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: srs.c
*  DESC: libsrs API sample program and test app
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

#include <stdio.h>       /* printf */
#include <time.h>        /* time */
#include <sys/types.h>   /* typedefs */
#include <sys/time.h>    /* timeval / timezone / gettimeofday */

#include "srs_cmd.h"

#ifdef size_t
#undef size_t
#define size_t unsigned long 
#endif

int main(int argc, char *argv[])
{
  enum SRS_ACT  ACT         = SRS_DEFAULT;  /* perform default action */  
  SRS_BOOL      QUIET       = FALSE;        /* no output (test mode) */
  u_int8_t      i           = 0;            /* utility */
  
  char          *margv      = NULL;         /* pointer to current argv element */
  
  int32_t       age         = 0;            /* expiry */
  int16_t       hash_len    = 4;            /* base64 chars 4 x 64 bits */
  int16_t       hash_min    = 4;            /* base64 chars */

  char          *secret     = NULL;         /* secret / key */
  char          *addr1      = NULL;         /* email address from */
  char          *addr2      = NULL;         /* email address to  */

  char          *tmp        = NULL;          /* utility pointer */
  
  conf.level = 0;

  if (argc <= 1 || strcmp(argv[1], "--help") == 0 ||
    strcmp(argv[1], "--h") == 0 || strcmp(argv[1], "-h") == 0)
  {
    do_help();
    return(FALSE);
  }

  for (i = 1; i < argc; i++)
  {
    tmp = argv[i];

    if (*tmp && *tmp == '-')
    {
      margv = (tmp + 3);
      
      switch (*(tmp + 1))
      {
        case 'a' :
          age      = atoi(margv);
          xvprintf("AGE: %u\n", age);
          break;
        case 'b' :
          hash_min = atoi(margv);
          xvprintf("HASH MIN: %u\n", hash_min);
          break;
        case 'c' :
          hash_len = atoi(margv);
          xvprintf("HASH LEN: %u\n", hash_len);
          break;
        case 'd' :
          if ((conf.level = atoi(margv)) > 0)
          {
            switch(conf.level)
            {
              case 0:
                break;
              case 1:
                conf.level = FL_A;
                break;
              case 2:
                conf.level = FL_B;
                break;
              case 3:
                conf.level = (FL_A + FL_B);
                break;
              case 4:
                conf.level = FL_D;
                break;
              case 5:
                conf.level = (FL_A + FL_D);
                break;
              case 6:
                conf.level = (FL_B + FL_D);
                break;
              case 7:
                conf.level = (FL_A + FL_B + FL_D);
                break;
            }
          }
          xvprintf("DEBUG level: %u\n", conf.level);
          break;
        case 'f' :
          addr1  = xstrndup(margv, MAX_RWA);
          xvprintf("ADDR: %s\n", addr1);
          ACT = SRS_FORWARD;
          break;
        case 'q' :
          QUIET = atoi(margv);
          break;
        case 'r' :
          addr1  = xstrndup(margv, MAX_HOSTNAME);
          xvprintf("FORWARDER: %s\n", addr1);
          ACT = SRS_REVERSE;
          break;
        case 's' :
          secret = xstrndup(margv, MAX_SECRET);
          xvprintf("SECRET: %s\n", secret);
          break;
        case 't' :
          addr2  = xstrndup(margv, MAX_HOSTNAME);
          xvprintf("FORWARDER: %s\n", addr2);
          break;
        case 'z':
          ACT    = SRS_DEFAULT;
          i      = argc;
          break;
      } /* switch */
    }
  } /* for */

  if (QUIET == FALSE)
  {
    printf("SRS Query v%s - James Couzens <jcouzens@obscurity.org>\n\n", 
      SRS_BVERSION);
  }

  if (ACT == SRS_DEFAULT)
  {
    return(do_default(QUIET));
  }

  if (secret == NULL)
  {
    printf("You need to define a secret!\n\n");
    do_help();
    return(FALSE);
  }
  else if (addr1 == NULL)
  {
    printf("You need to specify a from address\n\n");
    do_help();
    return(FALSE);
  }
  else if (ACT == SRS_FORWARD && addr2 == NULL)
  {
    printf("You need to specify a destination address\n\n");
    do_help();
    return(FALSE);
  }
  
  switch (ACT)
  {
    case SRS_FORWARD:
      return(do_forward(secret, age, hash_len, hash_min, addr1, addr2, QUIET));
    case SRS_REVERSE:
      return(do_reverse(secret, age, hash_len, hash_min, addr1, QUIET));
    case SRS_DEFAULT:
      break;
    default:
      do_help();
      break;
  }

  xfree(addr1);
  xfree(addr2);
  xfree(secret); 
  return(FALSE); 
}

SRS_BOOL do_forward(char *secret, int32_t age, int32_t hash_len, int32_t hash_min, 
  char *addr1, char *addr2, SRS_BOOL QUIET)
{
  size_t len;               /* utility (length of srsaddr) */
  srs_t *srs      = NULL;   /* srs structure */
  char *srsaddr   = NULL;   /* srs address storage */

  if (secret == NULL || addr1 == NULL || addr2 == NULL)
  {
    printf("do_forward :: Passed a NULL char!  Abort!\n");
    return(FALSE);
  }

  if (QUIET == FALSE)
  {
    printf("SRS Forward: from: (%s) to: (%s)\n", addr1, addr2);
  }
  
  gettimeofday(&srs_time[0].start, &srs_time[0].tzp);

  srs = SRS_new(secret, age, hash_len, hash_min);
 
  /* simulate email from HOSTA to HOSTB */
  if ((srsaddr = SRS_forward(srs, addr1, addr2)) == NULL)
  {
    printf("Rewriting of address (%s) failed!  !@$#\n", addr1);
    srs = SRS_del(srs);
    return(FALSE);
  }

  len = strlen(srsaddr);

  if (QUIET == FALSE)
  {
    printf("SRS Forward complete: envelope-from: (%s) : %lu\n",
      srsaddr, (u_long)len);
  
    gettimeofday(&srs_time[0].finish, &srs_time[0].tzp);
    printf("Rewrite executed in %lu.%u seconds\n",
      (srs_time[0].finish.tv_sec    - srs_time[0].start.tv_sec),
      (u_int8_t)(srs_time[0].finish.tv_usec - srs_time[0].start.tv_usec));
  }
 
  xfree(secret);
  xfree(addr1);
  xfree(addr2);
  xfree(srsaddr);
  srs = SRS_del(srs);

  return(TRUE);
}


SRS_BOOL do_reverse(char *secret, int32_t age, int32_t hash_len, int32_t hash_min, 
  char *addr, SRS_BOOL QUIET)
{
  size_t len;                  /* utility (length of srsaddr) */
  srs_t *srs        = NULL;    /* srs structure */
  char  *srsaddr    = NULL;    /* srs address storage */

  if (secret == NULL || addr == NULL)                             
  {
    printf("do_reverse :: Passed a NULL char!  Abort!\n");
    return(FALSE);
  } 

  if (QUIET == FALSE)
  {
    printf("SRS Reverse: envelope-from: (%s)\n", addr);
  }
  
  gettimeofday(&srs_time[0].start, &srs_time[0].tzp);

  srs = SRS_new(secret, age, hash_len, hash_min);
  len = strlen(srsaddr);

  /* simulate HOSTD reversing address */
  if ((srsaddr = SRS_reverse(srs, addr)) == NULL)
  {
    printf("Rewriting of address (%s) failed!  !@$#\n", addr);
    srs = SRS_del(srs);
    return(FALSE);
  }
  else
  {
    printf("SRS Reverse complete: HMAC verify succeeded, bouncing email : "
      "enevelope-from: (%s) : %lu\n", srsaddr, (u_long)len);
    return(TRUE);
  }

  if (QUIET == FALSE)
  {
    printf("SRS Reverse complete: HMAC verify failed, dropping email : "
      "enevelope-from: (%s) : %lu\n", srsaddr, (u_long)len);
  
    gettimeofday(&srs_time[0].finish, &srs_time[0].tzp);
    printf("Rewrite executed in %lu.%u seconds\n",
      (srs_time[0].finish.tv_sec    - srs_time[0].start.tv_sec),
      (u_int8_t)(srs_time[0].finish.tv_usec - srs_time[0].start.tv_usec));
  }
 
  xfree(secret);
  xfree(addr);
  xfree(srsaddr);
  srs = SRS_del(srs);
  
  return(FALSE);
}

SRS_BOOL do_default(SRS_BOOL QUIET)
{
  srs_t         *srs        = NULL;     /* srs structure */

  char          *srsaddr    = NULL;     /* srs address structure */
  char          *addr1      = NULL;     /* email address from */
  char          *addr2      = NULL;     /* email address to  */
  char          *addr3      = NULL;     /* email address forwarded to */
  char          *addr4      = NULL;     /* email address forwarded to again */

  gettimeofday(&srs_time[0].start, &srs_time[0].tzp);

  srs   = SRS_new((char *)"asshat", (int32_t)THREE_DAYS, 4, 4);
  addr1 = strdup((char *)"james@hosta.org");
  addr2 = strdup((char *)"james@hostb.org");
  addr3 = strdup((char *)"james@hostc.org");
  addr4 = strdup((char *)"james@hostd.org");

  /* simulate email from HOSTA to HOSTB */
  if ((srsaddr = SRS_forward(srs, addr1, addr2)) == NULL)
  {
    printf("HOSTB: Rewriting of address failed!  !@$#\n");
    srs = SRS_del(srs);
    return(FALSE);
  }

  if (QUIET == FALSE)
  {
    printf("From: (%s) to: (%s) which forwards to: (%s)\n", addr1, addr2, addr3);
    printf("HOSTA Send: envelope-from: (%s) : %lu\n", addr1, (u_long)strlen(addr1));  
    printf("HOSTB Rewrite: envelope-from: (%s) : %lu\n", srsaddr, (u_long)strlen(srsaddr));
  }
  
  xfree(addr1);
  addr1 = NULL;
  xfree(addr2);
  addr2 = strdup(srsaddr);
  xfree(srsaddr);
  srsaddr = NULL;
  
  if (addr3 != NULL)
  {  
    /* simulate email from HOSTA to HOSTB forwarded to HOSTC */
    if ((srsaddr = SRS_forward(srs, addr2, addr3)) == NULL)
    {
      printf("HOSTC: Rewriting of address failed!  !@$#\n");
      srs = SRS_del(srs);
      return(FALSE);
    }

    if (QUIET == FALSE)
    {
      printf("HOSTC Rewrite: envelope-from: (%s) : %lu\n", srsaddr, (u_long)strlen(srsaddr));
    }

    xfree(addr2);
    addr2 = NULL;
    xfree(addr3);
    addr3 = strdup(srsaddr);
    xfree(srsaddr);
    srsaddr = NULL;
  }
  else 
  {
    srsaddr = addr2;
  }
  
  if (addr4 != NULL)
  { 

    /* simulate email from HOSTA to HOSTB forwarded to HOSTC forwarded to HOSTD */
    if ((srsaddr = SRS_forward(srs, addr3, addr4)) == NULL)
    {
      printf("HOSTB: Rewriting of address failed!  !@$#\n");
      srs = SRS_del(srs);
      return(FALSE);
    }

    if (QUIET == FALSE)
    {
      printf("HOSTD Rewrite: envelope-from: (%s) : %lu\n", srsaddr, (u_long)strlen(srsaddr));
    }
    
    xfree(addr3);
    xfree(addr4);
  }
  else if (addr3 != NULL)
  {
    srsaddr = addr3;
  }
  else
  {
    srsaddr = addr2;
  }
  
  /* simulate HOSTD reversing address */
  if ((addr3 = SRS_reverse(srs, srsaddr)) == NULL)
  {
    printf("HOSTD Rewriting of address failed!  !@$#\n");
    srs = SRS_del(srs);
    return(FALSE);
  }

  if (QUIET == FALSE)
  {
    printf("HOSTB Bounce: envelope-from: (%s) : %lu\n", addr3, (u_long)strlen(addr3));
  }
  
  xfree(srsaddr);

  /* simulate HOSTB receiving the bounce, and verifying the hash */
  if ((addr2 = SRS_reverse(srs, addr3)) == NULL)
  {
    printf("Rewriting of address failed!  !@$#\n");
    srs = SRS_del(srs);
    return(FALSE);
  }
  else
  {
    if (QUIET == FALSE)
    {
      printf("HOSTB HMAC VERIFIED : enevelope-from: (%s) : %lu\n", 
        addr2, (u_long)strlen(addr2));
    }
  }
  
  if (QUIET == FALSE)
  {
    gettimeofday(&srs_time[0].finish, &srs_time[0].tzp);
    printf("Rewrite executed in %lu.%u seconds\n",
      (srs_time[0].finish.tv_sec - srs_time[0].start.tv_sec),
      (u_int8_t)(srs_time[0].finish.tv_usec - srs_time[0].start.tv_usec));
  }
  
  xfree(addr3);
  xfree(addr2);
  srs = SRS_del(srs);
  
  return(TRUE);
}

void do_help(void)
{
  printf("Useage: srs [options]:\nOptions:\n"
    "  -a\t\tValid age of hash (in seconds) EG: 1 day = 86400\n"
    "  -b\t\tShortest hash to auth against (default 4)\n"
    "  -c\t\tNbytes of Base64 chars to use (default 4)\n"
    "  -d\t\tDebug: 0 = all debugging off (default)\n"
    "\t\t- 1 = regular (general debugging output from SRS_ functions)\n"
    "\t\t- 2 = verbose (general debugging from UTIL functions)\n"
    "\t\t- 3 = both (combined regular and verbose debugging)\n");
  printf("\t\t- 4 = profiling (timers on HMAC and base64 enc/decoding)\n"
    "\t\t- 5 = regular + profile (regular debugging with profile)\n"
    "\t\t- 6 = verbose + profile (verbose debugging with profile)\n"
    "\t\t- 7 = regular, verbose, and profile (Super Mega Debugging!!! !@#$)\n"
    "  -f f@d.tld\tPerform an SRS forward using f@d.tld as the from address\n");
  printf(
    "  -q\tQuiet : For testing purposes, quiets all output persiod.\n"
    "  -r RWA\tPerform an SRS reverse using RWA (eg: SRS0=HiSf=FT=dom=user@fw.tld)\n"
    "  -s\t\tSecret to use when crypting\n"
    "  -t t@d.tld\tTo address.  Required for SRS Forward\n"
    "  -z\t\tTest mode, fairly complete API test.  Use -d 1 or -d 2 for.\n\n");
    
#ifndef DEBUG
  printf("To enable regular and verbose debugging compile with -DDEBUG\n");
#endif
  
  return;
}

/* end srsquery.c */
