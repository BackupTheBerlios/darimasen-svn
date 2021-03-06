#!/bin/sh

cd ..
mkdir darimasen-0.0.8a
cp -R 0.0.x/* darimasen-0.0.8a/
cd darimasen-0.0.8a/

rm -Rf .svn/
rm -Rf src/.svn/
rm -Rf pixmaps/.svn/
rm -Rf pixmaps/48x48/.svn/
rm -Rf pixmaps/scalable/.svn/

export WANT_AUTOCONF=2.5
export WANT_AUTOMAKE=1.7

./autogen.sh --prefix=/usr/local


rm autom4te.cache/ -Rf
rm aclocal.m4
rm autogen.sh
rm config.log
rm darimasen-svn-0.1-r2.ebuild
rm distribute.sh

cd ..

tar -cjvf darimasen-0.0.8a.tar.bz2 darimasen-0.0.8a/
