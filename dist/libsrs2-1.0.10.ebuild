SRC_URI="http://www.libsrs.org/srs/${P}.tgz"
DESCRIPTION="SRS email address rewriting engine"
HOMEPAGE="http://www.libsrs2.org/"
LICENSE="GPL-2 BSD"

SLOT="0"
KEYWORDS="x86"

S=${WORKDIR}/${P}

src_compile() {
	econf || die
	emake || die
}

src_install() {
	make DESTDIR=${D} install || die
}
