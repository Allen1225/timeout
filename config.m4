dnl $Id$
dnl config.m4 for extension timeout

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(timeout, for timeout support,
Make sure that the comment is aligned:
[  --with-timeout             Include timeout support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(timeout, whether to enable timeout support,
dnl Make sure that the comment is aligned:
dnl [  --enable-timeout           Enable timeout support])

if test "$PHP_TIMEOUT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-timeout -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/timeout.h"  # you most likely want to change this
  dnl if test -r $PHP_TIMEOUT/$SEARCH_FOR; then # path given as parameter
  dnl   TIMEOUT_DIR=$PHP_TIMEOUT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for timeout files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TIMEOUT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TIMEOUT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the timeout distribution])
  dnl fi

  dnl # --with-timeout -> add include path
  dnl PHP_ADD_INCLUDE($TIMEOUT_DIR/include)

  dnl # --with-timeout -> check for lib and symbol presence
  dnl LIBNAME=timeout # you may want to change this
  dnl LIBSYMBOL=timeout # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TIMEOUT_DIR/$PHP_LIBDIR, TIMEOUT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TIMEOUTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong timeout lib version or lib not found])
  dnl ],[
  dnl   -L$TIMEOUT_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TIMEOUT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(timeout, timeout.c timeout_signal.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
