/* libsrs - Sender Rewriting Scheme library
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: base.h
*  DESC: base encoding header file
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

#ifndef _BASE_H
#define _BASE_H  1

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc / free */
#include <string.h>     /* memcpy, sprintf etc.. */
#include <strings.h>    /* casecmp etc.. */

__BEGIN_DECLS

#define BASE_32_INVALID 32

#define BASE_32 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  \
                "234567"
                 
#define BASE_64 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  \
                "abcdefghijklmnopqrstuvwxyz"  \
                "0123456789+/"

u_char    *BASE_10_64_s(const u_char *s);
u_char    BASE_10_64_c(u_char c);
u_char    BASE_64_10_c(u_char c);
u_char    BASE_10_32_c(u_int8_t c);
u_int8_t  BASE_32_10_c(u_char c);
u_char    BASE_32_itoc(u_int8_t c);
u_int8_t  BASE_32_ctoi(u_char c);

__END_DECLS

#endif

/* end base.h */
