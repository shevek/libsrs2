SUBDIRS = . tools

lib_LIBRARIES = libsrs.a
libsrs_a_SOURCES = main.c util.c crypto.c base.c error.c

lib_LTLIBRARIES = libsrs.la
libsrs_la_LIBADD = libsrs.a -lcrypto
libsrs_la_LDFLAGS = -Wl,-soname,libsrs.so.1
libsrs_la_SOURCES =
