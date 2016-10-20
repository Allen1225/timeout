/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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

#ifndef PHP_TIMEOUT_H
#define PHP_TIMEOUT_H

extern zend_module_entry timeout_module_entry;
#define phpext_timeout_ptr &timeout_module_entry

#define PHP_TIMEOUT_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_TIMEOUT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TIMEOUT_API __attribute__ ((visibility("default")))
#else
#	define PHP_TIMEOUT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define TT_CALL_FUNCTION_SUCC           0
#define TT_SET_FUNCTION_CALL_INFO_ERROR 100
#define TT_FUNCTION_CALL_FAILURE        101
#define TT_SET_SIGNAL_ERROR             102
#define TT_SET_TIMER_ERROR              103
#define TT_CALL_FUNCTION_TIMEOUT        104


#define TIMEOUT_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(timeout, v)

#if defined(ZTS) && defined(COMPILE_DL_TIMEOUT)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_TIMEOUT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
