/* libsrs - Sender Rewriting Scheme library
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: main.h
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

#ifndef _MAIN_H
#define _MAIN_H  1

#include "srs.h"            /* libsrs structures / defines */
#include "util.h"           /* libsrs utility */
#include "crypto.h"         /* libsrs crypto */
#include "error.h"          /* libsrs error handling */

__BEGIN_DECLS

#define SRS0            0           /* srs address tag type */
#define SRS1            1           /* srs address wrap type */
#define SRS_TAG         "SRS0"      /* root re-writer tag */
#define SRS_WRAP        "SRS1"      /* 1 + N re-writer tag */

#define PRECISION       (60*60*24)  /* used in timestamp creation/validation */
#define THREE_DAYS      259200      /* used when comparing timestamps */
#define TICKSLOTS       1024        /* two base32 chars */
#define MAX_TIMESTAMP   3           /* max len of timestamp */
#define MAX_RTYPE       5           /* max len of an RTYPE (SRS0, SRS1) */
#define MAX_SECRET      32          /* max len of secret */
#define MAX_DIGEST      33          /* max len of digest */
#define MAX_HOSTNAME    256         /* max len of hostname domain */ 
#define MAX_RWA         1024        /* max len of re-written address */
#define MAX_DEBUG_LEN   1024        /* max len of debug output */
 
/*
* In ANSI C, and indeed any rational implementation, size_t is also the
* type returned by sizeof().  However, it seems there are some irrational
* implementations out there, in which sizeof() returns an int even though
* size_t is defined as long or unsigned long.  To ensure consistent results
* we always use this SIZEOF() macro in place of using sizeof() directly.
*/
#define SIZEOF(object)  ((size_t) sizeof(object))

/* for handing debug modes */
#define f_bit_set(fl_bit_vector, bit) ((int)((fl_bit_vector)&(bit))) 

srs_t     *SRS_init(void);
srs_t     *SRS_new(const char *secret, const int32_t age,
            const int16_t hash_len, const int16_t hash_min);
srs_t     *SRS_del(srs_t *srs);
char      *SRS_forward(srs_t *srs, char *sender, char *alias);
char      *SRS_reverse(srs_t *srs, char *addr);
char      *SRS_compile(srs_t *srs, srs_addr_t *addr);
char      *SRS_parse(srs_t *srs, char *user);
char      *SRS_timestamp_create(time_t timestamp);
SRS_BOOL  SRS_timestamp_check(srs_t *srs, const char *timestamp);
SRS_BOOL  SRS_time_check(srs_t *srs);
u_char    *SRS_hash_create(const char *secret, const int32_t s_len,
            const u_char *data, const int32_t d_len);
SRS_BOOL  SRS_hash_verify(const char *secret, const int32_t s_len,
            const u_char *data, const int32_t d_len, const u_char *digest,
            const int16_t hash_min, const int16_t hash_len);
SRS_BOOL  SRS_set_secret(srs_t *srs, const char *s);

__END_DECLS

#endif

/* end main.h */
