#!/bin/sh
# generated 2004/9/10 11:30:35 CEST by nick@frisbee.mnc.ch.(none)
# using glademm V2.6.0_cvs

if test ! -f install-sh ; then touch install-sh ; fi

MAKE=`which gnumake`
if test ! -x "$MAKE" ; then MAKE=`which make` ; fi
if test ! -x "$MAKE" ; then MAKE=`which gmake` ; fi
HAVE_GNU_MAKE=`$MAKE --version|grep -c "Free Software Foundation"`

if test "$HAVE_GNU_MAKE" != "1"; then 
echo Only non-GNU make found: $MAKE
else
echo `$MAKE --version | head -1` found
fi

if test ! -x `which aclocal`
then echo you need autoconfig and automake to generate the Makefile
fi
if test ! -x `which automake`
then echo you need automake to generate the Makefile
fi

echo This script runs configure and make...

# autoreconf$AC_POSTFIX -fim _might_ do the trick, too.
#  chose to your taste
aclocal$AM_POSTFIX
libtoolize --force --copy
autoheader$AC_POSTFIX
automake$AM_POSTFIX --add-missing --copy --gnu
autoconf$AC_POSTFIX
