/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: srs.h
*  DESC: main library header file 
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

#ifndef _SRS_H
#define _SRS_H  1

#include <time.h>       /* time */
#include <sys/types.h>  /* tyepdefs */
#include <sys/time.h>   /* timeval / timezone struct */

__BEGIN_DECLS

/* typedefs for working with nicer signed and unsigned types */
#ifdef USE_SUNOS 
typedef unsigned char  u_char;
typedef unsigned char  u_int8_t;
typedef unsigned short int  u_int16_t;
typedef unsigned int  u_int32_t;
typedef char  int8_t;
typedef short int  int16_t;
typedef int  int32_t;
#endif

#define SRS_LVERSION    "0.3"       /* Version of this library */
#define SRS_VERSION     1           /* SRS Protocol Version we support */

/* debugging logfile */
#define DEBUG_LOG_FILE  "/var/log/srs_debug"

/* SRS_BOOL
*
*  Our own internal boolean enumeration, simple true or false.
*
*/
typedef enum SRS_BOOL
{
  FALSE = 0,
  TRUE
} SRS_BOOL;


/* srs_time_t
*
* Global config structure
*
*/
typedef struct srs_time_s
{
  struct timeval  start;    /* sample start */
  struct timeval  finish;   /* sample finish */
  struct timezone tzp;      /* timezone */
} srs_time_t;

/* srs_config_t
*
* Global config structure
*
*/
typedef struct srs_config_s
{
  u_int8_t level;         /* level of output 0 = off */
} srs_config_t;


/* srs_data_s
*
* Address structure used to store user@domain.tld and the
* length of each string.
*
*/
typedef struct srs_s
{
  char      *secret;     /* secret/key */
  int32_t   s_len;       /* length of secret/key */

  int16_t   hash_len;    /* base64 chars: 4 x 64 bits */
  int16_t   hash_min;    /* base64 chars */
  int32_t   age;         /* maximum age (days) */
} srs_t;


/* srs_addr_t
*
* Email address broken into user / host.
*
*/
typedef struct srs_addr_s
{
  char        *user;    /* local-part */
  int16_t     u_len;    /* length of local-part */
  char        *host;    /* domain.tld */
  int16_t     h_len;    /* length of domain.tld */
} srs_addr_t;


extern srs_config_t  conf;          /* global config structure */
extern srs_time_t    srs_time[3];   /* timer structure for profile */

extern srs_t     *SRS_init(void);
extern srs_t     *SRS_new(const char *secret, const int32_t age,
                    const int16_t hash_len, const int16_t hash_min);
extern srs_t     *SRS_del(srs_t *srs);
extern char      *SRS_forward(srs_t *srs, char *sender, char *alias);
extern char      *SRS_reverse(srs_t *srs, char *addr);
extern char      *SRS_compile(srs_t *srs, srs_addr_t *addr);
extern char      *SRS_parse(srs_t *srs, char *user);
extern char      *SRS_timestamp_create(time_t timestamp);
extern SRS_BOOL  SRS_timestamp_check(srs_t *srs, const char *timestamp);
extern SRS_BOOL  SRS_time_check(srs_t *srs);
extern u_char    *SRS_hash_create(const char *secret, const int32_t s_len,
                    const u_char *data, const int32_t d_len);
extern SRS_BOOL  SRS_hash_verify(const char *secret, const int32_t s_len,
                    const u_char *data, const int32_t d_len, const u_char *digest,
                    const int16_t hash_min, const int16_t hash_len);
extern SRS_BOOL  SRS_set_secret(srs_t *srs, const char *s);

__END_DECLS

#endif

/* end srs.h */
