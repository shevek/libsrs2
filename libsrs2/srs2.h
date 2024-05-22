/* Copyright (c) 2004 Shevek (srs@anarres.org)
 * All rights reserved.
 *
 * This file is a part of libsrs2 from http://www.libsrs2.net/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, under the terms of either the GNU General Public
 * License version 2 or the BSD license, at the discretion of the
 * user. Copies of these licenses have been included in the libsrs2
 * distribution. See the the file called LICENSE for more
 * information.
 */

#ifndef __SRS2_H__
#define __SRS2_H__

__BEGIN_DECLS

#define SRS_VERSION_MAJOR			1
#define SRS_VERSION_MINOR			0
#define SRS_VERSION_PATCHLEVEL		14
#define SRS_VERSION_FROM(m, n, p)	(((m) << 16) + ((n) << 8) + (p))
#define SRS_VERSION		SRS_VERSION_FROM(SRS_VERSION_MAJOR, \
										SRS_VERSION_MINOR, \
										SRS_VERSION_PATCHLEVEL)

/* This is ugly, but reasonably safe. */
#undef TRUE
#define TRUE 1
#undef FALSE
#define FALSE 0

#define SRSSEP	'='
#define SRS0TAG	"SRS0"
#define SRS1TAG	"SRS1"

/* Error codes */

#define SRS_ERRTYPE_MASK		0xF000
#define SRS_ERRTYPE_NONE		0x0000
#define SRS_ERRTYPE_CONFIG		0x1000
#define SRS_ERRTYPE_INPUT		0x2000
#define SRS_ERRTYPE_SYNTAX		0x4000
#define SRS_ERRTYPE_SRS			0x8000

#define SRS_SUCCESS				(0)
#define SRS_ENOTSRSADDRESS		(1)

#define SRS_ENOSECRETS			(SRS_ERRTYPE_CONFIG | 1)
#define SRS_ESEPARATORINVALID	(SRS_ERRTYPE_CONFIG | 2)

#define SRS_ENOSENDERATSIGN		(SRS_ERRTYPE_INPUT | 1)
#define SRS_EBUFTOOSMALL		(SRS_ERRTYPE_INPUT | 2)

#define SRS_ENOSRS0HOST			(SRS_ERRTYPE_SYNTAX | 1)
#define SRS_ENOSRS0USER			(SRS_ERRTYPE_SYNTAX | 2)
#define SRS_ENOSRS0HASH			(SRS_ERRTYPE_SYNTAX | 3)
#define SRS_ENOSRS0STAMP		(SRS_ERRTYPE_SYNTAX | 4)
#define SRS_ENOSRS1HOST			(SRS_ERRTYPE_SYNTAX | 5)
#define SRS_ENOSRS1USER			(SRS_ERRTYPE_SYNTAX | 6)
#define SRS_ENOSRS1HASH			(SRS_ERRTYPE_SYNTAX | 7)
#define	SRS_EBADTIMESTAMPCHAR	(SRS_ERRTYPE_SYNTAX | 8)
#define SRS_EHASHTOOSHORT		(SRS_ERRTYPE_SYNTAX | 9)

#define SRS_ETIMESTAMPOUTOFDATE	(SRS_ERRTYPE_SRS | 1)
#define SRS_EHASHINVALID		(SRS_ERRTYPE_SRS | 2)

#define SRS_ERROR_TYPE(x) ((x) & SRS_ERRTYPE_MASK)

/* SRS implementation */

#define SRS_IS_SRS_ADDRESS(x) ( \
				(strncasecmp((x), "SRS", 3) == 0) && \
				(strchr("01", (x)[3]) != NULL) && \
				(strchr("-+=", (x)[4]) != NULL) \
			)

typedef void *(*srs_malloc_t)(size_t);
typedef void *(*srs_realloc_t)(void *, size_t);
typedef void (*srs_free_t)(void *);

typedef int srs_bool;

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
	srs_bool  alwaysrewrite;	/* Rewrite even into same domain? */
} srs_t;

/* Interface */
int		 srs_set_malloc(srs_malloc_t m, srs_realloc_t r, srs_free_t f);
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
int		 srs_add_secret(srs_t *srs, const char *secret);
const char *
		 srs_get_secret(srs_t *srs, int idx);
	/* You probably shouldn't call these. */
int		 srs_timestamp_create(srs_t *srs, char *buf, time_t now);
int		 srs_timestamp_check(srs_t *srs, const char *stamp);

#define SRS_PARAM_DECLARE(n, t) \
	int srs_set_ ## n (srs_t *srs, t value); \
	t srs_get_ ## n (srs_t *srs);

SRS_PARAM_DECLARE(alwaysrewrite, srs_bool)
SRS_PARAM_DECLARE(separator, char)
SRS_PARAM_DECLARE(maxage, int)
SRS_PARAM_DECLARE(hashlength, int)
SRS_PARAM_DECLARE(hashmin, int)

/* SHA1 implementation */

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
