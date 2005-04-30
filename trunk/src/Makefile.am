
bin_PROGRAMS = darimasen

darimasen_SOURCES = \
	darimasen.cpp \
	iconmodes.cpp \
	supplimental.cpp \
        firsttime.cpp \
	main.cpp \
	damenu.cpp 

noinst_HEADERS = \
	darimasen.h \
	iconmodes.h \
	supplimental.h \
        firsttime.h \
	main.h \
	damenu.h 

AM_CXXFLAGS = -I../.. @PKG_CFLAGS@

darimasen_LDADD = -lpthread @LIBS@ @PKG_LIBS@ 
