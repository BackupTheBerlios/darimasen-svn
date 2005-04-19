
bin_PROGRAMS = darimasen

darimasen_SOURCES = \
	darimasen.cpp \
	iconmodes.cpp \
	supplimental.cpp \
        firsttime.cpp \
	main.cpp \
	damenu.cpp \
        prefix.c

noinst_HEADERS = \
	darimasen.h \
	iconmodes.h \
	supplimental.h \
        firsttime.h \
	main.h \
	damenu.h \
        prefix.h


AM_CXXFLAGS = -I../.. @PKG_CFLAGS@ @BINRELOC_CFLAGS@

darimasen_LDADD = -lpthread @LIBS@ @PKG_LIBS@ 
