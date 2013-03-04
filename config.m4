dnl $Id$
dnl config.m4 for extension boyermoore

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(boyermoore, for boyermoore support,
dnl Make sure that the comment is aligned:
dnl [  --with-boyermoore             Include boyermoore support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(boyermoore, whether to enable boyermoore support,
[  --enable-boyermoore           Enable boyermoore support])

if test "$PHP_BOYERMOORE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-boyermoore -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/boyermoore.h"  # you most likely want to change this
  dnl if test -r $PHP_BOYERMOORE/$SEARCH_FOR; then # path given as parameter
  dnl   BOYERMOORE_DIR=$PHP_BOYERMOORE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for boyermoore files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BOYERMOORE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BOYERMOORE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the boyermoore distribution])
  dnl fi

  dnl # --with-boyermoore -> add include path
  dnl PHP_ADD_INCLUDE($BOYERMOORE_DIR/include)

  dnl # --with-boyermoore -> check for lib and symbol presence
  dnl LIBNAME=boyermoore # you may want to change this
  dnl LIBSYMBOL=boyermoore # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BOYERMOORE_DIR/lib, BOYERMOORE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BOYERMOORELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong boyermoore lib version or lib not found])
  dnl ],[
  dnl   -L$BOYERMOORE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BOYERMOORE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(boyermoore, boyermoore.c, $ext_shared)
fi
