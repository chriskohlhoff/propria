AC_INIT(propria, [0.0.1])
AC_CONFIG_SRCDIR(include/propria.hpp)
AM_MAINTAINER_MODE
AM_INIT_AUTOMAKE([tar-ustar])

AC_CANONICAL_HOST
AM_PROG_CC_C_O
AC_PROG_CXX
AC_LANG(C++)
AC_PROG_RANLIB

AC_ARG_WITH(boost,
  AC_HELP_STRING([--with-boost=DIR],[location of boost distribution]),
[
  CPPFLAGS="$CPPFLAGS -I${withval}"
])

AC_MSG_CHECKING([whether C++11 is enabled])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [[#if __cplusplus < 201103L]]
    [[#error C++11 not available]]
    [[#endif]])],
  [AC_MSG_RESULT([yes])
    HAVE_CXX11=yes;],
  [AC_MSG_RESULT([no])
    HAVE_CXX11=no;])

AC_MSG_CHECKING([whether C++14 is enabled])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [[#if __cplusplus < 201402L]]
    [[#error C++14 not available]]
    [[#endif]])],
  [AC_MSG_RESULT([yes])
    HAVE_CXX14=yes;],
  [AC_MSG_RESULT([no])
    HAVE_CXX14=no;])

AC_MSG_CHECKING([whether C++17 is enabled])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [[#if __cplusplus < 201703L]]
    [[#error C++17 not available]]
    [[#endif]])],
  [AC_MSG_RESULT([yes])
    HAVE_CXX17=yes;],
  [AC_MSG_RESULT([no])
    HAVE_CXX17=no;])

AM_CONDITIONAL(HAVE_CXX11,test x$HAVE_CXX11 = xyes)

AM_CONDITIONAL(HAVE_CXX14,test x$HAVE_CXX14 = xyes)

AM_CONDITIONAL(HAVE_CXX17,test x$HAVE_CXX17 = xyes)

AC_OUTPUT([
  Makefile
  include/Makefile
  tests/Makefile
  examples/cpp20/Makefile])
