DESCRIPTION="A file manager with menu-based navigation"
HOMEPAGE="http://darimasen.berlios.de/"
SRC_URI="http://download.berlios.de/darimasen/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="x86"

IUSE=""

DEPEND=">=dev-libs/libsigc++-2.0.3
        >=dev-cpp/gtkmm-2.4.11
        >=dev-cpp/gnome-vfsmm-2.6.0
        x11-themes/gnome-icon-theme"

RDEPEND="!x11-misc/darimasen-svn"

src_compile() {
	export WANT_AUTOCONF=2.5

	cd ${S}
        ./autogen.sh
	econf ${myconf} || die
	emake || die
}




src_install() {
	einfo "Installing..."
	#dodir /usr/share/fluxbox
	make DESTDIR=${D} install || die "make install failed"
	dodoc README* AUTHORS TODO* COPYING
}

