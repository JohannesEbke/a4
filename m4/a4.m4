# A4_INIT()
# -----------------------------------------------
# Set up --with-a4
AC_DEFUN([A4_INIT], [
AC_ARG_WITH([a4],
   [AS_HELP_STRING([--with-a4=DIR],
                   [prefix of A4 @<:@guess@:>@])])dnl
AC_ARG_VAR([A4_ROOT],[Location of A4 installation or source directory])dnl

# If A4_ROOT is set and the user has not provided a value to
# --with-a4, then treat A4_ROOT as if it the user supplied it.
if test x"$A4_ROOT" != x; then
  if test x"$with_a4" = x; then
    AC_MSG_NOTICE([Detected A4_ROOT; continuing with --with-a4=$A4_ROOT])
    with_a4=$A4_ROOT
  else
    AC_MSG_NOTICE([Detected A4_ROOT=$A4_ROOT, but overridden by --with-a4=${with_a4}])
  fi
fi

# Set default a4 root directory if not specified
if test x"$with_a4" = x; then
  if test x$PACKAGE_NAME != xa4; then
    with_a4=$(cd $srcdir && cd .. && pwd)
  else
    with_a4=$(cd $srcdir && pwd)
  fi
fi

AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
         ["$DISTCHECK_CONFIGURE_FLAGS '--with-a4=${with_a4}'"])dnl

A4_CPPFLAGS=""
A4_LIBS=""
with_a4_is_setup=yes
])# A4_REQUIRE

# A4_REQUIRE([PACKAGE], [HEADERFILE])
# -----------------------------------------------
# Look for the given A4 package, containing HEADERFILE
# Appends to A4_CPPFLAGS and A4_LIBS 
# On failure, calls the optional
# ACTION-IF-NOT-FOUND action if one was supplied.
# Otherwise aborts with an error message.
AC_DEFUN([A4_REQUIRE], [
    if test x"$with_a4_is_setup" == x; then
        AC_MSG_ERROR([Put A4\_INIT before any A4\_REQUIRE macros!]);
    fi
    AC_LANG_ASSERT([C++])
    if test x"$with_a4" != x; then
        if test -d ${with_a4}/$1; then
            # Source directory
            # Try to use either this directory or that build directory
            A4_package_CPPFLAGS=" -I${with_a4}/$1/src -I../$1/src "
            A4_package_LIBS=" ../$1/.libs/lib$1.la "
        else
            # Installation directory
            A4_package_CPPFLAGS+=" -I${with_a4}/include "
            A4_package_LIBS+=" -L${with_a4}/lib -l$1 "
        fi
    fi
    a4_cppflags_save=$CPPFLAGS
    CPPFLAGS+=$BOOST_CPPFLAGS
    CPPFLAGS+=$PROTOBUF_CFLAGS
    CPPFLAGS+=$A4_CPPFLAGS
    CPPFLAGS+=$A4_package_CPPFLAGS
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <a4/$2>]])],
        [:],
        [AC_MSG_FAILURE([Could not compile program with $1 headers (a4/$2)!])])
    # We cannot check the linker since the other pack might not be compiled yet
    CPPFLAGS=$a4_cppflags_save
    A4_CPPFLAGS+=$A4_package_CPPFLAGS
    A4_LIBS+=$A4_package_LIBS
    AC_SUBST(AS_TR_CPP([A4_$1_CPPFLAGS]), [$A4_package_CPPFLAGS])dnl
    AC_SUBST(AS_TR_CPP([A4_$1_LIBS]), [$A4_package_LIBS])dnl
    AC_SUBST([A4_CPPFLAGS], [$A4_CPPFLAGS])dnl
])# A4_REQUIRE_PACKAGE
