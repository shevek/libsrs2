CFLAGS = -Wall @CFLAGS@

include_HEADERS = srs2.h

lib_LIBRARIES = libsrs2.a
libsrs2_a_SOURCES = srs2.c

lib_LTLIBRARIES = libsrs2.la
libsrs2_la_LIBADD = -lcrypto
libsrs2_la_LDFLAGS = -version-info 0:0:0
libsrs2_la_SOURCES = srs2.c

# -Wl,-soname,libsrs.so.1
