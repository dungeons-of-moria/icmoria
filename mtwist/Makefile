#
# Makefile for ``Mersenne Twister''
# This was written for linux, it works on my system and may not do you any good
#
# redhat 5.0 likes this to be /usr/lib
# my slackware wants it in    /usr/local/lib
#
LIBDEST =	/usr/local/lib
#LIBDEST =	/usr/lib
CC =		gcc

#
# version numbers, this becomes part of the library name:
#     libmtwist.so.<major>.<minor>
#
MAJOR = 1
MINOR = 0

#COPTS =		-Wall -O3 -g3
COPTS =		-Wall -O3
#COPTS =		-Wall
#COPTS =		-Wall -g3

CFLAGS =	$(COPTS) $(INCLUDES) $(DEFINES)
LDFLAGS =	

all:	obj lib

#
#              Non shared library version
#
obj	::
	$(CC) $(CFLAGS) -c mt-optimized-cokus.c -o mtwist.o

#
#              Library version
#
lib	::
	$(CC) $(CFLAGS) -fPIC -c mt-optimized-cokus.c -o mtwist-lib.o
	$(CC) -shared -Wl,-soname,libmtwist.so.$(MAJOR) -o libmtwist.so.$(MAJOR).$(MINOR) mtwist-lib.o

install	::
	cp libmtwist.so.$(MAJOR).$(MINOR) $(LIBDEST)/libmtwist.so.$(MAJOR).$(MINOR)
	rm -f $(LIBDEST)/libmtwist.so.$(MAJOR) $(LIBDEST)/libmtwist.so
	/sbin/ldconfig
	( cd $(LIBDEST) ; ln -s libmtwist.so.$(MAJOR) libmtwist.so )

clean	::
	rm -f *.o *~ libmtwist.so*

#### end of makefile ####
