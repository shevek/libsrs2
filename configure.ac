dnl Process this file with autoconf to produce a configure script.
AC_INIT(libsrs2/srs2.h)
AM_INIT_AUTOMAKE(libsrs2, 1.0.13)

dnl Weird Unix variants
AC_AIX
AC_MINIX

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL

dnl More weird Unix variants
AC_ISC_POSIX

AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_C_VOLATILE
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_CHAR_UNSIGNED
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(unsigned long)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(strings.h time.h sys/time.h sys/types.h unistd.h)

dnl AC_CHECK_HEADER(openssl/ssl.h, , [
dnl 	echo "OpenSSL is required to build this library."
dnl 	exit 1;
dnl ])

dnl Checks for libraries.
dnl Replace `main' with a function in -lcrypto:
dnl AC_CHECK_LIB(crypto, EVP_sha1, , [
dnl 	echo "OpenSSL is required to build this library."
dnl 	exit 1;
dnl ])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Checks for library functions.
AC_CHECK_FUNCS(strdup strstr)
dnl Unix versions
AC_CHECK_FUNCS(alloca strcasecmp strncasecmp)
dnl Windows versions
AC_CHECK_FUNCS(_alloca _stricmp _strnicmp)

AM_CONFIG_HEADER(config.h)
AC_OUTPUT(libsrs2/Makefile tools/Makefile Makefile)
