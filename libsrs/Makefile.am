noinst_HEADERS = main.h util.h crypto.h base.h error.h
include_HEADERS = srs.h

lib_LIBRARIES = libsrs.a
libsrs_a_SOURCES = main.c util.c crypto.c base.c error.c

lib_LTLIBRARIES = libsrs.la
libsrs_la_LIBADD = -lcrypto
libsrs_la_LDFLAGS = -version-info 0:0:0
libsrs_la_SOURCES = main.c util.c crypto.c base.c error.c

# -Wl,-soname,libsrs.so.1
