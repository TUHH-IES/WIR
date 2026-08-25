SUBDIRS			= @LIBUSEFUL@ tests arch . doc


#
# Target to build
#

.SILENT:

wirlibdir		= $(libdir)

wirlib_LTLIBRARIES	= libwir.la

pkgdata_DATA		= AUTHORS LICENSE README.md Releases

EXTRA_DIST		= AUTHORS LICENSE Makefile.bootstrap README.md Releases


#
# Source files of the target
#

libwir_la_SOURCES	=

include analyses/Makefile.src
include arch/Makefile.src
include containers/Makefile.src
include flowfacts/Makefile.src
include optimizations/Makefile.src
include wir/Makefile.src


#
# Compiler and linker flags
#

libwir_la_LDFLAGS	= -avoid-version
libwir_la_LIBADD	= @LIBUSEFUL_PREFIX@/libuseful/libuseful.la
