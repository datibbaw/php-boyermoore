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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_boyermoore.h"

/* If you declare any globals in php_boyermoore.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(boyermoore)
*/

/* True global resources - no need for thread safety here */
static int le_boyermoore;
static int le_boyermoore_needle;

/* {{{ boyermoore_functions[]
 *
 * Every user visible function must have an entry in boyermoore_functions[].
 */
const zend_function_entry boyermoore_functions[] = {
	PHP_FE(boyermoore_indexof,	NULL)
	PHP_FE(boyermoore_study,        NULL)
	PHP_FE_END	/* Must be the last line in boyermoore_functions[] */
};
/* }}} */

/* {{{ boyermoore_module_entry
 */
zend_module_entry boyermoore_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"boyermoore",
	boyermoore_functions,
	PHP_MINIT(boyermoore),
	PHP_MSHUTDOWN(boyermoore),
	PHP_RINIT(boyermoore),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(boyermoore),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(boyermoore),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BOYERMOORE
ZEND_GET_MODULE(boyermoore)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("boyermoore.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_boyermoore_globals, boyermoore_globals)
    STD_PHP_INI_ENTRY("boyermoore.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_boyermoore_globals, boyermoore_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_boyermoore_init_globals
 */
/* }}} */

static void php_boyermoore_needle_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    php_boyermoore_needle *needle = (php_boyermoore_needle *)rsrc->ptr;
    if (needle) {
        if (needle->needle) {
            efree(needle->needle);
        }
        if (needle->delta2) {
            efree(needle->delta2);
        }
        efree(needle);
    }
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(boyermoore)
{
	le_boyermoore_needle = zend_register_list_destructors_ex(php_boyermoore_needle_dtor, NULL, PHP_BOYERMOORE_NEEDLE_RES_NAME, module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(boyermoore)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(boyermoore)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(boyermoore)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(boyermoore)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "boyermoore support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ make_delta1(delta1, pat, patlen)
 */
static void make_delta1(int *delta1, uint8_t *pat, int32_t patlen)
{
	register int patmax = patlen-1;
	int i;

	for (i=0; i != ALPHABET_LEN; i++) {
		delta1[i] = NOT_FOUND;
	}

	for (i=0; i < patmax; i++) {
		delta1[pat[i]] = patmax - i;
	}
}
/* }}} */

/* {{{ is_prefix(word, wordlen, pos)
 */
static int is_prefix(uint8_t *word, int wordlen, int pos)
{
	return !memcmp(word, word + pos, wordlen - pos);
}
/* }}} */

/* {{{ suffix_length(word, wordlen, pos)
 */
static int suffix_length(uint8_t *word, int wordlen, int pos)
{
        int i;

        // increment suffix length i to the first mismatch or beginning
        // of the word
        for (i = 0; (word[pos-i] == word[wordlen-1-i]) && (i < pos); i++);

        return i;
}
/* }}} */

/* {{{ make_delta2(delta2, pat, patlen) 
 * */
static void make_delta2(int *delta2, uint8_t *pat, int32_t patlen)
{
	register int patmax = patlen-1;
	int p;
	int last_prefix_index = patmax;

	// first loop
	for (p=patmax; p>=0; p--) {
		if (is_prefix(pat, patlen, p+1)) {
			last_prefix_index = p+1;
		}
		delta2[p] = last_prefix_index + (patmax - p);
	}

	// second loop
	for (p=0; p < patmax; p++) {
		int slen = suffix_length(pat, patlen, p);
		if (pat[p - slen] != pat[patmax - slen]) {
			delta2[patmax - slen] = patmax - p + slen;
		}
	}
}
/* }}} */

/* {{{ boyer_moore_studied(string, stringlen, pat, patlen, delta1, delta2)
 */
static uint8_t* boyer_moore_studied(uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen, int *delta1, int *delta2)
{
    register int patmax;
    int i;

    patmax = patlen - 1;

    i = patmax;
    while (i < stringlen) {
        int j = patmax;
        while (j >= 0 && (string[i] == pat[j])) {
            --i;
            --j;
        }
        if (j < 0) {
            return (string + i+1);
        }

        i += max(delta1[string[i]], delta2[j]);
    }

    return NULL;
}
/* }}} */

/* {{{ boyer_moore(string, stringlen, pat, patlen)
 */
static uint8_t* boyer_moore(uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen)
{
    int delta1[ALPHABET_LEN];
    int *delta2 = emalloc(patlen * sizeof(int));

    make_delta1(delta1, pat, patlen);
    make_delta2(delta2, pat, patlen);

    uint8_t *found = boyer_moore_studied(string, stringlen, pat, patlen, delta1, delta2);

    efree(delta2);
    return found;
}
/* }}} */

/* {{{ proto int boyermoore_indexof(string haystack, string|resource needle [, int offset])
   Return position of needle in haystack or false */
PHP_FUNCTION(boyermoore_indexof)
{
        php_boyermoore_needle *needle;
        zval *pattern;
        char *haystack;
        int haystack_len;
        long offset = 0;

        char *found;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|l", &haystack, &haystack_len, &pattern, &offset) == FAILURE) {
                return;
        }

        if (offset < 0 || offset > haystack_len) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Offset not contained in string");
            RETURN_FALSE;
        }

        if (Z_TYPE_P(pattern) == IS_RESOURCE) {
            ZEND_FETCH_RESOURCE(needle, php_boyermoore_needle*, &pattern, -1, PHP_BOYERMOORE_NEEDLE_RES_NAME, le_boyermoore_needle);
            if (!needle) {
                RETURN_FALSE;
            }
            found = (char *)boyer_moore_studied((uint8_t *)haystack + offset, haystack_len - offset, (uint8_t *)needle->needle, needle->needle_len, needle->delta1, needle->delta2);
        } else if (Z_TYPE_P(pattern) == IS_STRING) {
            if (!Z_STRLEN_P(pattern)) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty needle");
                RETURN_FALSE;
            }
            found = (char *)boyer_moore((uint8_t *)haystack + offset, haystack_len - offset, (uint8_t *)Z_STRVAL_P(pattern), Z_STRLEN_P(pattern));
        } else {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Needle must be a string or studied pattern");
            RETURN_FALSE;
        }

        if (found) {
                RETURN_LONG(found - haystack);
        } else {
                RETURN_FALSE;
        }
}
/* }}} */

/* {{{ proto resource boyermoore_study(needle)
 */
PHP_FUNCTION(boyermoore_study)
{
	char *pattern;
	int pattern_len;
	php_boyermoore_needle *needle;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &pattern, &pattern_len) == FAILURE) {
		return;
	}

	needle = emalloc(sizeof(php_boyermoore_needle));
	needle->needle = estrndup(pattern, pattern_len);
	needle->needle_len = pattern_len;
	needle->delta2 = emalloc(pattern_len * sizeof(int));

	make_delta1(needle->delta1, (uint8_t *)pattern, pattern_len);
	make_delta2(needle->delta2, (uint8_t *)pattern, pattern_len);

	ZEND_REGISTER_RESOURCE(return_value, needle, le_boyermoore_needle);
}
/* }}} */


/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
