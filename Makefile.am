SUBDIRS = libsrs2 tools
EXTRA_DIST = perl win32 dist

dist-hook:
	find $(distdir)/perl -name .svn | xargs rm -rf
	find $(distdir)/win32 -name .svn | xargs rm -rf
	find $(distdir)/dist -name .svn | xargs rm -rf
	rm -rf $(distdir)/perl/Makefile.old
