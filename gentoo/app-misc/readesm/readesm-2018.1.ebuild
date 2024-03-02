# Copyright 1999-2018 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="6"

inherit cmake-utils

DESCRIPTION="digital tachograph file reader"
HOMEPAGE="http://readesm.sourceforge.net/"
SRC_URI="mirror://sourceforge/${PN}/${P}-src.tar.xz"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE="nls"

DEPEND=">=dev-libs/boost-1.63.0
	>=dev-libs/gmp-6.1.2
	>=dev-libs/libgcrypt-1.8.1
	>=dev-qt/qtcore-5.9.6
	>=dev-qt/qtgui-5.9.6
	>=dev-qt/qtprintsupport-5.9.6
	>=dev-qt/qtwebengine-5.9.6"

RDEPEND="${DEPEND}"

src_prepare () {
	find fileformat \( ! -regex '.*/\..*' ! -name "generate.py" \) | \
		xargs grep -snH "AUTO-GENERATED" --color | cut -f1 -d':' | \
		while read filename; do rm "$filename"; done
	cd fileformat
	./generate.py || die "could not prepare source: generate.py failed"
	cd -

	cmake-utils_src_prepare
}

src_configure() {
	local mycmakeargs=(
		-DWITH_L10N="$(usex nls)"
	)
	cmake-utils_src_configure
}

pkg_postinst() {
	xdg_desktop_database_update
	xdg_mimeinfo_database_update
}

pkg_postrm() {
	xdg_desktop_database_update
	xdg_mimeinfo_database_update
}
