SUBDIRS = libsrs2 tools
EXTRA_DIST = perl win32

dist-hook:
	find $(distdir)/perl -name .svn | xargs rm -rf
	rm -rf $(distdir)/perl/Makefile.old
