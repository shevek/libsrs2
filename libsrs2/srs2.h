/* Copyright (c) 2004 Shevek (srs@anarres.org)
 * All rights reserved.
 *
 * This file is a part of libsrs2 from http://www.libsrs2.org/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, under the terms of either the GNU General Public
 * License version 2 or the BSD license, at the discretion of the
 * user. Copies of these licenses have been included in the libsrs2
 * distribution. See the the file called LICENSE for more
 * information.
 */

#ifndef __SRS_H__
#define __SRS_H__

#ifdef _WIN32
#include "win32.h"
#else
# ifdef HAVE_CONFIG_H
#  include "../config.h"
# endif
#endif

#ifdef HAVE_TIME_H
#include <time.h>       /* time */
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>  /* tyepdefs */
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>   /* timeval / timezone struct */
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

__BEGIN_DECLS

/* This is ugly, but reasonably safe. */
#undef TRUE
#define TRUE 1
#undef FALSE
#define FALSE 0
#undef bool
#define bool char

#define SRSSEP	'='
#define SRS0TAG	"SRS0"
#define SRS1TAG	"SRS1"

#define SRS_ERRTYPE_MASK		0xF000
#define SRS_ERRTYPE_CONFIG		0x1000
#define SRS_ERRTYPE_INPUT		0x2000
#define SRS_ERRTYPE_SYNTAX		0x4000
#define SRS_ERRTYPE_BADSRS		0x8000

#define SRS_SUCCESS				(0)
#define SRS_ENOTSRSADDRESS		(1)

#define SRS_ENOSECRETS			(SRS_ERRTYPE_CONFIG | 1)

#define SRS_ENOSENDERATSIGN		(SRS_ERRTYPE_INPUT | 1)
#define SRS_EBUFTOOSMALL		(SRS_ERRTYPE_INPUT | 2)

#define SRS_ENOSRS0HOST			(SRS_ERRTYPE_SYNTAX | 1)
#define SRS_ENOSRS0USER			(SRS_ERRTYPE_SYNTAX | 2)
#define SRS_ENOSRS0HASH			(SRS_ERRTYPE_SYNTAX | 3)
#define SRS_ENOSRS0STAMP		(SRS_ERRTYPE_SYNTAX | 4)
#define SRS_ENOSRS1HOST			(SRS_ERRTYPE_SYNTAX | 5)
#define SRS_ENOSRS1USER			(SRS_ERRTYPE_SYNTAX | 6)
#define SRS_ENOSRS1HASH			(SRS_ERRTYPE_SYNTAX | 7)
#define SRS_ESEPARATORINVALID	(SRS_ERRTYPE_SYNTAX | 8)
#define	SRS_EBADTIMESTAMPCHAR	(SRS_ERRTYPE_SYNTAX | 9)
#define SRS_EHASHTOOSHORT		(SRS_ERRTYPE_SYNTAX | 10)

#define SRS_ETIMESTAMPOUTOFDATE	(SRS_ERRTYPE_BADSRS | 1)
#define SRS_EHASHINVALID		(SRS_ERRTYPE_BADSRS | 2)

#define SRS_ERROR_TYPE(x) ((x) & SRS_ERRTYPE_MASK)

/* SRS implementation */

typedef
struct _srs_t {
	/* Rewriting parameters */
	char	**secrets;
	int		  numsecrets;
	char	  separator;

	/* Security parameters */
	int		  maxage;			/* Maximum allowed age in seconds */
	int		  hashlength;
	int		  hashmin;

	/* Behaviour parameters */
	bool	  alwaysrewrite;	/* Rewrite even into same domain? */
} srs_t;

/* Interface */
srs_t	*srs_new();
void	 srs_init(srs_t *srs);
void	 srs_free(srs_t *srs);
int		 srs_forward(srs_t *srs, char *buf, int buflen,
				const char *sender, const char *alias);
int		 srs_forward_alloc(srs_t *srs, char **sptr,
				const char *sender, const char *alias);
int		 srs_reverse(srs_t *srs, char *buf, int buflen,
				const char *sender);
int		 srs_reverse_alloc(srs_t *srs, char **sptr, const char *sender);
const char *
		 srs_strerror(int code);
void	 srs_add_secret(srs_t *srs, const char *secret);

#define SRS_PARAM_DECLARE(n, t) \
	int srs_set_ ## n (srs_t *srs, t value); \
	t srs_get_ ## n (srs_t *srs);

SRS_PARAM_DECLARE(separator, char)
SRS_PARAM_DECLARE(maxage, int)
SRS_PARAM_DECLARE(hashlength, int)
SRS_PARAM_DECLARE(hashmin, int)

/* SHA1 implementation */

#if SIZEOF_UNSIGNED_LONG < 4
#error "SHA1 requires an unsigned long of at least 32 bits"
#endif
typedef unsigned long	ULONG;	 /* 32-or-more-bit quantity */
typedef unsigned char	sha_byte;

#define SHA_BLOCKSIZE				64
#define SHA_DIGESTSIZE				20

typedef struct {
	ULONG digest[5];				/* message digest */
	ULONG count_lo, count_hi;		/* 64-bit bit count */
	sha_byte data[SHA_BLOCKSIZE];		/* SHA data buffer */
	int local;						/* unprocessed amount in data */
} SHA_INFO;

typedef
struct _srs_hmac_ctx_t {
	SHA_INFO	sctx;
	char		ipad[SHA_BLOCKSIZE + 1];
	char		opad[SHA_BLOCKSIZE + 1];
} srs_hmac_ctx_t;

void	 srs_hmac_init(srs_hmac_ctx_t *ctx, char *secret, int len);
void	 srs_hmac_update(srs_hmac_ctx_t *ctx, char *data, int len);
void	 srs_hmac_fini(srs_hmac_ctx_t *ctx, char *out);


__END_DECLS

#endif
