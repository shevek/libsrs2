#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

#undef VERSION	/* From XS command line */
#include "../libsrs2/srs2.h"

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

typedef srs_t *Mail__SRS_XS;

MODULE = Mail::SRS_XS		PACKAGE = Mail::SRS_XS

PROTOTYPES: ENABLE

Mail::SRS_XS
new(class, args)
	SV*	class
	HV*	args
	PREINIT:
		SV		**svp;
		srs_t	 *srs;
	CODE:
		svp = hv_fetch(args, "Secret", 6, FALSE);
		if (!SvPOK(*svp))
			croak("Usage: new( { Secret => 'secret' } )");
		Newz(0, srs, 1, srs_t);
		srs_init(srs);
		srs_add_secret(srs, SvPV_nolen(*svp));
		RETVAL = srs;
	OUTPUT:
		RETVAL

void
DESTROY(srs)
	Mail::SRS_XS	srs
	CODE:
		srs_fini(srs);
		Safefree(srs);

const char *
forward(srs, sender, alias)
	Mail::SRS_XS	srs
	const char *	sender
	const char *	alias
	PREINIT:
		char	buf[BUFSIZ];
		int		ret;
	CODE:
		ret = srs_forward(srs, buf, BUFSIZ, sender, alias);
		if (ret != SRS_SUCCESS)
			croak(srs_strerror(ret));
		else
			RETVAL = buf;
	OUTPUT:
		RETVAL

const char *
reverse(srs, sender)
	Mail::SRS_XS	srs
	const char *	sender
	PREINIT:
		char	buf[BUFSIZ];
		int		ret;
	CODE:
		ret = srs_reverse(srs, buf, BUFSIZ, sender);
		if (ret != SRS_SUCCESS)
			croak(srs_strerror(ret));
		else
			RETVAL = buf;
	OUTPUT:
		RETVAL
