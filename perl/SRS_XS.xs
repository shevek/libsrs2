#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>
#include "../libsrs/srs.h"

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
		SV	**svp;
	CODE:
		svp = hv_fetch(args, "Secret", 6, FALSE);
		if (!SvPOK(*svp))
			croak("Usage: new( { Secret => 'secret' } )");
		srs_t	*srs = srs_new();
		srs_add_secret(srs, SvPV_nolen(*svp));
		RETVAL = srs;
	OUTPUT:
		RETVAL

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
