#!/usr/bin/make -f
#
# File: Makefile.qmail
# Desc: Temporary static makefile for Qmail
#
# Contributed by Orngetide from #c on EFNet 
#
################################################################################
CFLAGS:=-Wall -ggdb3 -DDEBUG -D_USE_BSD_SOURCE #-D_USE_GNU_SOURCE
CPPFLAGS:=
LDFLAGS:=
################################################################################
TARGETS:= \
  srs.o \
  shared \
  test

SRCS:= \
  srs.c \
  test.c

OBJS:=$(SRCS:%.c=%.o)
################################################################################
all : $(TARGETS)

clean :
	-$(RM) $(OBJS) $(TARGETS)
	rm srs

.PHONY : all clean

srs.o : srs.c $(INCS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

shared : LDFLAGS+= ../libsrs.so.1.0.1
shared : srs.o 
	$(CC) $(CFLAGS) $(TARGET_ARCH) -o $@ $^ $(LDFLAGS)
	mv shared srs

static : srs.o -lcrypto ../libsrs.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^
	mv static srs

static-obsd : srs.o ../libsrs.a /usr/lib/libcrypto_p.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^
	mv static-obsd srs


test : test.c
	$(CC) $^  -o $@

install :
	cp ./srs /usr/local/bin/srs
