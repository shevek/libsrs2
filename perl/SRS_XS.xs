#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

#undef VERSION	/* From XS command line */
#include "../libsrs2/srs2.h"

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

typedef srs_t *Mail__SRS_XS;

#define SRS_SET(s, x, v) do { \
		ret = srs_set_ ## x(s, v); \
		if (ret != SRS_SUCCESS) { \
			srs_free(srs); \
			croak("Error from SRS library: %s", srs_strerror(ret)); \
		} \
	} while(0)

MODULE = Mail::SRS_XS		PACKAGE = Mail::SRS_XS

PROTOTYPES: ENABLE

Mail::SRS_XS
new(class, args)
	SV*	class
	HV*	args
	PREINIT:
		SV		**svp;
		srs_t	 *srs;
		int		  ret;
	CODE:
		svp = hv_fetch(args, "Secret", 6, FALSE);
		if (!SvPOK(*svp))
			croak("Usage: new( { Secret => 'secret', ... } )");
		srs = srs_new();
		srs_add_secret(srs, SvPV_nolen(*svp));

		svp = hv_fetch(args, "Separator", 9, FALSE);
		if (svp && SvPOK(*svp))
			SRS_SET(srs, separator, (SvPV_nolen(*svp))[0]);

		svp = hv_fetch(args, "MaxAge", 6, FALSE);
		if (svp && SvIOK(*svp))
			SRS_SET(srs, maxage, (SvIV(*svp)));

		svp = hv_fetch(args, "HashLength", 9, FALSE);
		if (svp && SvIOK(*svp))
			SRS_SET(srs, hashlength, (SvIV(*svp)));

		svp = hv_fetch(args, "HashMin", 7, FALSE);
		if (svp && SvIOK(*svp))
			SRS_SET(srs, hashmin, (SvIV(*svp)));

		RETVAL = srs;
	OUTPUT:
		RETVAL

void
DESTROY(srs)
	Mail::SRS_XS	srs
	CODE:
		srs_free(srs);

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
