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

#include "../config.h"

#ifdef HAVE_TIME_H
#include <time.h>       /* time */
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>  /* tyepdefs */
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>   /* timeval / timezone struct */
#endif

#ifdef _WIN32
#include "win32.h"
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

#define SRS_SUCCESS				0
#define SRS_ENOSENDERATSIGN		1
#define SRS_EBUFTOOSMALL		2
#define	SRS_EBADTIMESTAMPCHAR	3
#define SRS_ETIMESTAMPOUTOFDATE	4
#define SRS_ENOSRS0HOST			5
#define SRS_ENOSRS0USER			6
#define SRS_ENOSRS1HOST			7
#define SRS_ENOSRS1USER			8
#define SRS_EHASHTOOSHORT		9
#define SRS_EHASHINVALID		10
#define SRS_ENOSECRETS			11
#define SRS_ENOTSRSADDRESS		12
#define SRS_ENOSRS1HASH			13
#define SRS_ENOSRS0HASH			14
#define SRS_ENOSRS0STAMP		15

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
int		 srs_forward(srs_t *srs, char *buf, int buflen,
				const char *sender, const char *alias);
int		 srs_reverse(srs_t *srs, char *buf, int buflen,
				const char *sender);
const char *
		srs_strerror(int code);
void	 srs_add_secret(srs_t *srs, const char *secret);

#define SRS_PARAM_DECLARE(n, t) \
	void srs_set_ ## n (srs_t *srs, t value); \
	t srs_get_ ## n (srs_t *srs);


__END_DECLS

#endif
