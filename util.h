/* libsrs - Sender Rewriting Scheme library
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: util.h
*  DESC: utility functions header file
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

#ifndef _UTIL_H
#define _UTIL_H 1

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc / free */
#include <stdarg.h>     /* va_list */
#include <string.h>     /* memcpy, sprintf etc.. */
#include <strings.h>    /* casecmp etc.. */    

#include "main.h"       /* libsrs private defines etc.. */
#include "srs.h"        /* libsrs public defines etc... */

__BEGIN_DECLS

#define FL_A            2           /* normal debugging */
#define FL_B            4           /* verbose debugging */
#define FL_C            8           /* normal and verbose debugging */
#define FL_D            16          /* profile results */
#define FL_E            32          /* normal debug + profile results */
#define FL_F            64          /* verbose debug + profile results */
#define FL_G            128         /* normal + verbose + profile results */

/* malloc wrapper macros */
#define xmalloc(n)      UTIL_malloc(n, __FILE__, __LINE__, __FUNCTION__)
#define xrealloc(m, n)  UTIL_realloc(m, n, __FILE__, __LINE__, __FUNCTION__)
#define xfree(m)        UTIL_free(m, __FILE__, __LINE__, __FUNCTION__)

/* printf wrapper macros */
#define xprintf(format,...)   dbg_printf(FL_A, __FUNCTION__, format, __VA_ARGS__)
#define xvprintf(format,...)  dbg_printf(FL_B, __FUNCTION__, format, __VA_ARGS__)
#define xpprintf(format,...)  dbg_printf(FL_D, __FUNCTION__, format, __VA_ARGS__)
#define xeprintf(s)           fprintf(stderr, s)

#ifdef _GNU_SOURCE 
#define xstrndup(s, n) strndup(s, n)
#else
/* strndup wrapper macro for non GNU users */
#define xstrndup(s, n) UTIL_strndup(s, n)
#endif

#if defined DEBUG
#define dbg_printf _printf_dbg
#endif

#ifndef DEBUG
#define dbg_printf dummy_debug
#endif

void        _printf_dbg(const u_int8_t level, const char *function,
              const char *format,...);
void        dummy_debug(const u_int8_t level, const char *function,
              const char *format,...);
void        *UTIL_malloc(const int32_t, const char *, const int32_t, 
              const char *);
void        *UTIL_realloc(void *, const int32_t, const char *, const int32_t,
              const char *);
void        UTIL_free(void *, const char *, const int32_t, const char *);
char        *UTIL_strndup(const char *s, const size_t n);
int32_t     UTIL_varlen(const int32_t, ...);
u_char      *UTIL_create_hashdata(const char *, const char *, const char *);
srs_addr_t  *UTIL_split_addr(const char *);
char        *UTIL_get_rtype_sep(char *s);

__END_DECLS

#endif

/* end util.h */
