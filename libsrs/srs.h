#ifndef __SRS_H__
#define __SRS_H__

#include <time.h>       /* time */
#include <sys/types.h>  /* tyepdefs */
#include <sys/time.h>   /* timeval / timezone struct */

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


#define SRS_PARAM_DECLARE(n, t) \
	void srs_set_ ## n (srs_t *srs, t value); \
	t srs_get_ ## n (srs_t *srs);

__END_DECLS

#endif
