#!/usr/bin/make -f
#
# File: Makefile.qmail
# Desc: Temporary static makefile for Qmail
#
# Contributed by Orngetide from #c on EFNet 
#
################################################################################
CFLAGS:=-Wall -ggdb3 -DDEBUG -D_USE_BSD_SOURCE #-D_GNU_SOURCE
CPPFLAGS:=
LDFLAGS:=-lcrypto #-I/usr/lib64
################################################################################
TARGETS:= \
  libsrs.so.1.0.1 \
  libsrs.a \
  srs

SRCS:= \
  main.c \
  util.c \
  crypto.c \
  base.c \
  error.c

SHARED_OBJS:=$(SRCS:%.c=shared/%.o)
STATIC_OBJS:=$(SRCS:%.c=static/%.o)
################################################################################
all : $(TARGETS)

clean :
	-$(RM) $(STATIC_OBJS) $(SHARED_OBJS) $(TARGETS)

.PHONY : all clean

shared/%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -fPIC -c -o $@ $^

static/%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $^

libsrs.a : -lcrypto $(STATIC_OBJS)
	$(AR) r $@ $^

libsrs.so.1.0.1 : LDFLAGS+=-shared -Wl,-soname,libsrs.so.1
libsrs.so.1.0.1 : $(SHARED_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^

install :
	cp ./libsrs.so.1.0.1 /usr/local/lib/
	cp ./libsrs.a /usr/local/lib/
	ldconfig
	ldconfig -p | /bin/grep srs

srs : 
	cd tools; make static; make install
