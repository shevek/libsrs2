SUBDIRS = libsrs tools
EXTRA_DIST = perl

dist-hook:
	find $(distdir)/perl -name .svn | xargs rm -rf
	rm -rf $(distdir)/perl/Makefile.old
