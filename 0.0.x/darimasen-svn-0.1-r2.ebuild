# Copyright 1999-2005 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit subversion

DESCRIPTION="A file manager with menu-based navigation"
HOMEPAGE="http://darimasen.berlios.de/"
LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86 ~amd64 ~ppc"
IUSE=""
SLOT="0"

DEPEND=">=dev-libs/libsigc++-2.0.3
        >=dev-cpp/gtkmm-2.6.1
        >=dev-cpp/gnome-vfsmm-2.6.0
        x11-themes/gnome-icon-theme"
RDEPEND="!x11-misc/darimasen"


ESVN_REPO_URI="svn://svn.berlios.de/darimasen/0.0.x"
ESVN_PROJECT="darimasen"
ESVN_BOOTSTRAP="autogen.sh"


src_compile() {
	export WANT_AUTOCONF=2.5
	econf || die
	emake || die
}

src_install() {
	einfo "Installing..."
	make DESTDIR=${D} install || die "make install failed"
	dodoc README* AUTHORS TODO* COPYING
}

