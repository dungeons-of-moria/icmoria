###############################################################################
# Blah blah blah  (used /usr/local/src/hfs/hfsutils-2.0/Makefile as template)
#
# A hopefuly helpful list of things that probably need to be done:
#
# You should fill in the correct paths and stuff in configure.h
# Then pick a uid/gid for the game
# Next pick how to link with mtwist library
# make mtwist
# make imoria
# run imoria, and it should create a bunch of files and quit
# make privs
#
###############################################################################
DESTDIR =	/usr/local

BINDEST =	$(DESTDIR)/bin
MANDEST =	$(DESTDIR)/man

#LIBDEST =	$(DESTDIR)/lib
LIBDEST =	/usr/lib
INCDEST =	$(DESTDIR)/include

MANEXT =	1

BININSTALL =	install -c -m 755
LIBINSTALL =	install -c -m 644
HARDLINK =	ln -f

# For BSD Systems
CURSES = -lncurses -ltermcap

CC =		gcc
#INCLUDES =	-I/usr/include/tcl
#INCLUDES =	-I.

DEFINES =	-DDO_DEBUG=0
LIBS =		$(CURSES) -lm -lgdbm

#COPTS =		-Wall -O2 -g3
#COPTS =		-Wall -O2
#COPTS =		-Wall
COPTS =		-Wall -g3

CFLAGS =	$(COPTS) $(INCLUDES) $(DEFINES)
LDFLAGS =	

#
# the owner and group for the game and data files
#
OWNER =	games
GROUP =	games

#
# privs needed on each file depends on if it is for read or write
#
READFILES =	hours.dat moria.dat monsters.dat moria_gcustom.mst
WRITEFILES =	death.log moriamas.dat moriatop.dat moriatrd.dat
DATAFILES =	$(READFILES) $(WRITEFILES)

all:	mtwist imoria

#############################################################################
#
# You have several choices on how to link the random number generator
# library into the game.  The "good" way makes a dynamic library that
# needs to be installed to /usr/lib or /lib.  This is best because it
# keeps the GNU Library GPL happy.  However if you are not able to install
# the library (root needs to do it) then you can use the easy non-library
# way.
#
# The easy way is first, rather than create a library the obj file is
# linked just like all the IMORIAOBJS, section 6 of the GNU Library GPL
# covers this.  Imoria is not supposed to be under the GPL, so do not
# distribute binaries made this way (see mtwist/COPYING).  I think that
# I satisfy the exception stuff in section 7.1, but I don't really know.
#
# The second way compiles the library and copies it to /usr/local/lib
# so it can be loaded at runtime.
#
# A third option is to just ignore the entire issue and use rand().
# You need to change the USE_MTWIST setting in configure.h
#

#              Non shared library version
#mtwist ::
#	(cd mtwist ; make obj )
#MTWIST = mtwist/mtwist.o

#              Library version
mtwist	::
	(cd mtwist ; make lib)
	(cd mtwist ; make install)
MTWIST = -lmtwist

#              Do not bother version
#mtwist ::
#	echo mtwist libary skipped
#MTWIST = 


IMORIAOBJS =	bank.o bj.o blow.o casino.o create.o creature.o death.o debug.o desc.o dungeon.o eat.o encrypt.o files.o generate.o help.o horse.o insurance.o inven.o io.o magic.o master.o misc.o monk.o monsters.o moria.o netopen.o pascal.o play.o player.o port.o potions.o prayer.o quest.o random.o river.o rooms.o save.o screen.o scrolls.o sing.o slots.o spells.o staffs.o store.o term.o termdef.o trade.o traps.o treasure.o unix.o wands.o wizard.o


imoria: $(IMORIAOBJS)
	$(CC) $(LDFLAGS) $(IMORIAOBJS) $(LIBS) $(MTWIST) -o $@
	chown $(OWNER):$(GROUP) imoria
	chmod 2711 imoria
	echo


depend ::
	( sed -n '1,/^### DEPEND/p' Makefile;  \
	  echo;  \
	  $(CC) -MM $(CFLAGS) *.c;  \
	) > Makefile.new
	mv -f Makefile.new Makefile

privs ::
	chown $(OWNER):$(GROUP) imoria $(DATAFILES)
	chmod 2711        imoria
	chmod 640         $(READFILES)
	chmod 660         $(WRITEFILES)
	chmod 755         mhelp.pl

nodata ::
	rm -f hours.dat moria.dat death.log moriamas.dat moriatop.dat moriatrd.dat moria_gcustom.mst TRADE.DUMP

clean ::
	( cd mtwist ; make clean )
	rm -f *.o *~ core imoria i2 Debug.out mm

spotless : nodata clean


#############################################################################
### DEPENDENCIES FOLLOW #######################################################

bank.o: bank.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
bj.o: bj.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h casino.h bj.h
blackmarket.o: blackmarket.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
blow.o: blow.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
casino.o: casino.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 casino.h slots.h horse.h bj.h
create.o: create.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
creature.o: creature.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
death.o: death.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h master.h
debug.o: debug.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
desc.o: desc.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
dungeon.o: dungeon.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
eat.o: eat.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
encrypt.o: encrypt.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
files.o: files.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h master.h
generate.o: generate.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 generate.h
help.o: help.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
horse.o: horse.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h casino.h \
 horse.h
insurance.o: insurance.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
inven.o: inven.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
io.o: io.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
magic.o: magic.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
master.o: master.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 master.h
misc.o: misc.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
monk.o: monk.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
monsters.o: monsters.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
moria.o: moria.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h values.h
netopen.o: netopen.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
pascal.o: pascal.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
play.o: play.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
player.o: player.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
port.o: port.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
potions.o: potions.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
prayer.o: prayer.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
quest.o: quest.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h
random.o: random.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
river.o: river.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h generate.h
rooms.o: rooms.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h generate.h
save.o: save.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h master.h
screen.o: screen.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
scrolls.o: scrolls.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
sing.o: sing.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
slots.o: slots.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h casino.h \
 slots.h
spells.o: spells.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
staffs.o: staffs.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
store.o: store.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
term.o: term.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h config.h
termdef.o: termdef.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
trade.o: trade.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h trade.h
traps.o: traps.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h \
 trade.h
treasure.o: treasure.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h
unix.o: unix.c term.h
wands.o: wands.c imoria.h mtwist/mtwist.h patchlevel.h configure.h constants.h \
 types.h pascal.h routines.h term.h debug.h variables.h dungeon.h
wizard.o: wizard.c imoria.h mtwist/mtwist.h patchlevel.h configure.h \
 constants.h types.h pascal.h routines.h term.h debug.h variables.h \
 dungeon.h
