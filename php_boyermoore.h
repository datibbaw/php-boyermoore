/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_BOYERMOORE_H
#define PHP_BOYERMOORE_H

extern zend_module_entry boyermoore_module_entry;
#define phpext_boyermoore_ptr &boyermoore_module_entry

#ifdef PHP_WIN32
#	define PHP_BOYERMOORE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_BOYERMOORE_API __attribute__ ((visibility("default")))
#else
#	define PHP_BOYERMOORE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define ALPHABET_LEN 255
#define NOT_FOUND patlen
#define max(a, b) ((a < b) ? b : a)

typedef struct _php_boyermoore_needle {
    char *needle;
    int needle_len;
    int delta1[ALPHABET_LEN];
    int *delta2;
} php_boyermoore_needle;

#define PHP_BOYERMOORE_NEEDLE_RES_NAME "Boyer-Moore Studied Needle"

PHP_MINIT_FUNCTION(boyermoore);
PHP_MSHUTDOWN_FUNCTION(boyermoore);
PHP_RINIT_FUNCTION(boyermoore);
PHP_RSHUTDOWN_FUNCTION(boyermoore);
PHP_MINFO_FUNCTION(boyermoore);

PHP_FUNCTION(boyermoore_indexof);
PHP_FUNCTION(boyermoore_study);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(boyermoore)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(boyermoore)
*/

/* In every utility function you add that needs to use variables 
   in php_boyermoore_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as BOYERMOORE_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define BOYERMOORE_G(v) TSRMG(boyermoore_globals_id, zend_boyermoore_globals *, v)
#else
#define BOYERMOORE_G(v) (boyermoore_globals.v)
#endif

#endif	/* PHP_BOYERMOORE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
