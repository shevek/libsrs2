# Copyright 1999-2004 Gentoo Technologies, Inc.
# Distributed under the terms of the GNU General Public License v2
# $Header: $

RESTRICT="nomirror"
SRC_URI="http://www.libsrs2.org/srs/libsrs2-${PV}.tar.gz"
DESCRIPTION="Library to perform SMTP SRS rewriting"
HOMEPAGE="http://www.libsrs2.org/"
LICENSE="GPL-2"

SLOT="0"
KEYWORDS="~x86 ~sparc"
IUSE=""

S=${WORKDIR}/${P}

src_compile() {
	econf || die
	emake || die
}

src_install() {
	make DESTDIR=${D} install || die
}
