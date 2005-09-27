
bin_PROGRAMS = darimasen

darimasen_SOURCES = \
	darimasen.cpp \
	iconmodes.cpp \
	supplimental.cpp \
        firsttime.cpp \
	main.cpp \
	damenu.cpp \
        optionsManager.cpp
noinst_HEADERS = \
	darimasen.h \
	iconmodes.h \
	supplimental.h \
        firsttime.h \
	main.h \
	damenu.h \
        optionsManager.h
AM_CXXFLAGS = -I../.. @GTKMM_CFLAGS@

darimasen_LDADD = -lpthread @LIBS@ @GTKMM_LIBS@ 
