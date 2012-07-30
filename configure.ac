AC_INIT([collectdfs],[1.0.1])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([.])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_SRCDIR([src/main.cc])
AC_PROG_CC
AC_PROG_CXX

AC_CHECK_LIB(fuse, fuse_main, [],
   AC_MSG_ERROR("Can't find FUSE library"))

CPPFLAGS="-Wall -g -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26"

AC_CHECK_HEADERS(fuse.h, [], 
    AC_MSG_ERROR("Can't locate FUSE headers"))

AC_ARG_WITH(libcollectdclient, [AS_HELP_STRING([--with-libcollectdclient@<:@=PREFIX@:>@], [Path to libcollectdclient.])],
[
	if test "x$withval" != "xno" && test "x$withval" != "xyes"
	then
		CFLAGS="$CFLAGS -I$withval/include"
		CPPFLAGS="$CPPFLAGS -I$withval/include"
		LDFLAGS="$LDFLAGS -L$withval/lib"
	fi
],[])

AC_CHECK_HEADERS(collectd/client.h, [], 
    AC_MSG_ERROR("Can't locate collectd client headers"))

AC_CHECK_LIB(collectdclient, lcc_getval, [], 
    AC_MSG_ERROR("Can't locate collectd client library"))

AC_OUTPUT
