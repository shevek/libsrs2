#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "srs.h"

static const char *srs_separators = "=-+";

const char *
srs_strerror(int code)
{
	/* XXX Fill these in */
	return "Error!";
}

srs_t *
srs_new()
{
	srs_t	*srs = (srs_t *)malloc(sizeof(srs_t));
	srs->secrets = NULL;
	srs->numsecrets = 0;
	srs->separator = '=';
	srs->maxage = 21;
	srs->hashlength = 4;
	srs->hashmin = srs->hashlength;
	srs->alwaysrewrite = FALSE;
	return srs;
}

void
srs_add_secret(srs_t *srs, const char *secret)
{
	int		newlen = (srs->numsecrets + 1) * sizeof(char *);
	srs->secrets = (char **)realloc(srs->secrets, newlen);
	srs->secrets[srs->numsecrets++] = strdup(secret);
}

void
srs_add_secrets(srs_t *srs, FILE *fp)
{
	char	line[BUFSIZ];
	while (fgets(line, BUFSIZ, stdin)) {
		line[strcspn(line, "\r\n")] = '\0';
		srs_add_secret(srs, line);
	}
}

#define SRS_PARAM_DEFINE(n, t) \
	void srs_set_ ## n (srs_t *srs, t value) { \
		srs->n = value; \
	} \
	t srs_get_ ## n (srs_t *srs) { \
		return srs->n; \
	}

	/* XXX Check separator is valid */
SRS_PARAM_DEFINE(separator, char)
SRS_PARAM_DEFINE(maxage, int)
	/* XXX Check hashlength >= hashmin */
SRS_PARAM_DEFINE(hashlength, int)
SRS_PARAM_DEFINE(hashmin, int)
SRS_PARAM_DEFINE(alwaysrewrite, bool)

/* Don't mess with these unless you know what you're doing well
 * enough to rewrite the timestamp functions. These are based on
 * a 2 character timestamp. Changing these in the wild is probably
 * a bad idea. */
#define SRS_TIME_PRECISION	(60 * 60 * 24)	/* One day */
#define SRS_TIME_BASEBITS	5		/* 2^5 = 32 = strlen(CHARS) */
/* This had better be a real variable since we do arithmethic
 * with it. */
const char *SRS_TIME_BASECHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
#define SRS_TIME_SIZE		2
#define SRS_TIME_SLOTS		(1<<(SRS_TIME_BASEBITS<<(SRS_TIME_SIZE-1)))

static void
srs_timestamp_create(srs_t *srs, char *buf)
{
	time_t	now;

	time(&now);
	now = now / SRS_TIME_PRECISION;
	buf[0] = SRS_TIME_BASECHARS[now & ((1 << SRS_TIME_BASEBITS) - 1)];
	now = now >> SRS_TIME_BASEBITS;
	buf[1] = SRS_TIME_BASECHARS[now & ((1 << SRS_TIME_BASEBITS) - 1)];
	buf[2] = '\0';
}

static int
srs_timestamp_check(srs_t *srs, char *stamp)
{
	char	*sp;
	char	*bp;
	int		 off;
	time_t	 now;
	time_t	 then;

	/* We had better go around this loop exactly twice! */
	then = 0;
	for (sp = stamp; *sp; sp++) {
		bp = strchr(SRS_TIME_BASECHARS, *sp);
		if (bp == NULL)
			return SRS_EBADTIMESTAMPCHAR;
		off = SRS_TIME_BASECHARS - bp;
		then = (then << SRS_TIME_BASEBITS) + off;
	}

	time(&now);
	now = now / SRS_TIME_PRECISION;
	while (now < then)
		now = now + SRS_TIME_SLOTS;

	if (now <= then + srs->maxage)
		return SRS_SUCCESS;
	return SRS_ETIMESTAMPOUTOFDATE;
}

static void
srs_hash_create_v(srs_t *srs, char *buf, int nargs, va_list ap)
{
	strncpy(buf, "XXXXHASH", srs->hashlength);
	buf[srs->hashlength] = '\0';
}

void
srs_hash_create(srs_t *srs, char *buf, int nargs, ...)
{
	va_list	ap;
	va_start(ap, nargs);
	srs_hash_create_v(srs, buf, nargs, ap);
	va_end(ap);
}

int
srs_hash_check(srs_t *srs, char *hash, int nargs, ...)
{
	va_list	ap;
	char	*srshash;
	char	*tmp;
	int		 len;

	len = strlen(hash);
	if (len < srs->hashmin)
		return SRS_EHASHTOOSHORT;
	if (len < srs->hashlength) {
		tmp = alloca(srs->hashlength + 1);
		strncpy(tmp, hash, srs->hashlength);
		tmp[srs->hashlength] = '\0';
		hash = tmp;
		len = srs->hashlength;
	}
	va_start(ap, nargs);
	srshash = alloca(srs->hashlength + 1);
	srs_hash_create_v(srs, srshash, nargs, ap);
	va_end(ap);
	if (strncasecmp(hash, srshash, len) != 0)
		return SRS_EHASHINVALID;
	return SRS_SUCCESS;
}

