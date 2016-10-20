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
  | Author: pangudashu                                                   |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_timeout.h"
#include "timeout_signal.h"

static int le_timeout;

ZEND_BEGIN_ARG_INFO_EX(arginfo_call_func_with_timeout, 0, 0, 3)
	ZEND_ARG_INFO(0, callable)
	ZEND_ARG_INFO(0, args)
	ZEND_ARG_INFO(0, intval)
	ZEND_ARG_INFO(1, retval)
ZEND_END_ARG_INFO()


/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("timeout.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_timeout_globals, timeout_globals)
    STD_PHP_INI_ENTRY("timeout.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_timeout_globals, timeout_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(timeout)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	REGISTER_LONG_CONSTANT("TT_CALL_FUNCTION_SUCC", TT_CALL_FUNCTION_SUCC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TT_SET_FUNCTION_CALL_INFO_ERROR", TT_SET_FUNCTION_CALL_INFO_ERROR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TT_FUNCTION_CALL_FAILURE", TT_FUNCTION_CALL_FAILURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TT_SET_SIGNAL_ERROR", TT_SET_SIGNAL_ERROR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TT_SET_TIMER_ERROR", TT_SET_TIMER_ERROR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TT_CALL_FUNCTION_TIMEOUT", TT_CALL_FUNCTION_TIMEOUT, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(timeout)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(timeout)
{
#if defined(COMPILE_DL_TIMEOUT) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(timeout)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(timeout)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "timeout support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

PHP_TIMEOUT_API int timeout_set_fcall_func_by_callable(zend_fcall_info *fci, zval *callable, zval *obj) /*{{{*/
{
	zval *call_class,*call_method = NULL;
	zend_class_entry *ce;

	switch(Z_TYPE_P(callable)){
		case IS_STRING:
			ZVAL_STRING(&fci->function_name, Z_STRVAL_P(callable));
			fci->function_table = EG(function_table);
			break;
		case IS_ARRAY:
			call_class =zend_hash_index_find(Z_ARR_P(callable), 0);
			call_method = zend_hash_index_find(Z_ARR_P(callable), 1);

			if(call_method){
				ZVAL_STRING(&fci->function_name, Z_STRVAL_P(call_method));
			}

			switch(Z_TYPE_P(call_class)){
				case IS_STRING:
					ce = zend_lookup_class_ex(Z_STR_P(call_class), NULL, 1);
					if(!ce){
						return 0;
					}

					object_init_ex(obj, ce);

					fci->function_table = &ce->function_table;
					fci->object = Z_OBJ_P(obj);
					break;
				case IS_OBJECT:
					fci->function_table = &Z_OBJCE_P(call_class)->function_table;
					fci->object = Z_OBJ_P(call_class);
					break;
			}

			break;
	}

	return 1;
}
/*}}}*/

PHP_TIMEOUT_API int timeout_execute_with_params(zval *call_func, HashTable *args, zval *retval) /*{{{*/
{
	int call_ret;
	zend_fcall_info fci;
	zval _retval;
	zval obj;
	zend_object *pre_obj;
	uint32_t params_count = zend_hash_num_elements(args);
	zval params[params_count];

	fci.size = sizeof(fci);
	fci.symbol_table = NULL;
	fci.object = NULL;
	fci.no_separation = 1;

	if(retval){
		fci.retval = retval;
	}else{
		fci.retval = &_retval;
	}

	zval *entry;
	zend_ulong num_key;
	zend_string *var_name;

	ZEND_HASH_FOREACH_KEY_VAL_IND(args, num_key, var_name, entry){
		ZVAL_COPY(&params[num_key], entry);
	}ZEND_HASH_FOREACH_END();

	fci.params = params;
	fci.param_count = params_count;

	if(!timeout_set_fcall_func_by_callable(&fci, call_func, &obj)){
		call_ret = TT_SET_FUNCTION_CALL_INFO_ERROR;
		goto CLEAN;
	}
	pre_obj = fci.object;

	if(zend_call_function(&fci, NULL) == FAILURE) {
		call_ret = TT_FUNCTION_CALL_FAILURE;
		goto CLEAN;
	}

	if(pre_obj != fci.object){
		zval_ptr_dtor(&obj);
	}

	if(!retval && (&_retval)){
		zval_ptr_dtor(&_retval);
	}

	call_ret = TT_CALL_FUNCTION_SUCC;

CLEAN:
	if(fci.object){
		zval_ptr_dtor(&obj);
	}
	zval_ptr_dtor(&fci.function_name);
	int i;
	for(i = 0; i < fci.param_count; i++){
		zval_ptr_dtor(&fci.params[i]);
	}
	
	return call_ret;
}
/*}}}*/

PHP_TIMEOUT_API int timeout_execute_with_0_params(zval *call_func, zval *retval) /*{{{*/
{
	int call_ret;
	zend_fcall_info fci;
	zval obj;
	zend_object *pre_obj;
	zval _retval;

	fci.size = sizeof(fci);
	fci.symbol_table = NULL;
	fci.object = NULL;
	fci.no_separation = 1;
	fci.params = NULL;
	fci.param_count = 0;

	if(retval){
		fci.retval = retval;
	}else{
		fci.retval = &_retval;
	}

	if(!timeout_set_fcall_func_by_callable(&fci, call_func, &obj)){
		call_ret = TT_SET_FUNCTION_CALL_INFO_ERROR;
		goto CLEAN;
	}
	pre_obj = fci.object;

	if(zend_call_function(&fci, NULL) == FAILURE) {
		call_ret = TT_FUNCTION_CALL_FAILURE;
		goto CLEAN;
	}

	if(pre_obj != fci.object){
		zval_ptr_dtor(&obj);
	}

	if(!retval && &_retval){
		zval_ptr_dtor(&_retval);
	}
	call_ret = TT_CALL_FUNCTION_SUCC;

CLEAN:
	if(fci.object){
		zval_ptr_dtor(&obj);
	}
	zval_ptr_dtor(&fci.function_name);
	int i;
	for(i = 0; i < fci.param_count; i++){
		zval_ptr_dtor(&fci.params[i]);
	}

	return call_ret;
}
/*}}}*/

PHP_TIMEOUT_API int call_func_with_timeout(zval *call_func, int interval, HashTable *args, zval *retval) /*{{{*/
{
	int call_ret;

	sigfunc *sf = timeout_set_signal(SIGALRM, timeout_signal_handler);
	if(sf == SIG_ERR){
		call_ret = TT_SET_SIGNAL_ERROR;
		goto END;
	}

	struct itimerval tick;
	struct itimerval oldtick;
	tick.it_value.tv_sec = interval/1000;
	tick.it_value.tv_usec = (interval%1000) * 1000;
	tick.it_interval.tv_sec = interval/1000;
	tick.it_interval.tv_usec = (interval%1000) * 1000;

	if (setitimer(ITIMER_REAL, &tick, &oldtick) < 0) {
		call_ret = TT_SET_TIMER_ERROR;
		goto END;
	}

	if (sigsetjmp(invoke_env, SIGALRM) == 0) {
		if(!args){
			call_ret = timeout_execute_with_0_params(call_func, retval);
		}else{
			call_ret = timeout_execute_with_params(call_func, args, retval);
		}
		if(call_ret != TT_CALL_FUNCTION_SUCC){
			goto END;
		}
	}else{
		//function execute timeout
		CG(unclean_shutdown) = 1;
		CG(active_class_entry) = NULL;
		CG(in_compilation) = 0;
		EG(current_execute_data) = NULL;

		call_ret = TT_CALL_FUNCTION_TIMEOUT;
		goto END;
	}

	call_ret = TT_CALL_FUNCTION_SUCC;

END:
	setitimer(ITIMER_REAL, &oldtick, NULL);
	timeout_set_signal(SIGALRM, sf);

	return call_ret;
}
/*}}}*/

/*{{{ proto int call_func_with_timeout(mixed callable, int interval [, array args [, mixed retval]])
 */
PHP_FUNCTION(call_func_with_timeout)
{
	zend_long interval;
	zval *call_func;
	zval *args = NULL;
	zval *retval_ptr = NULL;
	zval *retval;
	HashTable *args_ht = NULL;
	int call_ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "zl|zz", &call_func, &interval, &args, &retval_ptr) == FAILURE){
		RETURN_FALSE;
	}

	if(args && Z_TYPE_P(args) == IS_ARRAY){
		args_ht = Z_ARR_P(args);
	}

	/**
	 * retval_ptr's type is IS_REFERENCE
	 * retval is the real zval that retval_ptr pointed to
	 */
	if(retval_ptr){
		retval = Z_REFVAL_P(retval_ptr);
		call_ret = call_func_with_timeout(call_func, interval, args_ht, retval);
	}else{
		call_ret = call_func_with_timeout(call_func, interval, args_ht, NULL);
	}

	ZVAL_LONG(return_value, call_ret);
}
/*}}}*/

/* {{{ timeout_functions[]
 *
 * Every user visible function must have an entry in timeout_functions[].
 */
const zend_function_entry timeout_functions[] = {
	PHP_FE(call_func_with_timeout,	arginfo_call_func_with_timeout)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in timeout_functions[] */
};
/* }}} */

/* {{{ timeout_module_entry
 */
zend_module_entry timeout_module_entry = {
	STANDARD_MODULE_HEADER,
	"timeout",
	timeout_functions,
	PHP_MINIT(timeout),
	PHP_MSHUTDOWN(timeout),
	PHP_RINIT(timeout),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(timeout),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(timeout),
	PHP_TIMEOUT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TIMEOUT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(timeout)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
