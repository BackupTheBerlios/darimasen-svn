
ESVN_REPO_URI="svn://svn.berlios.de/darimasen/trunk"
ESVN_PROJECT="darimasen"
ESVN_BOOTSTRAP="autogen.sh"
inherit subversion

DESCRIPTION="A file manager with menu-based navigation"
HOMEPAGE="http://darimasen.berlios.de/"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86"

IUSE=""

DEPEND=">=dev-libs/libsigc++-2.0.3
        >=dev-cpp/gtkmm-2.4.11
        >=dev-cpp/gnome-vfsmm-2.6.0
        x11-themes/gnome-icon-theme"
RDEPEND="!x11-misc/darimasen"


subversion_svn_fetch() {

	# ESVN_REPO_URI is empty.
	[ -z "${ESVN_REPO_URI}" ] && die "subversion.eclass: ESVN_REPO_URI is empty."

	# http and https only...
	case ${ESVN_REPO_URI%%:*} in
		http)	;;
		https)	;;
		svn)	;;
		*)
			die "subversion.eclass: fetch from "${ESVN_REPO_URI%:*}" is not yet implemented."
			;;
	esac

	# every time
	addwrite "${ESVN_STORE_DIR}"
	addwrite "/etc/subversion"

	# -userpriv
	addwrite "/root/.subversion"

	if [ ! -d "${ESVN_STORE_DIR}" ]; then
		mkdir -p "${ESVN_STORE_DIR}" || die "subversion.eclass: can't mkdir ${ESVN_STORE_DIR}."
		chmod -f o+rw "${ESVN_STORE_DIR}" || die "subversion.eclass: can't chmod ${ESVN_STORE_DIR}."
		einfo "created store directory: ${ESVN_STORE_DIR}"
		einfo
	fi

	cd "${ESVN_STORE_DIR}"

	if [ -z ${ESVN_REPO_URI##*/} ]; then
		ESVN_REPO_FIX="${ESVN_REPO_URI%/}"
	else
		ESVN_REPO_FIX="${ESVN_REPO_URI}"
	fi

	ESVN_CO_DIR="${ESVN_PROJECT}/${ESVN_REPO_FIX##*/}"

	if [ ! -d "${ESVN_CO_DIR}/.svn" ]; then
		# first check out
		einfo "subversion check out start -->"
		einfo
		einfo "check out from: ${ESVN_REPO_URI}"

		mkdir -p "${ESVN_PROJECT}" || die "subversion.eclass: can't mkdir ${ESVN_PROJECT}."
		chmod -f o+rw "${ESVN_PROJECT}" || die "subversion.eclass: can't chmod ${ESVN_PROJECT}."
		cd "${ESVN_PROJECT}"
		${ESVN_FETCH_CMD} "${ESVN_REPO_URI}" || die "subversion.eclass: can't fetch from ${ESVN_REPO_URI}."
		einfo "     stored in: ${ESVN_STORE_DIR}/${ESVN_CO_DIR}"

	else
		# update working copy
		einfo "subversion update start -->"
		einfo
		einfo "   update from: ${ESVN_REPO_URI}"

		cd "${ESVN_CO_DIR}"
		${ESVN_UPDATE_CMD} || die "subversion.eclass: can't update from ${ESVN_REPO_URI}."
		einfo "    updated in: ${ESVN_STORE_DIR}/${ESVN_CO_DIR}"
	fi

	# permission fix
	chmod -Rf o+rw . 2>/dev/null

	# copy to the ${WORKDIR}
	cp -Rf "${ESVN_STORE_DIR}/${ESVN_CO_DIR}" "${WORKDIR}/${P}" || die "subversion.eclass: can't copy to ${WORKDIR}/${P}."
	einfo

}

src_compile() {
	export WANT_AUTOCONF=2.5

	./configure ${myconf} || die
	emake || die
}




src_install() {
	einfo "Installing..."
	make DESTDIR=${D} install || die "make install failed"
	dodoc README* AUTHORS TODO* COPYING
}