int
srs_compile_shortcut(srs_t *srs,
				char *buf, int buflen,
				char *sendhost, char *senduser,
				const char *aliashost) {
	char	*srshash;
	char	 srsstamp[SRS_TIME_SIZE + 1];
	int		 len;

	/* This never happens if we get called from guarded() */
	if ((strncasecmp(senduser, "SRS0", 4) == 0) &&
		(strchr(srs_separators, senduser[4]) != NULL)) {
		sendhost = senduser + 5;
		if (*sendhost == '\0')
			return SRS_ENOSRS0HOST;
		senduser = strchr(sendhost, SRSSEP);
		if ((senduser == NULL) || (*senduser == '\0'))
			return SRS_ENOSRS0USER;
	}

	len = strlen(SRS0TAG) + 1 +
		srs->hashlength + 1 +
			SRS_TIME_SIZE + 1 +
				strlen(sendhost) + 1 + strlen(senduser)
			+ 1 + strlen(aliashost);
	if (len >= buflen)
		return SRS_EBUFTOOSMALL;

	srs_timestamp_create(srs, srsstamp);
	srshash = alloca(srs->hashlength + 1);
	srs_hash_create(srs, srshash, 3, srsstamp, sendhost, senduser);

	sprintf(buf, SRS0TAG "%c%s%c%s%c%s%c%s@%s", srs->separator,
					srshash, SRSSEP, srsstamp, SRSSEP,
						sendhost, SRSSEP, senduser,
							aliashost);

	return SRS_SUCCESS;
}

int
srs_compile_guarded(srs_t *srs,
				char *buf, int buflen,
				char *sendhost, char *senduser,
				const char *aliashost) {
	char	*srshost;
	char	*srsuser;
	char	*srshash;
	int		 len;

	if ((strncasecmp(senduser, "SRS1", 4) == 0) &&
		(strchr(srs_separators, senduser[4]) != NULL)) {
		srshost = senduser + 5;
		if (*srshost == '\0')
			return SRS_ENOSRS1HOST;
		srsuser = strchr(srshost, SRSSEP);
		if ((srsuser == NULL) || (*srsuser == '\0'))
			return SRS_ENOSRS1USER;
		srshash = alloca(srs->hashlength + 1);
		srs_hash_create(srs, srshash, 2, srshost, srsuser);
		len = strlen(SRS1TAG) + 1 +
			srs->hashlength + 1 +
				strlen(srshost) + 1 + strlen(srsuser)
			+ 1 + strlen(aliashost);
		if (len >= buflen)
			return SRS_EBUFTOOSMALL;
		sprintf(buf, SRS1TAG "%c%s%c%s%c%s@%s", srs->separator,
						srshash, SRSSEP,
							srshost, SRSSEP, srsuser,
								aliashost);
		return SRS_SUCCESS;
	}
	else if ((strncasecmp(senduser, "SRS0", 4) == 0) &&
		(strchr(srs_separators, senduser[4]) != NULL)) {
		srshash = alloca(srs->hashlength + 1);
		srs_hash_create(srs, srshash, 2, sendhost, senduser);
		len = strlen(SRS1TAG) + 1 +
			srs->hashlength + 1 +
				strlen(sendhost) + 1 + strlen(senduser)
			+ 1 + strlen(aliashost);
		if (len >= buflen)
			return SRS_EBUFTOOSMALL;
		sprintf(buf, SRS1TAG "%c%s%c%s%c%s@%s", srs->separator,
						srshash, SRSSEP,
							sendhost, SRSSEP, senduser,
								aliashost);
	}
	else {
		return srs_compile_shortcut(srs, buf, buflen,
						sendhost, senduser, aliashost);
	}

	return SRS_SUCCESS;
}

int
srs_forward(srs_t *srs, char *buf, int buflen,
				const char *sender, const char *alias)
{
	char	*senduser;
	char	*sendhost;
	char	*tmp;
	int		 len;

	/* This is allowed to be a plain domain */
	while ((tmp = strchr(alias, '@')) != NULL)
		alias = tmp + 1;

	tmp = strchr(sender, '@');
	if (tmp == NULL)
		return SRS_ENOSENDERATSIGN;
	sendhost = tmp + 1;

	len = strlen(sender);

	if (! srs->alwaysrewrite) {
		if (strcasecmp(sendhost, alias) == 0) {
			if (strlen(sender) >= buflen)
				return SRS_EBUFTOOSMALL;
			strcpy(buf, sender);
			return SRS_SUCCESS;
		}
	}

	/* Reconstruct the whole show into our alloca() buffer. */
	senduser = alloca(len + 1);
	strcpy(senduser, sender);
	tmp = (senduser + (tmp - sender));
	sendhost = tmp + 1;
	*tmp = '\0';

	/* XXX Check strategy */
	return srs_compile_guarded(srs, buf, buflen, sendhost, senduser, alias);
}
