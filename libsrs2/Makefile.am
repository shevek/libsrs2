CFLAGS = -Wall @CFLAGS@

include_HEADERS = srs2.h
noinst_HEADERS = win32.h

lib_LIBRARIES = libsrs2.a
libsrs2_a_SOURCES = srs2.c sha1.c

lib_LTLIBRARIES = libsrs2.la
libsrs2_la_LDFLAGS = -version-info 0:0:0
libsrs2_la_SOURCES = srs2.c sha1.c

# -Wl,-soname,libsrs.so.1
