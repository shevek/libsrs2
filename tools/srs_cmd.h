/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: srs.h
*  DESC: libsrs API sample program and test app header
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

#ifndef _SRS_CMD_H
#define _SRS_CMD_H 1

#include "../libsrs/srs.h"
#include "../libsrs/util.h"

__BEGIN_DECLS

#define SRS_BVERSION "0.3"

/* timers for 'profiling' if you will */
srs_time_t srs_time[3];

enum SRS_ACT
{
  SRS_FORWARD = 0,
  SRS_REVERSE,
  SRS_DEFAULT
};

SRS_BOOL  do_forward(char *secret, int32_t age, int32_t hash_len, int32_t hash_min, 
            char *addr1, char *addr2, SRS_BOOL QUIET);
SRS_BOOL  do_reverse(char *secret, int32_t age, int32_t hash_len, int32_t hash_min, 
            char *addr, SRS_BOOL QUIET);
SRS_BOOL  do_default(SRS_BOOL QUIET);
void      do_help(void);

__END_DECLS

#endif

/* end srs_cmd.h */
