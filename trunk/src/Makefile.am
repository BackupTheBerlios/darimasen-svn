
bin_PROGRAMS = darimasen

darimasen_SOURCES = \
  darimasen.cpp \
  directory.cpp \
  main.cpp \
  preferences.cpp \
  tabs.cpp \
  icon.cpp \
  menu.cpp \
  supplimental.cpp

noinst_HEADERS = \
  darimasen.h \
  directory.h \
  main.h \
  preferences.h \
  tabs.h \
  icon.h \
  menu.h \
  supplimental.h


AM_CXXFLAGS = -I../.. @PKG_CFLAGS@

darimasen_LDADD = -lpthread @LIBS@ @PKG_LIBS@ 
