# Copyright 1999-2004 Gentoo Technologies, Inc.
# Distributed under the terms of the GNU General Public License v2
# $Header: $


DESCRIPTION="A file manager with menu-based navigation"
HOMEPAGE="http://darimasen.berlios.de/"
SRC_URI="http://download.berlios.de/darimasen/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="x86"

IUSE=""
SLOT="0"

DEPEND=">=dev-libs/libsigc++-2.0.3
        >=dev-cpp/gtkmm-2.4.11
        >=dev-cpp/gnome-vfsmm-2.6.0
        x11-themes/gnome-icon-theme"

RDEPEND="!x11-misc/darimasen-svn"

src_compile() {

	./configure --prefix=/usr ${myconf} || die
	emake || die
}




src_install() {
	einfo "Installing..."
	make DESTDIR=${D} install || die "make install failed"
	dodoc README* AUTHORS TODO* COPYING
}

